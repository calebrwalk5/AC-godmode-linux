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
  // Stuff for assembly
  const char message[] = "Done searching\n";
  int edi = 0;
  int esi = 0x7fffffff;
  int ecx = TARGET_VALUE;
  int edx = 0;
  int eax = 0;
  int ebx = 1;
  // Assembly
  __asm__(
    "movl %0, %%edi \n"
    "movl %1, %%esi \n"
    "mov %2, %%ecx \n"
    "movl %3, %%edx \n"
    "movl %4, %%eax \n"
    // Helped
    : // Output operands (none)
    : "m"(edi), "m"(esi), "r"(TARGET_VALUE), "m"(ecx), "m"(edx), "m"(eax), "m"(ebx), "r"(message) // Input operands
    : "memory" // Clobbered registers
  );

  ptrace(PTRACE_DETACH, pid, NULL, NULL);
  return 0;
}
