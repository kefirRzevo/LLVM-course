#pragma once

#include "GUILib.h"

#include <algorithm>
#include <array>
#include <iomanip>
#include <iostream>
#include <list>
#include <sstream>
#include <vector>

class CPU {
public:
  using reg_type = int64_t;
  using ptr_type = reg_type;
  using byte_type = int8_t;

  static const size_t reg_count = 32;

private:
  std::array<reg_type, reg_count> m_regs = {};
  std::vector<reg_type> m_stack = {};
  std::list<std::vector<byte_type>> m_mems = {};
  reg_type m_pc = {};
  bool m_is_exit = false;

public:
  reg_type get_reg(size_t indx) const { return m_regs.at(indx); }
  void set_reg(size_t indx, reg_type val) { m_regs.at(indx) = val; }

  reg_type pop_reg() {
    auto val = m_stack.back();
    m_stack.pop_back();
    return val;
  }
  void push_reg(reg_type val) { m_stack.push_back(val); }

  ptr_type get_pc() const { return m_pc; }
  void set_pc(ptr_type pc) { m_pc = pc; }

  bool get_is_exit() const { return m_is_exit; }
  void set_is_exit(bool is_exit) { m_is_exit = is_exit; }

  ptr_type create_mem(size_t size) {
    m_mems.emplace_back(size, byte_type{});
    return (ptr_type)m_mems.back().data();
  }
  void delete_mem(ptr_type ptr) {
    auto found = std::find_if(m_mems.begin(), m_mems.end(), [=](auto &&mem) {
      return (ptr_type)mem.data() == ptr;
    });
    if (found == m_mems.end())
      throw std::runtime_error("can not find ptr '" + std::to_string(ptr) +
                               "'");
    m_mems.erase(found);
  }

  std::string dump_regs() const {
    std::stringstream res;
    res << "registers\n";
    for (auto i = size_t{}; i != m_regs.size(); ++i)
      res << "x" << i << " " << m_regs[i] << "\n";
    return res.str();
  }

  std::string dump_mem(ptr_type ptr, size_t size) const {
    std::stringstream res;
    res << "mem at '" << ptr << "' with size '" << size << "'" << std::endl;
    res << std::hex << std::setfill('0');
    for (auto i = size_t{}; i != size; ++i)
      res << (int)*((char *)ptr + i);
    res << "\n";

    return res.str();
  }

  reg_type *regs_ptr() { return m_regs.data(); }
};
