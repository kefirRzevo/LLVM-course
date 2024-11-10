#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"

#include "CPU.h"
#include "Insn.h"
#include <fstream>
#include <iostream>
#include <unordered_map>

class Builder {
  std::string_view m_filepath = "";
  CPU m_cpu = CPU{};
  std::vector<Insn> m_insns = {};
  std::unordered_map<std::string, llvm::BasicBlock *> m_bb_map;
  std::unordered_map<std::string, llvm::Function *> m_func_map;

  llvm::LLVMContext m_context;
  llvm::Module *m_module;
  llvm::IRBuilder<> m_builder;

  llvm::GlobalVariable *m_regfile;
  llvm::Function *m_main_func;

public:
  Builder(std::string_view filepath)
      : m_filepath{filepath}, m_cpu{CPU{}}, m_insns{}, m_context{},
        m_module{new llvm::Module{filepath, m_context}},
        m_builder{llvm::IRBuilder<>{m_context}} {
    llvm::FunctionType *void_type =
        llvm::FunctionType::get(m_builder.getVoidTy(), false);
    llvm::ArrayType *regfile_type =
        llvm::ArrayType::get(m_builder.getInt64Ty(), CPU::reg_count);
    m_main_func = llvm::Function::Create(
        void_type, llvm::Function::ExternalLinkage, "main", m_module);
    m_func_map["main"] = m_main_func;
    m_bb_map["__start"] =
        llvm::BasicBlock::Create(m_context, "__start", m_main_func);
    m_module->getOrInsertGlobal("regfile", regfile_type);
    m_regfile = m_module->getNamedGlobal("regfile");
  }

  bool parse_file();

  bool execute();
};
