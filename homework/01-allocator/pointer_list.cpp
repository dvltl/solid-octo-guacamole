#include "pointer_list.h"

PointerList::PointerList(): p(Pointer(nullptr)), next(nullptr) {
}

PointerList::PointerList(Pointer p): p(p), next(nullptr){
}

bool PointerList::hasNext() const {
   return next != nullptr;
}

PointerList * PointerList::getNext() const {
    return next;
}

void PointerList::setNext(PointerList * newNext) {
    next = newNext;
}

Pointer & PointerList::get() {
    return p;
}

void PointerList::setPointer(Pointer & p) {
    this -> p = p;
}
