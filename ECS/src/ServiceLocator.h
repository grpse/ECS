#pragma once

class ServiceLocator {

public:
    template <typename ServiceType>
    inline static ServiceType* get() {
        static ServiceType service;
        return &service;
    }
};
