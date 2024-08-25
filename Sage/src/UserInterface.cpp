#include "SageCore.h"


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

	AskQuestion();
	calcLikelihoods(likelihoods, question_id, response);
	calcPosteriors(likelihoods, posteriors);

	// Decide whether to continue
	GetTopGuesses();

	return top_guess <= 1.5 * runner_up_guess;
}
