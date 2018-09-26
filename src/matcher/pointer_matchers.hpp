#ifndef RUNTIME_TESTING_MATCHER_POINTER_MATCHERS_H_
#define RUNTIME_TESTING_MATCHER_POINTER_MATCHERS_H_

#include "matcher.hpp"

namespace matcher {

class IsNullptrMatcher: public Matcher<void*> {
public:
    bool matches(void*const& obj) override;

    void describe(void*const& obj, Description* description) override;
};

class IsNotNullptrMatcher: public Matcher<void*> {
public:
    bool matches(void*const& obj) override;

    void describe(void*const& obj, Description* description) override;
};

}

#endif