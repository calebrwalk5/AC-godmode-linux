#include <iostream>

#include <stdio.h>

#include <string.h>

#include <unistd.h>

#include <sys/ptrace.h>

#include <sys/wait.h>

#include <sys/user.h>

#include <sys/types.h>

#include <limits>

#include <cmath>

#include <vector>

#include <cstdio>

#include <cstdlib>

#include <unistd.h>

constexpr int TARGET_VALUE = 100;

// Struct to represent a range of memory addresses
struct MemoryRange {
  long start_addr;
  long end_addr;
};

// Loads the value at the specified memory address
long loadMemory(pid_t pid, long addr) {
  long data;
  data = ptrace(PTRACE_PEEKDATA, pid, (void * ) addr, NULL);
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

  // Get the memory ranges that are likely to be mapped to physical memory
  std::vector < MemoryRange > memory_ranges;
  char maps_path[64];
  snprintf(maps_path, sizeof(maps_path), "/proc/%d/maps", pid);
  FILE * maps_file = fopen(maps_path, "r");
  if (!maps_file) {
    std::cout << "Could not open " << maps_path << std::endl;
    return 1;
  }
  char line[256];
  while (fgets(line, sizeof(line), maps_file)) {
    long start_addr, end_addr;
    char permissions[5];
    if (sscanf(line, "%lx-%lx %4s", & start_addr, & end_addr, permissions) == 3) {
      if (strcmp(permissions, "r-xp") == 0) {
        MemoryRange range = {
          start_addr,
          end_addr
        };
        memory_ranges.push_back(range);
      }
    }
  }
  fclose(maps_file);
  // Search for the target value within the memory ranges
  for (const MemoryRange & range: memory_ranges) {
    for (long addr = range.start_addr; addr < range.end_addr; addr += sizeof(long)) {
      long data = loadMemory(pid, addr);
      if (data == TARGET_VALUE) {
        std::cout << "Found " << TARGET_VALUE << " at " << std::hex << addr << std::dec << std::endl;
        system("sudo chmod u+x /proc " + pid);
        system(("sudo ./src/debug/memedit " + std::to_string(addr) + " " + std::to_string(TARGET_VALUE) + " " + std::to_string(pid)).c_str());
        break;
      }
    }
  }
  ptrace(PTRACE_DETACH, pid, NULL, NULL);
  return 0;
}