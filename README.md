# LLVM Course

## Build
To build all projects, run:
```
        git clone https://github.com/kefirRzevo/LLVM-course.git
        cd LLVM-course
        cmake -S . -B build
        cmake --build build
```

## GUI Application
This is simple implementation for cellular automaton using SDL2 (wrapped with API). [Here](https://en.wikipedia.org/wiki/Cellular_automaton) you can read more about this interesting phenomena.

To run program:
```
    ./build/gui-app/MyGUI
```
To generate its LLVM-IR for one cycle run:
```
clang-18 -std=c23 -emit-llvm -S gui-app/src/OneCycleApplication.c -O3 -o gui-app/llvm-ir/OneCycleApplication.ll -I gui-app/include/
```

