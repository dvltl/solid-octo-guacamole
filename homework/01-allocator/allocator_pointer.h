#ifndef ALLOCATOR_POINTER
#define ALLOCATOR_POINTER

#include "sys/types.h"

// Forward declaration. Do not include real class definition
// to avoid expensive macros calculations and increase compile speed
class Allocator;

class Pointer {
    char * mem;
    size_t size;
    char * nextFree; // for memory allocation purposes
    size_t freeSize;
public:
    Pointer() {}
    Pointer(void *);
    Pointer(char *, size_t, size_t);
    void setMem(void *);
    void setSize(size_t);
    void * get() const;
    size_t getSize() const;
    void setNextFree(char *);
    char * getNextFree() const;
    void setFreeSize(size_t);
    size_t getFreeSize() const;
};

class PointerList {
    Pointer p;
    PointerList * next;
public:
    PointerList();
    PointerList(Pointer);
    Pointer & get();
    void setPointer(Pointer &);
    bool hasNext() const;
    PointerList * getNext() const;
    void setNext(PointerList *);
};

#endif //ALLOCATOR_POINTER
