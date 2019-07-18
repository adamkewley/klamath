#include <iostream>
#include <string>
#include <map>

int cmd_aafprint(int argc, char** argv);
int cmd_dat2extract(int argc, char** argv);
int cmd_frmheader(int argc, char** argv);
int cmd_frmshow(int argc, char** argv);
int cmd_paldump(int argc, char** argv);
int cmd_mapheader(int argc, char** argv);
int cmd_mapglobals(int argc, char** argv);
int cmd_maplocals(int argc, char** argv);
int cmd_maptiles(int argc, char** argv);
int cmd_lststrip(int argc, char** argv);
int cmd_mapshow(int argc, char** argv);

namespace {
  typedef int (*subcommand)(int, char **);

  std::map<std::string, subcommand> subcommands {
    { "dat2extract", &cmd_dat2extract },
    { "aafprint", &cmd_aafprint },
    { "palprint", &cmd_paldump },
    { "frmshow", &cmd_frmshow },
    { "frmheader", &cmd_frmheader },
    { "mapheader", &cmd_mapheader },
    { "mapglobals", &cmd_mapglobals },
    { "maplocals", &cmd_maplocals },
    { "maptiles", &cmd_maptiles },
    { "lststrip", &cmd_lststrip },
    { "mapshow", &cmd_mapshow },
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

