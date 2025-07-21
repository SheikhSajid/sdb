#include <editline/readline.h>
#include <vector>
#include <sys/types.h>
#include <string_view>
#include <sys/ptrace.h>
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#include <sstream>
#include <string.h>
#include <libsdb/process.hpp>
#include <signal.h>

namespace { // Anonymous namespace to limit scope
  void print_stop_reason(
    const sdb::process& process,
    sdb::stop_reason reason
  ) {
    std::cout << "Process " << process.pid() << ' ';

    switch (reason.reason) {
      case sdb::process_state::exited:
        std::cout << "exited with status "
                  << static_cast<int>(reason.info);
        break;
      case sdb::process_state::terminated:
        std::cout << "terminated with signal "
                  <<  sigabbrev_np(reason.info);
        break;
      case sdb::process_state::stopped:
        std::cout << "stopped with signal " << sigabbrev_np(reason.info);
        break;
    }

    std::cout << std::endl;
  }

  std::unique_ptr<sdb::process> attach(int argc, const char** argv) {
    /*
      If the user passes "-p <pid>", attach to the process with the given PID.
      Example: ./sdb -p 1234
        - "-p" indicates attach mode
        - "1234" is the PID of the process to attach to
      Otherwise, treat the argument as a program name to launch and debug.
      Example: ./sdb /bin/ls
        - "/bin/ls" is the program to launch under the debugger
    */
    if (argc == 3 && argv[1] == std::string_view("-p")) { // string_view (C++17)
      pid_t pid = std::atoi(argv[2]);
      return sdb::process::attach(pid);
    } else {
      const char* program_path = argv[1];
      return sdb::process::launch(program_path);
    }
  }

  std::vector<std::string> split(std::string_view str, char delimiter) {
    std::vector<std::string> out{};
    std::stringstream ss {std::string{str}};
    std::string item;

    while (std::getline(ss, item, delimiter)) {
      out.push_back(item);
    }

    return out;
  }

  bool is_prefix(std::string_view str, std::string_view of) {
    if (str.size() > of.size()) return false;
    return std::equal(str.begin(), str.end(), of.begin());
  }

  void handle_command(
    std::unique_ptr<sdb::process>& process,
    std::string_view line
  ) {
    auto args = split(line, ' ');
    auto command = args[0];

    if (is_prefix(command, "continue")) {
      process->resume();
      auto reason = process->wait_on_signal();
      print_stop_reason(*process, reason);
    } else {
      std::cerr << "Unknown command\n";
    }
  }
}

int main(int argc, const char** argv) {
  if (argc == 1) {
    std::cerr << "No arguments given" << std::endl;
    return -1;
  }

  std::unique_ptr<sdb::process> target_pid = attach(argc, argv);
  target_pid->wait_on_signal();

  char* line = nullptr;
  while ((line = readline("sdb> ")) != nullptr) {
    handle_command(target_pid, line);
    add_history(line);
    free(line);
  }

  return 0;
}
