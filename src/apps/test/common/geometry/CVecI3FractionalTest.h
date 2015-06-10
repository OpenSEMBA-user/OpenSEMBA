#include "gtest/gtest.h"
#include "geometry/CVecI3Fractional.h"

TEST(GeometryCVecI3FractionalTest, Basic) {
    CVecI3Frac aux(CVecI3(3),CVecR3(0.4));
    EXPECT_EQ(aux, aux);
    CVecI3Frac auxCpy(aux);
    EXPECT_EQ(aux, auxCpy);
    auxCpy = aux;
    EXPECT_EQ(aux, auxCpy);
}

