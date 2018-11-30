#pragma once

#include "EntityManager.h"
#include "EntityBase.h"
#include "ECSDefinitions.h"

class Entity : public EntityBase {

public:

    // Entity(EntityId nextId, EntityManager* entityManager) :
    //     mId(nextId), mEntityManager(entityManager) {
        
    // }

    // Entity(const Entity& other) : 
    //     mId(other.mId) {

    // }

    // Entity(const Entity&& other) : mId(other.mId) {

    // }

    virtual ~Entity() {
        //TODO: Remove entity and it's components from component manager
    }

    EntityId id() const { return mId; }

    template <typename CompType, typename... TypeArgs>
    inline CompType* AttachComponent(TypeArgs&&... args) {
        return mEntityManager->AttachComponent<CompType>(mId, std::forward<TypeArgs>(args)...);
    }

    template <typename CompType>
    inline CompType* AttachComponent() {
        return mEntityManager->AttachComponent<CompType>(mId);
    }

    template <typename CompType>
    inline CompType* GetComponent() {
        return mEntityManager->GetComponent<CompType>(mId);
    }

};