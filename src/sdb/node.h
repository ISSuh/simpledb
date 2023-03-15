/**
 *
 *  Copyright:  Copyright (c) 2023, ISSuh
 *
 */

#ifndef SDB_NODE_H_
#define SDB_NODE_H_

#include <string>
#include <vector>
#include <limits>

namespace sdb {

template<typename KeyType, typename ValueType>
struct Item {
  KeyType key;
  ValueType value;
};

template<typename KeyType, typename ValueType>
class Node {
 public:
  explicit Node(size_t level, bool is_head_or_tail_for_debug = false);

  ~Node();

  void Set(KeyType key, ValueType value);
  KeyType Key() const;
  ValueType Value() const;

  inline bool Match(KeyType key) const;
  size_t Level() const;

  void AppendTail(Node<KeyType, ValueType>* tail);
  void AppendOnLevel(Node<KeyType, ValueType>* new_node, size_t target_level);
  void Detach(size_t target_level);

  Node<KeyType, ValueType>* PrevNodeOnLevel(size_t level) const;
  Node<KeyType, ValueType>* NextNodeOnLevel(size_t level) const;

 private:
  void AppendInternal(Node<KeyType, ValueType>* history, size_t current_level);

  bool is_head_or_tail_for_debug_;
  size_t level_;
  std::vector<Node<KeyType, ValueType>*> prev_;
  std::vector<Node<KeyType, ValueType>*> next_;
  Item<KeyType, ValueType> item_;
};

template<typename KeyType, typename ValueType>
Node<KeyType, ValueType>::Node(size_t level, bool is_head_or_tail_for_debug)
  : is_head_or_tail_for_debug_(is_head_or_tail_for_debug),
    level_(level),
    prev_(level_ + 1, nullptr),
    next_(level_ + 1, nullptr) {
}

template<typename KeyType, typename ValueType>
Node<KeyType, ValueType>::~Node() {
}

template<typename KeyType, typename ValueType>
void Node<KeyType, ValueType>::Set(KeyType key, ValueType value) {
  item_.key = key;
  item_.value = value;
}

template<typename KeyType, typename ValueType>
KeyType Node<KeyType, ValueType>::Key() const {
  return item_.key;
}

template<typename KeyType, typename ValueType>
ValueType Node<KeyType, ValueType>::Value() const {
  return item_.value;
}

template<typename KeyType, typename ValueType>
bool Node<KeyType, ValueType>::Match(KeyType key) const {
  return (key == item_.key);
}

template<typename KeyType, typename ValueType>
size_t Node<KeyType, ValueType>::Level() const {
  return level_;
}

template<typename KeyType, typename ValueType>
void Node<KeyType, ValueType>::AppendTail(Node<KeyType, ValueType>* tail) {
  for (size_t level = 0 ; level <= level_ ; ++level) {
    next_[level] = tail;
    tail->prev_[level] = this;
  }
}

template<typename KeyType, typename ValueType>
void Node<KeyType, ValueType>::AppendOnLevel(
  Node<KeyType, ValueType>* new_node, size_t target_level) {
  // if (target_level > level_) {
  //   return;
  // }

  if (nullptr != next_[target_level]) {
    next_[target_level]->prev_[target_level] = new_node;
  }

  new_node->prev_[target_level] = this;
  new_node->next_[target_level] = next_[target_level];

  next_[target_level] = new_node;
}

template<typename KeyType, typename ValueType>
void Node<KeyType, ValueType>::Detach(size_t target_level) {
  // if (target_level > level_) {
  //   return;
  // }
  if (nullptr != next_[target_level]) {
    next_[target_level]->prev_[target_level] = prev_[target_level];
  }

  if (nullptr != prev_[target_level]) {
    prev_[target_level]->next_[target_level] = next_[target_level];
  }
}

template<typename KeyType, typename ValueType>
Node<KeyType, ValueType>* Node<KeyType, ValueType>::PrevNodeOnLevel(
  size_t level) const {
  return prev_[level];
}

template<typename KeyType, typename ValueType>
Node<KeyType, ValueType>* Node<KeyType, ValueType>::NextNodeOnLevel(
  size_t level) const {
  return next_[level];
}

}  // namespace sdb

#endif  // SDB_NODE_H_
