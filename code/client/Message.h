#ifndef REDSQUARE_COMMON_MESSAGE_H
#define REDSQUARE_COMMON_MESSAGE_H

#include "../common/Packet.h"
#include "../common/Entity.h"
#include "ClientEntity.h"

#include <gf/Message.h>
#include <gf/Id.h>

using namespace gf::literals;

namespace redsquare
{
    struct SpellUpdateMessage : public gf::Message
    {
        static const gf::Id type = "SpellUpdate"_id;
        std::array<SpellType, MAX_SPELL_PER_PLAYER> spells;
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
        ClientEntity *player;
    };

    struct MyPlayerDeadMessage : public gf::Message
    {
        static const gf::Id type = "MyPlayerDead"_id;
    };
}

#endif // REDSQUARE_COMMON_MESSAGE_H