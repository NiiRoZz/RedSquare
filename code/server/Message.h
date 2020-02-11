#ifndef REDSQUARE_SERVER_MESSAGE_H
#define REDSQUARE_SERVER_MESSAGE_H

#include "ServerEntity.h"

#include <gf/Message.h>
#include <gf/Id.h>

using namespace gf::literals;

namespace redsquare
{
    struct UpdateEntityCharacteristic : public gf::Message
    {
        static const gf::Id type = "UpdateEntityCharacteristic"_id;
        ServerEntity *entity;
    };
}

#endif // REDSQUARE_SERVER_MESSAGE_H