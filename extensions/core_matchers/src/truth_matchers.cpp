#include <kktest_ext/core_matchers_impl/truth_matchers.hpp>

namespace kktest {
namespace core_matchers {

detail::IsTrueMatcher isTrue;

detail::IsFalseMatcher isFalse;

namespace detail {

bool IsTrueMatcher::matches(const bool& object) {
    return object;
}

void IsTrueMatcher::describe(Description* description) {
    (*description) << "true";
}

void IsTrueMatcher::describeMismatch(Description* description, const bool&) {
    (*description) << "false";
}

bool IsFalseMatcher::matches(const bool& object) {
    return !object;
}

void IsFalseMatcher::describe(Description* description) {
    (*description) << "false";
}

void IsFalseMatcher::describeMismatch(Description* description, const bool&) {
    (*description) << "true";
}

}
}
}
