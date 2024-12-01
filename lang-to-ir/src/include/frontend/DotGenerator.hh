#pragma once

namespace paracl {

class TranslationUnit;

void generateDot(std::string_view filepath, TranslationUnit* root);

} // namespace paracl
