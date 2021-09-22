

#pragma once

#include "gtest/gtest.h"
#include "argument/Object.h"

class ObjectTest :
        public ::testing::Test,
        public ::testing::WithParamInterface<const char*> {

};





