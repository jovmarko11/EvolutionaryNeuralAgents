//
// Created by Marko Jovanovic on 11. 9. 2026..
//

#ifndef EVOLUTIONARYNEURALAGENTS_NEURALNETWORK_H
#define EVOLUTIONARYNEURALAGENTS_NEURALNETWORK_H
#include <vector>

#include "Layer.h"


class NeuralNetwork {
private:
    std::vector<Layer> layers;
public:
    explicit NeuralNetwork(std::vector<std::size_t> layerSizes);
    Matrix forward(const Matrix& input) const;

    std::size_t getParameterCount() const;
    std::vector<double> getParameters() const;
    void setParameters(const std::vector<double>& genome);
};


#endif //EVOLUTIONARYNEURALAGENTS_NEURALNETWORK_H