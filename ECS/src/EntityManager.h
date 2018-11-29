#pragma once

#include <vector>
#include <map>
#include <utility>
#include "Entity.h"
#include "ComponentManager.h"
#include "ServiceLocator.h"
#include "ScopedLock.h"

class ComponentManager;

class Entity;

class EntityManager {

private:
    std::map<size_t, Entity> mEntities;
    int nextId;
    ComponentManager* mComponentManager;
    PoolFactory* mPoolFactory;

public:
    EntityManager() {
        nextId = 0;
        mPoolFactory = new PoolFactory(65536);
        mComponentManager = new ComponentManager(mPoolFactory);
    }

    virtual ~EntityManager() {
        delete mComponentManager;
        delete mPoolFactory;
    }
    
    inline Entity* New() {

        SCOPED_LOCK;

        int currentId = nextId++;
        mEntities.emplace(std::make_pair(currentId, currentId));
        return &mEntities[currentId];
    }

    inline void Remove(long entityId) {
        
        SCOPED_LOCK;

        auto entityIterator = mEntities.find(entityId);
        mEntities.erase(entityIterator);
    }

    template <typename CompType, typename... TypeArgs>
    inline CompType* AttachComponent(long entityId, TypeArgs&&... args) {
        return mComponentManager->AttachToEntity<CompType>(entityId, std::forward<TypeArgs>(args)...);
    }

    template <typename CompType>
    inline CompType* AttachComponent(long entityId) {
        return mComponentManager->AttachToEntity<CompType>(entityId);
    }

    template <typename CompType>
    inline CompType* GetComponent(long entityId) {
        return mComponentManager->GetComponentForEntityId<CompType>(entityId);
    }
};