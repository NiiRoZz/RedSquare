#ifndef REDSQUARE_SERVER_PROP_H
#define REDSQUARE_SERVER_PROP_H

#include "ServerEntity.h"

namespace redsquare
{
    class Prop: public ServerEntity
    {
    public: 
        Prop(gf::Id entityID, EntitySubType entitySubType);
    };
}

#endif