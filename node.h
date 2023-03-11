/**
 *
 *  Copyright:  Copyright (c) 2023, ISSuh
 *
 */

#include <string>
#include <vector>
#include <limits>

namespace ssl {

template<typename KeyType, typename ValueType>
struct Item {
  KeyType key;
  ValueType value;
};

template<typename KeyType, typename ValueType>
class Node {
 public:
  explicit Node(size_t level);

  ~Node();

  void Set(KeyType key, ValueType value);
  KeyType Key() const;
  ValueType Value() const;

  bool Match(KeyType key) const;

  size_t Level() const;

  void AppendTail(Node<KeyType, ValueType>* tail);
  void AppendOnLevel(Node<KeyType, ValueType>* new_node, size_t target_level);
  void Detach(size_t target_level);

  Node<KeyType, ValueType>* PrevNodeOnLevel(size_t level) const;
  Node<KeyType, ValueType>* NextNodeOnLevel(size_t level) const;

 private:
  void AppendInternal(Node<KeyType, ValueType>* history, size_t current_level);

  std::string debug_;
  size_t level_;
  std::vector<Node<KeyType, ValueType>*> prev_;
  std::vector<Node<KeyType, ValueType>*> next_;
  Item<KeyType, ValueType> item_;
};

template<typename KeyType, typename ValueType>
Node<KeyType, ValueType>::Node(size_t level)
  : level_(level),
    prev_(level_, nullptr),
    next_(level_, nullptr) {
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
  for (size_t level = 0 ; level < level_ ; ++level) {
    next_[level] = tail;
    tail->prev_[level] = this;
  }
}

template<typename KeyType, typename ValueType>
void Node<KeyType, ValueType>::AppendOnLevel(
  Node<KeyType, ValueType>* new_node, size_t target_level) {
  if (target_level > level_) {
    return;
  }

  if (nullptr != next_[target_level]) {
    next_[target_level]->prev_[target_level] = new_node;
  }

  new_node->prev_[target_level] = this;
  new_node->next_[target_level] = next_[target_level];

  next_[target_level] = new_node;
}

template<typename KeyType, typename ValueType>
void Node<KeyType, ValueType>::Detach(size_t target_level) {
  if (target_level > level_) {
    return;
  }

  next_[target_level]->prev_[target_level] = prev_[target_level];
  prev_[target_level]->next_[target_level] = next_[target_level];
}

template<typename KeyType, typename ValueType>
Node<KeyType, ValueType>* Node<KeyType, ValueType>::PrevNodeOnLevel(
  size_t level) const {
  return (level < prev_.size()) ? prev_[level] : nullptr;
}

template<typename KeyType, typename ValueType>
Node<KeyType, ValueType>* Node<KeyType, ValueType>::NextNodeOnLevel(
  size_t level) const {
  return (level < next_.size()) ? next_[level] : nullptr;
}

}  // namespace ssl
