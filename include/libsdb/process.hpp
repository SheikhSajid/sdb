#ifndef SDB_PROCESS_HPP
#define SDB_PROCESS_HPP

#include <filesystem>
#include <memory>
#include <sys/types.h>

namespace sdb {
  /**
   * An `enum class` is a scoped and strongly typed enumeration introduced in C++11.
   * Differences from regular enums:
   * - Scoped: Enum values are accessed with the scope operator (e.g., `process_state::stopped`),
   *   avoiding name conflicts.
   * - Strongly typed: Enum values are not implicitly convertible to integers, ensuring type safety.
   * - Better type checking: Prevents unintended operations or comparisons with unrelated types.
   */
  enum class process_state {
    stopped,
    running,
    exited,
    terminated
  };

  struct stop_reason {
    stop_reason(int wait_status);

    process_state reason;
    std::uint8_t info;
  };

  class process {
    process() = delete; // delete default constructor
    process(const process&) = delete; // delete copy constructor
    process& operator=(const process&) = delete; // delete copy assignment operator

    public:
      ~process();

      // users should only be able to construct process objects through one of
      // these two static functions
      static std::unique_ptr<process> launch(
        std::filesystem::path path,
        bool debug = true
      );
      static std::unique_ptr<process> attach(pid_t pid);

      void resume();
      stop_reason wait_on_signal();
      pid_t pid() const { return pid_; }
      process_state state() const { return state_; }

    private:
      process(pid_t pid, bool terminate_on_end, bool is_attached)
              : pid_(pid),
                terminate_on_end_(terminate_on_end),
                is_attached_(is_attached) {}
      pid_t pid_ = 0;
      bool terminate_on_end_ = true;
      process_state state_ = process_state::stopped;
      bool is_attached_ = true;
  };
}

#endif
