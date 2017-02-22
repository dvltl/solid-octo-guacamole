#include "allocator.h"
#include "allocator_pointer.h"
#include "allocator_error.h"
#include <iostream>

Allocator::Allocator(void * b, size_t size){
    this -> base = (char *) b;
    this -> size = size;
    this -> freeSize = size;
    this -> pNum = 0;
    this -> pList = (PointerList *) (base + (size - sizeof(PointerList)) );
}

Pointer Allocator::alloc (size_t N) {
//    std::cout << "Alloc" << std::endl;
    if (freeSize > N + sizeof(Pointer)) {
        Pointer p = Pointer(base, size - (freeSize - pNum * sizeof(Pointer)), N ); // what if fragmentation?
//        std::cout << "Mid: " << (void*) base << ' ' << p.get() << std::endl;
        appendPointer(pList, p);
    //    std::cout << "Done" << std::endl;
        return p;
    } else {
        throw AllocError(AllocErrorType::NoMemory, "Not enough free memory to allocate");
    }
}

bool Allocator::lookUp(PointerList * list, Pointer & p) {
    PointerList * buf = list;
    bool found = false;
    while (buf != nullptr) {
        if (buf->get().get() == p.get() && buf->get().getSize() == p.getSize()) {
            found = true;
            break;
        }
        buf = buf -> getNext();
    }
    return found;
}

void Allocator::realloc (Pointer& p, size_t N) {
    if (lookUp(pList, p)) {
        if (p.getNextFree() - (char*) p.get() == p.getSize() && N <= p.getSize() + p.getFreeSize()) {
            p.setNextFree((char*) p.get() + N);
            p.setFreeSize(p.getSize() + p.getFreeSize() - N);
        } else {
            free(p);
            p = alloc(N);
        }
    } else {
        p = alloc(N);
    }
}

void Allocator::free (Pointer& p) {
    if (!deletePointer(pList, p)) {
        throw AllocError(AllocErrorType::InvalidFree, "Attempt of freeing the non-existent pointer");
    }
}

void Allocator::defrag () {
    
}

void Allocator::sanityCheck(PointerList * list) {
    PointerList * buf = list;
    PointerList * prev;

    while (buf != nullptr) {
        if ((char*) buf > base + size || (char*) buf < base) {
            prev -> setNext(nullptr);
            break;
        }
        prev = buf;
        buf = buf -> getNext();
    }
}

PointerList * Allocator::findFreeSpace(PointerList * list) {
    PointerList * buf = list;
    PointerList * prev = nullptr;
    std::ostringstream log;
    size_t iter = 0;
 //   std::cout << "FFS: " << std::endl;
    while (buf != nullptr) {
        if (prev != nullptr){
//            std::cout << "Diff: " << prev - buf << std::endl; // 'prev' has bigger addr than 'buf'
            if ( prev - buf > 1) {
                return prev - 1;
            }
        }
        prev = buf;

        log << "Buf[" << iter++ << "] = " << (void*) buf << std::endl;
        if ((char*) buf < base || (char*) buf > base + size){
            std::ostringstream oss;
            oss << "Base: " << (void*) base << ' ' << "Buf: " << (void*) buf << std::endl;
            sanityCheck(list);
            throw 1;
            //throw AllocError(AllocErrorType::MemoryOverwrite, "Memory overwritten.\n" + log.str() + oss.str());
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
