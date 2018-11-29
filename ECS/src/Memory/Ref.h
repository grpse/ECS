#pragma once

        #include <iostream>
        #include <typeinfo>

#include <memory>
#include <utility>

class DefaultAllocator {

public:

	inline static DefaultAllocator* getStaticInstance() {
		static DefaultAllocator allocator;
		return &allocator;
	}

	template <class T, typename... TypeArgs>
	inline void construct(T*& nilRef, TypeArgs&&... args) {
		nilRef = (T*)::malloc(sizeof(T));
		::new(nilRef) T(std::forward<TypeArgs>(args)...);
	}

    template <class T>
	inline void construct(T*& nilRef) {
		nilRef = (T*)::malloc(sizeof(T));
		::new(nilRef) T();
	}

	template <class T>
	inline bool destruct(T* rawRef) {
		delete rawRef;
		return true;
	}
};

template <class T, class AllocatorType = DefaultAllocator>
class Ref {

public:
    explicit Ref() {
        AllocatorType::getStaticInstance()->construct(ref);
    }

	template <typename... TypeArgs>
	explicit Ref(TypeArgs&&... args) {
		AllocatorType::getStaticInstance()->construct(ref, std::forward<TypeArgs>(args)...);
	}

	Ref(const Ref<T, AllocatorType>& other) {
		ref = other.ref;
	}

	Ref(const T* rawRef) {
		ref = rawRef;
	}

	~Ref() {
        const char* cName = typeid(T).name();
        std::cout << "Destroy : " << std::string(cName) << std::endl;
		AllocatorType::getStaticInstance()->destruct(ref);
	}

    inline T& operator=(const T& other) {
        ref = other.ref;
        return *ref;
    }

	inline T* operator->() {
		return ref;
	}

	inline T& operator*() {
		return *ref;
	}

private:
	T* ref = nullptr;
};