#include "skiplist/skiplist.h"
#include <cassert>
#include <iostream>

int main() {
    SkipList<int, float, 4> skList;

    for (int i = 0; i < 20; ++i) {
        assert(nullptr == skList.Put(i, -i));
        std::cout << i << std::endl;
        std::cout << skList.dump() << std::endl;
    }

    std::cout << "Put in empty list work OK" << std::endl;

    float * buf;
    for (int i = 0; i < 20; ++i) {
        buf = skList.Get(i);
        assert(buf != nullptr);
        assert(*buf == (float) -i);
    }

    std::cout << "Get after Put work OK" << std::endl;

    for (int i = 0; i < 20; ++i) {
        buf = skList.Put(i, i*2.0);
        assert(buf != nullptr);
        assert(*buf == (float) -i);
    }

    std::cout << "Put existing keys works OK" <<std::endl;

    return 0;
}
