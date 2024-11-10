#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include "Builder.h"

using namespace llvm;

bool Builder::parse_file() {
  std::string arg1;
  std::string arg2;
  std::string arg3;
  std::string arg4;

  std::ifstream is{m_filepath.data(), std::ios::in};
  std::vector<std::string> last_func_names;
  last_func_names.emplace_back("main");

  for (;;) {
    Insn::Attrs attrs{};
    std::string name;

    is >> name;
    if (name.empty())
      break;

    if (!name.substr(name.size() - 1, 1).compare(":")) {
      if (!name.substr(0, 1).compare(".")) {
        assert(name.size() > 2);
        std::string label_name = name.substr(1, name.size() - 2);
        std::string last_func = last_func_names.back();
        auto mangled = last_func + "_" + label_name;
        outs() << "label " << mangled << "\n";

        m_bb_map[mangled] =
            BasicBlock::Create(m_context, mangled, m_func_map[last_func]);
        attrs.m_label1 = mangled;
        m_insns.emplace_back(attrs, InsnType::label, mangled, nullptr);
        continue;
      } else {
        assert(name.size() > 1);
        std::string func_name = name.substr(0, name.size() - 1);
        outs() << "func_start " << func_name << "\n";

        FunctionType *void_type =
            FunctionType::get(m_builder.getVoidTy(), false);
        m_func_map[func_name] = Function::Create(
            void_type, Function::ExternalLinkage, func_name, m_module);
        auto mangled = "__" + func_name;
        m_bb_map[mangled] =
            BasicBlock::Create(m_context, mangled, m_func_map[func_name]);
        attrs.m_label1 = func_name;
        m_insns.emplace_back(attrs, InsnType::func_start, func_name, nullptr);
        last_func_names.push_back(func_name);
        continue;
      }
    } else if (!name.substr(0, 1).compare(":")) {
      assert(name.size() > 1);
      std::string func_name = name.substr(1, name.size() - 1);
      outs() << "func_end " << func_name << "\n";
      attrs.m_label1 = func_name;
      m_insns.emplace_back(attrs, InsnType::func_end, func_name, nullptr);
      last_func_names.pop_back();
      continue;
    }

    outs() << name << "\n";

    if (0) {
    }
#define DEF_INSN(insn_name, exec_cpu, decode, exec_ir)                         \
  else if (!name.compare(#insn_name)) {                                        \
    decode m_insns.emplace_back(attrs, InsnType::insn_name, name,              \
                                exec_##insn_name##_insn);                      \
  }
#include "Insns.def"
    else {
      m_insns.emplace_back(attrs, InsnType::unknown, name, nullptr);
    }

#undef DEF_INSN
  }
  return true;
}

bool Builder::execute() {

  FunctionType *insn_call_type =
      FunctionType::get(m_builder.getVoidTy(),
                        {m_builder.getPtrTy(), m_builder.getPtrTy()}, false);
  ArrayType *regfile_type =
      ArrayType::get(m_builder.getInt64Ty(), CPU::reg_count);

  Value *cpu_ptr = m_builder.getInt64((uint64_t)&m_cpu);

  if (m_insns.at(0).get_type() == InsnType::label)
    m_builder.SetInsertPoint(m_bb_map[m_insns.at(0).get_attrs().m_label1]);
  else
    m_builder.SetInsertPoint(m_bb_map["__start"]);

  std::vector<BasicBlock *> func_bb_stack;
  std::vector<std::string> func_names_stack;
  BasicBlock *bb_cur = m_builder.GetInsertBlock();

#define DEF_INSN(insn_name, exec_cpu, decode, exec_ir)                         \
  case InsnType::insn_name:                                                    \
    exec_ir break;

  for (auto &insn : m_insns) {
    InsnType type = insn.get_type();
    switch (type) {
    case InsnType::label: {
      BasicBlock *bb = m_bb_map[insn.get_attrs().m_label1];
      bb_cur = bb;
      m_builder.SetInsertPoint(bb);
      break;
    }
    case InsnType::func_start: {
      func_bb_stack.push_back(bb_cur);
      BasicBlock *bb = m_bb_map["__" + insn.get_attrs().m_label1];
      bb_cur = bb;
      m_builder.SetInsertPoint(bb);
      break;
    }
    case InsnType::func_end: {
      bb_cur = func_bb_stack.back();
      func_bb_stack.pop_back();
      m_builder.SetInsertPoint(bb_cur);
      break;
    }
#include "Insns.def"
    default: {
      Value *insn_ptr = m_builder.getInt64((uint64_t)&insn);
      m_builder.CreateCall(
          m_module->getOrInsertFunction(insn.get_name(), insn_call_type),
          {cpu_ptr, insn_ptr});
      break;
    }
    }
  }
#undef DEF_INSN
  outs() << "\n#[LLVM IR start]:\n";
  m_module->print(outs(), nullptr);
  outs() << "\n:#[LLVM IR end]\n";

  verifyFunction(*m_main_func);
  InitializeNativeTarget();
  InitializeNativeTargetAsmPrinter();
  InitializeNativeTargetAsmParser();

  ExecutionEngine *ee =
      EngineBuilder(std::unique_ptr<Module>(m_module)).create();
  if (ee == nullptr) {
    errs() << "error occured in EngineBuilder(...).create()\n";
    return false;
  }
  outs() << "EngineBuilder created\n";

  ee->InstallLazyFunctionCreator(Insn::lazy_function_creator);
  ee->addGlobalMapping(m_regfile, (void *)m_cpu.regs_ptr());
  ee->finalizeObject();

  gui_init();

  outs() << "\n#[Running code]\n";
  ee->runFunction(m_main_func, {});

  gui_exit();
  outs() << m_cpu.dump_regs();

  return true;
}
