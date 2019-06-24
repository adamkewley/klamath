#include <iostream>
#include <string>
#include <map>

int cmd_aafprint(int argc, char** argv);
int cmd_dat2extract(int argc, char** argv);
int cmd_frmheader(int argc, char** argv);
int cmd_frmshow(int argc, char** argv);
int cmd_paldump(int argc, char** argv);

namespace {
  typedef int (*subcommand)(int, char **);

  std::map<std::string, subcommand> subcommands {
    { "dat2extract", &cmd_dat2extract },
    { "aafprint", &cmd_aafprint },
    { "palprint", &cmd_paldump },
    { "frmshow", &cmd_frmshow },
    { "frmheader", &cmd_frmheader },
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

