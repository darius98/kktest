#pragma once

#include <kktest.hpp>
#include <kktest_ext/matchers.hpp>

#define EXPECT_MATCHER_FAILS(...)                       \
    try {                                               \
        kktest::matchers::expect(__VA_ARGS__);          \
        fail("expect(" #__VA_ARGS__ ") did not fail!"); \
    } catch (const std::exception&) {}

#define EXPECT_MATCHER_MATCHES(...)                  \
    try {                                            \
        kktest::matchers::expect(__VA_ARGS__);       \
    } catch (const std::exception&) {                \
        fail("expect(" #__VA_ARGS__ ") failed!");    \
    }
