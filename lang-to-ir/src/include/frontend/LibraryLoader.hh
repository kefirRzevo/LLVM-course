#pragma once

#include <vector>

#include "frontend/Driver.hh"
#include "frontend/Nodes.hh"
#include "frontend/SymTables.hh"
#include "frontend/Types.hh"

namespace paracl {

class LibraryLoader final {
  Driver &driver_;

public:
  LibraryLoader(Driver &driver) : driver_(driver) {}

  void loadLibrary();
};

} // namespace paracl
