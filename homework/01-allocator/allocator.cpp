#include "allocator.h"
#include "allocator_pointer.h"
#include "allocator_error.h"
#include <iostream>

Allocator::Allocator(void * b, size_t size){
    this -> base = (char *) b;
    this -> size = size;
    this -> freeSize = size;
    this -> pNum = 0;
    this -> pList = (PointerList *) (base + (size - sizeof(PointerList)));
}

Pointer Allocator::alloc (size_t N) {
//    std::cout << "Alloc" << std::endl;
    if (freeSize > N + sizeof(Pointer)) {
        Pointer p = Pointer(base, size - (freeSize - pNum * sizeof(Pointer)), N / sizeof(char)); // what if fragmentation?
//        std::cout << "Mid: " << (void*) base << ' ' << p.get() << std::endl;
        appendPointer(pList, p);
    //    std::cout << "Done" << std::endl;
        return p;
    } else {
        throw AllocError(AllocErrorType::NoMemory, "Not enough free memory to allocate");
    }
}

void Allocator::realloc (Pointer& p, size_t N) {
    
}

void Allocator::free (Pointer& p) {
    if (!deletePointer(pList, p)) {
        throw AllocError(AllocErrorType::InvalidFree, "Attempt of freeing the non-existent pointer");
    }
}

void Allocator::defrag () {
    
}

PointerList * Allocator::findFreeSpace(PointerList * list) {
    PointerList * buf = list;
    PointerList * prev = nullptr;
 //   std::cout << "FFS: " << std::endl;
    while (buf != nullptr) {
        if (prev != nullptr){
//            std::cout << "Diff: " << prev - buf << std::endl; // 'prev' has bigger addr than 'buf'
            if ( prev - buf > 1) {
                return prev - 1;
            }
        }
        prev = buf;

        if ((char*) buf < base || (char*) buf > base + size){
            if ((char*) buf < base) {
                throw AllocError(AllocErrorType::InternalError,
                                 "Out of bounds while looking for free memory for system info (lower)");
            } else {
                throw AllocError(AllocErrorType::InternalError,
                                 "Out of bounds while looking for free memory for system info (higher)");
            }
        }

        buf = buf -> getNext();
    }
    if (prev == nullptr) {
        return list;
    } else {
        return prev - 1;
    }
}

void Allocator::appendPointer (PointerList * list, Pointer & p) {
    PointerList * buf = list;
    PointerList * prev;
    if (!pNum){
        list -> setPointer(p);
        list -> setNext(nullptr);
    } else {
        PointerList * neo = findFreeSpace(list);
        bool added = false;

//        std::cout << "New addr: " << (void*) neo << std::endl;
//        std::cout << "Base: " << (void*) base << std::endl;
        neo -> setPointer(p);

        while (buf != nullptr) {
            if ( buf->get().get() > p.get() ) {
                neo -> setNext(buf);
                prev -> setNext(neo);
                added = true;
                break;
            }
            prev = buf;
            buf = buf -> getNext();
        }

        if (!added) {
            prev -> setNext(neo);
            neo -> setNext(nullptr);
        }
    }
    freeSize -= sizeof(Pointer) + p.getSize();
    ++pNum;
}

void Allocator::addAsFree(PointerList * list, void * mem, size_t size) {
    PointerList * buf = list;
    while (buf != nullptr) {
        if (buf->get().get() < (char *) mem) {
            if (buf->get().getNextFree() + buf->get().getFreeSize() == mem) {
                buf->get().setFreeSize(buf->get().getFreeSize() + size);
            } else {
                buf->get().setNextFree((char *) mem);
                buf->get().setFreeSize(size);
            }
        } else {
            break;
        }
        buf = buf -> getNext();
    }
}

bool Allocator::deletePointer (PointerList * list, Pointer& p) {
    PointerList * buf = list;
    PointerList * prev;
    bool found = false;

    if (!pNum) {
        return false;
    }

    if (list->get().get() == p.get()) {
        list = list -> getNext();
        found = true;
    } else {
        while (buf != nullptr) {
            if ( buf->get().get() == p.get() ) {
                prev -> setNext( buf -> getNext() );
                found = true;
                break;
            }
            prev = buf;
            buf = buf -> getNext();
        }
    }

    if (found) {
        freeSize += p.getSize() + sizeof(Pointer);
        --pNum;

        addAsFree(list, p.get(), p.getSize());

        p.setMem(nullptr);
        p.setSize(0);
    }
    return found;
}
