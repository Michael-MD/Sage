// Sage.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define DEBUGGING

#include <iostream>
#include <vector>
#include <array>
#include <map>

#include "SageCore.h"

std::vector<std::array<float, 20>> true_answers;
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
    "woody",
    "buzz_lightyear",
    "sully",
    "mr_incredible",
    "lightning_mcqueen",
    "wall_e",
    "remy",
    "joy",
    "merida",
    "dory",
    "elastigirl",
    "miguel",
    "slinky",
    };

    questions = {
        "Is your character human?",
        "Is your character an animal?",
        "Is your character a toy?",
        "Does your character have superpowers?",
        "Is your character a robot?",
        "Is your character a child?",
        "Is your character brave?",
        "Does your character drive a vehicle?",
        "Is your character from a fantasy world?",
        "Is your character associated with food?",
        "Is your character emotional?",
        "Is your character a leader?",
        "Does your character have fur?",
        "Is your character aquatic?",
        "Is your character from another planet?",
        "Is your character small in size?",
        "Does your character play a musical instrument?",
        "Is your character a sidekick?",
        "Is your character a mentor?",
        "Is your character comedic?"
    };

    true_answers = {
        {0.25f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f, 0.75f}, // Woody
        {0.25f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.5f, 0.75f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.5f, 0.5f, 1.0f}, // Buzz Lightyear
        {0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f, 1.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.25f, 0.5f, 0.25f}, // Sully
        {1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.75f, 0.75f, 1.0f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.75f, 0.5f}, // Mr. Incredible
        {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.5f, 0.5f}, // Lightning McQueen
        {0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.75f, 0.0f, 0.0f, 0.0f, 0.5f}, // WALL-E
        {0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.25f, 0.0f, 0.5f}, // Remy
        {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.25f, 1.0f}, // Joy
        {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.7f, 0.5f}, // Merida
        {0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.75f, 0.0f, 0.0f, 0.25f, 1.0f}, // Dory
        {1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.5f, 1.0f, 0.0f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f, 0.5f}, // Elastigirl
        {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.75f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.25f, 0.0f, 0.5f}, // Miguel
        {0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.25f, 0.0f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.25f, 0.5f, 0.75f}, // Slinky
    };




    std::map<char, float> response_map = {
        {'y', 0.9f},
        {'n', 0.1f},
        {'p', 0.65f},
        {'u', 0.35f},
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

    std::cout << characters[sage.TopGuess()];
}

