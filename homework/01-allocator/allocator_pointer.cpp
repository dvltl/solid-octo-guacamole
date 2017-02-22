#include "allocator_pointer.h"

Pointer::Pointer(void * ptr) {
    this -> mem = (char *) ptr;
}

Pointer::Pointer(char * head, size_t offset, size_t size) {
    this -> mem = head + offset;
    this -> size = size;
}

void Pointer::setMem(void * mem) {
    this -> mem = (char*) mem;
}

void Pointer::setSize(size_t size) {
    this -> size = size;
}

void * Pointer::get() const {
    return mem;
}

size_t Pointer::getSize() const {
    return size;
}

void Pointer::setNextFree(char * nfree) {
    this -> nextFree = nfree;
}

void Pointer::setFreeSize(size_t size) {
    this -> freeSize = size;
}

char * Pointer::getNextFree() const {
    return nextFree;
}

size_t Pointer::getFreeSize() const {
    return freeSize;
}

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
