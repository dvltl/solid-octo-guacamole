#include "include/skiplist/skiplist.h"
#include <cassert>
#include <iostream>

int main() {
    SkipList<int, float, 4> skList;

    for (int i = 0; i < 10; ++i) {
        assert(nullptr == skList.Put(i, float(-i)));
    }

    std::cout << skList.dump();
    std::cout << "Put in empty list works OK" << std::endl;

    float * buf;
    for (int i = 0; i < 20; ++i) {
        if (i < 10) {
            buf = skList.PutIfAbsent(i, float(i * 0.3));
            assert(nullptr != buf);
            std::cout << *buf << ' ' << -i << std::endl;
            assert(*buf == float(-i));
            delete buf;
        } else {
            assert(nullptr == skList.PutIfAbsent(i, i * 0.3));
        }
    }

    std::cout << skList.dump();

    std::cout << "PutIfAbsent works OK" << std::endl;

    for (int i = 0; i < 10; ++i) {
        buf = skList.Put(i, i + 2.0);
        assert(buf != nullptr);
        std::cout << *buf << ' ' << -i << std::endl;
        assert(*buf == float(-i));
        delete buf;
    }

    std::cout << "Put existing keys works OK" <<std::endl;
    std::cout << skList.dump();

    for (int i = 0; i < 20; ++i) {
        buf = skList.Get(i);
        assert(buf != nullptr);
        if (i < 10) {
            assert(*buf == i + 2.0);
        } else {
            assert(*buf == float(i * 0.3));
        }
        delete buf;
    }

    std::cout << "Get after Put work OK" << std::endl;


    return 0;
}
