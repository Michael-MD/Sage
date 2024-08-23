#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <array>
#include <map>
#include <stdexcept>

#include "SageCore.h"

std::vector<std::vector<std::string>> data;

std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

// Function to read the tab-separated values (TSV) file
std::vector<std::vector<std::string>> readTSV(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    std::vector<std::vector<std::string>> data;

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return data;
    }

    while (std::getline(file, line)) {
        std::vector<std::string> row = split(line, ',');
        data.push_back(row);
    }

    file.close();
    return data;
}

std::vector<std::array<float, 28>> true_answers;
std::vector<std::string> questions;

class Sage : public SageCore {
public:
    // Constructor accepts a callable object
    //using SageCore::SageCore;

    Sage(size_t character_total, size_t question_total, std::map<char, float> response_map) : SageCore(character_total, question_total, response_map) {
        if (!bDataSetIsValid()) {
            std::cerr << "Some options cannot be uniquely identified by the questions.\n";
        }
    }

    // Implement the pure virtual function
    float LookupAnswer(size_t character_i, size_t question_j) override {
        if (question_j < 12) {
            return (float)std::stoul(data[character_i + 1][question_j + 1]);
        }
        else if (question_j >= 12 && question_j <= 20) { // Legs question
            int correct_answer = std::stoul(data[character_i + 1][12 + 1]);
            int number_of_legs = (question_j - 12);
            if (correct_answer == number_of_legs) return 1.0f;
            else return 0.0f;
        }
        else if (question_j >= 24) {    // class type
            int correct_answer = std::stoul(data[character_i + 1][16 + 1]);
            int class_type_ans = (question_j - 23) + 3;
            if (correct_answer == class_type_ans) return 1.0f;
            else return 0.0f;
        }
        else {
            return (float)std::stoul(data[character_i + 1][question_j + 1 - 21 + 13]);
        }

    }

    std::string LookupQuestionPrompt(size_t question_j) {
        return questions[question_j];
    }
};

enum AnimalAttributes {
    ANIMAL_NAME,
    HAIR,
    FEATHERS,
    EGGS,
    MILK,
    AIRBORNE,
    AQUATIC,
    PREDATOR,
    TOOTHED,
    BACKBONE,
    BREATHES,
    VENOMOUS,
    FINS,
    LEGS,
    TAIL,
    DOMESTIC,
    CATSIZE,
};


std::map<char, float> response_map = {
        {'y', 0.8f},
        {'n', 0.2f},
        {'p', 0.65f},
        {'u', 0.35f},
};

int main() {
    std::string filename = "C:/Users/61481/Downloads/zoo2.csv"; // Replace with your file path
    data = readTSV(filename);

    questions = {
        "hair?",
        "feathers?",
        "eggs?",
        "milk?",
        "airborne?",
        "aquatic?",
        "predator?",
        "toothed?",
        "backbone?",
        "breathes?",
        "venomous?",
        "fins?",
        "0 legs?",
        "1 leg?",
        "2 legs?",
        "3 legs?",
        "4 legs?",
        "5 legs?",
        "6 legs?",
        "7 legs?",
        "8 legs?",
        "tail?",
        "domestic?",
        "catsize?",
        "class_type 3?",
        "class_type 4?",
        "class_type 5?",
        "class_type 6?",
        "class_type 7?",
    };

    Sage sage(data.size() - 1, questions.size() - 1, response_map);

    while (sage.RefineGuess()) {
        sage.PrintInternalState();
    }
    sage.PrintInternalState();

    std::cout << data[sage.TopGuess() + 1][0];

    return 0;
}