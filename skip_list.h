/**
 *
 *  Copyright:  Copyright (c) 2023, ISSuh
 *
 */

#include <list>
#include <utility>

namespace ssl {

template<typename Key, typename Value>
struct Item {
  Key key;
  Value Value;
};

template<typename Key, typename Value>
class List {
 public:
  struct Node {
    Node* prev : nullptr;
    Node* next : nullptr;
    Item<Key, Value> item;
  };

  List();
  ~List();

  void InsertNode(Key key, Value value);
  void DeleteNode(Key key);
  Node* Search(Key key);

 private:
  inline bool MatchKey(Node* current, Key key) const;

  size_t size_;
  Node<Key, Value>* head_;
  Node<Key, Value>* tail_;
};

template<typename Key, typename Value>
List<Key, Value>::List()
  : size_(0), head_(nullptr), tail_(nullptr) {
  head_ = new Node();
  tail_ = new Node();

  head_->next = tail_;
  tail_->prev = head_
}

template<typename Key, typename Value>
List<Key, Value>::~List() {
}

template<typename Key, typename Value>
void List<Key, Value>::InsertNode(Key key, Value value) {
}

template<typename Key, typename Value>
void List<Key, Value>::DeleteNode(Key key) {
}

template<typename Key, typename Value>
List<Key, Value>::Node* List<Key, Value>::Search(Key key) {
  Node* current = head_;
  while (current != tail_) {
    if (MatchKey(current, key)) {
      return current;
    }
    current = current->next;
  }
  return nullptr;
}

template<typename Key, typename Value>
bool List<Key, Value>::MatchKey(Node* current, Key key) const {
  return (key == current->item.key);
}

template<typename Key, typename Value>
class SkipList {
 public:
  SkipList();
  ~SkipList();

  void Update(const Key key, Value value);
  bool Remove(const Key key);
  bool Find(const Key key, Value* value);

 private:
  using Elements = std::list<Node<Key, Vaule>>;
  using Levels = std::list<Elements>;
  Levels list_;
};

template<typename Key, typename Value>
SkipList<Key, Value>::SkipList()
  : list_(1, std::list<Item>(0)) {
}

template<typename Key, typename Value>
SkipList<Key, Value>::~SkipList() {
}

template<typename Key, typename Value>
void SkipList<Key, Value>::Update(const Key key, Value value) {
}

template<typename Key, typename Value>
bool SkipList<Key, Value>::Remove(const Key key) {
}

template<typename Key, typename Value>
bool SkipList<Key, Value>::Find(const Key key, Value* value) {
  Item temp;
  Levels::Iterator levels_iter = list_.rbegin();
  while (Levels != list_.rend()) {
    Elements::Iterator elements_iter = levels_iter->begin();
    while (elements_iter != elements_iter.end() && elements_iter->first < key) {
      ++elements_iter;
    }
    ++iter;
  }

  return false;
}

}  // namespace ssl
