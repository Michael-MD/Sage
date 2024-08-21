#include "SageCore.h"
#include"ProbabilityUtilities.h"

#include <iostream>
#include <iomanip>  // For std::fixed and std::setprecision
#include <limits>
#include <numeric> // For std::accumulate

#define DEBUGGING

SageCore::SageCore(size_t character_total, size_t question_total, std::map<char, float> response_map)
	: character_total(character_total),
	question_total(question_total),
	response_map(response_map)
{
	likelihoods.resize(character_total);

	// Set up character priors
	posteriors.resize(character_total);
	posteriors.assign(character_total, 1.0f / (float)character_total);
}

SageCore::SageCore(size_t characters_total, size_t question_total, std::map<char, float> response_map, std::vector<float>& priors)
	: SageCore(characters_total, question_total, response_map)
{
	posteriors = priors;
}

SageCore::~SageCore() {

}

void SageCore::AskQuestion(const std::string& question_prompt, size_t question_id) {
	this->question_id = question_id;

	char response_char;

	while (true) {
		// Ask the user the question and get a response
		std::cout << question_prompt;
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

void SageCore::calcLikelihoods(std::vector<float>& likelihoods, size_t question_id, float response) {
	for (size_t character_i = 0; character_i < character_total; character_i++) {
		likelihoods[character_i] = 1 - std::abs(LookupAnswer(character_i, question_id) - response);
	}
}

void SageCore::UpdateLikelihoods() {
	calcLikelihoods(likelihoods, question_id, response);
}

void SageCore::calcPosteriors(std::vector<float>& posteriors, std::vector<float>& likelihoods) {
	float normalization = 0.0;
	for (size_t character_i = 0; character_i < character_total; character_i++) {
		normalization += posteriors[character_i] * likelihoods[character_i];
	}

	normalization = 1.0f / normalization;

	for (size_t character_i = 0; character_i < character_total; character_i++) {
		posteriors[character_i] *= likelihoods[character_i] * normalization;
	}
}

void SageCore::UpdatePosteriors() {
	calcPosteriors(posteriors, likelihoods);
}

void SageCore::PrintInternalState() const {
	std::cout << "\nPosteriors: ";

	// Iterate over the vector and print each element
	for (const float& posterior : posteriors) {
		std::cout << std::fixed << std::setprecision(2) << posterior << " ";  // Print with 2 decimal places
	}

	std::cout << std::endl;
}

static void find_greatest_and_second_greatest(const std::vector<float>& values, float& greatest, float& second_greatest, size_t& top_guess) {
	if (values.size() < 2) {
		throw std::runtime_error("Vector must contain at least two elements.");
	}

	// Initialize the greatest and second_greatest with minimum possible values
	greatest = -std::numeric_limits<float>::infinity();
	second_greatest = -std::numeric_limits<float>::infinity();

	int k = 0;
	for (const auto& value : values) {
		if (value >= greatest) {
			second_greatest = greatest;
			greatest = value;
			top_guess = k;
		}
		else if (value > second_greatest && value < greatest) {
			second_greatest = value;
		}

		k++;
	}

	// Handle the case where all elements are equal
	if (second_greatest == -std::numeric_limits<float>::infinity()) {
		throw std::runtime_error("All elements in the vector are equal or second greatest was not found.");
	}
}

bool SageCore::RefineGuess() {
	DecideNextQuestion();

	AskQuestion(LookupQuestionPrompt(question_id), question_id);
	UpdateLikelihoods();
	UpdatePosteriors();

	// Decide whether to stop
	float greatest, second_greatest;
	find_greatest_and_second_greatest(posteriors, greatest, second_greatest, top_guess);
	if (greatest > 3 * second_greatest) {
		return false;
	}
	else {
		return true;
	}
}

void SageCore::DecideNextQuestion() {
	std::vector<float> conditional_probabilities(character_total); // P(Q_j=Ans|C_i)
	std::vector<float> future_likelihoods(character_total);        // P(C_i|Q_other)
	std::vector<float> future_posteriors(character_total);         // P(C_i|Q_other)
	std::vector<float> answer_evidence(response_map.size()); // For storing evidence values

	float next_question_entropy = std::numeric_limits<float>::infinity();

	for (size_t question_i = 0; question_i < question_total; question_i++) {
		// Initialize expected entropy for this question
		float tmp_question_i_expected_entropy = 0;

		// Calculate evidence for each possible answer
		size_t k = 0;
		for (const auto& pair : response_map) {
			float ans = pair.second;
			// Calculate conditional probabilities and evidence
			calcLikelihoods(conditional_probabilities, question_i, ans);
			answer_evidence[k++] = evidence(conditional_probabilities, posteriors);
		}

		// Normalize evidence
		float sum = std::accumulate(answer_evidence.begin(), answer_evidence.end(), 0.0f);
		if (sum != 0.0f) {
			for (float& value : answer_evidence) {
				value /= sum;
			}
		}

		// Calculate expected entropy for the current question
		k = 0;
		for (const auto& pair : response_map) {
			float ans = pair.second;

			// Compute future posteriors and entropy
			future_posteriors = posteriors;
			calcLikelihoods(future_likelihoods, question_i, ans);
			calcPosteriors(future_posteriors, future_likelihoods);
			float tmp_entropy = entropy(future_posteriors);

			// Calculate weighted entropy
			tmp_question_i_expected_entropy += tmp_entropy * answer_evidence[k++];
		}

		// Update the best question to ask
		if (next_question_entropy > tmp_question_i_expected_entropy) {
			next_question_entropy = tmp_question_i_expected_entropy;
			question_id = question_i;
		}
	}
}
