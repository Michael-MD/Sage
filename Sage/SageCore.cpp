#include "SageCore.h"
#include"ProbabilityUtilities.h"

#include <iostream>
#include <iomanip>  // For std::fixed and std::setprecision
#include <limits>
#include <numeric> // For std::accumulate
#include <unordered_set>
#include <future>

#include "Timer.h"

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

	// Set up temporary posterior
	tmp_posteriors.resize(character_total);

	// Extract values from response_map
	for (const auto& pair : response_map) {
		responses_numeric.push_back(pair.second);
	}

	question_id = question_total;
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

inline void SageCore::calcLikelihoods(size_t question_id, float response) {
	for (size_t character_i = 0; character_i < character_total; character_i++) {
		likelihoods[character_i] = 1 - std::abs(LookupAnswer(character_i, question_id) - response);
	}
}

inline void SageCore::calcPosteriors(bool tmp) {
	std::vector<float>& vec_posteriors = tmp ? tmp_posteriors : posteriors;

	float normalization = 1.0f / std::inner_product(posteriors.begin(), posteriors.end(), likelihoods.begin(), 0.0f);

	for (size_t character_i = 0; character_i < character_total; character_i++) {
		vec_posteriors[character_i] = posteriors[character_i] * likelihoods[character_i] * normalization;
	}
}

inline float SageCore::calcEntropy(size_t question_id, float response, bool tmp) {
	calcLikelihoods(question_id, response);
	calcPosteriors(tmp);

	std::vector<float>& vec_posteriors = tmp ? tmp_posteriors : posteriors;

#ifdef DEBUGGING
	for (float posterior : vec_posteriors) {
		if (posterior > 1 || posterior < 0) {
			throw std::runtime_error("Probabilities must be between zero and one");
		}
	}
#endif

	float entropy = 0.0f;
	for (float posterior : vec_posteriors) {
		if (posterior > 0) { // Avoid log(0) which is undefined
			entropy -= posterior * std::log2(posterior);
		}
	}

	return entropy;
}

void SageCore::PrintInternalState() const {
	std::cout << "\nPosteriors: ";

	// Iterate over the vector and print each element
	for (const float& posterior : posteriors) {
		std::cout << std::fixed << std::setprecision(6) << posterior << " ";  // Print with 2 decimal places
	}

	std::cout << std::endl;
}

void SageCore::GetTopGuesses() {
	if (posteriors.size() < 2) {
		throw std::runtime_error("Vector must contain at least two elements.");
	}

	// Initialize the greatest and second_greatest with minimum possible values
	top_guess = -std::numeric_limits<float>::infinity();
	runner_up_guess = -std::numeric_limits<float>::infinity();

	for (size_t k = 0; k < posteriors.size(); k++) {

		if (posteriors[k] >= top_guess) {
			runner_up_guess = top_guess;
			top_guess = posteriors[k];
			top_guess_id = k;
		}
		else if (posteriors[k] > runner_up_guess && posteriors[k] < top_guess) {
			runner_up_guess = posteriors[k];
			runner_up_guess_id = k;
		}
	}

	// Handle the case where all elements are equal
	if (runner_up_guess == -std::numeric_limits<float>::infinity()) {
		throw std::runtime_error("All elements in the vector are equal or second greatest was not found.");
	}
}

bool SageCore::RefineGuess() {
	DecideNextQuestion();

	AskQuestion(LookupQuestionPrompt(question_id), question_id);
	calcLikelihoods(question_id, response);
	calcPosteriors();

	// Decide whether to continue
	GetTopGuesses();

	return top_guess <= 1.5 * runner_up_guess;
}

void SageCore::DecideNextQuestion() {
	std::vector<float> answer_evidence(response_map.size());		// P(Q_j=Ans)

	float tmp_entropy, tmp_question_i_expected_entropy;

	float next_question_entropy = std::numeric_limits<float>::infinity();
	float normalization;

	size_t question_last_asked = question_id;

	for (size_t question_i = 0; question_i < question_total; question_i++) {
		// Don't ask the same question twice in a row
		if (question_last_asked == question_i) continue;

		// Initialize expected entropy for this question
		tmp_question_i_expected_entropy = 0;

		// Calculate evidence for each possible answer
		//Timer timer("Evidence calc");
		for (size_t i = 0; i < responses_numeric.size(); i++) {
			calcLikelihoods(question_i, responses_numeric[i]);		// P(Q_j=Ans|C_i)
			answer_evidence[i] = evidence(likelihoods, posteriors);
		}
		//timer.~Timer();

		// Normalize evidence
		normalization = 1.0f / std::accumulate(answer_evidence.begin(), answer_evidence.end(), 0.0f);
		for (float& value : answer_evidence) {
			value *= normalization;
		}

		//Timer ghebh("expected entropy");
		// Calculate expected entropy for the current question
		for (size_t i = 0; i < responses_numeric.size(); i++) {
			// Compute future posteriors and entropy
			calcLikelihoods(question_i, responses_numeric[i]);
			calcPosteriors(true);	// P(C_i|Q_other)
			tmp_entropy = calcEntropy(question_i, responses_numeric[i]);

			// Calculate weighted entropy
			tmp_question_i_expected_entropy += tmp_entropy * answer_evidence[i];
		}
		//ghebh.~Timer();

		// Update the best question to ask
		if (next_question_entropy > tmp_question_i_expected_entropy) {
			next_question_entropy = tmp_question_i_expected_entropy;
			question_id = question_i;
		}
	}
}

struct VectorHash {
	std::size_t operator()(const std::vector<float>& vec) const {
		std::size_t hash = 0;
		std::hash<float> hasher;
		for (float v : vec) {
			hash ^= hasher(v) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
		}
		return hash;
	}
};

bool SageCore::bDataSetIsValid() {
	std::unordered_set<std::vector<float>, VectorHash> unique_questions;

	std::vector<float> character_questions(question_total);
	for (size_t character_i = 0; character_i < character_total; character_i++) {
		for (size_t question_i = 0; question_i < question_total; question_i++) {
			character_questions[question_i] = LookupAnswer(character_i, question_i);
		}

		unique_questions.insert(character_questions);

	}

	return unique_questions.size() == character_total;
}