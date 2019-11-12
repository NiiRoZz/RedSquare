#ifndef REDSQUARE_SERVER_PROP_H
#define REDSQUARE_SERVER_PROP_H

#include "../common/Entity.h"
#include "World.h"

namespace redsquare
{
    class Prop: public Entity
    {
    public: 
        Prop(gf::Id entityID, EntitySubType type);
        void spawnProps( World &world,gf::Vector4i currentRoom);
    };

    
}

#endif