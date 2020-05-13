#include <lapwing/ofdbuf.h>

#include <iostream>
#include <string>

int main(int argc, char** argv) {
  lapwing::OFdBuf fds(STDOUT_FILENO, 500);
  std::ostream os(&fds);

  os << "Hello, World!\n";
}