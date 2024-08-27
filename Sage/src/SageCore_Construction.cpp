#include "SageCore.h"

#include <limits>
#include <algorithm> // For std::find
#include <map>



SageCore::SageCore(size_t character_total, size_t question_total, std::map<char, float> response_map)
	: character_total(character_total),
	question_total(question_total),
	response_map(response_map)
{
	likelihoods.resize(character_total);

	// Set up character priors
	posteriors.resize(character_total);
	posteriors.assign(character_total, 1.0f / (float)character_total);
	
	entropies.resize(question_total);
	entropy_futures.resize(question_total);

	// Extract values from response_map
	for (const auto& pair : response_map) {
		responses_numeric.push_back(pair.second);
	}

	question_id = question_total;
	previous_question_ids.resize(questions_asked_memory, question_total);
	oldest_question = 0;
}

SageCore::SageCore(size_t characters_total, size_t question_total, std::map<char, float> response_map, std::vector<float>& priors)
	: SageCore(characters_total, question_total, response_map)
{
	posteriors = priors;
}

SageCore::~SageCore() {

}
