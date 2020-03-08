#include "vEBTree.hpp"
#include <bits/stdc++.h>
#include "Set.hpp"
#include "RSvEBTree.hpp"

using namespace std;

struct ScopedProf {
  string name;
  chrono::steady_clock::time_point start;
  ScopedProf(const string name) : name(name) {
    start = chrono::steady_clock::now();
  }

  ~ScopedProf() {
    const auto stop = chrono::steady_clock::now();
    const auto elapsed =
        chrono::duration_cast<chrono::milliseconds>(stop - start);
    cerr << name << "\t" << elapsed.count() << "[ms]" << endl;
  }
};

#define ASSERT_EQ(x, y)                                          \
  do {                                                           \
    if ((x) != (y)) {                                            \
      cerr << "TEST FAILED : expected " << x << ", actual " << y \
           << " at line " << __LINE__ << std::endl;              \
      exit(1);                                                   \
    }                                                            \
  } while (0)

template <class T, template <class> class Tree>
void test(const size_t limit_lg2, const size_t N) {
  using TTree = Tree<T>;
  const size_t limit = 1ll << limit_lg2;

  uniform_int_distribution<T> dist;
  auto engine = mt19937(123);

  vector<T> nums;
  nums.reserve(N);
  {
    set<T> memo;
    while (nums.size() < N) {
      auto x = dist(engine) % limit;
      if (memo.count(x)) continue;

      nums.emplace_back(x);
      memo.insert(x);
    }
  }

  TTree a(limit_lg2);

  //! test insert and lookup
  for (auto x : nums) {
    ASSERT_EQ(a.lookup(x), false);
    a.insert(x);
    ASSERT_EQ(true, a.lookup(x));
  }

  sort(begin(nums), end(nums));

  //! test succ
  for (size_t i = 0; i + 1 < N; ++i) {
    const auto &crt = nums[i];
    const auto &succ = nums[i + 1];
    auto elem = a.upper_bound(crt);

    ASSERT_EQ(true, elem.has_value());
    ASSERT_EQ(succ, elem.value());
  }
  //! test succ (nil)
  ASSERT_EQ(false, a.upper_bound(nums.back()).has_value());

  //! test erase, lookup and minimum
  {
    auto elem = a.minimum();
    ASSERT_EQ(true, elem.has_value());
    ASSERT_EQ(nums[0], elem.value());
  }
  for (size_t i = 0; i + 1 < N; ++i) {
    const auto &crt = nums[i];
    const auto &mini = nums[i + 1];

    a.erase(crt);
    ASSERT_EQ(false, a.lookup(crt));

    auto elem = a.minimum();
    if (i + 1 < N) {
      ASSERT_EQ(true, elem.has_value());
      ASSERT_EQ(mini, elem.value());
    } else {
      ASSERT_EQ(false, elem.has_value());
    }
  }
}

int main() {
  cout << "Test Set ..." << endl;
  test<int, Set>(60, 10000);
  cout << "PASSED!" << endl << endl;

  //! test small size (u = 2^20), because vEBTree requires O(u) space
  cout << "Test vEBTree ..." << endl;
  test<int, vEBTree>(20, 10000);
  cout << "PASSED!" << endl << endl;

  cout << "Test RS-vEBTree ..." << endl;
  test<long long, RSvEBTree>(60, 10000);
  cout << "PASSED!" << endl << endl;

  return 0;
}