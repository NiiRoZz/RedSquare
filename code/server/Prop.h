#ifndef REDSQUARE_SERVER_PROP_H
#define REDSQUARE_SERVER_PROP_H

#include "../common/Entity.h"

namespace redsquare
{
    class Prop: public Entity
    {
    public: 
        Prop(gf::Id entityID, EntityClass type, gf::Vector2i pos);
    };
}

#endif