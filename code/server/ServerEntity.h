#ifndef REDSQUARE_SERVER_SERVERENTITY_H
#define REDSQUARE_SERVER_SERVERENTITY_H

#include "../common/Entity.h"
#include "../common/Packet.h"

namespace redsquare
{
    class ServerEntity: public Entity
    {
    public:
        ServerEntity();
        ServerEntity(gf::Id entityID, const uint8_t typeOfEntity);

        virtual void createCarPacket(Packet &packet);
    };
}

#endif