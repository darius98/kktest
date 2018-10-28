#ifndef KKTEST_CORE_MACROS_H_
#define KKTEST_CORE_MACROS_H_

#include <utils/filename.hpp>

#include "definer.hpp"

/**
 * Create a test.
 *
 * Syntax: void test(std::string description, std::function<void()> testFunc);
 *
 * Example usage:
 * ```
 * test("!!1 == true", [&]() {
 *   expect(!!1);
 *   expectMatches(!!1, isTrue);
 * });
 * ```
 */
#define test kktest::TestDefiner(__FILENAME__, __LINE__)

/**
 * Create a test group.
 *
 * Syntax: void group(std::string description, std::function<void()> groupFunc);
 *
 * Example usage:
 * ```
 * group("!!(non-zero integers) == true", [&]() {
 *   test("!!1 == true", [&]() {
 *     expect(!!1);
 *     expectMatches(!!1, isTrue);
 *   }
 *
 *   test("!!2 == true", [&]() {
 *     expect(!!2);
 *     expectMatches(!!2, isTrue);
 *   }
 *
 *   for (int i = 100; i <= 200; ++ i) {
 *     test("!!" + std::to_string(i) + " == true", [&]() {
 *       expect(!!i);
 *       expectMatches(!!i, isTrue);
 *     });
 *   }
 * });
 * ```
 */
#define group kktest::GroupDefiner(__FILENAME__, __LINE__)

/**
 * Add a setUp code to be executed before every test inside the current scope.
 *
 * **Note**: only one setUp can exist per scope.
 */
#define setUp kktest::SetUpDefiner(__FILENAME__, __LINE__)

/**
 * Add a tearDown code to be executed after every test inside the current scope.
 *
 * **Note**: only one tearDown can exist per group.
 */
#define tearDown kktest::TearDownDefiner(__FILENAME__, __LINE__)

/**
 * Assertion macro.
 *
 * Use this to verify a boolean condition is satisfied. Fails the test, printing
 * the body of the boolean expression when it evaluates to false.
 */
#define expect(...) kktest::ExpectDefiner(__FILENAME__, __LINE__)              \
                                         (__VA_ARGS__, #__VA_ARGS__ " is false")

/**
 * Macro for forcing a test to fail. `expect` and `expectMatches` are
 * preferred where possible.
 */
#define failTest(...) kktest::ExpectDefiner(__FILENAME__, __LINE__)            \
                                           (false, __VA_ARGS__)

#endif
