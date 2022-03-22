// RUN: clang -Xclang -load -Xclang ../llvm-pass-skeleton/build/skeleton/libSkeletonPass.so %s | FileCheck %s
int main() {
    int a = 1;
    int b = 1;
    int c = a + b;
// CHECK: store i32 %{{[0-9]+}}, i32* %{{[0-9]+}}
// CHECK-NOT: add
    int d = a + b;
    return 0;
}
