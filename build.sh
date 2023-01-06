#!/bin/bash

ptrace_scope=$(cat /proc/sys/kernel/yama/ptrace_scope)

if [ "$ptrace_scope" -eq "1" ]; then
  sudo sysctl kernel.yama.ptrace_scope=0
fi

if which xdotool >/dev/null; then
    echo "xdotool is installed"
else
    echo "xdotool is not installed, please install it with your package manager"
fi

if which g++ >/dev/null; then
    echo "g++ is installed"
else
    echo "g++ is not installed, please install it with your package manager"
fi

sudo sysctl kernel.yama.ptrace_scope=0
g++ ./src/main.cpp -o main
cargo build --target-dir=./src