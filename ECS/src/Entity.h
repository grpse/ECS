#pragma once
#include <vector>
#include <set>
#include <utility>
#include "ComponentManager.h"
#include "ScopedLock.h"
#include "ECSDefinitions.h"

class ComponentManager;
class EntityManager;

class Entity {

public:
    EntityId mId;
    EntityManager* mEntityManager;
    
    EntityId id() const { return mId; }
    
    inline void destroy();

    template <typename CompType, typename... TypeArgs>
    inline CompType* AttachComponent(TypeArgs&&... args);

    template <typename CompType>
    inline CompType* AttachComponent();

    template <typename CompType>
    inline CompType* GetComponent();

private:


    friend class EntityManager;
};


class EntityManager {

private:
    std::map<EntityId, Entity> mEntities;
    size_t nextId;
    ComponentManager* mComponentManager;
    PoolFactory* mPoolFactory;

public:
    EntityManager() {
        nextId = 0;
        mPoolFactory = new PoolFactory(1048576);
        mComponentManager = new ComponentManager(mPoolFactory);
    }

    virtual ~EntityManager() {
        delete mComponentManager;
        delete mPoolFactory;
    }
    
    inline Entity* New() {

        SCOPED_LOCK;

        EntityId currentId = ++nextId;
        mEntities[currentId] = { currentId, this };
        return &mEntities[currentId];
    }

    inline void Remove(EntityId entityId) {
        
        SCOPED_LOCK;
        mEntities.erase(entityId);
        mComponentManager->DetachFromEntity(entityId);
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


template <typename CompType, typename... TypeArgs>
inline CompType* Entity::AttachComponent(TypeArgs&&... args) {
    return mEntityManager->AttachComponent<CompType>(mId, std::forward<TypeArgs>(args)...);
}

template <typename CompType>
inline CompType* Entity::AttachComponent() {
    return mEntityManager->AttachComponent<CompType>(mId);
}

template <typename CompType>
inline CompType* Entity::GetComponent() {
    return mEntityManager->GetComponent<CompType>(mId);
}

inline void Entity::destroy() {
    mEntityManager->Remove(id());
}