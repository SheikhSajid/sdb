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
#include <string>

namespace { // Anonymous namespace to limit scope
  // TODO: Move this to libsdb
  pid_t attach(int argc, const char** argv) {
    pid_t pid = 0;
    
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
      pid = std::atoi(argv[2]);
      
      if (pid <= 0) {
        std::cerr << "Invalid pid\n";
        return -1;
      }

      if (ptrace(PTRACE_ATTACH, pid, /*addr=*/nullptr, /*data=*/nullptr) < 0) {
        std::perror("Could not attach");
        return -1;
      }
    } else {
      const char* program_path = argv[1];
      if ((pid = fork()) < 0) {
        std::perror("fork failed");
        return -1;
      }

      if (pid == 0) { 
        // In child process (tracee)
        if (ptrace(PTRACE_TRACEME, 0, nullptr, nullptr) < 0) {
          std::perror("Tracing failed");
          return -1;
        }

        if (execlp(program_path, program_path, nullptr) < 0) {
          std::perror("Exec failed");
          return -1;
        }
      }
    }

    return pid;
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

  void resume(pid_t pid) {
    if (ptrace(PTRACE_CONT, pid, nullptr, nullptr) < 0) {
      std::cerr << "Couldn't continue\n";
      std::exit(-1);
    }
  }

  void wait_on_signal(pid_t pid) {
    int wait_status;
    int options = 0;
    if (waitpid(pid, &wait_status, options) < 0) {
      std::perror("waitpid failed");
      std::exit(-1);
    }
  }

  void handle_command(pid_t pid, std::string_view line) {
    auto args = split(line, ' ');
    auto command = args[0];

    if (is_prefix(command, "continue")) {
      resume(pid);
      wait_on_signal(pid);
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

  pid_t target_pid = attach(argc, argv);
  int wait_status;
  int options = 0;
  if (waitpid(target_pid, &wait_status, options) < 0) {
    std::perror("waitpid failed");
  }

  char* line = nullptr;
  while ((line = readline("sdb> ")) != nullptr) {
    handle_command(target_pid, line);
    add_history(line);
    free(line);
  }

  return 0;
}
