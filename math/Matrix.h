//
// Created by Marko Jovanovic on 11. 9. 2026..
//

#ifndef EVOLUTIONARYNEURALAGENTS_MATRIX_H
#define EVOLUTIONARYNEURALAGENTS_MATRIX_H
#include <cassert>
#include <functional>
#include <vector>

class Matrix {
    size_t rows;
    size_t cols;
    std::vector<double> data;

public:
    Matrix(std::size_t rows, std::size_t cols);
    Matrix(std::size_t rows, std::size_t cols, double initValue);
    static Matrix zeroes(std::size_t rows, std::size_t cols);
    static Matrix identity(std::size_t rows, std::size_t cols);
    static Matrix random(std::size_t rows, std::size_t cols);
    static Matrix randomNormal(std::size_t rows, std::size_t cols);

    Matrix operator+(const Matrix& other) const;
    Matrix operator-(const Matrix& other) const;
    Matrix operator*(const Matrix& other) const;      // matrično množenje
    Matrix operator*(double scalar) const;
    Matrix& operator+=(const Matrix& other);
    Matrix& operator*=(double scalar);

    Matrix hadamard(const Matrix& other) const;        // element-wise množenje, treba ti za backprop
    Matrix apply(std::function<double(double)> f) const; // za sigmoid/ReLU/tanh
    Matrix transpose() const;

    double sum() const;
    double mean() const;
    double max() const;
    std::size_t size() const;
    size_t argmax() const;

    double& operator() (std::size_t row, std::size_t col);
    const double& operator() (std::size_t row, std::size_t col) const;

    std::size_t getRows() const;
    std::size_t getCols() const;
};

#endif //EVOLUTIONARYNEURALAGENTS_MATRIX_H