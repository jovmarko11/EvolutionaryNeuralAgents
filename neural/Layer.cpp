//
// Created by Marko Jovanovic on 11. 9. 2026..
//

#include "Layer.h"
#include <stdexcept>

Layer::Layer(std::size_t inputSize, std::size_t outputSize, std::function<double(double)> activation)
    : weights(Matrix::randomNormal(outputSize, inputSize)),
        biases(Matrix::zeroes(outputSize, 1)),
        activation(std::move(activation)) {}


Matrix Layer::forward(const Matrix& input) const {
    if (input.getRows() != weights.getCols() || input.getCols() != 1)
        throw std::invalid_argument("Layer::forward: ulaz nije kolona odgovarajuce duzine");

    Matrix result(weights.getRows(), 1); // outputsize x 1

    for (std::size_t i = 0; i < weights.getRows(); ++i) {
        double sum = 0;

        for (std::size_t j = 0; j < weights.getCols(); ++j) {
            sum += weights(i, j) * input(j, 0);
        }

        result(i, 0 ) = activation(sum + biases(i, 0));
    }

    return result;
}
