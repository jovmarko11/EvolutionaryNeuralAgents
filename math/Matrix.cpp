//
// Created by Marko Jovanovic on 11. 9. 2026..
//
#include "Matrix.h"
#include "Random.h"
#include <random>
#include <cassert>
#include <stdexcept>
#include <cmath>
#include <algorithm>

Matrix::Matrix(std::size_t rows, std::size_t cols) : rows(rows), cols(cols) {
    data.assign(rows * cols, 0.0);
}

Matrix::Matrix(std::size_t rows, std::size_t cols, double initValue) : rows(rows), cols(cols) {
    data.assign(rows * cols, initValue);
}
Matrix Matrix::zeroes(std::size_t rows, std::size_t cols) {
    return {rows, cols, 0.0};
}

Matrix Matrix::identity(std::size_t rows, std::size_t cols) {
    Matrix m(rows, cols);
    std::size_t n = std::min(rows, cols);
    for (std::size_t i = 0; i < n; ++i) {
        m(i, i) = 1.0;
    }
    return m;
}

Matrix Matrix::random(std::size_t rows, std::size_t cols) {
    Matrix m(rows, cols);
    for (std::size_t i = 0; i < rows; ++i) {
        for (std::size_t j = 0; j < cols; ++j) {
            m(i, j) = Random::uniform(-1.0, 1.0);
        }
    }
    return m;
}

Matrix Matrix::randomNormal(std::size_t rows, std::size_t cols) {
    // He inicijalizacija: stddev = sqrt(2 / fan_in), a fan_in je broj kolona.
    const double stddev = std::sqrt(2.0 / static_cast<double>(cols));

    Matrix m(rows, cols);
    for (std::size_t i = 0; i < rows; ++i) {
        for (std::size_t j = 0; j < cols; ++j) {
            m(i, j) = Random::normal(0.0, stddev);
        }
    }
    return m;
}

Matrix Matrix::operator+(const Matrix& other) const {
    if (rows != other.rows || cols != other.cols)
        throw std::invalid_argument("Matrix::operator+: dimenzije se ne slazu");
    Matrix result(rows, cols);

    for (std::size_t i = 0; i < rows; ++i) {
        for (std::size_t j = 0; j < cols; ++j) {
            result(i, j) = (*this)(i,j) + other(i,j);
        }
    }
    return result;
}

Matrix Matrix::operator-(const Matrix& other) const {
    if (rows != other.rows || cols != other.cols)
        throw std::invalid_argument("Matrix::operator-: dimenzije se ne slazu");
    Matrix result(rows, cols);
    for (std::size_t i = 0; i < rows; ++i) {
        for (std::size_t j = 0; j < cols; ++j) {
            result(i, j) = (*this)(i,j) - other(i,j);
        }
    }
    return result;
}

Matrix Matrix::operator*(const Matrix& other) const {
    if (cols != other.rows)
        throw std::invalid_argument("Matrix::operator*: dimenzije se ne slazu za mnozenje");
    Matrix result(rows, other.cols);
    for (std::size_t i = 0; i < rows; ++i) {
        for (std::size_t k = 0; k < cols; ++k) {
            double a_ik = (*this)(i, k);
            if (a_ik == 0.0) continue;
            for (std::size_t j = 0; j < other.cols; ++j) {
                result(i, j) += a_ik * other(k, j);
            }
        }
    }
    return result;
}

Matrix Matrix::operator*(double scalar) const {
    Matrix result(rows, cols);
    for (std::size_t i = 0; i < rows; ++i) {
        for (std::size_t j = 0; j < cols; ++j) {
            result(i, j) = (*this)(i, j) * scalar;
        }
    }
    return result;
}

Matrix& Matrix::operator+=(const Matrix& other) {
    if (rows != other.rows || cols != other.cols) {
        throw std::invalid_argument("Matrix::operator+=: dimenzije se ne slazu");
    }
    for (std::size_t i = 0; i < rows; ++i) {
        for (std::size_t j = 0; j < cols; ++j) {
            (*this)(i, j) += other(i, j);
        }
    }
    return *this;
}

Matrix& Matrix::operator*=(double scalar) {
    for (std::size_t i = 0; i < rows; ++i) {
        for (std::size_t j = 0; j < cols; ++j) {
            (*this)(i, j) *= scalar;
        }
    }
    return *this;
}

const double& Matrix::operator() (std::size_t row, std::size_t col) const {
    assert(row < rows && col < cols);
    return data[row * cols + col];
}

double& Matrix::operator() (std::size_t row, std::size_t col) {
    assert(row < rows && col < cols);
    return data[row * cols + col];
}

Matrix Matrix::hadamard(const Matrix& other) const {
    if (rows != other.rows || cols != other.cols)
        throw std::invalid_argument("Matrix::hadamard: dimenzije se ne slazu");
    Matrix result(rows, cols);
    for (std::size_t i = 0; i < rows; i++) {
        for (std::size_t j = 0; j < cols; j++) {
            result(i, j) = (*this)(i,j) * other(i,j);
        }
    }
    return result;
}

Matrix Matrix::apply(std::function<double(double)> f) const {
    Matrix result(rows, cols);
    for (std::size_t i = 0; i < rows; i++) {
        for (std::size_t j = 0; j < cols; j++) {
            result(i, j) = f((*this)(i, j));
        }
    }
    return result;
}

Matrix Matrix::transpose() const {
    Matrix result(cols, rows);
    for (std::size_t i = 0; i < rows; ++i) {
        for (std::size_t j = 0; j < cols; ++j) {
            result(j, i) = (*this)(i, j);
        }
    }
    return result;
}

double Matrix::sum() const {
    double result = 0.0;
    for (std::size_t i = 0; i < rows; ++i) {
        for (std::size_t j = 0; j < cols; ++j) {
            result += (*this)(i, j);
        }
    }
    return result;
}

double Matrix::mean() const {
    return sum() / static_cast<double>(size());
}

double Matrix::max() const {
    assert(rows > 0 && cols > 0);
    double result = (*this)(0, 0);
    for (std::size_t i = 0; i < rows; ++i) {
        for (std::size_t j = 0; j < cols; ++j) {
            if ((*this)(i, j) > result) {
                result = (*this)(i, j);
            }
        }
    }
    return result;
}

std::size_t Matrix::argmax() const {
    assert(rows > 0 && cols > 0);
    std::size_t bestIndex = 0;
    double bestValue = (*this)(0, 0);

    for (std::size_t i = 0; i < rows; ++i) {
        for (std::size_t j = 0; j < cols; ++j) {
            double value = (*this)(i, j);
            if (value > bestValue) {
                bestValue = value;
                bestIndex = i * cols + j; // flat indeks, konzistentno sa data layout-om
            }
        }
    }
    return bestIndex;
}

std::size_t Matrix::size() const {
    return (int) rows * cols;
}

std::size_t Matrix::getRows() const {
    return rows;
}

std::size_t Matrix::getCols() const {
    return cols;
}
