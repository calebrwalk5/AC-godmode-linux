#include <iostream>

#include <stdio.h>

#include <string.h>

#include <unistd.h>

#include <sys/ptrace.h>

#include <sys/wait.h>

#include <sys/user.h>

#include <sys/types.h>

#include <limits>

const int TARGET_VALUE = 100;
const int AMMO_VALUE = 20;

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
  while (addr < std::numeric_limits < long > ::max()) {
    errno = 0;
    long value = ptrace(PTRACE_PEEKDATA, pid, addr, NULL);
    if (value == -1 && errno) {
      tries++;
    } else {
      int32_t int_value = * ((int32_t * ) & value);
      if (int_value == TARGET_VALUE) {
        std::cout << "Found target value at address: 0x" << std::hex << addr << std::endl;
        break;
      }
    }
    addr += sizeof(int32_t);
  }
  std::cout << tries << " memory addresses scanned before finding the right one" << std::endl;
  ptrace(PTRACE_DETACH, pid, NULL, NULL);
  return 0;
}