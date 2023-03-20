/**
 *
 *  Copyright:  Copyright (c) 2023, ISSuh
 *
 */

#ifndef SDB_SKIP_LIST_H_
#define SDB_SKIP_LIST_H_

#include <iostream>
#include <vector>

#include "sdb/node.h"
#include "sdb/rand.h"

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

  void Print();

 private:
  Node<Key, Value>* FindIntenal(
    const Key key,
    std::vector<Node<Key, Value>*>* history);

  Node<Key, Value>* CreateNode(const Key key, const Value value);
  void InsertNode(
    Node<Key, Value>* new_node, const std::vector<Node<Key, Value>*>& history);
  void DeleteNode(Node<Key, Value>* node);
  size_t RandomLevel();

  void PrintLevel(size_t level);

  const size_t max_level_;
  Random rand_;

  Node<Key, Value>* head_;
  Node<Key, Value>* tail_;
  uint64_t size_;
};

template<typename Key, typename Value>
SkipList<Key, Value>::SkipList(size_t max_level)
  : max_level_(max_level),
    rand_(0xdeadbeef),
    head_(new Node<Key, Value>(max_level_)),
    tail_(new Node<Key, Value>(max_level_)),
    size_(0u) {
  for (size_t i = 0 ; i < max_level_ ; ++i) {
    head_->AppendOnLevel(tail_, i);
  }
}

template<typename Key, typename Value>
SkipList<Key, Value>::~SkipList() {
  Node<Key, Value>* current = head_->NextNodeOnLevel(0);
  while (tail_ != current) {
    Node<Key, Value>* next = current->NextNodeOnLevel(0);
    DeleteNode(current);
    current = next;
  }

  delete head_;
  delete tail_;
}

template<typename Key, typename Value>
void SkipList<Key, Value>::Update(const Key key, Value value) {
  std::vector<Node<Key, Value>*> history(max_level_, nullptr);
  Node<Key, Value>* node = FindIntenal(key, &history);
  if (nullptr != node) {
    node->Set(key, value);
    return;
  }

  node = CreateNode(key, value);
  InsertNode(node, history);
}

template<typename Key, typename Value>
void SkipList<Key, Value>::Erase(const Key key) {
  std::vector<Node<Key, Value>*> history(max_level_, nullptr);
  Node<Key, Value>* node = FindIntenal(key, &history);
  if (nullptr == node) {
    return;
  }

  DeleteNode(node);
}

template<typename Key, typename Value>
bool SkipList<Key, Value>::Find(const Key key, Value* value) {
  std::vector<Node<Key, Value>*> history(max_level_, nullptr);
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
  return size_;
}

template<typename Key, typename Value>
Node<Key, Value>* SkipList<Key, Value>::FindIntenal(
  const Key key, std::vector<Node<Key, Value>*>* history) {
  Node<Key, Value>* current = head_;
  for (int32_t i = max_level_ - 1 ; i >= 0 ; --i) {
    while (tail_ != current->NextNodeOnLevel(i) &&
           current->NextNodeOnLevel(i)->Key() < key) {
      current = current->NextNodeOnLevel(i);
    }
    history->at(i) = current;
  }
  current = current->NextNodeOnLevel(0);
  return (current->IsEndNode() || !current->Match(key)) ? nullptr : current;
}

template<typename Key, typename Value>
Node<Key, Value>* SkipList<Key, Value>::CreateNode(
  const Key key, const Value value) {
  size_t target_level = RandomLevel();
  return new Node<Key, Value>(key, value, target_level);
}

template<typename Key, typename Value>
void SkipList<Key, Value>::InsertNode(
  Node<Key, Value>* new_node,
  const std::vector<Node<Key, Value>*>& history) {
  // insert new node for each level
  size_t target_level = new_node->Level();
  for (size_t i = 0 ; i < target_level ; ++i) {
    history[i]->AppendOnLevel(new_node, i);
  }

  ++size_;
}

template<typename Key, typename Value>
void SkipList<Key, Value>::DeleteNode(Node<Key, Value>* node) {
  for (size_t i = 0 ; i < node->Level() ; ++i) {
    node->Detach(i);
  }
  delete node;

  --size_;
}

template<typename Key, typename Value>
size_t SkipList<Key, Value>::RandomLevel() {
  static const uint32_t kBranching = 4;
  size_t level = 1;
  while (level < max_level_ && (0 == rand_.Uniform(kBranching))) {
    level++;
  }
  return level;
}

template<typename Key, typename Value>
void SkipList<Key, Value>::Print() {
  for (int32_t i = max_level_ - 1 ; i >= 0 ; --i) {
    PrintLevel(i);
  }
  std::cout << "---------------------------------\n";
}

template<typename Key, typename Value>
void SkipList<Key, Value>::PrintLevel(size_t level) {
  std::cout << "[" << level << "] : ";
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

    current = current->NextNodeOnLevel(level);
    if (nullptr == current) {
      current = tail_;
    }
  }
  std::cout << std::endl;
}

}  // namespace sdb

#endif  // SDB_SKIP_LIST_H_
