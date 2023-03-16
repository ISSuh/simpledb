/**
 *
 *  Copyright:  Copyright (c) 2023, ISSuh
 *
 */

#ifndef SDB_SKIP_LIST_H_
#define SDB_SKIP_LIST_H_

#include <iostream>
#include <vector>

#include "node_list.h"
#include "rand.h"

namespace sdb {

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
    head_(max_level_, true),
    tail_(max_level_, true) {

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

  size_t target_level = 1;
  while (CoinFlip() && (target_level < max_level_)) {
    ++target_level;
  }

  Node<Key, Value>* new_node = new Node<Key, Value>(target_level);
  new_node->Set(key, value);

  // insert new node for each level
  for (size_t i = 1 ; i <= target_level; ++i) {
    history[i]->AppendOnLevel(new_node, i);
  }
}

template<typename Key, typename Value>
void SkipList<Key, Value>::Erase(const Key key) {
  std::vector<Node<Key, Value>*> history(max_level_ + 1, nullptr);
  Node<Key, Value>* node = FindIntenal(key, &history);
  if (nullptr == node) {
    return;
  }

  for (size_t i = 1 ; i <= node->Level() ; ++i) {
    node->Detach(i);
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
  Node<Key, Value>* current = &head_;
  for (auto i = max_level_ ; i >= 1 ; --i) {
    while (&tail_ != current->NextNodeOnLevel(i) &&
           current->NextNodeOnLevel(i)->Key() < key) {
      current = current->NextNodeOnLevel(i);
    }
    history->at(i) = current;
  }
  current = current->NextNodeOnLevel(1);
  return (current->IsEndNode() || !current->Match(key)) ? nullptr : current;
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
