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

namespace ssl {

size_t GenerateRandomLevel(size_t range_from, size_t range_to) {
  std::random_device rand_dev;
  std::mt19937 generator(rand_dev());
  std::uniform_int_distribution<size_t> distribution(range_from, range_to);
  return distribution(generator);
}

template<typename Key, typename Value>
class List {
 public:
  List();
  ~List();

  Node<Key, Value>* Head() const {
    return head_;
  }

  Node<Key, Value>* Tail() const {
    return tail_;
  }

  void InsertNode(Node<Key, Value>* last_pos, Key key, Value value);
  void DeleteNode(Key key);
  bool Search(Key key, Node<Key, Value>** node);

  void Print(size_t level) const;

 private:
  inline bool MatchKey(Node<Key, Value>* current, Key key) const;

  size_t size_;
  size_t current_level_;
  Node<Key, Value>* head_;
  Node<Key, Value>* tail_;
};

template<typename Key, typename Value>
List<Key, Value>::List()
  : size_(0), head_(nullptr), tail_(nullptr) {
  head_ = new Node<Key, Value>();
  tail_ = new Node<Key, Value>();

  head_->Append(tail_, 0);
}

template<typename Key, typename Value>
List<Key, Value>::~List() {
}

template<typename Key, typename Value>
void List<Key, Value>::InsertNode(
  Node<Key, Value>* last_pos, Key key, Value value) {
  // create new node
  Node<Key, Value>* new_node = new Node<Key, Value>();
  new_node->item.key = key;
  new_node->item.value = value;

  Node<Key, Value>* current = (last_pos == nullptr) ? head_ : last_pos;
  if (nullptr != current->next) {
    current->next->prev = new_node;
  }

  new_node->prev = current;
  new_node->next = current->next;

  current->next = new_node;

  ++size_;
}

template<typename Key, typename Value>
void List<Key, Value>::DeleteNode(Key key) {
}

template<typename Key, typename Value>
bool List<Key, Value>::Search(Key key, Node<Key, Value>** last_node) {
  Node<Key, Value>* current = head_;
  while (current != tail_) {
    *last_node = current;
    if (MatchKey(current, key)) {
      return true;
    }
    current = current->next;
  }
  return false;
}

template<typename Key, typename Value>
bool List<Key, Value>::MatchKey(Node<Key, Value>* current, Key key) const {
  return (key == current->item.key);
}

template<typename Key, typename Value>
void List<Key, Value>::Print(size_t level) const {
  std::cout << "[" << level << "] : ";
  Node<Key, Value>* current = head_;
  while (current != tail_) {
    if (current != head_)
      std::cout << current->item.key << "\t";
    current = current->next;
  }
  std::cout << std::endl;
}

template<typename Key, typename Value>
class SkipList {
 public:
  SkipList();
  ~SkipList();

  void Update(const Key key, Value value);
  bool Remove(const Key key);
  bool Find(const Key key, Value* value);

  void Print() const;

 private:
  Node<Key, Value>* FindIntenal(
    const Key key,
    std::vector<Node<Key, Value>*>* history);

  size_t max_level_ = 5;

  using Levels = std::vector<List<Key, Value>>;
  Levels list_;
};

template<typename Key, typename Value>
SkipList<Key, Value>::SkipList()
  : list_(1) {
}

template<typename Key, typename Value>
SkipList<Key, Value>::~SkipList() {
}

template<typename Key, typename Value>
void SkipList<Key, Value>::Update(const Key key, Value value) {
  size_t list_size = list_.size();
  std::vector<Node<Key, Value>*> history(list_size, nullptr);
  Node<Key, Value>* node = FindIntenal(key, &history);
  if (nullptr != node) {
    node->item.value = value;
    return;
  }

  // generate target level
  size_t target_level = GenerateRandomLevel(1, max_level_);

  // if less list size than target_level,
  // create new list as musch as target level
  if (target_level > list_size) {
    for (int32_t i = target_level - list_size ; i > 0 ; --i) {
      list_.emplace_back();
    }
  }

  std::cout << __func__ << " - target : " << target_level << std::endl;

  // insert new node for each level
  size_t history_size = history.size();
  for (size_t i = 0 ; i < target_level; ++i) {
    Node<Key, Value>* last_node = (i < history_size) ? history[i] : nullptr;
    list_[i].InsertNode(last_node, key, value);
  }
}

template<typename Key, typename Value>
bool SkipList<Key, Value>::Remove(const Key key) {
  size_t list_size = list_.size();
  std::vector<Node<Key, Value>*> history(list_size, nullptr);
  Node<Key, Value>* node = FindIntenal(key, &history);
  if (nullptr == node) {
    return;
  }

  
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
  auto level_iter = list_.rbegin();
  while (level_iter != list_.rend()) {
    Node<Key, Value>* last_node = nullptr;
    bool res = level_iter->Search(key, &last_node);
    if (res) {
      return last_node;
    }

    if (nullptr != history) {
      size_t index = std::distance(list_.begin(), level_iter.base()) - 1;
      (*history)[index] = last_node;
    }

    ++level_iter;
  }
  return nullptr;
}

template<typename Key, typename Value>
void SkipList<Key, Value>::Print() const {
  std::cout << "---------------------------------\n";
  for (int32_t i = list_.size() - 1 ; i >= 0 ; --i) {
    list_[i].Print(i);
  }
}

}  // namespace ssl
