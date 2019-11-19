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
}

#endif // REDSQUARE_COMMON_MESSAGE_H