#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <array>
#include <map>
#include <stdexcept>
#include <unordered_set>
#include <algorithm>


#include "SageCore.h"

std::vector<std::vector<std::string>> data;
std::vector<std::string> column_names;
std::vector<std::vector<std::string>> category_options;
std::vector<std::string(*)(size_t, decltype(category_options)&)> question_prompts;
size_t number_of_rows, number_of_columns;

std::vector<std::vector<std::string>> readTSV(const std::string& filename);
void get_question_category(size_t question_j, size_t& question_category, size_t& question_i);

class Sage : public SageCore {
public:
    using SageCore::SageCore;

    // Implement the pure virtual function
    float LookupAnswer(size_t character_i, size_t question_j) override;
    std::string LookupQuestionPrompt(size_t question_j) override;
};


enum {
    SERIES_TITLE,
    RELEASED_YEAR,
    CERTIFICATE,
    RUNTIME,
    IMDB_RATING,
    META_SCORE,
    DIRECTOR,
    STAR1,
    STAR2,
    STAR3,
    STAR4,
} column_indices, tmp_indices;


int main() {
    std::string filename = "C:/Users/61481/Downloads/imdb_top_1000.csv"; // Replace with your file path
    data = readTSV(filename);

    number_of_columns = data[0].size();
    number_of_rows = data.size() - 1;    // exclude column title row

    // Get column names
    column_names.resize(number_of_columns);
    for (int column = 0; column < number_of_columns; column++) {
        column_names[column] = data[0][column];
    }


    // Get unique keywords of each column
    std::vector<std::unordered_set<std::string>> categories(number_of_columns);
    for (int column = 0; column < number_of_columns; column++) {
        for (int row = 1; row < number_of_rows; row++) {
            categories[column].insert(data[row][column]);
        }
    }

    category_options.resize(number_of_columns);
    for (int column = 0; column < number_of_columns; column++) {
        category_options[column].resize(categories[column].size());
        int index = 0;
        for (const auto& keyword : categories[column]) {
            category_options[column][index++] = keyword;
        }
    }

    // Print out the length of each category for debugging
    int k = 0;
    for (auto& category : category_options) {
        std::cout << column_names[k++] << ": " << category.size() << '\n';
    }

    unsigned int total_questions = 0;
    for (int column = 1; column < number_of_columns; column++) {
        total_questions += category_options[column].size();
    }

    // Make question prompts
    question_prompts.resize(total_questions);
    question_prompts = {
        [](size_t question_j, decltype(category_options)& category_options) -> std::string {    // released
            std::stringstream ss;
            ss << "Was the movie released in " << category_options[RELEASED_YEAR][question_j] << "?";
            return ss.str();
        },

        [](size_t question_j, decltype(category_options)& category_options) -> std::string {    // certificate
            std::stringstream ss;
            ss << "Is the movie rated " << category_options[CERTIFICATE][question_j] << "?";
            return ss.str();
        },

        [](size_t question_j, decltype(category_options)& category_options) -> std::string {    // runtime
            std::stringstream ss;
            ss << "Does the movie run for " << category_options[RUNTIME][question_j] << "?";
            return ss.str();
        },

        [](size_t question_j, decltype(category_options)& category_options) -> std::string {    // imdb_rating
            std::stringstream ss;
            ss << "Is the IMDB rating " << category_options[IMDB_RATING][question_j] << "?";
            return ss.str();
        },

        [](size_t question_j, decltype(category_options)& category_options) -> std::string {    // meta_score
            std::stringstream ss;
            ss << "Is the meta score " << category_options[META_SCORE][question_j] << "?";
            return ss.str();
        },

        [](size_t question_j, decltype(category_options)& category_options) -> std::string {    // director
            std::stringstream ss;
            ss << "Is the movie directed by " << category_options[DIRECTOR][question_j] << "?";
            return ss.str();
        },

        [](size_t question_j, decltype(category_options)& category_options) -> std::string {    // star1
            std::stringstream ss;
            ss << "Does the movie star " << category_options[STAR1][question_j] << "?";
            return ss.str();
        },

        [](size_t question_j, decltype(category_options)& category_options) -> std::string {    // star2
            std::stringstream ss;
            ss << "Is " << category_options[STAR2][question_j] << " a secondary character?";
            return ss.str();
        },

        [](size_t question_j, decltype(category_options)& category_options) -> std::string {    // star3
            std::stringstream ss;
            ss << "Is " << category_options[STAR3][question_j] << " a tertiary character?";
            return ss.str();
        },

        [](size_t question_j, decltype(category_options)& category_options) -> std::string {    // star4
            std::stringstream ss;
            ss << "Is " << category_options[STAR4][question_j] << " a minor character?";
            return ss.str();
        },

    };

    std::map<char, float> response_map = {
        {'y', 0.95f},
        {'n', 0.05f},
        //{'p', 0.75f},
        //{'u', 0.25f},
        //{'d', 0.5f},
    };

    Sage sage(category_options[0].size(), total_questions, response_map);
    //size_t question_j = 121, character_i = 1;
    //std::cout << sage.LookupQuestionPrompt(question_j);
    //std::cout << sage.LookupAnswer(character_i, question_j);
    //std::cout << data[0 + 1][0];

    //sage.PrintInternalState();

    while (sage.RefineGuess()) {
        //sage.PrintInternalState();
    }

    std::cout << data[sage.TopGuess() + 1][0];

    return 0;
}


// Read CSV
std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}
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

void get_question_category(size_t question_j, size_t& question_category, size_t& question_i) {
    question_i = 0;
    question_category = 0;


    for (question_category = 1; question_category < number_of_columns; question_category++) {
        if (question_j >= question_i && question_j < question_i + category_options[question_category].size()) break;;
        question_i += category_options[question_category].size();
    }

    question_i = question_j - question_i;
}

float Sage::LookupAnswer(size_t character_i, size_t question_j) {
    size_t question_category, question_i;
    get_question_category(question_j, question_category, question_i);

    std::string ans = data[character_i + 1][question_category];

    return ans == category_options[question_category][question_i];
}

std::string Sage::LookupQuestionPrompt(size_t question_j) {
    size_t question_category, question_i;
    get_question_category(question_j, question_category, question_i);
    return question_prompts[question_category - 1](question_i, category_options);
}