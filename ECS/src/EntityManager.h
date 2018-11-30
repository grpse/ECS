#pragma once

#include <vector>
#include <set>
#include <utility>
#include "EntityBase.h"
#include "ComponentManager.h"
#include "ServiceLocator.h"
#include "ScopedLock.h"
#include "ECSDefinitions.h"

//class Entity;

class ComponentManager;

class EntityManager {

private:
    std::map<EntityId, EntityBase> mEntities;
    EntityId nextId;
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
    
    inline EntityBase* New() {

        SCOPED_LOCK;

        EntityId currentId = nextId++;
        mEntities[currentId] = { currentId, this };
        return &mEntities[currentId];
    }

    inline void Remove(EntityId entityId) {
        
        SCOPED_LOCK;
        mEntities.erase(entityId);
    }

    template <typename CompType, typename... TypeArgs>
    inline CompType* AttachComponent(EntityId entityId, TypeArgs&&... args) {
        return mComponentManager->AttachToEntity<CompType>(entityId, std::forward<TypeArgs>(args)...);
    }

    template <typename CompType>
    inline CompType* AttachComponent(EntityId entityId) {
        return mComponentManager->AttachToEntity<CompType>(entityId);
    }

    template <typename CompType>
    inline CompType* GetComponent(EntityId entityId) {
        return mComponentManager->GetComponentForEntityId<CompType>(entityId);
    }
};