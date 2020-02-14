#ifndef REDSQUARE_REDSQUARE_PROTOCOL_H
#define REDSQUARE_REDSQUARE_PROTOCOL_H

#include <gf/Id.h>
#include <gf/Array2D.h>
#include <gf/Unused.h>

#include "ProtocolData.h"

using namespace gf::literals;

namespace redsquare
{
    /*
        Server ===> Client
    */
    struct RedsquareServerInitMap
    {
        static constexpr gf::Id type = "RedsquareServerInitMap"_id;
        gf::Array2D<Tile> world;
        uint floor;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, RedsquareServerInitMap& data)
    {
        return ar | data.world | data.floor;
    }

    struct RedsquareServerInitEntities
    {
        static constexpr gf::Id type = "RedsquareServerInitEntities"_id;
        std::vector<EntityData> allEntities;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, RedsquareServerInitEntities& data)
    {
        return ar | data.allEntities;
    }

    struct RedsquareServerInitEntity
    {
        static constexpr gf::Id type = "RedsquareServerInitEntity"_id;
        EntityData entity;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, RedsquareServerInitEntity& data)
    {
        return ar | data.entity;
    }

    struct RedsquareServerPlayerTurn
    {
        static constexpr gf::Id type = "RedsquareServerPlayerTurn"_id;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, RedsquareServerPlayerTurn& data)
    {
        gf::unused(data);
        return ar;
    }

    struct RedsquareServerPlayerDead
    {
        static constexpr gf::Id type = "RedsquareServerPlayerDead"_id;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, RedsquareServerPlayerDead& data)
    {
        gf::unused(data);
        return ar;
    }

    struct RedsquareServerMove
    {
        static constexpr gf::Id type = "RedsquareServerMove"_id;
        EntityType entityType;
        gf::Id id = gf::InvalidId;
        gf::Vector2i pos;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, RedsquareServerMove& data)
    {
        return ar | data.entityType | data.id | data.pos;
    }

    struct RedsquareServerUpdateCharacteristic
    {
        static constexpr gf::Id type = "RedsquareServerUpdateCharacteristic"_id;
        EntityType entityType;
        gf::Id id = gf::InvalidId;
        EntityCharacteristicData characteristics;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, RedsquareServerUpdateCharacteristic& data)
    {
        return ar | data.entityType | data.id | data.characteristics;
    }

    struct RedsquareServerDeleteEntity
    {
        static constexpr gf::Id type = "RedsquareServerDeleteEntity"_id;
        EntityType entityType;
        gf::Id id = gf::InvalidId;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, RedsquareServerDeleteEntity& data)
    {
        return ar | data.entityType | data.id;
    }

    struct RedsquareServerUpdateItem
    {
        static constexpr gf::Id type = "RedsquareServerUpdateItem"_id;
        gf::Id id = gf::InvalidId;
        EntityType entityType;
        InventorySlotType slotType;
        uint pos;
        ItemType typeItem;
        uint baseFloorItem;
        bool removeItem;
        uint8_t slotMask;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, RedsquareServerUpdateItem& data)
    {
        return ar | data.id | data.entityType | data.slotType | data.pos | data.typeItem | data.baseFloorItem | data.removeItem | data.slotMask;
    }

    /*
        Client ===> Server
    */
    struct RedsquareClientMove
    {
        static constexpr gf::Id type = "RedsquareClientMove"_id;
        gf::Vector2i dir;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, RedsquareClientMove& data)
    {
        return ar | data.dir;
    }

    struct RedsquareClientAttack
    {
        static constexpr gf::Id type = "RedsquareClientAttack"_id;
        gf::Vector2i pos;
        SpellType spellType;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, RedsquareClientAttack& data)
    {
        return ar | data.pos | data.spellType;
    }

    struct RedsquareClientPassTurn
    {
        static constexpr gf::Id type = "RedsquareClientPassTurn"_id;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, RedsquareClientPassTurn& data)
    {
        return ar;
    }

    struct RedsquareClientMoveItem
    {
        static constexpr gf::Id type = "RedsquareClientMoveItem"_id;
        gf::Id oldEntityID = gf::InvalidId;
        EntityType oldEntityType;
        InventorySlotType oldSlotType;
        uint oldPos;
        gf::Id newEntityID = gf::InvalidId;
        EntityType newEntityType;
        InventorySlotType newSlotType;
        uint newPos;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, RedsquareClientMoveItem& data)
    {
        return ar | data.oldEntityID | data.oldEntityType | data.oldSlotType | data.oldPos | data.newEntityID | data.newEntityType | data.newSlotType | data.newPos;
    }

    struct RedsquareClientDropItem
    {
        static constexpr gf::Id type = "RedsquareClientDropItem"_id;
        gf::Id entityId = gf::InvalidId;
        EntityType entityType;
        InventorySlotType slotType;
        uint pos;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, RedsquareClientDropItem& data)
    {
        return ar | data.entityId | data.entityType | data.slotType | data.pos;
    }

    struct RedsquareClientUseItem
    {
        static constexpr gf::Id type = "RedsquareClientUseItem"_id;
        gf::Id entityId = gf::InvalidId;
        EntityType entityType;
        InventorySlotType slotType;
        uint pos;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, RedsquareClientUseItem& data)
    {
        return ar | data.entityId | data.entityType | data.slotType | data.pos;
    }
}

#endif