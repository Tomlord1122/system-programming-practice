# V-Table Pointer Checker Pass in LLVM

## Introduction

In this repo, we implemented a V-Table pointer checker pass in LLVM based on LLVM 16.0.0. The goal of this pass is to ensure the security of V-Table pointers by verifying that they point to read-only memory regions. This write-up provides a detailed explanation of the source code, how to test the plugin and the runtime library, and the contributions from each team member.

## Source Code Explanation

### `hw4.cpp`

This file contains the implementation of the LLVM pass that checks V-Table pointers.

1. **Includes and Namespace**: The necessary LLVM headers are included, and the `llvm` and `std` namespaces are used.
2. **VTableCheckerModulePass Class**: This class inherits from `PassInfoMixin` and implements the `runOnModule` method.
3. **runOnModule Method**: This method iterates over all functions in the module, and for each instruction in the `main` function, it checks if the instruction is a `GetElementPtrInst`. If it is, it inserts a call to the `vtable_security_check` function with the base pointer of the V-Table.
4. **llvmGetPassPluginInfo Function**: This function registers the pass with the LLVM pass manager.

### `rtlib.c`

This file contains the runtime library function that performs the actual V-Table pointer check.

1. **Includes**: Necessary headers are included.
2. **vtable_security_check Function**: This function checks if the V-Table pointer is within the read-only memory region defined by `etext` and `edata`. If the pointer is writable, the program aborts; otherwise, it prints "Read-only".

### `test.cpp`

This file contains a test program that demonstrates the use of V-Table pointers and how they can be manipulated.

1. **FileDownload and CustomFileDownload Classes**: These classes define virtual functions `check` and `wget`.
2. **main Function**: This function creates an instance of `CustomFileDownload`, calls its virtual functions, and then overwrites the V-Table pointer with a fake V-Table. It calls the virtual functions again to demonstrate the effect of the overwritten V-Table.

## Testing the Plugin and Runtime Library

### Step-by-Step Instructions

1. **Compile the Runtime Library**:
   ```sh
   gcc -shared -o librtlib.so rtlib.c
   ```

2. **Compile the Test Program**:
   ```sh
   g++ -o test test.cpp
   ```

3. **Run the Test Program**:
   ```sh
   ./test
   ```

4. **Compile the LLVM Pass**:
   ```sh
   clang++ -fPIC -shared -o libVTableCheckerPass.so hw4.cpp `llvm-config --cxxflags --ldflags --system-libs --libs core`
   ```

5. **Run the LLVM Pass**:
   ```sh
   opt -load-pass-plugin=./libVTableCheckerPass.so -passes="vtable-checker" -disable-output test.bc
   ```

### Expected Output

When running the test program, you should see the following output:
