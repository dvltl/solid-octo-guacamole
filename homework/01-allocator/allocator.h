#ifndef ALLOCATOR
#define ALLOCATOR
#include <string>
#include <sstream>

// Forward declaration. Do not include real class definition
// to avoid expensive macros calculations and increase compile speed
//class Pointer;
//class PointerList;

#include "allocator_pointer.h"
#include "pointer_list.h"

/**
 * Wraps given memory area and provides defagmentation allocator interface on
 * the top of it.
 *
 *
 */
class Allocator {
    char * base;
    size_t size;
    PointerList * pList;
    size_t pNum;
    size_t freeSize;
    
    void appendPointer(PointerList *, Pointer &);
    bool deletePointer(PointerList *, Pointer &);
    PointerList * findFreeSpace(PointerList *);
    void addAsFree(PointerList *, void *, size_t);
    void sanityCheck(PointerList *);
    bool lookUp(PointerList *, Pointer &);
    
public:
    Allocator(void * b, size_t size);

    /**
     * TODO: semantics
     * @param N size_t
     */
    Pointer alloc(size_t N);

    /**
     * TODO: semantics
     * @param p Pointer
     * @param N size_t
     */
    void realloc(Pointer& p, size_t N);

    /**
     * TODO: semantics
     * @param p Pointer
     */
    void free(Pointer& p);

    /**
     * TODO: semantics
     */
    void defrag();

    /**
     * TODO: semantics
     */
    std::string dump() const {
        std::ostringstream oss;
        oss << "PNum: " << pNum << std::endl << "Dump: ";
        PointerList * buf = pList;
        while (buf != nullptr) {
            oss << buf->get().get();
            oss << ", ";
            buf = buf->getNext();
        }
        return oss.str();
    }
};

#endif // ALLOCATOR
