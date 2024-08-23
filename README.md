![Sage](sage_logo.jpg)

# What is this project?
Meet **Sage**, Sage tries to guess what you're thinking by asking a series of questions. Sage is flexible and can be customized to work with any set of questions and answers.
Think of the possibilities! You could set it up to guess your favourite fictional character or act as customer support for common problems or diagnose medical conditions
based on symptoms. You provide it with a database of answers and questions it can ask the user, based on whcih, it will decide through a series of questions the best- 
fitting answer. That's right, I said "best-fitting" answer because it take into account mistakes in the user's responses. Sound like magic? Sort of and some beautiful 
mathematics, right now let's look at a demo and see how to use it ourselves. 

# How to use Sage
## Set up Sage
To include and use the `SageCore.h` header file in your C++ project, follow these instructions.

First, clone the repository to your local machine, in a command prompt:
```bash
https://github.com/Michael-MD/Sage.git
```
Go in the newly created "Sage" directory and click on Sage.sln, and Visual Studio will start up.

## How to setup your own questions and answers
Sage is very easy to use, the hard part is getting the data! Sage is all contained in a C++ class called SageCore. You can include it using
```
#include "SageCore.h"
```
SageCore contains the brain of Sage but you will need to supply it with the questions and answers it needs. To do this extend the Sagecore class and define the virtual 
functions `std::string LookupQuestionPrompt(size_t question_j)` and `LookupAnswer(size_t character_i, size_t question_j)`:
```
class Sage : public SageCore {
public:
    using SageCore::SageCore;

    // Implement the pure virtual function
    float LookupAnswer(size_t character_i, size_t question_j) override {
        // Logic to determine answer to question_j for character_i
        return ans; 
    }

    // Implement the pure virtual function
    std::string LookupQuestionPrompt(size_t question_j) override {
        // Logic for finding corresponding the text for question_j
        return question_j_prompt;
    }
};
```

What just happened! Nothing much, we just built a new class called Sage ontop of SageCore. Occasionally, while sage is running it will want to ask the user a question to 
get more information, it will do so by calling the method `std::string LookupQuestionPrompt(size_t question_j)` with the number "question_j" as arguement. Let's say 
question_j=5, so Sage is asking for the prompt of question 5 (0-indexed) to show it to the user. The function might then return the string "Is your character tall?" etc.
This function will only be called once every time the user is asked a question. However, 'float LookupAnswer(size_t character_i, size_t question_j)' will be called much 
more often!

'float LookupAnswer(size_t character_i, size_t question_j)' is called whenever Sage wants to know the answer to qeustion_j about character_i. For example, if
character_i=2 and question_j=5 and character_i=2 repersents Robert Borone then Sage wants to know is Robert Berone tall. We want to say yes so we return 1.0f. To say no 
we would pass 0.0f. We are also free to pass anything inbetween like 0.5f which corresponds to "I don't know". That's it! You are now ready to start Sage.

You do this by specifying the set of acceptable responses to the user and the degree of certainty they correspond to. Here is an example:
```
std::map<char, float> response_map = {
        {'y', 0.8f},
        {'n', 0.2f},
        {'p', 0.65f},
        {'u', 0.35f},
};
```
This means the possible options per question are 'y' (yes), 'n' (no), 'p' (probabally), 'u' (unlikely). Why didn't I set yes to 1.0f for example? For a deeper dive check out 
sec::responses. However, for now suffice it to say if a user says yes or no we will not trust them 100%. For example if a user says "no" to the question 
"Is Robert tall?" then Robert would be entirely eliminated eventhough Robert may be the right and answer and the user made a mistake. Needless to say, Robert will be 
suppressed as a possible answer.

### Example
Let's do an example to get a better sense. 

1. Define your set of questions
```
std::vector<std::string> characters = {
    "Robert Borone",
    "Buzz Lightyear",
    "Phoebe Buffay"
};
```

2. Define a set of questions
```
std::vector<std::string> questions = {
    "Is your character a toy?",
    "Is your character known for their space adventures?",
    "Does your character play a musical instrument?",
    "Is your character a human?",
    "Does your character have a catchphrase?",
    "Is your character tall?"
};
``` 

3. Define the answers to each question for each character
```
std::vector<std::vector<float>> true_answers = {
    {0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f}, // Robert Borone
    {1.0f, 1.0f, 0.0f, 0.5f, 1.0f, 0.0f}, // Buzz Lightyear
    {0.0f, 0.0f, 1.0f, 1.0f, 0.35f, 0.0f}  // Phoebe Buffay
};
```
Here the rows correspond to a particular character and the column corresponds to a question. The order of the questions is the asme 
as the order we wrote them in step 2. 
Notice, for Buzz Lightyear, it's unclear if he is a human since he is also a toy so I put 0.5f for "I don't know".

4. Extend SageCore
```
class Sage : public SageCore {
public:
    using SageCore::SageCore;

    // Implement the pure virtual function
    float LookupAnswer(size_t character_i, size_t question_j) override {
        // Logic to determine answer to question_j for character_i
        return ans; 
    }

    // Implement the pure virtual function
    std::string LookupQuestionPrompt(size_t question_j) override {
        // Logic for finding corresponding the text for question_j
        return question_j_prompt;
    }
};
```

4. Implement `std::string LookupQuestionPrompt(size_t question_j)`
```
std::string LookupQuestionPrompt(size_t question_j) {
        return questions[question_j];
    }
```

