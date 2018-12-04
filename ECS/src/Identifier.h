#pragma once
#include <iostream>

class Identifier {

public:
    Identifier() : mId() { }
    Identifier(size_t id) : mId(id) { }
    Identifier(const Identifier& other) { mId = other.mId; }
    
    size_t id() const { return mId; }

    Identifier& operator=(const Identifier& other) {
        mId = other.mId;
        return *this;
    }

    Identifier& operator=(size_t id) {
        mId = id;
        return *this;
    }

    inline bool operator<(const Identifier& other) {
        return id() < other.id();
    }

    inline bool operator() (const Identifier& lhs, const Identifier& rhs) const
    {
        return lhs.id() < rhs.id();
    }

private:
    size_t mId;
};