#pragma once

class BaseComponent {

public:
    virtual ~BaseComponent() { }
};

template <typename CompType>
class Component : public BaseComponent {

private:
    CompType mData;

public:

    Component() {

    }

    Component(const Component<CompType>& other) {
        mData = other.mData;
    }

    virtual ~Component() {
        // TODO: DONT remove from manager
    }

    Component<CompType>& operator=(const Component<CompType>& other) {
        mData = other.mData;
        return *this;
    }

    CompType* data() { return &mData; }
};