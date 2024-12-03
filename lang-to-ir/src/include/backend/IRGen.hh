#pragma once

#include <string_view>

namespace paracl {

class TranslationUnit;

void execute(std::string_view filepath, TranslationUnit* root);

} // namespace paracl
