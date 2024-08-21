#include "UserInputUtilities.h"
#include <iostream>
#include <map>

float ask_question(const std::string& question_prompt, const std::map<char, float>& response_map) {
    char response_char;
    float response = 0.0f; // Default value for invalid input

    while (true) {
        // Ask the user the question and get a response
        std::cout << question_prompt;
        std::cin >> response_char;

        // Translate response to numeric based on the provided map
        auto it = response_map.find(response_char);
        if (it != response_map.end()) {
            return it->second;
        }
        else {
            std::cerr << "Invalid input." << std::endl;
        }
    }
}

