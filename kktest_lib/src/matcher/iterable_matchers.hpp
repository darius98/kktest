#ifndef KKTEST_MATCHER_ITERABLE_MATCHERS_H_
#define KKTEST_MATCHER_ITERABLE_MATCHERS_H_

#include "comparison_matchers.hpp"
#include <core/matcher.hpp>


namespace kktest {

class IsEmptyMatcher: public Matcher {
public:
    template<class T>
    bool matches(const T& object) {
        return object.empty();
    }

    void describe(Description& description) override;

    void describeMismatch(Description& description) override;
};

class IsNotEmptyMatcher: public Matcher {
public:
    template<class T>
    bool matches(const T& object) {
        return !object.empty();
    }

    void describe(Description& description) override;

    void describeMismatch(Description& description) override;
};

template<class M, IS_MATCHER(M)>
class IterableSizeMatcher: public Matcher {
public:
    explicit IterableSizeMatcher(M* _sizeMatcher): sizeMatcher(_sizeMatcher) {}

    template<class T>
    bool matches(const T& object) {
        return sizeMatcher->matches(object.size());
    }

    void describe(Description& description) override {
        description << "iterable where size is ";
        sizeMatcher->describe(description);
    }

    void describeMismatch(Description& description) override {
        description << "iterable where size is ";
        sizeMatcher->describeMismatch(description);
    }
private:
    M* sizeMatcher;
};

template<class M, IS_MATCHER(M)>
class IterableEachMatcher: public Matcher {
public:
    explicit IterableEachMatcher(M* _elementMatcher):
            elementMatcher(_elementMatcher) {}

    template<class T>
    bool matches(const T& iterable) {
        index = -1;
        for (const auto& obj: iterable) {
            index += 1;
            if (!elementMatcher->matches(obj)) {
                elementMatcher->describeMismatch(elementFailureDescription);
                return false;
            }
        }
        return true;
    }

    void describe(Description& description) override {
        description << "an iterable where each element is ";
        elementMatcher->describe(description);
    }

    void describeMismatch(Description& description) override {
        description << "an iterable where at index "
                    << index
                    << " the element is "
                    << elementFailureDescription.toString();
    }

private:
    M* elementMatcher;
    Description elementFailureDescription;
    int index = -1;
};

template<class M, IS_MATCHER(M)>
class IterableAnyMatcher: public Matcher {
public:
    explicit IterableAnyMatcher(M* _elementMatcher):
            elementMatcher(_elementMatcher) {}

    template<class T>
    bool matches(const T& collection) {
        index = -1;
        for (const auto& obj: collection) {
            index += 1;
            if (elementMatcher->matches(obj)) {
                return true;
            }
        }
        return false;
    }

    void describe(Description& description) override {
        description << "an iterable where at least one element is ";
        elementMatcher->describe(description);
    }

    void describeMismatch(Description& description) override {
        description << "an iterable where no element is ";
        elementMatcher->describe(description);
    }
private:
    M* elementMatcher;
    int index = -1;
};

extern IsEmptyMatcher* isEmpty;

extern IsNotEmptyMatcher* isNotEmpty;

template<class M, IS_MATCHER(M)>
IterableSizeMatcher<M>* hasSize(M* sizeMatcher) {
    return new IterableSizeMatcher<M>(sizeMatcher);
}

template<class T>
IterableSizeMatcher<ComparisonMatcher<T>>* hasSize(const T& object) {
    return new IterableSizeMatcher<ComparisonMatcher<T>>(isEqualTo(object));
}

template<class M, IS_MATCHER(M)>
IterableEachMatcher<M>* eachElement(M* elementMatcher) {
    return new IterableEachMatcher<M>(elementMatcher);
}

template<class T>
IterableEachMatcher<ComparisonMatcher<T>>* eachElement(const T &object) {
    return new IterableEachMatcher<ComparisonMatcher<T>>(isEqualTo(object));
}

template<class M, IS_MATCHER(M)>
IterableAnyMatcher<M>* anyElement(M *elementMatcher) {
    return new IterableAnyMatcher<M>(elementMatcher);
}

template<class T>
IterableAnyMatcher<ComparisonMatcher<T>>* anyElement(const T &object) {
    return new IterableAnyMatcher<ComparisonMatcher<T>>(isEqualTo(object));
}

}

#endif
