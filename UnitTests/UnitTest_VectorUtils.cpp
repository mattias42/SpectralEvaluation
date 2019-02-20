#include "catch.hpp"
#include <VectorUtils.h>

TEST_CASE("Min", "[Min][VectorUtils]")
{
    SECTION("Expected value")
    {
        std::vector<double> values{ 2, 3, 4, 5, 6, 7, 8, 9, 1, 8, 7, 6, 5, 4, 3 };

        size_t minIdx = 0U;
        const double result = Min(values, minIdx);

        REQUIRE(minIdx == 8);
        REQUIRE(result == Approx(1.0));
    }

    SECTION("Empty vector returns zero")
    {
        std::vector<double> values;

        size_t minIdx = 1U;
        const double result = Min(values, minIdx);

        REQUIRE(minIdx == 0);
        REQUIRE(result == Approx(0.0));
    }
}

TEST_CASE("Max", "[Max][VectorUtils]")
{
    SECTION("Expected value")
    {
        std::vector<double> values{ 2, 3, 4, 5, 6, 7, 8, 9, 1, 8, 7, 6, 5, 4, 3 };

        size_t maxIdx = 0U;
        const double result = Max(values, maxIdx);

        REQUIRE(maxIdx == 7);
        REQUIRE(result == Approx(9.0));
    }

    SECTION("Empty vector returns zero")
    {
        std::vector<double> values;

        size_t maxIdx = 1U;
        const double result = Max(values, maxIdx);

        REQUIRE(maxIdx == 0);
        REQUIRE(result == Approx(0.0));
    }
}

TEST_CASE("Normalize", "[Normalize][VectorUtils]")
{
    std::vector<double> values { 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3};
    std::vector<double> result;

    Normalize(values, result);

    SECTION("Result has correct length")
    {
        REQUIRE(result.size() == values.size());
    }

    SECTION("Minimum value is zero")
    {
        REQUIRE(0.0 == result[0]);
        REQUIRE(0.0 == Min(result));
    }

    SECTION("Maximum value is one")
    {
        REQUIRE(1.0 == result[7]);
        REQUIRE(1.0 == Max(result));
    }
}

TEST_CASE("Normalize with empty input", "[Normalize][VectorUtils]")
{
    std::vector<double> input, result;

    Normalize(input, result);

    REQUIRE(0 == result.size());
}



TEST_CASE("FindValue - Constantly increasing vector", "[FindValue][VectorUtils]")
{
    const std::vector<double> values{ 1, 2, 3, 4 };

    SECTION("Finds values at integer index points.")
    {
        REQUIRE(0.0 == Approx(FindValue(values, 1.0, 0, 4)));
        REQUIRE(2.0 == Approx(FindValue(values, 3.0, 0, 4)));
        REQUIRE(3.0 == Approx(FindValue(values, 4.0, 0, 4)));
    }

    SECTION("Finds values midway between points.")
    {
        REQUIRE(0.5 == Approx(FindValue(values, 1.5, 0, 4)));
        REQUIRE(2.5 == Approx(FindValue(values, 3.5, 0, 4)));
    }

    SECTION("Finds values quarter between points.")
    {
        REQUIRE(0.25 == Approx(FindValue(values, 1.25, 0, 4)));
        REQUIRE(2.25 == Approx(FindValue(values, 3.25, 0, 4)));
    }

    SECTION("Finds values three-quarters between points.")
    {
        REQUIRE(0.75 == Approx(FindValue(values, 1.75, 0, 4)));
        REQUIRE(2.75 == Approx(FindValue(values, 3.75, 0, 4)));
    }
}

TEST_CASE("FindValue - Constantly decreasing vector", "[FindValue][VectorUtils]")
{
    const std::vector<double> values{ 4, 3, 2, 1 };

    SECTION("Finds values at integer index points.")
    {
        REQUIRE(0.0 == Approx(FindValue(values, 4.0, 0, 4)));
        REQUIRE(2.0 == Approx(FindValue(values, 2.0, 0, 4)));
        REQUIRE(3.0 == Approx(FindValue(values, 1.0, 0, 4)));
    }

    SECTION("Finds values midway between points.")
    {
        REQUIRE(2.5 == Approx(FindValue(values, 1.5, 0, 4)));
        REQUIRE(0.5 == Approx(FindValue(values, 3.5, 0, 4)));
    }

    SECTION("Finds values quarter between points.")
    {
        REQUIRE(2.25 == Approx(FindValue(values, 1.75, 0, 4)));
        REQUIRE(0.25 == Approx(FindValue(values, 3.75, 0, 4)));
    }

    SECTION("Finds values three-quarters between points.")
    {
        REQUIRE(2.75 == Approx(FindValue(values, 1.25, 0, 4)));
        REQUIRE(0.75 == Approx(FindValue(values, 3.25, 0, 4)));
    }
}

TEST_CASE("FindValue - invalid ranges", "[FindValue][VectorUtils]")
{
    const std::vector<double> values{ 4, 3, 2, 1 };

    SECTION("Startidx above StopIdx.")
    {
        REQUIRE(-1.0 == Approx(FindValue(values, 4.0, 3, 1)));
    }

    SECTION("Startidx out of bounds.")
    {
        REQUIRE(-1.0 == Approx(FindValue(values, 4.0, 6, 9)));
    }
}


TEST_CASE("GetAt", "[GetAt][VectorUtils]")
{
    const std::vector<double> values{ 1, 2, 3, 4 };

    SECTION("Finds values at integer index points.")
    {
        REQUIRE(1.0 == Approx(GetAt(values, 0.0)));
        REQUIRE(3.0 == Approx(GetAt(values, 2.0)));
        REQUIRE(4.0 == Approx(GetAt(values, 3.0)));
    }

    SECTION("Finds values at quarter index points.")
    {
        REQUIRE(1.25 == Approx(GetAt(values, 0.25)));
        REQUIRE(3.25 == Approx(GetAt(values, 2.25)));
    }
}