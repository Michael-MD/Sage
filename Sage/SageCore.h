#pragma once

#include <vector>
#include <string>
#include <map>

class SageCore {
public:
	SageCore(size_t character_total, size_t question_total, std::map<char, float> response_map);
	SageCore(size_t characters_total, size_t question_total, std::map<char, float> response_map, std::vector<float>& priors);

	bool RefineGuess();

	virtual float LookupAnswer(size_t character_i, size_t question_j) = 0;
	virtual std::string LookupQuestionPrompt(size_t question_j) = 0;

	void PrintInternalState() const;

	~SageCore();

public:
	// Top Guess
	size_t top_guess;

private:
	void AskQuestion(const std::string& question_prompt, size_t question_id);
	void UpdateLikelihoods();
	void UpdatePosteriors();

	void calcLikelihoods(std::vector<float>& likelihoods, size_t question_id, float response);
	void calcPosteriors(std::vector<float>& posteriors, std::vector<float>& likelihoods);

	void DecideNextQuestion();

	// Contains character posteriors P(C_i|ans_1, ans_2, ..., ans_n, ans_n+1)
	std::vector<float> posteriors;

	// Contains character likelihoods after latest answer recieved P(ans_n+1 | C_i)
	std::vector<float> likelihoods;

	// Total number of characters under consideration
	size_t character_total;
	size_t question_total;

	// Set of possible user-allowed responses and corresponding numeric values
	std::map<char, float> response_map;

	// The id of the last question asked
	size_t question_id;

	// Response given by user to last question
	float response;
};