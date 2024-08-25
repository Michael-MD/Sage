#include "SageCore.h"

#include <numeric>
#include <cmath>

inline void SageCore::calcLikelihoods(std::vector<float>& tmp_likelihoods, size_t question_id, float response) {
	for (size_t character_i = 0; character_i < character_total; character_i++) {
		tmp_likelihoods[character_i] = 1 - std::abs(LookupAnswer(character_i, question_id) - response);
	}
}

inline void SageCore::calcPosteriors(std::vector<float>& tmp_likelihoods, std::vector<float>& tmp_posteriors) {

	float normalization = 1.0f / std::inner_product(posteriors.begin(), posteriors.end(), tmp_likelihoods.begin(), 0.0f);

	for (size_t character_i = 0; character_i < character_total; character_i++) {
		tmp_posteriors[character_i] = posteriors[character_i] * tmp_likelihoods[character_i] * normalization;
	}
}

inline float SageCore::calcEntropy(std::vector<float>& tmp_likelihoods, std::vector<float>& tmp_posteriors, size_t question_id, float response) {
	calcLikelihoods(tmp_likelihoods, question_id, response);
	calcPosteriors(tmp_likelihoods, tmp_posteriors);

#ifdef DEBUGGING
	for (float posterior : curr_posteriors) {
		if (posterior > 1 || posterior < 0) {
			throw std::runtime_error("Probabilities must be between zero and one");
		}
	}
#endif

	float entropy = 0.0f;
	for (float posterior : tmp_posteriors) {
		if (posterior > 0) { // Avoid log(0) which is undefined
			entropy -= posterior * std::log2(posterior);
		}
	}

	return entropy;
}