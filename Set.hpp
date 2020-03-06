#pragma once

#include <optional>
#include <set>
#include <utility>

template <class T>
class Set {
 private:
  std::set<T> s;

 public:
  Set() {}
  Set(T) {}

  void insert(const T &x) { s.insert(x); }
  void erase(const T &key) { s.erase(key); }
  bool lookup(const T &key) const { return s.find(key) != end(s); }

  std::optional<T> upper_bound(const T &key) const {
    auto it = s.upper_bound(key);
    if (it == end(s)) return std::nullopt;

    return *it;
  }

  std::optional<T> minimum() const {
    if (s.empty()) return std::nullopt;

    return *begin(s);
  }
};