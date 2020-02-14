#ifndef REDSQUARE_CLIENT_MESSAGE_H
#define REDSQUARE_CLIENT_MESSAGE_H

#include "../common/ProtocolData.h"
#include "../common/Entity.h"
#include "ClientEntity.h"
#include "Player.h"

#include <gf/Message.h>
#include <gf/Id.h>

using namespace gf::literals;

namespace redsquare
{
    struct SpellUpdateMessage : public gf::Message
    {
        static const gf::Id type = "SpellUpdate"_id;
        std::vector<SpellType> spells;
    };
    
    struct ItemUpdateUIMessage : public gf::Message
    {
        static const gf::Id type = "ItemUpdate"_id;
        gf::Id entityID;
        InventorySlotType slotType;
        uint slotPos;
        bool itemRemoved;
        ClientItem *item;
    };

    struct MyPlayerReceivedTypeMessage : public gf::Message
    {
        static const gf::Id type = "MyPlayerReceived"_id;
        Player *player;
    };

    struct MyPlayerDeadMessage : public gf::Message
    {
        static const gf::Id type = "MyPlayerDead"_id;
    };
}

#endif // REDSQUARE_CLIENT_MESSAGE_H