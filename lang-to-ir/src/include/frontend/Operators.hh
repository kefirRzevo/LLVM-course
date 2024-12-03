#pragma once

#include <stdexcept>
#include <string>

namespace paracl {

enum class UnaryOpcode {
  UN_SUB,
  UN_ADD,
  UN_NOT,
};

inline std::string toString(UnaryOpcode op) {
  switch (op) {
  case UnaryOpcode::UN_SUB:
    return "-";
  case UnaryOpcode::UN_ADD:
    return "+";
  case UnaryOpcode::UN_NOT:
    return "!";
  default:
    throw std::runtime_error("Unknown unary opcode");
  }
}

enum class BinaryOpcode {
  BIN_MUL,
  BIN_DIV,
  BIN_MOD,

  BIN_ADD,
  BIN_SUB,

  BIN_SHL,
  BIN_SHR,

  BIN_L,
  BIN_G,
  BIN_LE,
  BIN_GE,

  BIN_EQ,
  BIN_NE,
  BIN_AND,
  BIN_OR,

  BIN_BIT_AND,
  BIN_BIT_OR,
  BIN_BIT_XOR,

  BIN_ASSIGN,
  BIN_COMMA,
};

inline std::string toString(BinaryOpcode op) {
  switch (op) {
  case BinaryOpcode::BIN_MUL:
    return "*";
  case BinaryOpcode::BIN_DIV:
    return "/";
  case BinaryOpcode::BIN_MOD:
    return "%";
  case BinaryOpcode::BIN_ADD:
    return "+";
  case BinaryOpcode::BIN_SUB:
    return "-";
  case BinaryOpcode::BIN_SHL:
    return "<<";
  case BinaryOpcode::BIN_SHR:
    return ">>";
  case BinaryOpcode::BIN_L:
    return "<";
  case BinaryOpcode::BIN_G:
    return ">";
  case BinaryOpcode::BIN_LE:
    return "<=";
  case BinaryOpcode::BIN_GE:
    return ">=";
  case BinaryOpcode::BIN_EQ:
    return "==";
  case BinaryOpcode::BIN_NE:
    return "!=";
  case BinaryOpcode::BIN_AND:
    return "&&";
  case BinaryOpcode::BIN_OR:
    return "||";
  case BinaryOpcode::BIN_BIT_AND:
    return "&";
  case BinaryOpcode::BIN_BIT_OR:
    return "|";
  case BinaryOpcode::BIN_BIT_XOR:
    return "^";
  case BinaryOpcode::BIN_ASSIGN:
    return "=";
  case BinaryOpcode::BIN_COMMA:
    return ",";
  default:
    throw std::runtime_error("Unknown binary opcode");
  }
}

inline constexpr bool isArithmetic(BinaryOpcode op) {
  switch (op) {
  case BinaryOpcode::BIN_MUL:
  case BinaryOpcode::BIN_DIV:
  case BinaryOpcode::BIN_MOD:
  case BinaryOpcode::BIN_ADD:
  case BinaryOpcode::BIN_SUB:
  case BinaryOpcode::BIN_SHL:
  case BinaryOpcode::BIN_SHR:
  case BinaryOpcode::BIN_L:
  case BinaryOpcode::BIN_G:
  case BinaryOpcode::BIN_LE:
  case BinaryOpcode::BIN_GE:
  case BinaryOpcode::BIN_EQ:
  case BinaryOpcode::BIN_NE:
  case BinaryOpcode::BIN_AND:
  case BinaryOpcode::BIN_OR:
  case BinaryOpcode::BIN_BIT_AND:
  case BinaryOpcode::BIN_BIT_OR:
  case BinaryOpcode::BIN_BIT_XOR:
    return true;
  default:
    return false;
  }
}

} // namespace paracl
