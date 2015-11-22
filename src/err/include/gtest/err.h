#pragma once

#include "gtest/gtest.h"

#define ASSERT_EHRET(code, expression) \
    ASSERT_EQ(code, expression);       \
    EH_RESET
