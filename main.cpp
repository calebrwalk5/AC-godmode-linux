#include <iostream>

#include <stdio.h>

#include <string.h>

#include <unistd.h>

#include <sys/ptrace.h>

#include <sys/wait.h>

#include <sys/user.h>

#include <sys/types.h>

#include <limits>

#include <cmath>  // for ceil function

constexpr int TARGET_VALUE = 100;

long loadMemory(pid_t pid, long addr) {
    long data;
    __asm__ __volatile__ (
        "mov $0, %%rax\n"
        "mov %1, %%rdi\n"
        "mov %2, %%rsi\n"
        "mov %3, %%rdx\n"
        "syscall"
        : "=r" (data)  // output
        : "r" (pid), "rm" (addr), "r" (NULL), "r" (PTRACE_PEEKDATA)  // input
        : "%rdi", "%rsi", "%rdx", "%rax"  // clobbered registers
    );
    return data;
}

int main() {
  std::cout << "Written by anvsO1 (https://www.unknowncheats.me/forum/members/5175763.html)" << std::endl;

  // Get the window ID of the Assault Cube window
  char buf[256];
  FILE * xdotool = popen("xdotool search --name 'AssaultCube'", "r");
  if (!xdotool) {
    std::cout << "popen failed: " << strerror(errno) << std::endl;
    return 1;
  } else {
    std::cout << "xdotool found" << std::endl;
  }
  if (!fgets(buf, sizeof(buf), xdotool)) {
    std::cout << "fgets failed: " << strerror(errno) << std::endl;
    std::cout << "Could not find Assault Cube window" << std::endl;
    return 1;
  }
  std::cout << "fgets called" << std::endl;
  std::cout << "buf: " << buf << std::endl;
  int window_id = atoi(buf);
  if (window_id == 0) {
    std::cout << "atoi failed to parse window ID" << std::endl;
    return 1;
  }
  std::cout << "window_id: " << window_id << std::endl;

  // Get the PID of the ac_client process
  snprintf(buf, sizeof(buf), "xdotool getwindowpid %d", window_id);
  xdotool = popen(buf, "r");
  if (!xdotool) {
    std::cout << "popen failed: " << strerror(errno) << std::endl;
    return 1;
  }
  if (!fgets(buf, sizeof(buf), xdotool)) {
    std::cout << "fgets failed: " << strerror(errno) << std::endl;
    std::cout << "Could not get PID of Assault Cube process" << std::endl;
    return 1;
  }
  pid_t pid = atoi(buf);
  if (pid == 0) {
    std::cout << "atoi failed to parse PID" << std::endl;
    return 1;
  }
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
  bool found = false;
  long data = 0;
  long total_memory = std::numeric_limits < long > ::max(); // total amount of virtual memory to scan
  long memory_scanned = 0; // amount of virtual memory scanned so far
  int loading_bar_size = 50; // number of characters in the loading bar
  while (addr < total_memory) {
    data = loadMemory(pid, addr);
    if (data == -1) {
      found = false;
    }
    for (int i = 0; i < sizeof(long); i++) {
      if (((char * ) & data)[i] == TARGET_VALUE) {
        std::cout << "Found target value at address: " << addr + i << std::endl;
        found = true;
      }
    }
    addr += sizeof(long);
    memory_scanned += sizeof(long);

    // Update the loading bar
    int num_bars = std::ceil((double) memory_scanned / total_memory * loading_bar_size);
    std::cout << "[";
    for (int i = 0; i < num_bars; i++) {
      std::cout << "=";
    }
    for (int i = 0; i < loading_bar_size - num_bars; i++) {
      std::cout << " ";
    }
    std::cout << "] " << memory_scanned << " / " << total_memory << "  " << memory_scanned / total_memory << "%" << "\r";
    std::cout.flush();
  }
  std::cout << std::endl; // move to the next line after printing the loading bar
  if (found == false) {
    std::cout << "Could not find target value in virtual memory" << std::endl;
  }

  ptrace(PTRACE_DETACH, pid, NULL, NULL);
  return 0;
}