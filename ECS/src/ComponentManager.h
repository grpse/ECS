#pragma once

#include <vector>
#include <map>
#include <set>
#include <utility>
#include <typeinfo>
#include <typeindex>
#include "Component.h"
#include "ScopedLock.h"
#include "Memory/PoolFactory.h"
#include "Memory/PoolAllocator.h"
#include "ECSDefinitions.h"
#include "Identifier.h"

template <typename CompType>
class Component;

class ComponentManager {

private:
    typedef std::pair<EntityId, ComponentTypeId> EntityIdComponentTypeKeyType; 

public:

    explicit ComponentManager(PoolFactory* poolFactory) { 
        mPoolFactory = poolFactory;
    }

    virtual ~ComponentManager() { }

    template <typename CompType, typename... TypeArgs>
    inline CompType* AttachToEntity(Identifier entityId, TypeArgs&&... args) {
        
        SCOPED_LOCK;

        // Get the right pool allocator for components creation
        ComponentTypeId componentTypeId = GetComponentTypeId<CompType>();
        auto pool = GetRightPoolAllocator<CompType>();

        // Create components
        std::shared_ptr<BaseComponent> component = pool->template create< Component<CompType> >(std::forward<TypeArgs>(args)...);

        // Save their references
        EntityIdComponentTypeKeyType key = {entityId, componentTypeId};
        mEntityIdComponents[key] = component;
        mComponentTypeIdEntitys[componentTypeId].insert(entityId);
        mEntityIdComponentsTypeId[entityId].push_back(componentTypeId);
        
        return ComponentManager::StaticCastComponentType<CompType>(component);
    }

    template <typename CompType>
    inline CompType* AttachToEntity(Identifier entityId) {
        
        SCOPED_LOCK;

        // Get the right pool allocator for components creation
        ComponentTypeId componentTypeId = GetComponentTypeId<CompType>();
        auto pool = GetRightPoolAllocator<CompType>();

        // Create components
        std::shared_ptr<BaseComponent> component = pool->template create< Component<CompType> >();

        // Save their references
        EntityIdComponentTypeKeyType key = {entityId, componentTypeId};
        mEntityIdComponents[key] = component;
        mComponentTypeIdEntitys[componentTypeId].insert(entityId);
        mEntityIdComponentsTypeId[entityId].push_back(componentTypeId);
        
        return ComponentManager::StaticCastComponentType<CompType>(component);
    }

    inline void DetachFromEntity(Identifier entityId) {

        SCOPED_LOCK;

        // get the iterators and delete
        for (ComponentTypeId componentTypeId : mEntityIdComponentsTypeId[entityId]) {
            mComponentTypeIdEntitys.erase(componentTypeId);
            mEntityIdComponents.erase({entityId, componentTypeId});
        }

        mEntityIdComponentsTypeId.erase(entityId);
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
        ComponentTypeId componentTypeId = GetComponentTypeId<CompType>();
        EntityIdComponentTypeKeyType key = {entityId, componentTypeId};
        return ComponentManager::StaticCastComponentType<CompType>(mEntityIdComponents[key]);
    }

    template <typename CompType>
    inline ComponentTypeId GetComponentTypeId() const {
        static ComponentTypeId _unique_id = typeid(CompType).hash_code();
        return _unique_id;
    }

    template <typename CompType>
    inline static CompType* StaticCastComponentType(std::shared_ptr<BaseComponent>& component) {
        return static_cast< Component<CompType>* >(&*component)->data();
    }

private:
    std::map<ComponentTypeId, PoolAllocator* > mComponentPools;
    std::map<EntityId, std::vector<ComponentTypeId> > mEntityIdComponentsTypeId;
    std::map<EntityIdComponentTypeKeyType, std::shared_ptr<BaseComponent> > mEntityIdComponents;
    std::map<ComponentTypeId, std::set<EntityId> > mComponentTypeIdEntitys;
    
    PoolFactory* mPoolFactory;

    template <typename CompType>
    PoolAllocator* GetRightPoolAllocator() {
        
        ComponentTypeId componentTypeId = GetComponentTypeId<CompType>();

        if (IsComponentPoolNotExistent(componentTypeId)) {
            mComponentPools[componentTypeId] = mPoolFactory->createPool<CompType>(128);
        }
        return mComponentPools[componentTypeId];
    }

    bool IsComponentPoolNotExistent(ComponentTypeId componentTypeId) {
        return mComponentPools.count(componentTypeId) <= 0;
    }
};