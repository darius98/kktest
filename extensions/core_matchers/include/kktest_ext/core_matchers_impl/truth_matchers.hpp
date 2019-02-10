#ifndef KKTEST_EXTENSIONS_CORE_MATCHERS_KKTEST_EXT_CORE_MATCHERS_IMPL_TRUTH_MATCHERS_HPP_
#define KKTEST_EXTENSIONS_CORE_MATCHERS_KKTEST_EXT_CORE_MATCHERS_IMPL_TRUTH_MATCHERS_HPP_

#include <kktest_ext/core_matchers_impl/matcher.hpp>

namespace kktest {
namespace core_matchers {

class IsTrueMatcher: public Matcher {
 public:
    bool matches(const bool& object);

    void describe(Description* description);

    void describeMismatch(Description* description, const bool&);
};

extern IsTrueMatcher isTrue;

class IsFalseMatcher: public Matcher {
 public:
    bool matches(const bool& object);

    void describe(Description* description);

    void describeMismatch(Description* description, const bool&);
};

extern IsFalseMatcher isFalse;

}
}

#endif
