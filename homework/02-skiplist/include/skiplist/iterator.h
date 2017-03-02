#ifndef __ITERATOR_H
#define __ITERATOR_H
#include <cassert>
#include "node.h"

/**
 * Skiplist const iterator
 */
template<class Key, class Value>
class Iterator {
private:
  Node<Key, Value> *pCurrent;

public:
  Iterator(Node<Key,Value> *p) : pCurrent(p) {}
  virtual ~Iterator() {}

  virtual const Key& key() const {
    assert(pCurrent != nullptr);
    return pCurrent->key();
  };

  virtual const Value& value() const {
    assert(pCurrent != nullptr);
    return pCurrent->value();
  };

  virtual const Value& operator*() {
    assert(pCurrent != nullptr);
    return pCurrent->value();
  };

  virtual const Value& operator->() {
    assert(pCurrent != nullptr);
    return pCurrent->value();
  };

  virtual bool operator==(const Iterator & other) const {
    return this->pCurrent == other.pCurrent;
  };

  virtual bool operator!=(const Iterator & other) const {
    return !this->operator ==(other);
  };


  virtual Iterator& operator=(const Iterator & other) {
      pCurrent = other.pCurrent;
      return *this;
  };

  virtual Iterator& operator++() {
      pCurrent = &pCurrent->next();
      return *this;
  };

  virtual Iterator& operator++(int step) {
      int i = 0;
      while (i < step) {
          pCurrent = &pCurrent->next();
          ++i;
      }
      return *this;
  };
};

#endif // __ITERATOR_H
