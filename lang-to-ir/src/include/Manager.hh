#pragma once

#include <boost/program_options.hpp>
#include <filesystem>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>

#include "frontend/Driver.hh"

namespace po = boost::program_options;

namespace paracl {

class Manager final {
private:
  std::optional<std::string> inputFile_;
  std::optional<std::string> dotFile_;
  bool help_ = false;

  po::options_description cmdline_{"Options"};
  po::options_description visible_{"Allowed"};
  po::positional_options_description pos_;

  void initProgramOptions() {
    po::options_description generic("Generic options");
    generic.add_options()("help,h", "help message");

    po::options_description config("Configuration");
    config.add_options()("dot", po::value<std::string>()->implicit_value(""),
                         "ast dot dump");

    po::options_description hidden("Hidden options");
    hidden.add_options()("input", po::value<std::string>()->required(),
                         "input file");

    visible_.add(generic).add(config);
    cmdline_.add(generic).add(config).add(hidden);
    pos_.add("input", 1);
  }

  void parseProgramOptions(int argc, const char *argv[]) {
    po::variables_map vm;
    try {
      auto parser = po::command_line_parser(argc, argv);
      po::store(parser.options(cmdline_).positional(pos_).run(), vm);
      po::notify(vm);

      if (vm.count("help")) {
        std::cout << visible_ << std::endl;
        help_ = true;
        return;
      }
      std::filesystem::path fullpath;
      if (vm.count("input")) {
        auto input = vm["input"].as<std::string>();
        fullpath = input;
        inputFile_.emplace(input);
      }
      fullpath.replace_extension();
      if (vm.count("dot")) {
        auto dump = vm["dot"].as<std::string>();
        if (dump == "") {
          dump = fullpath.generic_string().append(".dot");
        }
        dotFile_.emplace(dump);
      }
    } catch (const po::required_option &e) {
      if (vm.count("help")) {
        std::cout << visible_ << std::endl;
        help_ = true;
        return;
      } else {
        throw;
      }
    }
  }

  void process() {
    if (help_) {
      return;
    }
    Driver driver;
    driver.setFilepath(inputFile_.value());
    driver.parse();
    //driver.analyze(driver.getRoot());
    if (driver.hasErrors()) {
      driver.reportAllErrors(std::cerr);
      return;
    }
    if (dotFile_.has_value()) {
      driver.generateDot(dotFile_.value());
    }
    // if (clFile_.has_value()) {
    //   driver.generateCl(clFile_.value());
    // }
    // if (cppFile_.has_value()) {
    //   driver.generateCpp(cppFile_.value());
    // }

    // NodeCodegen codegener;
    // auto im = codegener.codegen(driver.getRoot());
    // if (!im.has_value()) {
    //   throw std::runtime_error("Couldn't get Image");
    // }
    // if (disasmFile_.has_value()) {
    //   im->disassemble(disasmFile_.value());
    // }
    // VirtualMachine m;
    // m.loadImage(std::move(im.value()));
    // m.execute();
  }

public:
  void run(int argc, const char *argv[]) {
    initProgramOptions();
    parseProgramOptions(argc, argv);
    process();
  }
};

} // namespace paracl