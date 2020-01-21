#include "ServerEntity.h"

namespace redsquare
{
    ServerEntity::ServerEntity()
    {

    }

    ServerEntity::ServerEntity(gf::Id entityID)
    : Entity(entityID)
    {

    }

    ServerEntity::ServerEntity(gf::Id entityID, const EntitySubType type)
    : Entity(entityID,type)
    {

    }

    void ServerEntity::createCarPacket(Packet &packet)
    {

    }
}