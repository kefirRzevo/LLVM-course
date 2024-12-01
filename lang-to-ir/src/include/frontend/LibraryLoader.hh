#pragma once

#include <vector>

#include "frontend/Nodes.hh"
#include "frontend/Driver.hh"
#include "frontend/Types.hh"
#include "frontend/SymTables.hh"

namespace paracl {

class LibraryLoader final {
	Driver& driver_;

public:
	LibraryLoader(Driver& driver):
	driver_(driver) {}

	void loadLibrary();
};

} // namespace paracl
