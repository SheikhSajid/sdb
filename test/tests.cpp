#include <catch2/catch_test_macros.hpp>
#include <libsdb/process.hpp>
#include <sys/types.h>
#include <signal.h>
#include <libsdb/error.hpp>

using namespace sdb;

namespace {
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
