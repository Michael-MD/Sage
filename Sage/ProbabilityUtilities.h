#pragma once

#include <vector>

float evidence(std::vector<float>& conditional_probabilities, std::vector<float>& probabilities);
float entropy(std::vector<float>& probabilities);