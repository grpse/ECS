#pragma once

#include "MemoryCommonHeaders.h"
#include "PoolAllocator.h"


class PoolFactory {

public:

    explicit PoolFactory(size_t blockSize) {
        mBlock = new byte[blockSize];
        mBlockIndex = 0;
        mBlockSize = blockSize;
    }

    virtual ~PoolFactory() {
        delete mBlock;
    }

    PoolAllocator* createPool(size_t elementSize, size_t nElements) {
        size_t blockSize = nElements * elementSize;
        byte* blockPoolStartPtr = &mBlock[mBlockIndex];
        mBlockIndex = blockSize;
        mCreatedPools.emplace_back(blockPoolStartPtr, blockSize);
        return &mCreatedPools[mCreatedPools.size() - 1];
    }

private:
    size_t mBlockSize;
    byte* mBlock;
    size_t mBlockIndex;
    std::vector<PoolAllocator> mCreatedPools;
};