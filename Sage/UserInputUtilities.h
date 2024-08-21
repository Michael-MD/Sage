#pragma once

#include <string>
#include <map>

/// <summary>
/// Prompts the user with a question and returns a corresponding numerical value based on their response.
/// </summary>
/// <param name="question_prompt">A string containing the question to be presented to the user.</param>
/// <param name="response_map">A map associating response characters with their corresponding numeric values.</param>
/// <returns>A float value representing the user's response based on the provided map. Returns 0.0 for invalid input.</returns>
float ask_question(const std::string& question_prompt, const std::map<char, float>& response_map);