5. Implement `float LookupAnswer(size_t character_i, size_t question_j)`
```
float LookupAnswer(size_t character_i, size_t question_j) override {
        return true_answers[character_i][question_j];
    }
```

6. Define allowed user responses
```
std::map<char, float> response_map = {
        {'y', 0.9f},
        {'n', 0.1f},
        {'p', 0.65f},
        {'u', 0.35f},
        {'d', 0.5f}
    };
```

7. Instantiate Sage and start it up
```
Sage sage(characters.size(), questions.size(), response_map);
while (sage.RefineGuess());
std::cout << characters[sage.TopGuess()];
```

Done! Here is the full script:
```
#include <iostream>
#include <vector>
#include <array>
#include <map>

#include "SageCore.h"

const unsigned int number_of_questions = 6;
std::vector<std::array<float, number_of_questions>> true_answers;
std::vector<std::string> questions;

class Sage : public SageCore {
public:
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
        "Robert Borone",
        "Buzz Lightyear",
        "Phoebe Buffay"
    };

    questions = {
        "Is your character a toy?",
        "Is your character known for their space adventures?",
        "Does your character play a musical instrument?",
        "Is your character a human?",
        "Does your character have a catchphrase?",
        "Is your character tall?"
    };

    true_answers = {
        {0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f}, // Robert Borone (from "Everybody Loves Raymond")
        {1.0f, 1.0f, 0.0f, 0.5f, 1.0f, 0.0f}, // Buzz Lightyear
        {0.0f, 0.0f, 1.0f, 1.0f, 0.35f, 0.3f}  // Phoebe Buffay
    };

    std::map<char, float> response_map = {
        {'y', 0.9f},
        {'n', 0.1f},
        {'p', 0.65f},
        {'u', 0.35f},
    };

    Sage sage(characters.size(), questions.size(), response_map);
    while (sage.RefineGuess());
    std::cout << characters[sage.TopGuess()];
}
```

# Mathematical Underpinnings
Sage uses very elegant concepts from probability and information theory to update its guess. Using this approach as oppsoed to binary trees for example permits the 
user to answer the questions asked incorrectly occasionally. How impactful the mistake is depends on the response_map chosen.

## How does Sage use the user's responses to make a guess
Lets assume we are performing an experiment where the the user plays our guessing game. Now the outcome of this experiment is given by a cartesian product of 
all posible characters the user could have chosen from the available options, the questions asked and the answers given i.e. 
```math
\Omega = \cup_{k=1,...,Q_t} C \times \{q_1\}\times ... \times \{q_k\} = \{ \{C_i, Q_1=q_1, ..., Q_k=q_k | C_i \in C, Q_1, ..., Q_k \in Q\}, k=1,...,Q_t \}
```
where $C$ is the set of all possible characters, $Q$ is the set of all possible questions, $Ans$ are the set of responses given by the user, 
$Q_1, ..., Q_k$ are a set of random variables denoting the first, second, third etc. question asked, $C_1, ..., C_k$ are a set of
random variables denoting the character the user may be thinking of and $Q_t$ is the total number of questions which could be asked. I'm using $\{Q_1\}$ to 
denote the set of possible outcomes of $Q_1$.

As we ask questions and receive responses we would like to update our guess which we can achieve using Bayes' Theorem give by 
```math
P(A|B) = \frac{P(B|A)P(A)}{P(B)}
````
We can use this result to ask, given the response of the $n^{th}$ question, what are the chances we are thinking of the $i^{th}$ character i.e. 
```math
P(C_i|Q_1=q_1, Q_2=q_2,...,Q_n=q_n) = \frac{P(Q_1=q_1, Q_2=q_2,...,Q_n=q_n|C_i)P(C_i)}{P(Q_1=q_1, Q_2=q_2,...,Q_n=q_n)} = \frac{P(C|\cap_{k=1}^{n-1}Q_k=q_k) P(Q_n=q_n) }
{\sum_{C_i\in C} P(C_i|\cap_{k=1}^{n-1}Q_k=q_k) P(Q_n=q_n|C_i)}
````
This is exactly what we need to update our beliefs!
We begin by defining priors, by default Sage will give each character the same initial weight, this can be modified by specifying a list of priors when instantiating the 
class, each question and response then refine Sage's guess. 

## How does Sage decide on the next question
Sage aims to ask questions which minimize the entropy of the random variable, $X_n$, which describes the character chosen by the user given we have 
asked $n$ questions. This ensures Sage asks questions which will get it to the answer in as few questions as possible. More precisely, given $X_n$ with 
probability mass function $P(C_i|\cap_{k=1}^{n}Q_k=q_k)$ where $C_i$ is what makes $X_n$ a random variable, we wish to minimize the entropy of 
```math
H[X_{n+1}] = - \sum_i P(C_i|\cap_{k=1}^{n+1}Q_k=q_k) \log \, P(C_i|\cap_{k=1}^{n+1}Q_k=q_k)
```
If we ask question $Q_j$ we expect the entropy to be given by:
```math
E[H[X_{n+1}^{Q_{n+1}}]] = \sum_{q_j \in Q} H[C_i|\cap_{k=1}^{n}Q_k=q_k\cap Q_{n+1}=q_j]P(Q_{n+1}=q_j)
```
where $X_{n+1}^{Q_j}$ denotes denotes $X_{n+1}$ where the superscript emphasises that question $Q_{n+1}$ was asked.
