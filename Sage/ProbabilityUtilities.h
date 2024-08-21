#pragma once

#define DEBUGGING

#include <vector>

/// <summary>
/// Calculates the overall probability of an event using the law of total probability.
/// </summary>
/// <param name="conditional_probabilities">A vector containing the conditional probabilities of the event given different hypotheses or conditions. Each element represents \( P(A | B_i) \), the probability of event \( A \) given hypothesis \( B_i \).</param>
/// <param name="probabilities">A vector containing the prior probabilities of the different hypotheses or conditions. Each element represents \( P(B_i) \), the probability of hypothesis \( B_i \).</param>
/// <returns>A float value representing the overall probability of the event, calculated as the sum of the product of each conditional probability and its corresponding prior probability. This value reflects the overall likelihood of the event considering all possible conditions or hypotheses.</returns>
/// <exception cref="std::runtime_error">Thrown if the sizes of conditional_probabilities and probabilities do not match (when DEBUGGING is enabled).</exception>
/// <remarks>
/// The overall probability is calculated using the formula:
/// <code>
/// P(A) = ∑ (P(A | B_i) * P(B_i))
/// </code>
/// where \( P(A | B_i) \) is the conditional probability of the event \( A \) given hypothesis \( B_i \), and \( P(B_i) \) is the prior probability of hypothesis \( B_i \). The formula aggregates the probabilities over all possible hypotheses or conditions to compute the total probability of the event.
/// </remarks>
float evidence(std::vector<float>& conditional_probabilities, std::vector<float>& probabilities);


/// <summary>
/// Computes the entropy of a probability distribution, which quantifies the uncertainty or randomness of the distribution for a random variable.
/// </summary>
/// <param name="probabilities">A vector containing the probabilities of different outcomes for a random variable. All probabilities should be non-negative and sum up to 1, \( P(x_i) \).</param>
/// <returns>A float value representing the entropy of the probability distribution, calculated as the sum of the negative product of each probability and its logarithm (base e). This value measures the uncertainty associated with the distribution.</returns>
/// <exception cref="std::runtime_error">Thrown if any probability in the vector is outside the range [0, 1] (when DEBUGGING is enabled).</exception>
/// <remarks>
/// Entropy is calculated using the formula:
/// <code>
/// Entropy = - ∑ (P(x_i) * log(P(x_i)))
/// </code>
/// where \( P(x_i) \) is the probability of outcome \( x_i \), and \( log \) is the natural logarithm (base e). Entropy quantifies the average level of uncertainty or information content in the distribution.
/// </remarks>
float entropy(std::vector<float>& probabilities);
