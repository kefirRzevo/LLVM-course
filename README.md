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

## LLVM-Pass
To generate LLVM-IR with logs do this:
```
opt-18 -load-pass-plugin=build/llvm-pass/MyPass.so -passes=my-pass gui-app/llvm-ir/OneCycleApplication.ll -S -o gui-app/llvm-ir/LoggedOneCycleApplication.ll
```
To get application that runs one cycle and generates `runtime.json`, run this:
```
clang-18 -O3 -std=c23 gui-app/src/Start.c gui-app/lib/GUILib.c llvm-pass/src/Log.c -lSDL2 -I ./gui-app/include/ gui-app/llvm-ir/LoggedOneCycleApplication.ll
```
After that you can run application to get `runtime.json`:
```
./a.out
```
After that you can check the statistics of instructions:
```
python3 llvm-pass/src/StatGetter.py --groupCount 1 > llvm-pass/res/res1.txt
```
The results can be seen in directory `llvm-pass/res/`

## LLVM-IR Generation
To start interpretation LLVM-IR, run this:
```
./build/ir-gen/IR_Gen
```

## LLVM ASM-TO-IR Generation
To convert asm to LLVM-IR and start interpretation, run this:
```
./build/asm-to-ir/AsmToIR_Gen
```

## LLVM LANG-TO-IR Generation
To convert `paracl` lang to LLVM-IR and start interpretation, run this:
```
./build/lang-to-ir/LangToIR_Gen
```
