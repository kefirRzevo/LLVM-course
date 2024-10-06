# LLVM Course

## GUI Application
This is simple implementation for cellular automaton using SDL2 (wrapped with API). [Here](https://en.wikipedia.org/wiki/Cellular_automaton) you can read more about this interesting phenomena.

![Picture](./res/automata.png)

## Build
To build program:
```
        git clone https://github.com/kefirRzevo/LLVM-course.git
        cd LLVM-course
        cmake -S . -B build
        cmake --build build
```
To run program:
```
    ./build/gui-app/MyGUI
```

## LLVM-Pass
To generate LLVM-IR do this:
```
clang-18 -std=c23 -fpass-plugin=./build/llvm-pass/MyPass.so gui-app/src/OneCycleApplication.c gui-app/src/Start.c gui-app/lib/GUILib.c llvm-pass/src/Log.c -lSDL2 -I ./gui-app/include/
```
After that you can check the statistics of instructions:
```
python3 llvm-pass/src/StatGetter.py --groupCount 1
```
The results can be seen in directory `llvm-pass/res/`