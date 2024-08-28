#include "SageCore.h"

#include <iostream>
#include <numeric> // For std::accumulate and inner_product
#include "Timer.h"

void SageCore::AskQuestion() {
	char response_char;

	while (true) {
		// Ask the user the question and get a response
		std::cout << LookupQuestionPrompt(question_id);
		std::cin >> response_char;

		// Translate response to numeric based on the provided map
		auto it = response_map.find(response_char);
		if (it != response_map.end()) {
			response = it->second;
			return;
		}
		else {
			std::cerr << "Invalid input." << std::endl;
		}
	}
}

void SageCore::calcNextQuestionExpectedEntropy(size_t question_i) {
	float entropy = 0.0f;

	if (std::find(previous_question_ids.begin(), previous_question_ids.end(), question_i) != previous_question_ids.end()) {
		// Don't ask the same question twice in a row
		entropy = std::numeric_limits<float>::infinity();
	}
	else {
		std::vector<float> tmp_likelihoods(character_total);

		float tmp_probability;
		float normalization = 0.0f;

		for (size_t i = 0; i < responses_numeric.size(); i++) {
			calcLikelihoods(tmp_likelihoods, question_i, responses_numeric[i]);

			float total = 0.0f;

			for (size_t character_i = 0; character_i < character_total; character_i++) {
				tmp_probability = posteriors[character_i] * tmp_likelihoods[character_i];
				total += tmp_probability;
				entropy -= tmp_probability * std::log2(tmp_probability);
			}

			normalization += total;
			entropy += total * std::log2(total);
		}

		entropy /= normalization;
	}

	std::lock_guard<std::mutex> lock(entropy_mutex);
	entropies[question_i] = entropy;
}


void SageCore::DecideNextQuestion() {
	Timer timer("q");
	for (size_t question_i = 0; question_i < question_total; question_i++) {
		entropy_futures[question_i] = std::async(std::launch::async, &SageCore::calcNextQuestionExpectedEntropy, this, question_i);
	}

	// Find question with least entropy
	float least_entropy = std::numeric_limits<float>::infinity();
	for (size_t question_i = 0; question_i < question_total; question_i++) {
		// Wait for all future to complete
		entropy_futures[question_i].get();

		if (entropies[question_i] >= least_entropy) continue;
		question_id = question_i;
		least_entropy = entropies[question_i];
	}

	previous_question_ids[oldest_question++] = question_id;
	oldest_question %= questions_asked_memory;
}

