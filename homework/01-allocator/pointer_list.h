#ifndef ALLOCATOR_POINTER_LIST
#define ALLOCATOR_POINTER_LIST

#include "allocator_pointer.h"

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

#endif
