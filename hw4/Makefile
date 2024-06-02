CLANG_PATH ?= /home/tomlord/llvm-project/build/bin

# Generate the LLVM pass
libVTableCheckerPass.so: 
	cd build && cmake .. && make
	
# Compile the runtime library
librtlib.so:
	$(CLANG_PATH)/clang -c rtlib.c -o rtlib.o

# Compile the test program
test: test.cpp libVTableCheckerPass.so librtlib.so
	$(CLANG_PATH)/clang++ -fpass-plugin=./build/passes/hw4.so ./test.cpp ./rtlib.o

# Phony target to build everything
all: test

# Clean up generated files
clean:
	rm -f libVTableCheckerPass.so librtlib.so test
