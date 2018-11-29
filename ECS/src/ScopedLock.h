#pragma once

#include <mutex>

#define SCOPED_LOCK                                     \
        static std::mutex globalMutex;                  \
        std::lock_guard<std::mutex> lock(globalMutex);