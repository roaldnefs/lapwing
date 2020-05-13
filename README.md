# Lapwing

Lapwing is a shared library implementing a variety of C++ classes and templates extending the standard library. The library includes:

- File descriptor buffers.

## Example

The following defines a simple porgram using the file descriptor buffer to write a string to standard output:

```c++
#include <lapwing/ofdbuf.h>

#include <iostream>
#include <string>

int main(int argc, char** argv) {
  lapwing::OFdBuf fds(STDOUT_FILENO, 500);
  std::ostream os(&fds);

  os << "Hello, World!\n";
}
```

## Quickstart

Lapwing requires at least cmake 3.14 and the ability to compile C++20 code. The following shows how to compile and run the example:

```bash
$ cmake -Hexample -Bbuild/example
$ cmake --build build/example
$ ./build/example/example
```

## Installation and usage

Lapwing can be easily added to your project through [CPM.make](https://github.com/TheLartians/CPM.cmake).

```cmake
CPMAddPackage(
  NAME lapwing
  VERSION 0.0.1
  GIT_REPOSITORY https://github.com/roaldnefs/lapwing.git
)

target_link_libraries(myProject lapwing)
```
