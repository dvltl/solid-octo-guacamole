#ifndef __SKIPLIST_H
#define __SKIPLIST_H
#include <functional>
#include "node.h"
#include "iterator.h"
#include <iostream>
#include <sstream>

/**
 * Skiplist interface
 */
template<class Key, class Value, size_t MAXHEIGHT, class Compare = std::less<Key>>
class SkipList {
private:
  Node<Key, Value> *pHead;
  Node<Key, Value> *pTail;

  IndexNode<Key, Value> *pTailIdx;
  IndexNode<Key, Value> *aHeadIdx[MAXHEIGHT];

  Value ** pVal;

  IndexNode<Key,Value> * insert(IndexNode<Key, Value>* cur, IndexNode<Key, Value>* prev,
                                DataNode<Key,Value> * toAdd, IndexNode<Key,Value> * down) const {
      IndexNode<Key,Value> * buf = new IndexNode<Key,Value>(down, toAdd);

      buf->next(cur);
      toAdd->next( &static_cast<DataNode<Key,Value>&>(cur->root()));

      prev->next(buf);

      static_cast<DataNode<Key,Value>&>(prev->root()).next(toAdd);

      return buf;
  }

  IndexNode<Key,Value> * recInsert(IndexNode<Key,Value>* start, DataNode<Key,Value>* toAdd,
                                   const int level, const bool insPresent,
                                   Value * pVal, bool & init) const {
      if (level == 0) {
          IndexNode<Key,Value> * cur = &static_cast<IndexNode<Key,Value>&>(start->next());
          IndexNode<Key,Value> * prev = start;
          IndexNode<Key,Value> * newNode = nullptr;
          bool ins = false;

          while (cur != pTailIdx) {
              //need also pointers to upper levels to make insertion there possible
              if (cur->key() >= toAdd->key()) {
                  if (cur->key() == toAdd->key()) {
                      *pVal = cur->value();
                      init = true;
                      if (insPresent) {
                          newNode = insert(&static_cast<IndexNode<Key,Value>&>(cur->next()), prev, toAdd, nullptr);
                          delete cur;
                      }
                  } else {
                      newNode = insert(cur, prev, toAdd, nullptr);
                  }
                  ins = true;

                  break;
              } else {
                  prev = cur;
                  cur = &static_cast<IndexNode<Key,Value>&>(cur->next());
              }
          }

          if (!ins) {
              newNode = insert(cur, prev, toAdd, nullptr);
          }

          return newNode;
      } else {
          IndexNode<Key,Value> * newStart = start;
          IndexNode<Key,Value> * newEnd = &static_cast<IndexNode<Key,Value>&>(start->next());

          while (newEnd != pTailIdx && newEnd->key() < toAdd->key()) {
              newStart = newEnd;
              newEnd = &static_cast<IndexNode<Key,Value>&>(newEnd->next());
          }

          IndexNode<Key,Value> * down = recInsert(&static_cast<IndexNode<Key,Value>&>(newStart->down()),
                                                  toAdd, level - 1, insPresent, pVal, init);
          if (down != nullptr && rand() % 2) {
              IndexNode<Key,Value> * newNode;
              if (newEnd != pTailIdx && newEnd->key() == toAdd->key()){
                  IndexNode<Key,Value> * buf = newEnd;
                  newEnd = &static_cast<IndexNode<Key,Value>&>(newEnd->next());
                  delete buf;
              }
              newNode = insert(newEnd, newStart, toAdd, down);
              return newNode;
          } else {
              return nullptr;
          }
      }
  }

  /**
   * @brief search for the key in skiplist
   * @param key
   * @return pointer previous to the pointer to the highest IndexNode connected to the key or nullptr
   */
  IndexNode<Key,Value> * search(IndexNode<Key,Value> * begin, const Key & key, int & level) const {
      IndexNode<Key,Value> * cur = &static_cast<IndexNode<Key,Value>&>(begin->next());
      IndexNode<Key,Value> * prev = aHeadIdx[level];

      while (level >= 0) {
          while (cur != pTailIdx) {
              if (cur->key() < key) {
                  prev = cur;
                  cur = &static_cast<IndexNode<Key,Value>&>(cur->next());
              } else if (cur->key() == key) {
                  return prev;
              } else {
                  break;
              }
          }
          if (level > 0) {
              prev = &static_cast<IndexNode<Key,Value>&>(prev->down());
              cur = &static_cast<IndexNode<Key,Value>&>(prev->next());
          }
          --level;
      }

      return nullptr;
  }

public:
  /**
   * Creates new empty skiplist
   */
  SkipList() {
    pHead   = new DataNode<Key, Value>(nullptr, nullptr);
    pTail   = new DataNode<Key, Value>(nullptr, nullptr);

    srand(time(NULL));
    Node<Key, Value> *prev = pHead;
    pTailIdx = new IndexNode<Key, Value>(pTail, pTail);
    for (int i=0; i < MAXHEIGHT; i++) {
      aHeadIdx[i] = new IndexNode<Key, Value>(prev, pHead);
      aHeadIdx[i]->next(pTailIdx);
      prev = aHeadIdx[i];
    }

    pVal = new Value*;
  }

