/**
 *
 *  Copyright:  Copyright (c) 2023, ISSuh
 *
 */

#include <iostream>
#include <string>
#include <vector>
#include <random>

#include "node.h"

namespace sdb {

size_t GenerateRandomLevel(size_t range_from, size_t range_to) {
  std::random_device rand_dev;
  std::mt19937 generator(rand_dev());
  std::uniform_int_distribution<size_t> distribution(range_from, range_to);
  return distribution(generator);
}

template<typename Key, typename Value>
class List {
 public:
  List(size_t current_level, Node<Key, Value>* head, Node<Key, Value>* tail);
  ~List();

  Node<Key, Value>* Head() const {
    return head_;
  }

  Node<Key, Value>* Tail() const {
    return tail_;
  }

  size_t Size() const {
    return size_;
  }

  void InsertNode(Node<Key, Value>* last_node, Node<Key, Value>* new_node);
  void DeleteNode(Node<Key, Value>* node);
  bool Search(Key key, Node<Key, Value>** last_node);

  void Print(int32_t max_size) const;

 private:
  inline bool MatchKey(Node<Key, Value>* current, Key key) const;

  size_t size_;
  size_t current_level_;
  Node<Key, Value>* head_;
  Node<Key, Value>* tail_;
};

template<typename Key, typename Value>
List<Key, Value>::List(
  size_t current_level, Node<Key, Value>* head, Node<Key, Value>* tail)
  : size_(0),
    current_level_(current_level),
    head_(head),
    tail_(tail) {
}

template<typename Key, typename Value>
List<Key, Value>::~List() {
}

template<typename Key, typename Value>
void List<Key, Value>::InsertNode(
  Node<Key, Value>* last_node, Node<Key, Value>* new_node) {
  last_node->AppendOnLevel(new_node, current_level_);
  ++size_;
}

template<typename Key, typename Value>
void List<Key, Value>::DeleteNode(Node<Key, Value>* node) {
  node->Detach(current_level_);
  --size_;
}

template<typename Key, typename Value>
bool List<Key, Value>::Search(Key key, Node<Key, Value>** last_node) {
  Node<Key, Value>* current = head_;
  while (current != tail_) {
    *last_node = current;
    if (MatchKey(current, key)) {
      return true;
    }

    current = current->NextNodeOnLevel(current_level_);
    if (nullptr == current) {
      current = tail_;
    }
  }
  return false;
}

template<typename Key, typename Value>
bool List<Key, Value>::MatchKey(Node<Key, Value>* current, Key key) const {
  return current->Match(key);
}

template<typename Key, typename Value>
void List<Key, Value>::Print(int32_t max_size) const {
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


template<typename Key, typename Value>
class SkipList {
 public:
  explicit SkipList(size_t max_level = 5);
  ~SkipList();

  void Update(const Key key, Value value);
  bool Remove(const Key key);
  bool Find(const Key key, Value* value);

  void Print() const;

 private:
  Node<Key, Value>* FindIntenal(
    const Key key,
    std::vector<Node<Key, Value>*>* history);

  size_t max_level_;

  using Levels = std::vector<List<Key, Value>>;
  Levels lists_;

  using Nodes = std::vector<Node<Key, Value>>;
  Node<Key, Value> head_;
  Node<Key, Value> tail_;
};

template<typename Key, typename Value>
SkipList<Key, Value>::SkipList(size_t max_level)
  : max_level_(max_level),
    head_(max_level_),
    tail_(max_level_) {
  // create base list
  lists_.emplace_back(0, &head_, &tail_);
  head_.AppendTail(&tail_);
}

template<typename Key, typename Value>
SkipList<Key, Value>::~SkipList() {
}

template<typename Key, typename Value>
void SkipList<Key, Value>::Update(const Key key, Value value) {
  size_t list_size = lists_.size();
  std::vector<Node<Key, Value>*> history(list_size, nullptr);
  Node<Key, Value>* node = FindIntenal(key, &history);
  if (nullptr != node) {
    node->Set(key, value);
    return;
  }

  // generate target level
  size_t target_level = GenerateRandomLevel(1, max_level_);

  // if less list size than target_level,
  // create new list as musch as target level
  for (int32_t i = list_size ; i < target_level ; ++i) {
    lists_.emplace_back(i, &head_, &tail_);
  }

  std::cout << __func__ << " - "
            << "target_level : " << target_level
            << " / history.size() : " << history.size()
            << std::endl;

  Node<Key, Value>* new_node = new Node<Key, Value>(target_level);
  new_node->Set(key, value);

  // insert new node for each level
  size_t history_size = history.size();
  for (size_t i = 0 ; i < target_level; ++i) {
    Node<Key, Value>* last_node =
      (i < history_size) ? history[i] : lists_[i].Head();
    lists_[i].InsertNode(last_node, new_node);
  }
}

template<typename Key, typename Value>
bool SkipList<Key, Value>::Remove(const Key key) {
  size_t list_size = lists_.size();
  std::vector<Node<Key, Value>*> history(list_size, nullptr);
  Node<Key, Value>* node = FindIntenal(key, &history);
  if (nullptr == node) {
    return false;
  }

  for (size_t i = 0 ; i < node->Level() ; ++i) {
    lists_[i].DeleteNode(node);
  }
  return true;
}

template<typename Key, typename Value>
bool SkipList<Key, Value>::Find(const Key key, Value* value) {
  Node<Key, Value>* node = FindIntenal(key, nullptr);
  if (nullptr == node) {
    return false;
  }

  *value = node->item.value;
  return true;
}

template<typename Key, typename Value>
Node<Key, Value>* SkipList<Key, Value>::FindIntenal(
  const Key key, std::vector<Node<Key, Value>*>* history) {
  auto level_iter = lists_.rbegin();
  while (level_iter != lists_.rend()) {
    Node<Key, Value>* last_node = nullptr;
    bool res = level_iter->Search(key, &last_node);
    if (res) {
      return last_node;
    }

    if (nullptr != history) {
      size_t index = std::distance(lists_.begin(), level_iter.base()) - 1;
      (*history)[index] = last_node;
    }

    ++level_iter;
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
