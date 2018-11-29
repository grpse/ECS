#pragma once

#include <vector>
#include <map>
#include <set>
#include <utility>
#include "Component.h"
#include "ScopedLock.h"
#include "Memory/PoolFactory.h"
#include "Memory/PoolAllocator.h"

#define POOLS_TOTAL_SIZE 65536

template <typename CompType>
class Component;

class ComponentManager {

private:
    typedef size_t ComponentMemoryAddress;
    typedef size_t EntityId;
    typedef size_t ComponentTypeId;

public:

    explicit ComponentManager(PoolFactory* poolFactory) { 
        mPoolFactory = poolFactory;
    }

    virtual ~ComponentManager() { }

    template <typename CompType, typename... TypeArgs>
    inline CompType* AttachToEntity(size_t entityId, TypeArgs&&... args) {
        
        SCOPED_LOCK;

        // Get the right pool allocator for components creation
        ComponentTypeId componentTypeId = GetComponentTypeId<CompType>();
        PoolAllocator* pool = GetRightPoolAllocator<CompType>();

        // Create components
        std::shared_ptr<BaseComponent> component = pool->create< Component<CompType> >(std::forward<TypeArgs>(args)...);

        // Save their references
        mEntityIdComponents[entityId] = component;
        mComponentTypeIdEntitys[componentTypeId].insert(entityId);
        mEntityIdComponentsTypeId[entityId] = componentTypeId;
        
        return ComponentManager::StaticCastComponentType<CompType>(component);
    }

    template <typename CompType>
    inline CompType* AttachToEntity(size_t entityId) {
        
        SCOPED_LOCK;

        // Get the right pool allocator for components creation
        ComponentTypeId componentTypeId = GetComponentTypeId<CompType>();
        PoolAllocator* pool = GetRightPoolAllocator<CompType>();

        // Create components
        std::shared_ptr<BaseComponent> component = pool->create< Component<CompType> >();

        // Save their references
        mEntityIdComponents[entityId] = component;
        mComponentTypeIdEntitys[componentTypeId].insert(entityId);
        mEntityIdComponentsTypeId[entityId] = componentTypeId;
        
        return ComponentManager::StaticCastComponentType<CompType>(component);
    }

    inline void DetachFromEntity(size_t entityId) {

        SCOPED_LOCK;

        // get the iterators and delete
        ComponentTypeId componentTypeId = mEntityIdComponentsTypeId[entityId];
        mComponentTypeIdEntitys[componentTypeId].erase(entityId);
        mEntityIdComponents.erase(entityId);
    }    

    template <typename CompType>
    std::set<EntityId> GetEntityIdForComponent() {
        
        SCOPED_LOCK;
        ComponentTypeId componentTypeId = GetComponentTypeId<CompType>();
        return mComponentTypeIdEntitys[componentTypeId];
    }

    template <typename CompType>
    inline CompType* GetComponentForEntityId(size_t entityId) {
        
        SCOPED_LOCK;

        return ComponentManager::StaticCastComponentType<CompType>(mEntityIdComponents[entityId]);
    }

    template <typename CompType>
    inline ComponentTypeId GetComponentTypeId() const {
        static ComponentTypeId _unique_id = reinterpret_cast<ComponentTypeId>(typeid(CompType).name());
        return _unique_id;
    }

    template <typename CompType>
    inline static CompType* StaticCastComponentType(std::shared_ptr<BaseComponent>& component) {
        return reinterpret_cast< Component<CompType>* >(&*component)->data();
    }

private:

    std::map<ComponentTypeId, PoolAllocator*> mComponentPools;
    std::map<EntityId, ComponentTypeId > mEntityIdComponentsTypeId;
    std::map<EntityId, std::shared_ptr<BaseComponent> > mEntityIdComponents;
    std::map<ComponentTypeId, std::set<EntityId> > mComponentTypeIdEntitys;
    
    PoolFactory* mPoolFactory;

    template <typename CompType>
    PoolAllocator* GetRightPoolAllocator() {
        PoolAllocator* pool = nullptr;
        ComponentTypeId componentTypeId = GetComponentTypeId<CompType>();
        if (IsComponentPoolNotExistent(componentTypeId)) {
            mComponentPools[componentTypeId] = mPoolFactory->createPool< Component<CompType> >(4096);
        }
        pool = mComponentPools[componentTypeId];
        return pool;
    }

    bool IsComponentPoolNotExistent(ComponentTypeId componentTypeId) {
        return mComponentPools.find(componentTypeId) == mComponentPools.end();
    }
};