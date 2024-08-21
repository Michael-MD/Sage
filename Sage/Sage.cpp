// Sage.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define DEBUGGING

#include <iostream>
#include <vector>
#include <array>
#include <map>

#include "SageCore.h"

std::vector<std::array<float, 11>> true_answers;
std::vector<std::string> questions;

class Sage : public SageCore {
public:
    // Constructor accepts a callable object
    using SageCore::SageCore;

    // Implement the pure virtual function
    float LookupAnswer(size_t character_i, size_t question_j) override {
        return true_answers[character_i][question_j];
    }

    std::string LookupQuestionPrompt(size_t question_j) {
        return questions[question_j];
    }
};

int main()
{
    std::vector<std::string> characters = {
    "homer",
    "spongebob",
    "sandy",
    "marge",
    "patrick",
    "mr_krabs",
    "bart",
    "plankton",
    "lisa",
    "squidward",
    };


    questions = {
    "Is your character yellow?",
    "Is your character bald?",
    "Is your character a man?",
    "Is your character short?",
    "Does your character have hair?",
    "Is your character a child?",
    "Is your character green?",
    "Is your character greedy?",
    "Is your character intelligent?",
    "Does your character play a musical instrument?",
    "Are you thinking of lisa?",
    };


    true_answers = {
        {1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f}, // Homer
        {1.0f, 0.0f, 0.0f, 0.75f, 1.0f, 1.0f, 0.0f, 0.25f, 0.5f, 0.5f, 0.0f}, // Spongebob
        {0.0f, 1.0f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 0.25f, 0.75f, 0.0f, 0.0f}, // Sandy
        {1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.25f, 0.75f, 0.0f, 0.0f}, // Marge
        {0.0f, 1.0f, 0.0f, 0.25f, 0.0f, 1.0f, 0.0f, 0.25f, 0.0f, 1.0f, 0.0f}, // Patrick
        {0.0f, 1.0f, 1.0f, 0.75f, 0.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.0f, 0.0f}, // Mr. Krabs
        {1.0f, 0.0f, 0.0f, 0.75f, 1.0f, 1.0f, 0.0f, 0.25f, 0.50f, 0.0f, 0.0f}, // Bart
        {0.0f, 1.0f, 1.0f, 1.00f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f}, // Plankton
        {1.0f, 0.0f, 0.0f, 1.00f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f}, // Lisa
        {0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.25f, 0.5f, 1.0f, 0.0f}  // Squidward
    };



    std::map<char, float> response_map = {
        {'y', 0.9f},
        {'n', 0.1f},
        {'p', 0.75f},
        {'u', 0.25f},
        {'d', 0.5f}
    };

    Sage sage(characters.size(), questions.size(), response_map);

    //// Ask every question
    //for (int i = 0; i < questions.size(); i++) {
    //    sage.PrintInternalState();

    //    sage.RefineGuess(questions[i], i);
    //}

    //sage.PrintInternalState();

    while (sage.RefineGuess()) {
        sage.PrintInternalState();
    }
    sage.PrintInternalState();

    std::cout << characters[sage.top_guess];
}

