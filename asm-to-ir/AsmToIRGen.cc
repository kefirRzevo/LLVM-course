#include "Builder.h"

int main(int argc, const char *argv[]) {
  try {
    if (argc != 2) {
      std::cerr << "Usage: " << argv[0] << " <1> path to asm" << std::endl;
      return 0;
    }
    Builder builder{argv[1]};
    auto status = builder.parse_file();
    if (!status)
      return 1;
    return builder.execute();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
}
