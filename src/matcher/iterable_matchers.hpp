#ifndef RUNTIME_TESTING_MATCHER_ITERABLE_MATCHERS_H_
#define RUNTIME_TESTING_MATCHER_ITERABLE_MATCHERS_H_

#include "matcher.hpp"

namespace matcher {

template<class ElementMatcher, IS_MATCHER(ElementMatcher)>
class IterableEachMatcher: public BaseMatcher {
public:
    explicit IterableEachMatcher(ElementMatcher* elementMatcher):
            elementMatcher(elementMatcher) {}

    template<class T>
    bool matches(const T& iterable) {
        this->index = -1;
        for (const auto& obj: iterable) {
            this->index += 1;
            if (!this->elementMatcher->matches(obj)) {
                return false;
            }
        }
        return true;
    }

    void describeExpectation(Description* description) {
        description->append("an iterable where each element is ");
        this->elementMatcher->describeExpectation(description);
    }

    template<class T>
    void describeFailure(const T& iterable, Description* description) {
        description->append(
            "an iterable where at index ",
            this->index,
            " the element is "
        );
        for (const auto& obj: iterable) {
            if (!this->elementMatcher->matches(obj)) {
                this->elementMatcher->describeFailure(obj, description);
                return;
            }
        }
    }

    template<class T>
    void describeSuccess(const T& iterable, Description* description) {
        description->append("an iterable where each element is ");
        this->elementMatcher->describeExpectation(description);
    }

private:
    ElementMatcher* elementMatcher;
    int index = -1;
};

template<class ElementMatcher, IS_MATCHER(ElementMatcher)>
class IterableAnyMatcher: public BaseMatcher {
public:
    explicit IterableAnyMatcher(ElementMatcher* elementMatcher):
            elementMatcher(elementMatcher) {}

    template<class T>
    bool matches(const T& collection) {
        this->index = -1;
        for (const auto& obj: collection) {
            this->index += 1;
            if (this->elementMatcher->matches(obj)) {
                return true;
            }
        }
        return false;
    }

    void describeExpectation(Description* description) {
        description->append(
            "an iterable where at least one element is "
        );
        this->elementMatcher->describeExpectation(description);
    }

    template<class T>
    void describeSuccess(const T& iterable, Description* description) {
        description->append(
            "an iterable where at index ",
            this->index,
            " the element is "
        );
        this->elementMatcher->describeExpectation(description);
    }

    template<class T>
    void describeFailure(const T& iterable, Description* description) {
        description->append("an iterable where no element is ");
        this->elementMatcher->describeExpectation(description);
    }
private:
    ElementMatcher* elementMatcher;
    int index = -1;
};

}

#endif
