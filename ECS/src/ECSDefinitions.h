#pragma once
#include <typeindex>
#include <typeindex>
#include <map>
#include <iostream>
#include "Identifier.h"

typedef Identifier EntityId;
typedef Identifier ComponentId;
typedef Identifier ComponentTypeId;
typedef std::pair<EntityId, ComponentTypeId> EntityIdComponentTypeKeyType;

namespace std
{
    template <>
    struct less<EntityIdComponentTypeKeyType>
    {
        bool operator()(const EntityIdComponentTypeKeyType &lhs, const EntityIdComponentTypeKeyType &rhs) const
        {
            static size_t hashNumber = 23;
            size_t hashLeftKey = hashNumber * lhs.first.id() ^ (lhs.second.id() << 16);
            size_t hashRightKey = hashNumber * rhs.first.id() ^ (rhs.second.id() << 16);
            return hashLeftKey < hashRightKey;
        }
    };

    template <>
    struct less<Identifier>
    {
        bool operator()(const Identifier &lhs, const Identifier &rhs) const
        {
            return lhs.id() < rhs.id();
        }
    };

    std::ostream& operator << (std::ostream& os, EntityIdComponentTypeKeyType& key) {
        os << "EntityId { " << key.first.id() << "}; ComponentTypeId {" << key.second.id() << "}";
        return os;
    }
}