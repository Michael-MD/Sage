#pragma once

#include <vector>
#include <string>
#include <map>
#include <future>

class SageCore {
public:
	SageCore(size_t character_total, size_t question_total, std::map<char, float> response_map);
	SageCore(size_t characters_total, size_t question_total, std::map<char, float> response_map, std::vector<float>& priors);

	bool RefineGuess();

	virtual float LookupAnswer(size_t character_i, size_t question_j) = 0;
	virtual std::string LookupQuestionPrompt(size_t question_j) = 0;

	void PrintInternalState() const;

	size_t TopGuess() const { return top_guess_id; }
	size_t RunnerUpGuess() const { return runner_up_guess_id; }

	// Each answer should be reachable though a unique set of answers
	bool bDataSetIsValid();

	~SageCore();

public:
	// Store stop few guesses
	size_t top_guess_id, runner_up_guess_id;
	float top_guess, runner_up_guess;

private:
	void AskQuestion(const std::string& question_prompt, size_t question_id);

	inline void calcLikelihoods(size_t question_id, float response);
	inline void calcPosteriors(bool tmp = false);
	inline float calcEntropy(size_t question_id, float response, bool tmp = true);

	void calcQuestionEntropy(size_t question_i);

	void DecideNextQuestion();

	void GetTopGuesses();

	// Contains character posteriors P(C_i|ans_1, ans_2, ..., ans_n, ans_n+1)
	std::vector<float> posteriors;

	// Contains character likelihoods after latest answer recieved P(ans_n+1 | C_i)
	std::vector<float> likelihoods;

	// Helper variables
	std::vector<float> tmp_posteriors;

	std::vector<float> entropies;

	// Total number of characters under consideration
	size_t character_total, question_total;

	// Set of possible user-allowed responses and corresponding numeric values
	std::map<char, float> response_map;

	// Contains values of response_map
	std::vector<float> responses_numeric;

	// The id of the last question asked
	size_t question_id;

	// Response given by user to last question
	float response;

	// Parallelization variables
	// mutexes
	std::mutex entropy_mutex, likelihood_mutex, tmp_posterior_mutex;

	// futures
	std::vector<std::future<void>> entropy_futures;
};