  /**
   * Disable copy constructor
   */
  SkipList(const SkipList& that) = delete;


  /**
   * Destructor
   */
  virtual ~SkipList() {
    delete pTailIdx;
    for (int i=0; i < MAXHEIGHT; i++) {
      delete aHeadIdx[i];
    }

    delete pHead;
    delete pTail;
  }

  /**
   * @brief dumps list's contents into string (only datanodes)
   * @return
   */
  virtual std::string dump() const {
      std::ostringstream oss;
      Node<Key,Value> * cur = aHeadIdx[0];
      int i = 0;

      oss << "Dump: " << std::endl;

      while (cur != pTailIdx) {
          if (cur != aHeadIdx[0])
              oss << i++ << ' ' << cur->key() << ' ' << cur->value() << std::endl;
          cur = &cur->next();
      }

      oss << "End dump" << std::endl;

      return oss.str();
  }

  /**
   * @brief Is the list empty?
   * @return true if empty, false otherwise
   */
  virtual bool isEmpty() const {
      int i = MAXHEIGHT - 1;
      IndexNode<Key,Value> * cur = aHeadIdx[i];

      while (&cur->next() == pTailIdx && i > 0) {
          --i;
          cur = aHeadIdx[i];
      }

      return (&cur->next() == pTailIdx && i == 0);
  }

  /**
   * Assign new value for the key. If a such key already has
   * assosiation then old value returns, otherwise nullptr
   *
   * @param key key to be assigned with value
   * @param value to be added
   * @return old value for the given key or nullptr. Must free ptr after use if not nullptr
   */

  virtual Value* Put(const Key& key, const Value& value) const {
      Key * k = new Key;
      Value * v = new Value;

      *k = key;
      *v = value;

      DataNode<Key,Value> * toAdd = new DataNode<Key,Value>(k, v);

      const int i = MAXHEIGHT - 1;

      Value * pVal = new Value;
      bool inited = false;
      recInsert(aHeadIdx[i], toAdd, i, true, pVal, inited);

      if (inited) {
          return pVal;
      } else {
          delete pVal;
          return nullptr;
      }
  };

  /**
   * Put value only if there is no assosiation with key in
   * the list and returns nullptr
   *
   * If there is an established assosiation with the key already
   * method doesn't nothing and returns existing value
   *
   * @param key key to be assigned with value
   * @param value to be added
   * @return existing value for the given key or nullptr. Must free ptr after use if not nullptr
   */
  virtual Value* PutIfAbsent(const Key& key, const Value& value) {
      Key * k = new Key;
      Value * v = new Value;

      *k = key;
      *v = value;

      DataNode<Key,Value> * toAdd = new DataNode<Key,Value>(k, v);

      const int i = MAXHEIGHT - 1;

      Value * pVal = new Value;
      bool inited = false;
      recInsert(aHeadIdx[i], toAdd, i, false, pVal, inited);

      if (inited) {
          return pVal;
      } else {
          delete pVal;
          return nullptr;
      }
  };

  /**
   * Returns value assigned for the given key or nullptr
   * if there is no established assosiation with the given key
   *
   * @param key to find
   * @return value assosiated with given key or nullptr. Must free ptr after use if not nullptr
   */
  virtual Value* Get(const Key& key) const {
      int level = MAXHEIGHT - 1;
      IndexNode<Key,Value> * res = search(aHeadIdx[level], key, level);
      if (res != nullptr) {
          Value * ret = new Value(res->next().value());
          return ret;
      } else {
          return nullptr;
      }
  };

  /**
   * Remove given key from the skpiplist and returns value
   * it has or nullptr in case if key wasn't assosiated with
   * any value
   *
   * @param key to be added
   * @return value for the removed key or nullptr
   */
  virtual Value* Delete(const Key& key) {
      int level;
      IndexNode<Key,Value> * res = search(aHeadIdx[level], key, level);
      IndexNode<Key,Value> * prev = res;
      Value * ret = new Value;
      while (res != nullptr) {
          *ret = res->next().value();
          prev = res;
          res = &static_cast<IndexNode<Key,Value>&>(res->next());
          prev->next(&static_cast<IndexNode<Key,Value>&>(res->next()));
          delete res;
          --level;
          if (level > 0) {
            res = search(&static_cast<IndexNode<Key,Value>&>(prev->down()), key, level);
          } else {
            break;
          }
      }
      return ret;
  };

  /**
   * Same as Get
   */
  virtual const Value* operator[](const Key& key) {
    return Get(key);
  };

  /**
   * Return iterator onto very first key in the skiplist
   */
  virtual Iterator<Key, Value> cbegin() const {
    return Iterator<Key,Value>(&aHeadIdx[0]->next());
  };

  /**
   * Returns iterator to the first key that is greater or equals to
   * the given key
   */
  virtual Iterator<Key, Value> cfind(const Key &min) const {
      Iterator<Key,Value> result = cbegin();
      while (result != cend()) {
          if (result.key() >= min) {
              break;
          } else {
              result++;
          }
      }
      return result;
  };

  /**
   * Returns iterator on the skiplist tail
   */
  virtual Iterator<Key, Value> cend() const {
    return Iterator<Key,Value>(pTailIdx);
  };
};
#endif // __SKIPLIST_H
