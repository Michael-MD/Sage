#include "SageCore.h"
#include"ProbabilityUtilities.h"

#include <iostream>
#include <iomanip>  // For std::fixed and std::setprecision

SageCore::SageCore(size_t character_total, std::map<char, float> response_map)
	:	character_total(character_total),
		response_map(response_map)
{
	likelihoods.resize(character_total);

	// Set up character priors
	posteriors.resize(character_total);
	posteriors.assign(character_total, 1.0 / (float)character_total);
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

void SageCore::UpdateLikelihoods() {
	for (size_t character_i = 0; character_i < character_total; character_i++) {
		likelihoods[character_i] = 1 - std::abs(LookupAnswer(character_i, question_id) - response);
	}
}

void SageCore::UpdatePosteriors() {
	float normalization = 0;
	for (size_t character_i = 0; character_i < character_total; character_i++) {
		normalization += posteriors[character_i] * likelihoods[character_i];
	}

	normalization = 1.0 / normalization;

	for (size_t character_i = 0; character_i < character_total; character_i++) {
		posteriors[character_i] *= likelihoods[character_i] * normalization;
	}
}

void SageCore::PrintInternalState() const {
	std::cout << "\nPosteriors: ";

	// Iterate over the vector and print each element
	for (const float& posterior : posteriors) {
		std::cout << std::fixed << std::setprecision(2) << posterior << " ";  // Print with 2 decimal places
	}

	std::cout << std::endl;
}