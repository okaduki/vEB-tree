#pragma once

#include <memory>
#include <optional>
#include <stdexcept>
#include <utility>
#include <unordered_map>

//! require : T is hashable
//! time complexity = O(lg lg u)
//! space complexity = O(n lg n) ?
//!     where u = 2^K, n = query num
template <class T>
class RSvEBTree {
 private:
  using vEBTreePtr = std::unique_ptr<RSvEBTree>;

  const T K;  //! size == 2^K
  const T cluster_num, cluster_num_lg2, cluster_size, cluster_size_lg2;
  std::optional<T> min, max;
  vEBTreePtr summary;
  std::unordered_map<T, vEBTreePtr> child;


  // util functions
  T to_index(const T cluster_index, const T offset) const {
    return {cluster_index * cluster_size + offset};
  }

  std::pair<T, T> from_index(const T &index) const {
    return {index / cluster_size, index % cluster_size};
  }

  bool exists(const T &key) const {
    auto it = child.find(key);
    return (it != end(child));
  }

 protected:
  virtual T pow2(T x) {
    T acc = T{2};
    T p2 = T{1};
    while (x > T{0}) {
      if (x % T{2} == T{1}) {
        p2 *= acc;
      }
      acc *= acc;
      x /= T{2};
    }
    return p2;
  }

 public:
  RSvEBTree(const T K)  //! size == 2^K
      : K(K),
        cluster_num(pow2(K / T{2})),
        cluster_num_lg2(K / T{2}),
        cluster_size(pow2(K - K / T{2})),
        cluster_size_lg2(K - K / T{2}),
        min(std::nullopt), max(std::nullopt), summary(nullptr)
  {
    if (K > 1) {  // size > 2
    }
  }

  std::optional<T> minimum() const { return min; }
  std::optional<T> maximum() const { return max; }

  bool lookup(const T &key) const {
    if (min == key) return true;
    if (max == key) return true;
    if (K <= T{1})  // size <= 2
      return false;

    const auto [cluster_index, offset] = from_index(key);

    if (!exists(cluster_index)) return false;
    return child.at(cluster_index)->lookup(offset);
  }

  // precondition : key is NOT in tree
  void insert_simple(T key) {
    if (!min.has_value()) {
      min = max = key;
      return;
    }

    if (key < min) {
      std::swap(key, min.value());
    }

    if (K > T{1}) {  // size > 2
      const auto [cluster_index, offset] = from_index(key);

      if (!exists(cluster_index)) {
        child[cluster_index] = std::make_unique<RSvEBTree>(cluster_size_lg2);
      }

      const auto child_min = child[cluster_index]->minimum();

      if (!child_min.has_value()) {
        if(!summary) summary = std::make_unique<RSvEBTree>(cluster_num_lg2);

        summary->insert_simple(cluster_index);
      }
      child[cluster_index]->insert_simple(offset);
    }

    if (max < key) {
      max = key;
    }
  }
  void insert(const T &key) {
    if (lookup(key)) return;
    insert_simple(key);
  }

  // precondition : key is in tree
  void erase_simple(const T &key) {
    if (min == max) {
      min = max = std::nullopt;
    } else if (K <= T{1}) {  // size == 2
      if (key == T{0})
        min = T{1};
      else
        min = T{0};
      max = min;
    } else {
      T kill_value = key;
      if (kill_value == min) {
        const auto first_cluster = summary->minimum();
        kill_value =
            to_index(*first_cluster, *child[*first_cluster]->minimum());
        min = kill_value;
      }

      const auto [cluster_id, offset] = from_index(kill_value);
      child[cluster_id]->erase_simple(offset);

      if (!child[cluster_id]
               ->minimum()
               .has_value()) {  // if sub-cluster is empty after erase
        summary->erase_simple(cluster_id);

        if (kill_value == max) {  // if max should be updated
          const auto summary_max = summary->maximum();
          if (!summary_max.has_value()) {  // all sub-clusters are empty
            max = min;
          } else {
            const auto max_offset = child[*summary_max]->maximum();
            max = to_index(*summary_max, *max_offset);
          }
        }
      } else {
        const auto max_offset = child[cluster_id]->maximum();
        max = to_index(cluster_id, *max_offset);
      }
    }
  }
  void erase(const T &key) {
    if (!lookup(key)) return;
    erase_simple(key);
  }

  std::optional<T> upper_bound(const T &key) const {
    if (K == T{1}) {  // size == 2
      if (key == T{0} && max == T{1}) return T{1};
      return std::nullopt;
    }
    if (key < min) {
      return min;
    }

    const auto [cluster_index, offset] = from_index(key);
    const auto child_max = exists(cluster_index)
                               ? child.at(cluster_index)->maximum()
                               : std::nullopt;

    if (offset < child_max) {
      const auto max_offset = child.at(cluster_index)->upper_bound(offset);
      return to_index(cluster_index, *max_offset);
    } else {
      const auto succ_cluster = (summary ? summary->upper_bound(cluster_index) : std::nullopt);
      if (!succ_cluster.has_value()) return std::nullopt;

      const auto succ_offset = child.at(*succ_cluster)->minimum();
      return to_index(*succ_cluster, *succ_offset);
    }
    return std::nullopt;
  }
};