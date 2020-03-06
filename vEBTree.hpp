#pragma once

#include <optional>
#include <utility>

template <class T>
class vEBTree {
 private:
 public:
  void insert(const T &x) {}
  void erase(const T &key) {}
  bool lookup(const T &key) { return false; }
  std::optional<T> succ(const T &key) { return std::nullopt; }
  std::optional<T> minimum() { return std::nullopt; }
};