#include "ServerEntity.h"

namespace redsquare
{
    ServerEntity::ServerEntity()
    {

    }

    ServerEntity::ServerEntity(gf::Id entityID, const EntityClass typeOfEntity)
    : Entity(entityID, typeOfEntity)
    {

    }

    void ServerEntity::createCarPacket(Packet &packet)
    {

    }
}