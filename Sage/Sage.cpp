// Sage.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <array>
#include <map>

#include "SageCore.h"



std::vector<std::array<float, 4>> true_answers;

class Sage : public SageCore {
public:
    // Constructor accepts a callable object
    using SageCore::SageCore;

    // Implement the pure virtual function
    float LookupAnswer(size_t character_i, size_t question_j) override {
        return true_answers[character_i][question_j];
    }
};

int main()
{
    std::vector<std::string> characters = {
        "homer",
        "spongebob",
        "sandy",
    };

    std::vector<std::string> questions = {
        "Is your character yellow ?",
        "Is your character bald ?",
        "Is your character a man ?",
        "Is your character short ?",
    };

    true_answers = {
        {1.0f, 1.0f, 1.0f, 0.0f},
        {1.0f, 1.0f, 1.0f, 0.75f},
        {0.0f, 0.0f, 0.0f, 0.5f},
    };


    std::map<char, float> response_map = {
        {'y', 0.9f},
        {'n', 0.1f},
        {'p', 0.75f},
        {'u', 0.25f},
        {'d', 0.5f}
    };

    Sage sage(3, response_map);

    for (int i = 0; i < 4; i++) {
        sage.PrintInternalState();

        sage.AskQuestion(questions[i], i);

        sage.UpdateLikelihoods();

        sage.UpdatePosteriors();
    }

    sage.PrintInternalState();
}

