#include <iostream>

#include <stdio.h>

#include <string.h>

#include <unistd.h>

#include <sys/ptrace.h>

#include <sys/wait.h>

#include <sys/user.h>

#include <sys/types.h>

#include <limits>

constexpr int TARGET_VALUE = 100;

long loadMemory(pid_t pid, long addr) {
  std::cout << "Loading memory" << std::endl;
  long data = ptrace(PTRACE_PEEKDATA, pid, addr, NULL);
  return data;
}
int main() {
  std::cout << "Written by anvsO1 (https://www.unknowncheats.me/forum/members/5175763.html)" << std::endl;

  // Get the window ID of the Assault Cube window
  char buf[256];
  FILE * xdotool = popen("xdotool search --name 'AssaultCube'", "r");
  if (!xdotool) {
    std::cout << "Please install xdotool" << std::endl;
    return 1;
  } else {
    std::cout << "xdotool found" << std::endl;
  }
  if (!fgets(buf, sizeof(buf), xdotool)) {
    std::cout << "Could not find Assault Cube window" << std::endl;
    return 1;
  }
  std::cout << "fgets called" << std::endl;
  std::cout << "buf: " << buf << std::endl;
  int window_id = atoi(buf);
  std::cout << "window_id: " << window_id << std::endl;

  // Get the PID of the ac_client process
  snprintf(buf, sizeof(buf), "xdotool getwindowpid %d", window_id);
  xdotool = popen(buf, "r");
  if (!xdotool) {
    perror("popen");
    return 1;
  }
  if (!fgets(buf, sizeof(buf), xdotool)) {
    std::cout << "Could not get PID of Assault Cube process" << std::endl;
    return 1;
  }
  pid_t pid = atoi(buf);

  // Attach to the ac_client process
  if (ptrace(PTRACE_ATTACH, pid, NULL, NULL) == -1) {
    perror("ptrace attach");
    return 1;
  }
  wait(NULL);
  std::cout << "waiting NULL" << std::endl;
  struct user_regs_struct regs;

  long addr = 0;
  long int tries = 0;
  std::cout << "we're gonna scan all of virtual memory, this will take some time" << std::endl;
  // Search virtual memory for TARGET_VALUE
  bool found = base;
  long data = 0;
  while (addr < std::numeric_limits < long > ::max()) {
    data = loadMemory(pid, addr);
    if (data == -1) {
      break;
      std::cout << "error loading memory\n";
    }
    for (int i = 0; i < sizeof(long); i++) {
      if (((char * ) & data)[i] == TARGET_VALUE) {
        std::cout << "Found target value at address: " << addr + i << std::endl;
        found = true;
      }
    }
    addr += sizeof(long);
  }

  if (!found) {
    std::cout << "Could not find target value in virtual memory" << std::endl;
  }

  // Detach from the process
  ptrace(PTRACE_DETACH, pid, NULL, NULL);
  return 0;
}