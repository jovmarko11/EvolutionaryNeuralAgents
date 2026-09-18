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
