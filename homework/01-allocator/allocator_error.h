#ifndef ALLOCATOR_ERROR
#define ALLOCATOR_ERROR

#include <stdexcept>
#include <string>

enum class AllocErrorType {
    InvalidFree,
    NoMemory,
    InternalError,
};

class AllocError : std::runtime_error {
private:
    AllocErrorType type;

public:
    AllocError(AllocErrorType _type, std::string message)
        : runtime_error(message)
        , type(_type) {
    }

    AllocErrorType getType() const { return type; }
    const char * message() const {
        return this->what();
    }
};

#endif //ALLOCATOR_ERROR
