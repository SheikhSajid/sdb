#include <catch2/catch_test_macros.hpp>
#include <libsdb/process.hpp>
#include <sys/types.h>
#include <signal.h>
#include <libsdb/error.hpp>
#include <fstream>

using namespace sdb;

namespace {
  char get_process_status(pid_t pid) {
    std::ifstream stat("/proc/" + std::to_string(pid) + "/stat");
    std::string data;
    std::getline(stat, data);
    auto index_of_last_parenthesis = data.rfind(')');
    auto index_of_status_indicator = index_of_last_parenthesis + 2;

    return data[index_of_status_indicator];
  }

  bool process_exists(pid_t pid) {
    /**
     * Calling kill with a signal of 0 doesn’t send a signal to the process
     * but still carries out the existence and permission checks.
     */
    auto ret = kill(pid, /*signal=*/0);

    return ret != -1 and errno != ESRCH;
  }
}

TEST_CASE("process::launch success", "[process]") {
  auto proc = process::launch("yes");
  REQUIRE(process_exists(proc->pid()));
}

TEST_CASE("process::launch no such program", "[process]") {
  REQUIRE_THROWS_AS(process::launch("you_do_not_have_to_be_good"), error);
}

TEST_CASE("process::attach to existing success", "[process]") {
  auto target = process::launch("/root/sdb/build/test/targets/run_endlessly", /*debug=*/false); // TODO: make this work with relative paths
  auto proc = process::attach(target->pid());
  REQUIRE(get_process_status(target->pid()) == 't');
}

TEST_CASE("process::attach invalid PID", "[process]") {
  REQUIRE_THROWS_AS(process::attach(0), error);
}

TEST_CASE("process::resume success", "[process]") {
  // we create blocks to scope names within the same test case while avoiding collisions
  {
    auto proc = process::launch("/root/sdb/build/test/targets/run_endlessly");
    proc->resume();
    auto status = get_process_status(proc->pid());
    auto success = status == 'R' or status == 'S';
    REQUIRE(success);
  }

  {
    auto target = process::launch("/root/sdb/build/test/targets/run_endlessly", /*debug=*/false);
    auto proc = process::attach(target->pid());
    proc->resume();
    auto status = get_process_status(proc->pid());
    auto success = status == 'R' or status == 'S';
    REQUIRE(success);
  }
}

TEST_CASE("process::resume already terminated", "[process]") {
  auto proc = process::launch("/root/sdb/build/test/targets/end_immediately");
  proc->resume();
  proc->wait_on_signal();
  REQUIRE_THROWS_AS(proc->resume(), error);
}
