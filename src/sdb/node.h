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
  explicit Node(size_t level);
  Node(const KeyType key, const ValueType value, size_t level);

  ~Node();

  inline void Set(KeyType key, ValueType value);
  inline KeyType Key() const;
  inline ValueType Value() const;

  inline bool Match(KeyType key) const;
  inline size_t Level() const;
  inline bool IsEndNode() const;

  void AppendOnLevel(Node<KeyType, ValueType>* new_node, size_t target_level);
  void Detach(size_t target_level);

  inline Node<KeyType, ValueType>* PrevNodeOnLevel(size_t level) const;
  inline Node<KeyType, ValueType>* NextNodeOnLevel(size_t level) const;

 private:
  void AppendInternal(Node<KeyType, ValueType>* history, size_t current_level);

  size_t level_;
  bool is_head_or_tail_;
  std::vector<Node<KeyType, ValueType>*> prev_;
  std::vector<Node<KeyType, ValueType>*> next_;
  Item<KeyType, ValueType> item_;
};

template<typename KeyType, typename ValueType>
Node<KeyType, ValueType>::Node(size_t level)
  : level_(level),
    is_head_or_tail_(true),
    prev_(level_, nullptr),
    next_(level_, nullptr) {
}

template<typename KeyType, typename ValueType>
Node<KeyType, ValueType>::Node(
  const KeyType key, const ValueType value, size_t level)
  : level_(level),
    is_head_or_tail_(false),
    prev_(level_, nullptr),
    next_(level_, nullptr),
    item_({key, value}) {
}

template<typename KeyType, typename ValueType>
Node<KeyType, ValueType>::~Node() {
  prev_.clear();
  next_.clear();
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
bool Node<KeyType, ValueType>::IsEndNode() const {
  return is_head_or_tail_;
}

template<typename KeyType, typename ValueType>
void Node<KeyType, ValueType>::AppendOnLevel(
  Node<KeyType, ValueType>* new_node, size_t target_level) {
  if (nullptr != next_[target_level]) {
    next_[target_level]->prev_[target_level] = new_node;
  }

  new_node->prev_[target_level] = this;
  new_node->next_[target_level] = next_[target_level];

  next_[target_level] = new_node;
}

template<typename KeyType, typename ValueType>
void Node<KeyType, ValueType>::Detach(size_t target_level) {
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
