//
// Created by Marko Jovanovic on 11. 9. 2026..
//

#ifndef EVOLUTIONARYNEURALAGENTS_LAYER_H
#define EVOLUTIONARYNEURALAGENTS_LAYER_H
#include <functional>
#include <vector>
#include "../math/Matrix.h"


class Layer {
private:
    Matrix weights;
    Matrix biases;
    std::function<double(double)> activation;

public:
    Layer(std::size_t inputSize, std::size_t outputSize, std::function<double(double)> activation);

    Matrix forward(const Matrix& input) const;

    std::size_t getParameterCount() const;
    void flattenInto(std::vector<double>& out) const;
    std::size_t restoreFrom(const std::vector<double>& in, std::size_t offset);
};


#endif //EVOLUTIONARYNEURALAGENTS_LAYER_H