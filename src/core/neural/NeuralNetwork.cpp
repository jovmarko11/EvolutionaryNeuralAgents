//
// Created by Marko Jovanovic on 11. 9. 2026..
//

#include "NeuralNetwork.h"

#include <cmath>
#include <functional>
#include <stdexcept>

NeuralNetwork::NeuralNetwork(std::vector<std::size_t> layerSizes) {

    if (layerSizes.size() < 2)
        throw std::invalid_argument("NeuralNetwork: potrebno je najmanje dva sloja.");

    std::function<double(double)> sigmoid = [](double x) {
        return 1.0 / (1.0 + std::exp(-x));
    };

    std::function<double(double)> tanh = [](double x) {
        return std::tanh(x);
    };

    std::size_t inputSize = layerSizes[0];

    for (std::size_t i = 1; i < layerSizes.size(); i++) {
        layers.emplace_back(inputSize,layerSizes[i], tanh);

        inputSize = layerSizes[i];
    }
}

Matrix NeuralNetwork::forward(const Matrix& input) const {
    Matrix result = input;

    for (const auto& layer : layers) {
        result = layer.forward(result);
    }

    return result;
}


std::size_t NeuralNetwork::getParameterCount() const {
    std::size_t result = 0;
    for (const auto& layer : layers) {
        result += layer.getParameterCount();
    }
    return result;
}

std::vector<double> NeuralNetwork::getParameters() const {
    std::vector<double> result;
    result.reserve(getParameterCount());

    // za svaki layer, prvo weights pa biases
    for (const auto& layer : layers) {
        const Matrix& weights = layer.getWeights();
        for (std::size_t i = 0; i < weights.getRows(); i++) {
            for (std::size_t j = 0; j < weights.getCols(); j++) {
                result.push_back(weights(i,j));
            }
        }
        const Matrix& biases = layer.getBiases();
        for (std::size_t i = 0; i < biases.getRows(); i++) {
            for (std::size_t j = 0; j < biases.getCols(); j++) {
                result.push_back(biases(i,j));
            }
        }
    }
    assert(result.size() == getParameterCount());
    return result;
}

void NeuralNetwork::setParameters(const std::vector<double>& genome) {
    assert(genome.size() == getParameterCount());
    std::size_t offset = 0;
    for (Layer& layer : layers) {
        offset = layer.restoreFrom(genome, offset);
    }
    assert(offset == getParameterCount());
}
