#ifndef KKTEST_CORE_MODULE_H_
#define KKTEST_CORE_MODULE_H_

namespace kktest {

class Module {
public:
    virtual bool isEnabled() const = 0;

    virtual void install() = 0;
};

}

#endif