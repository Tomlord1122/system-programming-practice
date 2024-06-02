

# Generate the LLVM pass
libVTableCheckerPass.so: 
	cd build
	cmake ..
	make
	
# Compile the runtime library
librtlib.so: rtlib.c
	/pass/to/llvm/project/clang -c rtlib.c -o rtlib.o

# Compile the test program
test: test.cpp
	/path/to/llvm-project/build/bin/clang++ -fpass-plugin=`echo ../build/passes/hw4.so` ../test.cpp ./rtlib.o

# Phony target to build everything
all: libVTableCheckerPass.so librtlib.so test

# Clean up generated files
clean:
	rm -f libVTableCheckerPass.so librtlib.so test
