#pragma once

#include <mutex>

#define SCOPED_LOCK                                     \
        static std::mutex globalMutex;                  \
        std::lock_guard<std::mutex> lock(globalMutex);

#define SCOPED_LOCK_LOCAL(mutexName)                    \
        static std::mutex mutexName;                    \
        std::lock_guard<std::mutex> lock(mutexName);