#include <cassert>

#include "Insn.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

#define DEF_INSN(insn_name, exec_cpu, decode, exec_ir)                         \
  if (!name.compare(#insn_name))                                               \
    return reinterpret_cast<void *>(exec_##insn_name##_insn);

void *Insn::lazy_function_creator(const std::string &name) {
#include "Insns.def"

  outs() << "can not found '" << name << "'\n";
  return nullptr;
}

#undef DEF_INSN
