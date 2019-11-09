#include "Prop.h"

namespace redsquare
{
    Prop::Prop(gf::Id entityID, EntityClass type, gf::Vector2i pos)
    : redsquare::Entity(entityID,type)
    {
        m_Pos = pos;
    }
}