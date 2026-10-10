//
// Created by Marko Jovanovic on 22. 9. 2026..
//


#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "../math/Matrix.h"

using Catch::Approx;

TEST_CASE("Matrix identity je neutralni element mnozenja", "[matrix][math]") {
    Matrix a = Matrix::random(4, 4);
    Matrix identity = Matrix::identity(4, 4);

    Matrix result = a * identity;

    for (std::size_t r = 0; r < 4; r++)
        for (std::size_t c = 0; c < 4; c++)
            CHECK(result(r, c) == Approx(a(r, c)));
}

TEST_CASE("Matrix transpose menja dimenzije", "[matrix][math]") {
    Matrix a(2, 5);
    Matrix t = a.transpose();

    REQUIRE(t.getRows() == 5);
    REQUIRE(t.getCols() == 2);
}