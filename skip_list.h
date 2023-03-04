/**
 *
 *  Copyright:  Copyright (c) 2023, ISSuh
 *
 */

#include <vector>
#include <random>

namespace ssl {

template<typename T>
T GenerateRandomLevel(T range_from, T range_to) {
    std::random_device rand_dev;
    std::mt19937 generator(rand_dev());
    std::uniform_int_distribution<T> distribution(range_from, range_to);
    return distribution(generator);
}

template<typename Key, typename Value>
struct Item {
  Key key;
  Value value;
};

template<typename Key, typename Value>
struct Node {
  Node* prev = nullptr;
  Node* next = nullptr;
  Item<Key, Value> item;
};

template<typename Key, typename Value>
class List {
 public:
  List();
  ~List();

  void InsertNode(Node<Key, Value>* pos, Key key, Value value);
  void DeleteNode(Key key);
  bool Search(Key key, Node<Key, Value>* node);

 private:
  inline bool MatchKey(Node<Key, Value>* current, Key key) const;

  size_t size_;
  Node<Key, Value>* head_;
  Node<Key, Value>* tail_;
};

template<typename Key, typename Value>
List<Key, Value>::List()
  : size_(0), head_(nullptr), tail_(nullptr) {
  head_ = new Node<Key, Value>();
  tail_ = new Node<Key, Value>();

  head_->next = tail_;
  tail_->prev = head_;
}

template<typename Key, typename Value>
List<Key, Value>::~List() {
}

template<typename Key, typename Value>
void List<Key, Value>::InsertNode(Node<Key, Value>* pos, Key key, Value value) {
  Node<Key, Value>* node = new Node<Key, Value>();
  node->item.key = key;
  node->item.value = key;

  Node<Key, Value> current = pos;
  if (nullptr != current->prev) {
    current->prev->next = node;
  }

  if (nullptr != current->next) {
    current->next->prev = node;
  }

  ++size_;
}

template<typename Key, typename Value>
void List<Key, Value>::DeleteNode(Key key) {
}

template<typename Key, typename Value>
bool List<Key, Value>::Search(Key key, Node<Key, Value>* node) {
  Node<Key, Value>* current = head_;
  while (current != tail_) {
    node = current;
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
class SkipList {
 public:
  SkipList();
  ~SkipList();

  void Update(const Key key, Value value);
  bool Remove(const Key key);
  bool Find(const Key key, Value* value);

 private:
  Node<Key, Value>* FindIntenal(
    const Key key,
    std::vector<Node<Key, Value>*>* history);

  size_t max_level_ = 10;

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
  std::vector<Node<Key, Value>*> history(list_.size(), nullptr);
  Node<Key, Value>* node = FindIntenal(key, &history);
  if (nullptr != node) {
    node->item.value = value;
    return;
  }

  Node<Key, Value>* new_node = new Node<Key, Value>();
  new_node->item.key = key;
  new_node->item.value = value;

  size_t target_level = GenerateRandomLevel<size_t>(0, max_level_);
  
}

template<typename Key, typename Value>
bool SkipList<Key, Value>::Remove(const Key key) {
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
    bool res = level_iter->Search(key, last_node);
    if (res) {
      return last_node;
    }

    if (nullptr != history) {
      size_t index = std::distance(list_.begin(), level_iter.base()) - 1;
      (*history)[index] = last_node;
    }
  }
  return nullptr;
}

}  // namespace ssl
