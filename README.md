# CS6120 Advanced Compiler A8
## Loop Invariant Code Motion (LICM)


### Build the Pass
```
cd llvm-pass-skeleton
mkdir build && cd build
cmake ..
make
```
### Use clang to emit LLVM IR
```
$ clang -emit-llvm -S -o - main.cc
```
- `-S`: output LLVM assembly instead of binary
- `-o - `: output to stdio

### Run the Pass
Compile the program with clang and run the pass
```
clang -Xclang -load -Xclang ./llvm-pass-skeleton/build/skeleton/libSkeletonPass.so your_file.cc
```
There will be an `a.out` executable in the current directory. 

To run the pass: 
```
clang -Xclang -load -Xclang ./llvm-pass-skeleton/build/skeleton/libSkeletonPass.so main.cc
```

### Testing
To run the test suite:
```sh
lit tests
```
In the tests, I use FileCheck's commands to check the common sub-expressions are indeed eliminated. For example, 
```c++
// CHECK: add nsw i32 %{{[0-9]+}}, 2
// CHECK-NOT: add nsw i32
// CHECK: store i32 %{{[0-9]+}}, i32* %{{[0-9]+}}
```
The above checks states that there should not be an add instruction between `add nsw i32 %n, 2` and `store` instructions. The expected lit test output is: 
```
-- Testing: 6 tests, 6 workers --
PASS: test-suite :: for.c (1 of 6)
PASS: test-suite :: while.c (2 of 6)
PASS: test-suite :: basic.c (3 of 6)
PASS: test-suite :: if-both.c (4 of 6)
PASS: test-suite :: multiple.c (5 of 6)
PASS: test-suite :: if-single.c (6 of 6)

Testing Time: 0.63s
  Passed: 6
```
