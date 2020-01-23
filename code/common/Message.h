#ifndef REDSQUARE_COMMON_MESSAGE_H
#define REDSQUARE_COMMON_MESSAGE_H

#include "Packet.h"

#include <gf/Message.h>

using namespace gf::literals;

namespace redsquare
{
    struct SpellUpdateMessage : public gf::Message
    {
        static const gf::Id type = "SpellUpdate"_id;
        std::array<SpellType, MAX_SPELL_PER_PLAYER> spells;
    };

    struct InventoryUpdateMessage : public gf::Message
    {
        static const gf::Id type = "InventoryUpdate"_id;
    };
    
    struct ItemUpdateMessage : public gf::Message
    {
        static const gf::Id type = "ItemUpdate"_id;
        UpdateItem itemMessage;
    };

    struct ItemMoveMessage : public gf::Message
    {
        static const gf::Id type = "ItemMove"_id;
        MoveItem itemMessage;
    };

    struct MyPlayerReceivedTypeMessage : public gf::Message
    {
        static const gf::Id type = "MyPlayerReceived"_id;
        EntitySubType entityType;
    };

    struct MyPlayerDeadMessage : public gf::Message
    {
        static const gf::Id type = "MyPlayerDead"_id;
    };
}

#endif // REDSQUARE_COMMON_MESSAGE_H