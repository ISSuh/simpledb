/**
 *
 *  Copyright:  Copyright (c) 2023, ISSuh
 *
 */

#ifndef SDB_SKIP_LIST_H_
#define SDB_SKIP_LIST_H_

#include <iostream>
#include <vector>
#include <random>

#include "node_list.h"
#include "rand.h"

namespace sdb {

int coinFlip(){
 return (rand() % 2 == 1);
}

size_t GenerateRandomLevel(size_t range_from, size_t range_to) {
  std::random_device rand_dev;
  std::mt19937 generator(rand_dev());
  std::uniform_int_distribution<size_t> distribution(range_from, range_to);
  return distribution(generator);
}

template<typename Key, typename Value>
class SkipList {
 public:
  explicit SkipList(size_t max_level = 5);
  ~SkipList();

  void Update(const Key key, Value value);
  void Erase(const Key key);
  bool Find(const Key key, Value* value);

  size_t MaxLevel() const;
  size_t Size() const;

  void Print() const;

 private:
  Node<Key, Value>* FindIntenal(
    const Key key,
    std::vector<Node<Key, Value>*>* history);

  const size_t max_level_;

  using Levels = std::vector<NodeList<Key, Value>>;
  Levels lists_;

  using Nodes = std::vector<Node<Key, Value>>;
  Node<Key, Value> head_;
  Node<Key, Value> tail_;
};

template<typename Key, typename Value>
SkipList<Key, Value>::SkipList(size_t max_level)
  : max_level_(max_level),
    head_(max_level_ + 1, true),
    tail_(max_level_ + 1, true) {
  // create list
  for (size_t i = 0 ; i <= max_level_ ; ++i) {
    lists_.emplace_back(i, &head_, &tail_);
  }
  head_.AppendTail(&tail_);
}

template<typename Key, typename Value>
SkipList<Key, Value>::~SkipList() {
}

template<typename Key, typename Value>
void SkipList<Key, Value>::Update(const Key key, Value value) {
  std::vector<Node<Key, Value>*> history(max_level_ + 1, nullptr);
  Node<Key, Value>* node = FindIntenal(key, &history);
  if (nullptr != node) {
    node->Set(key, value);
    return;
  }

  // generate target level
  size_t target_level = GenerateRandomLevel(1, max_level_);
  // size_t target_level = redisLrand48() % max_level_;

  // size_t target_level = 0;
  // while (coinFlip() && (target_level < max_level_)) {
  //   ++target_level;
  // }

  // if less list size than target_level,
  // create new list as musch as target level
  // for (size_t i = list_size ; i < target_level ; ++i) {
  //   lists_.emplace_back(i, &head_, &tail_);
  // }

  Node<Key, Value>* new_node = new Node<Key, Value>(target_level);
  new_node->Set(key, value);

  // insert new node for each level
  for (size_t i = 1 ; i <= target_level; ++i) {
    Node<Key, Value>* last_node =
      (history[i] != nullptr) ? history[i] : lists_[i].Head();
    lists_[i].InsertNode(last_node, new_node);
  }
}

template<typename Key, typename Value>
void SkipList<Key, Value>::Erase(const Key key) {
  std::vector<Node<Key, Value>*> history(max_level_ + 1, nullptr);
  Node<Key, Value>* node = FindIntenal(key, &history);
  if (nullptr == node) {
    return;
  }

  for (size_t i = 0 ; i < node->Level() ; ++i) {
    lists_[i].DeleteNode(node);
  }

  delete node;
}

template<typename Key, typename Value>
bool SkipList<Key, Value>::Find(const Key key, Value* value) {
  std::vector<Node<Key, Value>*> history(max_level_ + 1, nullptr);
  Node<Key, Value>* node = FindIntenal(key, &history);
  if (nullptr == node) {
    return false;
  }

  *value = node->Value();
  return true;
}

template<typename Key, typename Value>
size_t SkipList<Key, Value>::MaxLevel() const {
  return max_level_;
}

template<typename Key, typename Value>
size_t SkipList<Key, Value>::Size() const {
  return lists_[0].Size();
}

template<typename Key, typename Value>
Node<Key, Value>* SkipList<Key, Value>::FindIntenal(
  const Key key, std::vector<Node<Key, Value>*>* history) {
  size_t index = max_level_;
  auto level_iter = lists_.rbegin();
  Node<Key, Value>* last_node = level_iter->Head();
  while (level_iter != lists_.rend() - 1) {
    Node<Key, Value>* node_history_on_level = nullptr;
    bool res = level_iter->Search(key, last_node, &node_history_on_level);
    if (res) {
      return node_history_on_level;
    }

    if (nullptr != history) {
      (*history)[index] = node_history_on_level;
      last_node = node_history_on_level;
    }

    ++level_iter;
    --index;
  }
  return nullptr;
}

template<typename Key, typename Value>
void SkipList<Key, Value>::Print() const {
  int32_t max_size = lists_[0].Size();
  for (int32_t i = lists_.size() - 1 ; i >= 0 ; --i) {
    lists_[i].Print(max_size);
  }
  std::cout << "---------------------------------\n";
}

}  // namespace sdb

#endif  // SDB_SKIP_LIST_H_
