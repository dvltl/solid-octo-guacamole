#ifndef __SKIPLIST_H
#define __SKIPLIST_H
#include <functional>
#include "node.h"
#include "iterator.h"
#include <iostream>

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

  Value val;

  void insert(IndexNode<Key, Value>* cur, IndexNode<Key, Value>* prev,
               DataNode<Key,Value> * toAdd) const {
      IndexNode<Key,Value> * buf = new IndexNode<Key,Value>( &(cur->down()), toAdd);
      prev->next(buf);
      toAdd->next( &static_cast<DataNode<Key,Value>&>(prev->root().next()));
      static_cast<DataNode<Key,Value>&>(prev->root()).next(toAdd);
      buf->next(cur);
  }

  bool recInsert(IndexNode<Key,Value>* start, IndexNode<Key,Value>* end, DataNode<Key,Value>* toAdd,
                 const int level, const Value * ret, const bool insPresent) const {
      if (level == 0) {
          // looking for the place to insert
          IndexNode<Key,Value> * cur = &static_cast<IndexNode<Key,Value>&>(start->next());
          IndexNode<Key,Value> * prev = start;
          bool ins = false;

          std::cout << "key [1]" << std::endl;
          while (cur != pTail) {
              //need also pointers to upper levels to make insertion there possible
              if (cur->key() <= toAdd->key()) {
                  if (cur->key() == toAdd->key()) {
                      ret = &cur->value();
                      if (insPresent) {
                          insert(cur, prev, toAdd);
                          ins = true;
                      }
                  } else {
                      ret = nullptr;
                      insert(cur, prev, toAdd);
                      ins = true;
                  }

                  break;
              } else {
                  prev = cur;
                  cur = &static_cast<IndexNode<Key,Value>&>(cur->next());
              }
          }
          std::cout << "key [1] OK" <<std::endl;

          if (!ins) {
              insert(cur, prev, toAdd);
          }

          return true;
      } else {
          IndexNode<Key,Value> * newStart = start;
          IndexNode<Key,Value> * newEnd = &static_cast<IndexNode<Key,Value>&>(start->next());

          std::cout << "key [2]" << std::endl;
          while (newEnd != pTailIdx && newEnd->key() <= toAdd->key()) {
              newStart = newEnd;
              newEnd = &static_cast<IndexNode<Key,Value>&>(newEnd->next());
          }
          std::cout << "key [2] OK" << std::endl;

          if( recInsert(&static_cast<IndexNode<Key,Value>&>(newStart->down()),
                        &static_cast<IndexNode<Key,Value>&>(newEnd->down()),
                        toAdd, level - 1, ret, insPresent) && rand() % 2) {
              insert(end, start, toAdd);
              return true;
          } else {
              return false;
          }
      }
  }

  void emptyInsert(DataNode<Key,Value> * toAdd) const
  {
      int i = MAXHEIGHT - 1;
      IndexNode<Key,Value> * cur = aHeadIdx[i];

      while (cur == aHeadIdx[i] && &cur->next() == pTailIdx) {
          if (i > 0) {
              --i;
              cur = aHeadIdx[i];
          } else {
              // lowest level has structure like that => empty list
              insert(&static_cast<IndexNode<Key,Value>&>(cur->next()), cur, toAdd);
              while (i < MAXHEIGHT && rand() % 2) {
                  insert(&static_cast<IndexNode<Key,Value>&>(aHeadIdx[i]->next()), aHeadIdx[i], toAdd);
                  ++i;
              }
              break;
          }
      }
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
   * @return old value for the given key or nullptr
   */

  virtual Value* Put(const Key& key, const Value& value) const {
      DataNode<Key,Value> * toAdd = new DataNode<Key,Value>(&key, &value);
      Value * ret;

      if (isEmpty()) {
          emptyInsert(toAdd);
      } else {
          const int i = MAXHEIGHT - 1;
          recInsert(aHeadIdx[i], pTailIdx, toAdd, i, ret, true);
      }

      return ret;
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
   * @return existing value for the given key or nullptr
   */
  virtual Value* PutIfAbsent(const Key& key, const Value& value) {
      DataNode<Key,Value> * toAdd = new DataNode<Key,Value>(&key, &value);
      Value * ret;

      if (isEmpty()) {
          emptyInsert(toAdd);
      } else {
          int i = MAXHEIGHT - 1;
          recInsert(aHeadIdx[i], pTailIdx, toAdd, i, ret, false);
      }

      return ret;
  };

  /**
   * Returns value assigned for the given key or nullptr
   * if there is no established assosiation with the given key
   *
   * @param key to find
   * @return value assosiated with given key or nullptr
   */
  virtual Value* Get(const Key& key) const {
    return nullptr;
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
    return nullptr;
  };

  /**
   * Same as Get
   */
  virtual const Value* operator[](const Key& key) {
    return nullptr;
  };

  /**
   * Return iterator onto very first key in the skiplist
   */
  virtual Iterator<Key, Value> cbegin() const {
    return Iterator<Key,Value>(pTail);
  };

  /**
   * Returns iterator to the first key that is greater or equals to
   * the given key
   */
  virtual Iterator<Key, Value> cfind(const Key &min) const {
    return Iterator<Key,Value>(pTail);
  };

  /**
   * Returns iterator on the skiplist tail
   */
  virtual Iterator<Key, Value> cend() const {
    return Iterator<Key,Value>(pTail);
  };
};
#endif // __SKIPLIST_H
