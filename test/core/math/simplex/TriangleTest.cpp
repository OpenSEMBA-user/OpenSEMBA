
#include "gtest/gtest.h"
#include "math/simplex/Triangle.h"

#include <type_traits>

using namespace SEMBA;
using namespace Math;

template <typename T>
class MathSimplexTriangleTest : public ::testing::Test {

};

using test_types = ::testing::Types<
    std::integral_constant<std::size_t,2>,
    std::integral_constant<std::size_t,3>,
    std::integral_constant<std::size_t,5>,
    std::integral_constant<std::size_t,8>,
    std::integral_constant<std::size_t,12>>;

TYPED_TEST_CASE(MathSimplexTriangleTest, test_types);

TYPED_TEST(MathSimplexTriangleTest, BasicOperations) {
    static constexpr std::size_t n = TypeParam::value;
    Simplex::Triangle<n> tri;

    Real sum = 0.0;
    std::vector<Real> weights = tri.getWeights();
    for (size_t i = 0; i < weights.size(); ++i) {
        sum += weights[i];
    }
    EXPECT_NEAR(1.0, sum, 1e-8);
}
