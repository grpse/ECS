#pragma once

#include "ECSDefinitions.h"

class EntityManager;

class EntityBase {

public:
    EntityId mId;
    EntityManager* mEntityManager;
};