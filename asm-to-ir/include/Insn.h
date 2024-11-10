#pragma once

#include "CPU.h"

#define DEF_INSN(insn_name, exec_cpu, decode, exec_ir) insn_name,

enum class InsnType {
#include "Insns.def"

  // pseudo insns

  label,
  func_start,
  func_end,
  unknown,
};

#undef DEF_INSN

class Insn {
public:
  using reg_type = CPU::reg_type;
  using ptr_type = CPU::ptr_type;
  using byte_type = CPU::byte_type;

  using insn_callback = void (*)(CPU &, const Insn &);

  struct Attrs {
    reg_type m_rd = {};
    reg_type m_rs1 = {};
    reg_type m_rs2 = {};
    reg_type m_rs3 = {};

    std::string m_label1 = "";
    std::string m_label2 = "";

    reg_type m_imm = {};
  };

private:
  Attrs m_attrs = Attrs{};
  InsnType m_type = InsnType::unknown;
  std::string m_name = "";
  insn_callback m_callback = nullptr;

public:
  Insn(const Attrs &attrs, InsnType type, std::string_view name,
       insn_callback callback)
      : m_attrs(attrs), m_type(type), m_name(name), m_callback(callback) {}

  const Attrs &get_attrs() const { return m_attrs; }
  InsnType get_type() const { return m_type; }
  const std::string &get_name() const { return m_name; }
  insn_callback get_callback() const { return m_callback; }

  void execute(CPU &cpu) const { m_callback(cpu, *this); }

  static void *lazy_function_creator(const std::string &name);
};

#define DEF_INSN(insn_name, exec_cpu, decode, exec_ir)                         \
  inline void exec_##insn_name##_insn(CPU &cpu, const Insn &insn) exec_cpu

#include "Insns.def"

#undef DEF_INSN
