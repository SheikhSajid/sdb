#include <iostream>
#include <libsdb/libsdb.hpp> // Header files in the public `include/libsdb` directory should always be included using angle brackets

void sdb::say_hello() {
  std::cout << "Hello from libsdb!" << std::endl;
}
