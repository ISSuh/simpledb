/**
 *
 *  Copyright:  Copyright (c) 2023, ISSuh
 *
 */

#ifndef SDB_NODE_LIST_H_
#define SDB_NODE_LIST_H_

#include <iostream>

#include "node.h"

namespace sdb {

template<typename Key, typename Value>
class NodeList {
 public:
  NodeList(
    size_t current_level, Node<Key, Value>* head, Node<Key, Value>* tail);
  ~NodeList();

  Node<Key, Value>* Head() const;
  Node<Key, Value>* Tail() const;

  bool Empty() const;
  size_t Size() const;

  void InsertNode(Node<Key, Value>* last_node, Node<Key, Value>* new_node);
  void DeleteNode(Node<Key, Value>* node);
  bool Search(Key key, Node<Key, Value>* last_node, Node<Key, Value>** history);

  void Print(int32_t max_size) const;

 private:
  inline bool MatchKey(Node<Key, Value>* current, Key key) const;

  size_t size_;
  const size_t current_level_;
  Node<Key, Value>* head_;
  Node<Key, Value>* tail_;
};

template<typename Key, typename Value>
NodeList<Key, Value>::NodeList(
  size_t current_level, Node<Key, Value>* head, Node<Key, Value>* tail)
  : size_(0),
    current_level_(current_level),
    head_(head),
    tail_(tail) {
}

template<typename Key, typename Value>
NodeList<Key, Value>::~NodeList() {
}

template<typename Key, typename Value>
Node<Key, Value>* NodeList<Key, Value>::Head() const {
  return head_;
}

template<typename Key, typename Value>
Node<Key, Value>* NodeList<Key, Value>::Tail() const {
  return tail_;
}

template<typename Key, typename Value>
bool NodeList<Key, Value>::Empty() const {
  return (0 == size_);
}

template<typename Key, typename Value>
size_t NodeList<Key, Value>::Size() const {
  return size_;
}

template<typename Key, typename Value>
void NodeList<Key, Value>::InsertNode(
  Node<Key, Value>* last_node, Node<Key, Value>* new_node) {
  last_node->AppendOnLevel(new_node, current_level_);
  ++size_;
}

template<typename Key, typename Value>
void NodeList<Key, Value>::DeleteNode(Node<Key, Value>* node) {
  node->Detach(current_level_);
  --size_;
}

template<typename Key, typename Value>
bool NodeList<Key, Value>::Search(
  Key key, Node<Key, Value>* last_node, Node<Key, Value>** history) {
  Node<Key, Value>* current = last_node;
  while (current != tail_) {
    *history = current;
    if (current->Key() < key) {
      return true;
    }

    if (current_level_ < current->Level()) {
      current = current->NextNodeOnLevel(current_level_);
    } else {
      current = tail_;
    }
  }
  return false;
}

template<typename Key, typename Value>
bool NodeList<Key, Value>::MatchKey(Node<Key, Value>* current, Key key) const {
  return current->Match(key);
}

template<typename Key, typename Value>
void NodeList<Key, Value>::Print(int32_t max_size) const {
  std::cout << "[" << current_level_ << "] : ";
  Node<Key, Value>* current = head_;
  while (current != tail_) {
    if (current != head_) {
      std::cout << "["
                << current->Key()
                << ","
                << current->Value()
                << "] "
                << "\t";
    }

    current = current->NextNodeOnLevel(current_level_);
    if (nullptr == current) {
      current = tail_;
    }
  }
  std::cout << std::endl;
}

}  // namespace sdb

#endif  // SDB_NODE_LIST_H_
