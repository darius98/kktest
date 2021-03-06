#pragma once

#include <mcga/cli.hpp>

#include "disallow_copy_and_move.hpp"
#include "hooks_manager.hpp"

namespace mcga::test {

class Extension {
  public:
    Extension() = default;

    MCGA_DISALLOW_COPY_AND_MOVE(Extension);

    virtual ~Extension() = default;

    virtual void registerCommandLineArgs(mcga::cli::Parser* /*parser*/) {
    }

    virtual void init(HooksManager* /*api*/) {
    }

    virtual void destroy() {
    }
};

}  // namespace mcga::test
