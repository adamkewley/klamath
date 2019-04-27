#include <iostream>
#include <string>
#include <map>

#include "src/commands/dat2extract.hpp"
#include "src/commands/aafprint.hpp"
#include "src/commands/paldump.hpp"
#include "src/commands/frmshow.hpp"
#include "src/commands/frmheader.hpp"
#include "src/commands/frmdump.hpp"


namespace {
  typedef int (*subcommand)(int, char **);

  std::map<std::string, subcommand> subcommands {
    { "dat2extract", &klmth::dat2_extract_main },
    { "aafprint", &klmth::aaf_print_main },
    { "palprint", &klmth::paldump_main },
    { "frmshow", &klmth::frm_show_main },
    { "frmheader", &klmth::frmheader_main },
    { "frmdump", &klmth::frmdump_main },
  };
}

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cerr << "usage: " << argv[0] << " <command>" << std::endl
              << std::endl
              << "<command> can be one of:" << std::endl;

    for (const auto& subcommand : subcommands) {
      std::cerr << subcommand.first << std::endl;
    }

    return 1;
  }

  std::string subcommand(argv[1]);

  if (subcommands.find(subcommand) == subcommands.end()) {
    std::cerr << argv[0] << ": '" << subcommand << "' is not a valid command. Valid commands are:" << std::endl;

    for (const auto& subcommand : subcommands) {
      std::cerr << subcommand.first << std::endl;
    }

    return 1;
  }

  return subcommands[subcommand](argc - 1, argv + 1);
}

