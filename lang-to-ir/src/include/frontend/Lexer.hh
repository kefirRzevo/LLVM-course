#pragma once

#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#undef YY_DECL
#define YY_DECL paracl::Parser::symbol_type paracl::Lexer::getNextToken()

#include <fstream>
#include <memory>
#include <string>

#include "location.hh"
#include "parser.tab.hh"

namespace paracl {

class Driver;

class Lexer final : public yyFlexLexer {
private:
  Driver &driver_;
  std::string filepath_;
  std::ifstream in_;
  position curPos_ = position{};

public:
  Lexer(Driver &driver) : driver_(driver) {}

  void setFilepath(std::string_view filepath) {
    filepath_ = filepath;
    in_.open(filepath_, std::ifstream::in);
    curPos_.initialize(std::addressof(filepath_));
    yyrestart(in_);
  }

  Parser::symbol_type getNextToken();

  location updateLocation() {
    auto old_ = curPos_;
    curPos_ += yyleng;
    return location{old_, curPos_};
  }
};

} // namespace paracl
