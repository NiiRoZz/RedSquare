#ifndef REDSQUARE_CLIENT_SPELLWIDGET_H
#define REDSQUARE_CLIENT_SPELLWIDGET_H

#include "../common/Packet.h"

#include <gf/Widgets.h>

namespace redsquare
{
    class SpellWidget: public gf::SpriteWidget
    {
    public:
        SpellWidget(SpellType type);

        SpellType m_SpellType;
        std::string m_Description;
        std::string m_ManaCost;
    };
}

#endif