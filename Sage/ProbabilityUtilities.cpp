#include "ProbabilityUtilities.h"


#include <stdexcept>
#include <cmath>
#include <numeric> // For std::inner_product

float evidence(std::vector<float>& conditional_probabilities, std::vector<float>& probabilities) {
	// Calcuates a probability through paritioning of sample space

#ifdef DEBUGGING
	if (conditional_probabilities.size() != probabilities.size()) {
		throw std::runtime_error("Fatal error, conditional probabilities and probabilities must be of the same size");
	}
#endif

	float total_probability = std::inner_product(probabilities.begin(), probabilities.end(), conditional_probabilities.begin(), 0.0f);
	return total_probability;
}

float entropy(std::vector<float>& probabilities) {
#ifdef DEBUGGING
	for (float probability : probabilities) {
		if (probability > 1 || probability < 0) {
			throw std::runtime_error("Probabilities must be between zero and one");
		}
	}
#endif

	float total_entropy = 0.0f;
	for (float probability : probabilities) {
		if (probability > 0) { // Avoid log(0) which is undefined
			total_entropy -= probability * std::log2(probability);
		}
	}

	return total_entropy;
}
