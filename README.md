# CS6120 Advanced Compiler A8
## Loop Invariant Code Motion (LICM)

I impelemented a loop invariant code motion pass with LLVM, and tested with a real-world embeded platform benchmark [Embench](https://github.com/embench/embench-iot).

## Build the Pass
```
cd licm-pass
mkdir build && cd build
cmake ..
make
```
The pass is built into a dynamic linked library (.so), which is loaded by clang at runtime.

## Build and Run the benchmark
The embench test suite can be configured to run on various platforms. The following steps builds and runs the speed test benchmark on Intel Macintosh platform with clang.

### Build the benchmark with LICM pass enabled

I added an option to the embench test build script to run my LICM pass during building with clang.

- Baseline build
  ```sh
  cd embench-iot
  ./build_all.py --arch mac --chip speed-test-clang-X86-64 --cc clang
  ```
- Build with LICM enabled
  ```sh
  cd embench-iot
  ./build_all.py --arch mac --chip speed-test-clang-X86-64 --cc clang --run_mypass
  ```

You can specify your pass with this option:
```sh
./build_all.py --arch mac --chip speed-test-clang-X86-64 --cc clang --run_mypass --pass_lib /path/to/your/pass.so
```

### Run the benchmark
```sh
./benchmark_speed.py --target-module run_mac
```

## Results

| Benchmark name | Baseline (ms) | With LICM (ms) |
| :------------: | :-----------: | :------------: |
|   aha-mont64   |     2.75      |      2.74      |
|     crc32      |     2.10      |      2.14      |
|     cubic      |     60.65     |     54.38      |
|      edn       |     1.41      |      1.41      |
|   huffbench    |     1.65      |      1.61      |
|  matmult-int   |     1.68      |      1.69      |
|     minver     |     10.37     |     10.30      |
|     nbody      |     64.43     |     60.99      |
|   nettle-aes   |     2.98      |      2.88      |
| nettle-sha256  |     2.87      |      2.78      |
|    nsichneu    |     3.46      |      3.63      |
|    picoipeg    |     1.42      |      1.39      |
|   primecount   |     1.22      |      1.25      |
|    qarduino    |     1.12      |      1.17      |
| sglib-combined |     1.41      |      1.31      |
|      slre      |     2.33      |      2.25      |
|       st       |     33.29     |     32.00      |
|   statemate    |     8.81      |      8.45      |
|    tarfind     |     7.18      |      7.14      |
|       ud       |     1.90      |      1.88      |
|    wikisort    |     7.93      |      7.61      |

## Discussion

For most test cases, we observe that LICM consistently brings speedup ranging from 0.01ms – 3.44ms. 



<p align="center">
<img src="https://res.cloudinary.com/dxzx2bxch/image/upload/v1648187979/work_uoxjif.jpg" alt="alt_text" title="image_tooltip" style="zoom:30%;" />
</p>

A few cases has a bit slow down, which could be caused by the change in instruction alignment.