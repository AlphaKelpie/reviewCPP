#include <oneapi/tbb.h>
#include <oneapi/tbb/partitioner.h>

#include <cassert>
#include <chrono>
#include <iostream>
#include <numeric>
#include <vector>

auto Foo(int i) {
  return ++i;
}

int main() {
  std::vector<int> v(100000);
  std::iota(v.begin(), v.end(), 0);

  auto start_parallel = std::chrono::high_resolution_clock::now();

  oneapi::tbb::parallel_for(
    oneapi::tbb::blocked_range<int>(0, 10, 2000),
    [&](const oneapi::tbb::blocked_range<int> r) {
      for (int i = r.begin(); i < r.end(); ++i) {
        Foo(v[i]);
      }
    },
    oneapi::tbb::simple_partitioner()
  );

  auto end_parallel = std::chrono::high_resolution_clock::now();
  std::cout << "Parallel time: "
            << std::chrono::duration_cast<std::chrono::microseconds>(end_parallel - start_parallel).count()
            << " us\n";

}