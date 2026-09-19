//
// Created by Marko Jovanovic on 11. 9. 2026..
//

#include "Layer.h"
#include <stdexcept>
#include <cassert>

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

std::size_t Layer::getParameterCount() const {
    return weights.getRows() * weights.getCols() + biases.getRows() * biases.getCols();
}

void Layer::flattenInto(std::vector<double> &out) const {

    for (std::size_t i = 0; i < weights.getRows(); ++i) {
        for (std::size_t j = 0; j < weights.getCols(); ++j) {
            out.push_back(weights(i, j));
        }
    }

    for (std::size_t i = 0; i < biases.getRows(); ++i) {
        out.push_back(biases(i, 0));
    }
}

std::size_t Layer::restoreFrom(const std::vector<double> &in, std::size_t offset) {
    Matrix w(weights.getRows(), weights.getCols());

    for (std::size_t i = 0; i < weights.getRows(); ++i) {
        for (std::size_t j = 0; j < weights.getCols(); ++j) {
            w(i, j) = in[offset++];
        }
    }
    setWeights(w);

    Matrix b(biases.getRows(), biases.getCols());
    for (std::size_t i = 0; i < biases.getRows(); ++i) {
        b(i, 0) = in[offset++];
    }
    setBiases(b);

    return offset;
}

void Layer::setWeights(const Matrix& w) {
    if (w.getRows() != weights.getRows() || w.getCols() != weights.getCols())
        throw std::invalid_argument("Layer::setWeights: pogresna dimenzija matrice tezina");
    weights = w;
}

void Layer::setBiases(const Matrix& b) {
    if (b.getRows() != biases.getRows() || b.getCols() != biases.getCols())
        throw std::invalid_argument("Layer::setBiases: pogresna dimenzija matrice tezina");
    biases = b;
}



