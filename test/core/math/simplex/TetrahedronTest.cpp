
#include "gtest/gtest.h"
#include "math/simplex/Tetrahedron.h"

using namespace SEMBA;
using namespace Math;
#include <type_traits>

template <typename T>
class MathSimplexTetrahedronTest : public ::testing::Test {

};

using test_types = ::testing::Types<
    std::integral_constant<std::size_t,2>,
    std::integral_constant<std::size_t,3>,
    std::integral_constant<std::size_t,5>,
    std::integral_constant<std::size_t,8>,
    std::integral_constant<std::size_t,12>>;

TYPED_TEST_CASE(MathSimplexTetrahedronTest, test_types);

TYPED_TEST(MathSimplexTetrahedronTest, BasicOperations) {
    static constexpr std::size_t n = TypeParam::value;
    Simplex::Triangle<n> tri;

    Real sum = 0.0;
    std::vector<Real> weights = tri.getWeights();
    for (size_t i = 0; i < weights.size(); ++i) {
        sum += weights[i];
    }
    EXPECT_NEAR(1.0, sum, 1e-8);
}
