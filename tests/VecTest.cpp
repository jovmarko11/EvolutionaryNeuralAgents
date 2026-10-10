#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "core/math/Vec2.h"

using Catch::Approx;

TEST_CASE("Vec2 length i normalizacija", "[vec2][math]") {
    Vec2 a(3, 4);

    SECTION("length je sqrt(x^2 + y^2)") {
        REQUIRE(a.length() == Approx(5.0));
    }

    SECTION("normalized ima jedinicnu duzinu") {
        REQUIRE(a.normalized().length() == Approx(1.0));
    }

    SECTION("nulti vektor se ne deli nulom") {
        Vec2 zero(0, 0);
        REQUIRE(zero.normalized().x == 0.0);
        REQUIRE(zero.normalized().y == 0.0);
    }
}

TEST_CASE("Vec2 dot i cross", "[vec2][math]") {
    Vec2 a(1, 0), b(0, 1);
    REQUIRE(a.dot(b) == Approx(0.0));
    REQUIRE(a.cross(b) == Approx(1.0));
}
