#include "SageCore.h"

#include <iomanip>  // For std::fixed and std::setprecision
#include <iostream>
#include <unordered_set>
#include <vector>

void SageCore::PrintInternalState() const {
	std::cout << "Posteriors: ";

	// Iterate over the vector and print each element
	for (const float& posterior : posteriors) {
		std::cout << std::fixed << std::setprecision(6) << posterior << " ";  // Print with 2 decimal places
	}

	std::cout << std::endl;


	std::cout << "Entropies: ";

	// Iterate over the vector and print each element
	for (const float& entropy : entropies) {
		std::cout << std::fixed << std::setprecision(6) << entropy << " ";  // Print with 2 decimal places
	}

	std::cout << '\n' << std::endl;
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

