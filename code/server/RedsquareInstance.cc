#include "RedsquareInstance.h"

#include <cinttypes>

#include <gf/Log.h>
#include <gf/Sleep.h>

#include "../common/Protocol.h"
#include "../common/RedsquareProtocol.h"

namespace redsquare
{
    RedsquareInstance::RedsquareInstance()
    : m_World(*this)
    , m_CountPlayers(0u)
    , m_PlayerIndexTurn(0u)
    , m_Floor(0)
    {
    }

    void RedsquareInstance::start()
    {
        m_CountPlayers = getPlayersCount();

        generateWorld();

        RedsquareServerInitMap map;
        map.world = m_World.m_World;
        map.floor = m_Floor;
        broadcast(map);

        auto players = getPlayers();
        int indexPlayer = 1;
        for(auto &player: players)
        {
            auto it = m_Players.emplace(player.id, std::move(Player(player.id, EntitySubType::Magus, player.name, *this)));
            assert(it.second);

            it.first->second.playerSpawn(m_World, indexPlayer++);
        }

        RedsquareServerInitEntities entities;
        for(auto &player: m_Players)
        {
            EntityData data;
            data.id = player.first;
            data.entityType = EntityType::Player;
            data.entitySubType = player.second.m_EntitySubType;
            data.pos = player.second.m_Pos;
            player.second.createCarPacket(data.characteristics);
            entities.allEntities.push_back(std::move(data));
        }
        for(auto &monster: m_Monsters)
        {
            EntityData data;
            data.id = monster.first;
            data.entityType = EntityType::Monster;
            data.entitySubType = monster.second.m_EntitySubType;
            data.pos = monster.second.m_Pos;
            monster.second.createCarPacket(data.characteristics);
            entities.allEntities.push_back(std::move(data));
        }
        for(auto &prop: m_Props)
        {
            EntityData data;
            data.id = prop.first;
            data.entityType = EntityType::Prop;
            data.entitySubType = prop.second.m_EntitySubType;
            data.pos = prop.second.m_Pos;
            prop.second.createCarPacket(data.characteristics);
            entities.allEntities.push_back(std::move(data));
        }
        for(auto &itemHolder: m_ItemHolders)
        {
            EntityData data;
            data.id = itemHolder.first;
            data.entityType = EntityType::ItemHolder;
            data.entitySubType = itemHolder.second.m_EntitySubType;
            data.pos = itemHolder.second.m_Pos;
            entities.allEntities.push_back(std::move(data));
        }

        broadcast(entities);

        for (auto &player: m_Players)
        {
            player.second.defaultInventoryStuff();
        }

        fillChest();

        auto *player = getPlayer(players[0].id);
        assert(player != nullptr);
        player->m_PlayerTurn = true;
        m_PlayerIndexTurn = 0;

        RedsquareServerPlayerTurn turn;
        send(players[0].id, turn);
    }

    bool RedsquareInstance::isFinished()
    {
        return getPlayersCount() == 0;
    }

    void RedsquareInstance::update(ServerPlayer& player, ProtocolBytes& bytes)
    {
        switch (bytes.getType())
        {
            case RedsquareClientMove::type:
            {
                gf::Log::info("(PemInstance) {%" PRIX64 "} RedsquareClientMove.\n", player.id);

                Player *playerTarget = getPlayer( player.id );
                if ( playerTarget != nullptr && playerTarget->m_PlayerTurn )
                {
                    auto in = bytes.as<RedsquareClientMove>();

                    bool moved = playerTarget->applyMove( in.dir, m_World );

                    if (moved)
                    {
                        playerTarget->m_PlayerTurn = false;

                        //If player is on the stair, regenerate the map
                        if (playerTarget->m_Pos == m_World.m_StairPosition)
                        {
                            for (auto &monster: m_Monsters)
                            {
                                RedsquareServerDeleteEntity packet;
                                packet.entityType = EntityType::Monster;
                                packet.id = monster.first;

                                broadcast(packet);
                            }
                            for (auto &prop: m_Props)
                            {
                                RedsquareServerDeleteEntity packet;
                                packet.entityType = EntityType::Prop;
                                packet.id = prop.first;

                                broadcast(packet);
                            }

                            m_Monsters.clear();
                            m_Props.clear();

                            m_Floor++;
                            if(m_Floor%4 == 0) // boss room every 4 floors 
                            { 
                                generateWorld(true);
                            }
                            else
                            {
                                generateWorld(false);
                            }

                            RedsquareServerInitMap map;
                            map.world = m_World.m_World;
                            map.floor = m_Floor;
                            broadcast(map);

                            int playerSpawnIndex = 1;
                            for(auto &player: m_Players)
                            {
                                player.second.playerSpawn(m_World, playerSpawnIndex++);

                                RedsquareServerMove packet;
                                packet.id = player.first;
                                packet.entityType = EntityType::Player;
                                packet.pos = player.second.m_Pos;
                                broadcast(packet);
                            }

                            RedsquareServerInitEntities entities;
                            for(auto &monster: m_Monsters)
                            {
                                EntityData data;
                                data.id = monster.first;
                                data.entityType = EntityType::Monster;
                                data.entitySubType = monster.second.m_EntitySubType;
                                data.pos = monster.second.m_Pos;
                                monster.second.createCarPacket(data.characteristics);
                                entities.allEntities.push_back(std::move(data));
                            }
                            for(auto &prop: m_Props)
                            {
                                EntityData data;
                                data.id = prop.first;
                                data.entityType = EntityType::Prop;
                                data.entitySubType = prop.second.m_EntitySubType;
                                data.pos = prop.second.m_Pos;
                                prop.second.createCarPacket(data.characteristics);
                                entities.allEntities.push_back(std::move(data));
                            }
                            for(auto &itemHolder: m_ItemHolders)
                            {
                                EntityData data;
                                data.id = itemHolder.first;
                                data.entityType = EntityType::ItemHolder;
                                data.entitySubType = itemHolder.second.m_EntitySubType;
                                data.pos = itemHolder.second.m_Pos;
                                entities.allEntities.push_back(std::move(data));
                            }
                            broadcast(entities);

                            fillChest();
                        }
                        else
                        {
                            RedsquareServerMove movePacket;
                            movePacket.entityType = EntityType::Player;
                            movePacket.id = player.id;
                            movePacket.pos = playerTarget->m_Pos;
                            broadcast(movePacket);

                            //Detect if the player walked on an item holder
                            ItemHolder *itemHolder = getItemHolder(playerTarget->m_Pos);
                            if ( itemHolder != nullptr )
                            {
                                ServerItem item(itemHolder->getItemTypeHolding(), itemHolder->getItemHoldingBaseFloor());
                                ssize_t pos = playerTarget->getInventory().addItem(InventorySlotType::Cargo, std::move(item));
                                if (pos != -1)
                                {
                                    RedsquareServerDeleteEntity packet;
                                    packet.entityType = EntityType::ItemHolder;
                                    packet.id = itemHolder->getEntityID();
                                    broadcast(packet);

                                    broadcast(playerTarget->createUpdateItemPacket(InventorySlotType::Cargo, false, pos));
                                }
                            }
                        }

                        goNextTurn();
                    }
                    else
                    {
                        RedsquareServerPlayerTurn turn;
                        send(player.id, turn);
                    }
                }
                break;
            }

            case RedsquareClientAttack::type:
            {
                gf::Log::info("(PemInstance) {%" PRIX64 "} RedsquareClientAttack.\n", player.id);

                Player *playerTarget = getPlayer( player.id );
                if ( playerTarget != nullptr && playerTarget->m_PlayerTurn )
                {
                    auto in = bytes.as<RedsquareClientAttack>();

                    ServerEntity *targetServerEntity;

                    Monster *targetMonster = getMonster(in.pos);
                    Player *targetPlayer = getPlayer(in.pos);
                    Prop *targetProp = getProp(in.pos);
                    assert(targetPlayer != nullptr || targetMonster != nullptr || targetProp != nullptr);

                    if (targetPlayer != nullptr)
                    {
                        targetServerEntity = dynamic_cast<ServerEntity*>(targetPlayer);
                        int level = playerTarget->m_Level;
                        Packet sendPacket;

                        if( in.spellType == SpellType::LightningStrike )
                        {
                            std::vector<Monster*> allPacket = playerTarget->attack(in.spellType, targetServerEntity, m_Monsters);
                            for(auto &currentMonster : allPacket)
                            {
                                if ( currentMonster->m_LifePoint <= 0 )
                                {
                                    RedsquareServerDeleteEntity packet;
                                    packet.entityType = currentMonster->getEntityType();
                                    packet.id = currentMonster->getEntityID();
                                    
                                    m_World.setWalkableFromEntity( currentMonster, true);
                                    m_World.setTransparentFromEntity( currentMonster, true );
                                    m_Monsters.erase(currentMonster->getEntityID());

                                    broadcast(packet);
                                }
                                else
                                {
                                    RedsquareServerUpdateCharacteristic packet;
                                    packet.entityType = currentMonster->getEntityType();
                                    packet.id = currentMonster->getEntityID();
                                    currentMonster->createCarPacket(packet.characteristics);

                                    broadcast(packet);
                                }
                            }
                        }
                        else
                        {
                            targetServerEntity = dynamic_cast<ServerEntity*>(targetPlayer);
                            playerTarget->attack(in.spellType, targetServerEntity);

                            RedsquareServerUpdateCharacteristic packet;
                            packet.entityType = targetServerEntity->getEntityType();
                            packet.id = targetServerEntity->getEntityID();
                            targetServerEntity->createCarPacket(packet.characteristics);

                            broadcast(packet);
                        }
                    }
                    else if (targetMonster != nullptr)
                    {
                        targetServerEntity = dynamic_cast<ServerEntity*>(targetMonster);
                        int level = playerTarget->m_Level;
                        Packet sendPacket;

                        if( in.spellType == SpellType::Reaper )
                        {
                            std::vector<Monster*> allPacket = playerTarget->attack(in.spellType, targetServerEntity, m_Monsters);
                            for(auto currentMonster : allPacket)
                            {
                                if ( currentMonster->m_LifePoint <= 0 )
                                {
                                    RedsquareServerDeleteEntity packet;
                                    packet.entityType = currentMonster->getEntityType();
                                    packet.id = currentMonster->getEntityID();
                                    
                                    m_World.setWalkableFromEntity( currentMonster, true);
                                    m_World.setTransparentFromEntity( currentMonster, true );
                                    m_Monsters.erase(currentMonster->getEntityID());

                                    broadcast(packet);
                                }
                                else
                                {
                                    RedsquareServerUpdateCharacteristic packet;
                                    packet.entityType = currentMonster->getEntityType();
                                    packet.id = currentMonster->getEntityID();
                                    currentMonster->createCarPacket(packet.characteristics);

                                    broadcast(packet);
                                }
                            }
                        }
                        else
                        {
                            playerTarget->attack(in.spellType, targetServerEntity);
                            Packet sendPacket;
                            if ( targetMonster->m_LifePoint > 0 )
                            {
                                RedsquareServerUpdateCharacteristic packet;
                                packet.entityType = targetMonster->getEntityType();
                                packet.id = targetMonster->getEntityID();
                                targetMonster->createCarPacket(packet.characteristics);

                                broadcast(packet);
                            }
                            else
                            {   
                                RedsquareServerDeleteEntity packet;
                                packet.entityType = targetMonster->getEntityType();
                                packet.id = targetMonster->getEntityID();
                                
                                m_World.setWalkableFromEntity( targetMonster, true);
                                m_World.setTransparentFromEntity( targetMonster, true );
                                m_Monsters.erase(targetMonster->getEntityID());

                                broadcast(packet);
                            }
                        }
                    }
                    else if (targetProp != nullptr)
                    {
                        RedsquareServerDeleteEntity packet;
                        packet.entityType = targetProp->getEntityType();
                        packet.id = targetProp->getEntityID();
                        
                        m_World.setWalkableFromEntity( targetProp, true);
                        m_World.setTransparentFromEntity( targetProp, true );
                        m_Props.erase(targetProp->getEntityID());

                        broadcast(packet);
                    }

                    RedsquareServerUpdateCharacteristic packet;
                    packet.entityType = playerTarget->getEntityType();
                    packet.id = playerTarget->getEntityID();
                    playerTarget->createCarPacket(packet.characteristics);

                    broadcast(packet);

                    playerTarget->m_PlayerTurn = false;
                    goNextTurn();
                }
                break;
            }

            case RedsquareClientPassTurn::type:
            {
                gf::Log::info("(PemInstance) {%" PRIX64 "} RedsquareClientPassTurn.\n", player.id);

                Player *playerTarget = getPlayer( player.id );
                if ( playerTarget != nullptr && playerTarget->m_PlayerTurn )
                {
                    playerTarget->m_PlayerTurn = false;
                    goNextTurn();
                }
                break;
            }

            case RedsquareClientMoveItem::type:
            {
                gf::Log::info("(PemInstance) {%" PRIX64 "} RedsquareClientMoveItem.\n", player.id);

                auto in = bytes.as<RedsquareClientMoveItem>();

                ServerEntity *oldEntity = nullptr;
                ServerEntity *newEntity = nullptr;

                switch (in.oldEntityType)
                {
                    case EntityType::Player:
                    {
                        oldEntity = getPlayer( in.oldEntityID );
                        break;
                    }

                    case EntityType::Monster:
                    {
                        oldEntity = getMonster( in.oldEntityID );
                        break;
                    }

                    case EntityType::Prop:
                    {
                        oldEntity = getProp( in.oldEntityID );
                        break;
                    }
                }
                assert(oldEntity != nullptr);

                switch (in.newEntityType)
                {
                    case EntityType::Player:
                    {
                        newEntity = getPlayer( in.newEntityID );
                        break;
                    }

                    case EntityType::Monster:
                    {
                        newEntity = getMonster( in.newEntityID );
                        break;
                    }

                    case EntityType::Prop:
                    {
                        newEntity = getProp( in.newEntityID );
                        break;
                    }
                }
                assert(newEntity != nullptr);

                ServerItem oldItem = std::move(oldEntity->getInventory().removeItem(in.oldSlotType, in.oldPos));
                broadcast(oldEntity->createUpdateItemPacket(in.oldSlotType, true, in.oldPos));

                ServerItem newItem;
                if (newEntity->getInventory().getItem(in.newSlotType, in.newPos) != nullptr)
                {
                    newItem = std::move(newEntity->getInventory().removeItem(in.newSlotType, in.newPos));
                    broadcast(newEntity->createUpdateItemPacket(in.newSlotType, true, in.newPos));
                }

                if (oldItem.getType() != ItemType::Unknow)
                {
                    newEntity->getInventory().addItem(in.newSlotType, std::move(oldItem), in.newPos);
                    broadcast(newEntity->createUpdateItemPacket(in.newSlotType, false, in.newPos));

                    if (newItem.getType() != ItemType::Unknow)
                    {
                        oldEntity->getInventory().addItem(in.oldSlotType, std::move(newItem), in.oldPos);
                        broadcast(oldEntity->createUpdateItemPacket(in.oldSlotType, false, in.oldPos));
                    }
                }

                break;
            }

            case RedsquareClientDropItem::type:
            {
                gf::Log::info("(PemInstance) {%" PRIX64 "} RedsquareClientDropItem.\n", player.id);

                auto in = bytes.as<RedsquareClientDropItem>();

                ServerEntity *entity = nullptr;

                switch (in.entityType)
                {
                    case EntityType::Player:
                    {
                        entity = getPlayer( in.entityId );
                        break;
                    }

                    case EntityType::Monster:
                    {
                        entity = getMonster( in.entityId );
                        break;
                    }

                    case EntityType::Prop:
                    {
                        entity = getProp( in.entityId );
                        break;
                    }
                }
                assert(entity != nullptr);

                Player *targetPlayer = getPlayer( player.id );
                assert(targetPlayer != nullptr);

                ServerItem *item = entity->getInventory().getItem(in.slotType, in.pos);
                if (item != nullptr && getItemHolder(targetPlayer->m_Pos) == nullptr)
                {
                    // Generate a new ID and Create a new itemHolder
                    gf::Id id = m_Random.computeId();
                    m_ItemHolders.emplace(id, ItemHolder(id, item->getType(), item->getBaseFloor(), targetPlayer->m_Pos));

                    RedsquareServerInitEntity packet;
                    packet.entity.id = id;
                    packet.entity.holdingItem = item->getType();
                    packet.entity.entityType = EntityType::ItemHolder;
                    packet.entity.pos = targetPlayer->m_Pos;
                    broadcast(packet);

                    entity->getInventory().removeItem(in.slotType, in.pos);
                    broadcast(entity->createUpdateItemPacket(in.slotType, true, in.pos));
                }

                break;
            }

            case RedsquareClientUseItem::type:
            {
                gf::Log::info("(PemInstance) {%" PRIX64 "} RedsquareClientUseItem.\n", player.id);

                auto in = bytes.as<RedsquareClientUseItem>();

                ServerEntity *entity = nullptr;

                switch (in.entityType)
                {
                    case EntityType::Player:
                    {
                        entity = getPlayer( in.entityId );
                        break;
                    }

                    case EntityType::Monster:
                    {
                        entity = getMonster( in.entityId );
                        break;
                    }

                    case EntityType::Prop:
                    {
                        entity = getProp( in.entityId );
                        break;
                    }
                }
                assert(entity != nullptr);

                Player *targetPlayer = getPlayer( player.id );
                assert(targetPlayer != nullptr);

                ServerItem *item = entity->getInventory().getItem(in.slotType, in.pos);
                if (item != nullptr && item->isUseable())
                {
                    targetPlayer->UseItem(item->getType());

                    entity->getInventory().removeItem(in.slotType, in.pos);
                    broadcast(entity->createUpdateItemPacket(in.slotType, true, in.pos));

                    RedsquareServerUpdateCharacteristic packet;
                    packet.entityType = targetPlayer->getEntityType();
                    packet.id = targetPlayer->getEntityID();
                    targetPlayer->createCarPacket(packet.characteristics);
                    broadcast(packet);
                }
                break;
            }
        }
    }

    void RedsquareInstance::generateWorld(bool boss)
    {
        m_World.generateWorld(boss); // generate map
        if(!boss)
        {
            placeProps(); // place props
            addNewMonsters(10); // place monsters TODO: make the number of monsters depends on the floor
            m_World.putStair(); // put stair on map
            m_World.getSpawnPoint(); // place the spawn of player
        }
        m_World.prettyPrint();  // print the map in server console
    }

    void RedsquareInstance::addNewMonsters(int nbMonster)
    {
        for(int i = 0; i < nbMonster ; ++i)
        {   
            // Generate a new ID
            gf::Id id = m_Random.computeId();
            std::map<gf::Id, Monster>::iterator itNewMonster;

            // Create a new monster
            std::tie(itNewMonster, std::ignore) = m_Monsters.emplace(id, Monster(id));
            m_World.monsterSpawn(itNewMonster->second, m_Floor);
        }
    }

    void RedsquareInstance::placeProps()
    {
        for(gf::Vector4u currentRoom : m_World.TabRoom) // for every room
        { 
            int randChest = rand() % 5; // rand if the room must contains a chest 
            int roomType = rand() % 13;

            gf::Id id; // if of the props
            std::map<gf::Id, Prop>::iterator itNewProp; // map of props

            if(randChest != 6){ // true chest
                id = m_Random.computeId();
                std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Chest));
                m_World.spawnProps(itNewProp->second,currentRoom);
            }

            switch (roomType)
                {
                case 0 : // SHELF ROOM 

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::BookShelf));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::BlankShelf));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::BrokenShelf));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Stool));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Stool));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::PotShelf));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::BlankShelf));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Torch));
                    m_World.spawnProps(itNewProp->second,currentRoom);


                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Torch));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::ExtinctTorch));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    break;
                case 1 : // PILLAR ROOM 
                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::BrownBrokenPillar));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyBrokenPillar));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyPillar1));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyPillar2));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyPillar1));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyPillar2));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::BrownBrokenPillar));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyBrokenPillar));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyBrokenPillar));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyBrokenPillar));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::BrownBrokenPillar));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::BrownPillar2));
                    m_World.spawnProps(itNewProp->second,currentRoom);
                    break;
                case 2 : // ROCK ROOM 

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Rock));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Rock));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Rock));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Rock));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Rock));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::BrownDualRock));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::BrownQuadRock));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::BrownRock));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyQuadRock));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyDualRock));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::BrownDualRock));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    break;
                case 3 : // BOXS /POTS 

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::PotShelf));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::BrokenPots));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::LitllePots));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::PotShelf));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Pot));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::PotShelf));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::BrokenPots));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::LitllePots));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::PotShelf));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Pot));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Pot));
                    m_World.spawnProps(itNewProp->second,currentRoom);


                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::BrokenPots));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::LitllePots));
                    m_World.spawnProps(itNewProp->second,currentRoom);
                    break;
                case 4 : // ARMORY 

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::WeaponShelf1));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::WeaponShelf2));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::WeaponShelf1));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::WeaponShelf2));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::WeaponShelf1));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::WeaponShelf2));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::EmptyStand));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::EmptyStand));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Torch));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Torch));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::ExtinctTorch));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::BlankShelf));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    break;
                case 5 : // THRONE 

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Throne));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Throne));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyPillar1));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyPillar1));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyPillar1));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyPillar1));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyPillar1));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyPillar1));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    break; 
                case 6 : // TRIVIA 

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::OpenedChest));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Box1));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::EmptyStand));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::EmptyStand));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::PotShelf));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::BrokenPots));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::LitllePots));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Torch));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::DualBox));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::PileWood));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Pot));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::BrokenPots));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Pot));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::BrokenPots));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    break; 
                case 7 : // TABLE ROOM 

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Table));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Stool));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Stool));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Stool));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::DualBox));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Box1));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Box2));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Box2));
                    m_World.spawnProps(itNewProp->second,currentRoom);
                    break;
                case 8 : // WAREHOUSE 
                    
                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::DualBox));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Box1));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Box2));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::DualBox));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Box1));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Box2));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::DualBox));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Box1));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Box2));
                    m_World.spawnProps(itNewProp->second,currentRoom);
                    break;  
                 case 10: // PRISON

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::LeftChain));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::RightChain));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::LeftChain));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::RightChain));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Box1));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Box2));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Box2));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Box2));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Box2));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Box2));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Stool));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Stool));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    break;
                case 11: // ROCK AND PILLAR

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Rock));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyDualRock));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyDualRock));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyBrokenPillar));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyBrokenPillar));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Throne));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyDualRock));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyQuadRock));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyPillar2));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyPillar1));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyPillar1));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    id = m_Random.computeId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyPillar1));
                    m_World.spawnProps(itNewProp->second,currentRoom);

                    break;
                case 12: // EMPTY
                    break;
                default:
                    break;
            }
        }
    }

    Player* RedsquareInstance::getPlayer( gf::Id playerID )
    {
        auto player = m_Players.find( playerID );

        if ( player != m_Players.end() )
        {
            return &player->second;
        }

        return nullptr;
    }

    Player* RedsquareInstance::getPlayer( gf::Vector2i pos )
    {
        auto it = m_Players.begin();
 
        // Iterate over the map using Iterator till end.
        while ( it != m_Players.end() )
        {
            if (it->second.isInsideMe(pos))
            {
                return &it->second;
            }

            ++it;
        }

        return nullptr;
    }

    Monster* RedsquareInstance::getMonster( gf::Id monsterID )
    {
        auto monster = m_Monsters.find( monsterID );

        if ( monster != m_Monsters.end() )
        {
            return &monster->second;
        }

        return nullptr;
    }

    Monster* RedsquareInstance::getMonster( gf::Vector2i pos )
    {
        auto it = m_Monsters.begin();
 
        // Iterate over the map using Iterator till end.
        while ( it != m_Monsters.end() )
        {
            if (it->second.isInsideMe(pos))
            {
                return &it->second;
            }

            ++it;
        }

        return nullptr;
    }

    Prop* RedsquareInstance::getProp( gf::Id propID )
    {
        auto prop = m_Props.find( propID );

        if ( prop != m_Props.end() )
        {
            return &prop->second;
        }

        return nullptr;
    }

    Prop* RedsquareInstance::getProp( gf::Vector2i pos )
    {
        auto it = m_Props.begin();
 
        // Iterate over the map using Iterator till end.
        while ( it != m_Props.end() )
        {
            if (it->second.isInsideMe(pos))
            {
                return &it->second;
            }

            ++it;
        }

        return nullptr;
    }

    ItemHolder* RedsquareInstance::getItemHolder( gf::Id itemHolderID )
    {
        auto itemHolder = m_ItemHolders.find( itemHolderID );

        if ( itemHolder != m_ItemHolders.end() )
        {
            return &itemHolder->second;
        }

        return nullptr;
    }

    ItemHolder* RedsquareInstance::getItemHolder( gf::Vector2i pos )
    {
        auto it = m_ItemHolders.begin();
 
        // Iterate over the map using Iterator till end.
        while ( it != m_ItemHolders.end() )
        {
            if (it->second.isInsideMe(pos))
            {
                return &it->second;
            }

            ++it;
        }

        return nullptr;
    }

    bool RedsquareInstance::canAttack(Monster &monster, gf::Vector2i targetPos)
    {
        if ( monster.isInsideMe( targetPos ) )
        {
            return false;
        }
        
        gf::Distance2<int> distFn = gf::manhattanDistance<int, 2>;

        float distance = distFn(monster.m_Pos, targetPos);

        if ( distance > monster.m_Range )
        {
            return false;
        }

        for(auto &player: m_Players)
        {
            if (player.second.isInsideMe(targetPos))
            {
                return true;
            }
        }

        return false;
    }

    void RedsquareInstance::goNextTurn()
    {
        uint nextPlayerIndex = ++m_PlayerIndexTurn;

        if (nextPlayerIndex >= getPlayersCount())
        {
            //monster turn
            for (auto it = m_Monsters.begin(); it != m_Monsters.end(); ++it)
            {
                bool hasFocus = false;
                bool isTarget = false;
                bool hasAttacked = false;
                m_World.m_SquareWorld.clearFieldOfVision();
                m_World.m_SquareWorld.computeLocalFieldOfVision(it->second.m_Pos,10);

                auto it2 = m_Players.begin();
                while ( it2 != m_Players.end() )
                {
                    if( m_World.m_SquareWorld.isInFieldOfVision(it2->second.m_Pos) )
                    {
                        hasFocus = true;
                        break;
                    }
                    ++it2;
                }

                if( hasFocus )
                {
                    if( canAttack(it->second, it2->second.m_Pos) )
                    {
                        ServerEntity *targetServerEntity = dynamic_cast<ServerEntity*>(&it2->second);
                        it->second.attack(targetServerEntity);

                        hasAttacked = true;

                        //Check if target is dead
                        if ( it2->second.m_LifePoint <= 0 )
                        {
                            gf::Id disconnectedID = it2->first;

                            RedsquareServerPlayerDead packet;
                            send(disconnectedID, packet);

                            m_Players.erase(it2);

                            RedsquareServerDeleteEntity packet2;
                            packet2.entityType = EntityType::Player;
                            packet2.id = disconnectedID;
                            broadcast(packet2);

                            removePlayer(getServerPlayer(disconnectedID));

                            hasAttacked = false;
                            isTarget = false;
                        }
                        else
                        {
                            RedsquareServerUpdateCharacteristic packet;
                            packet.id = it2->second.getEntityID();
                            packet.entityType = it2->second.getEntityType();
                            it2->second.createCarPacket(packet.characteristics);
                            broadcast(packet);
                        }
                    }
                    else
                    {
                        it->second.m_Routine = it2->second.m_Pos;
                        isTarget = true;
                    }
                }

                if( !hasAttacked )
                {
                    if( it->second.checkRoutine() )
                    {
                        m_World.drawRoutine(it->second);
                    }
                    else
                    {
                        if( isTarget )
                        {
                            m_World.m_SquareWorld.setWalkable(it->second.m_Routine, true);
                            m_World.m_SquareWorld.setTransparent(it->second.m_Routine, true);
                        }

                        if( !m_World.m_SquareWorld.isWalkable(it->second.m_Routine) )
                        {
                            m_World.drawRoutine(it->second);
                        }
                        
                        m_World.setWalkableFromEntity(static_cast<redsquare::Entity*>(&(it->second)), true);
                        m_World.setTransparentFromEntity( static_cast<redsquare::Entity*>(&(it->second)), true );

                        std::vector<gf::Vector2i> points = m_World.m_SquareWorld.computeRoute(it->second.m_Pos, it->second.m_Routine, 0.0);

                        if (points.empty())
                        {
                            m_World.drawRoutine(it->second);
                        }
                        else
                        {
                            it->second.m_Pos = points[1];
                            if( isTarget )
                            {
                                m_World.m_SquareWorld.setWalkable(it->second.m_Routine, false);
                            }

                            m_World.setWalkableFromEntity(static_cast<redsquare::Entity*>(&(it->second)), false);
                            m_World.setTransparentFromEntity( static_cast<redsquare::Entity*>(&(it->second)), false );

                            RedsquareServerMove packet;
                            packet.entityType = it->second.getEntityType();
                            packet.id = it->second.getEntityID();
                            packet.pos = it->second.m_Pos;
                            broadcast(packet);
                        }
                    }
                }
            }

            nextPlayerIndex = 0u;
        }

        auto players = getPlayers();

        Player *player = getPlayer(players[nextPlayerIndex].id);
        assert(player != nullptr);

        player->m_PlayerTurn = true;
        RedsquareServerPlayerTurn turn;
        send(players[nextPlayerIndex].id, turn);

        m_PlayerIndexTurn = nextPlayerIndex;
    }

    void RedsquareInstance::fillChest()
    {
        for(auto &prop: m_Props)
        {
            if (prop.second.m_EntitySubType == EntitySubType::Chest)
            {
                // at least 1 piece of equipment and 2 potion
                uint randomNumber = rand() % (m_Floor+1);
                ssize_t pos;

                for(uint nbItem = 0; nbItem < (randomNumber+1u) ; ++nbItem)
                {
                    int randomPotion;
                    if(m_Floor < 5)
                    {
                        randomPotion = rand() % POTION_TIER1; // potion tier 1 + potion tier 2
                    }
                    else if (m_Floor >= 5 && m_Floor < 15)
                    {
                        randomPotion = rand() % POTION_TIER2; // potion tier 1 + potion tier 2
                    }
                    else if (m_Floor >= 15)
                    {
                        randomPotion = rand() % POTION_TIER3; // potion tier 1 + potion tier 2
                    }

                    switch (randomPotion)
                    {
                        case 0:{
                            ServerItem item2(ItemType::HealthPot1, m_Floor+1);
                            pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        case 1:{
                            ServerItem item2(ItemType::ManaPot1, m_Floor+1);
                            pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        case 2:{
                            ServerItem item2(ItemType::EnergyPot1, m_Floor+1);
                            pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        case 3:{
                            ServerItem item2(ItemType::BoostAttack1, m_Floor+1);
                            pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        case 4:{
                            ServerItem item2(ItemType::BoostMana1, m_Floor+1);
                            pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        case 5:{
                            ServerItem item2(ItemType::BoostXP1, m_Floor+1);
                            pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        case 6:{
                            ServerItem item2(ItemType::BoostDefense1, m_Floor+1);
                            pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        case 7:{
                            ServerItem item2(ItemType::BoostHP1, m_Floor+1);
                            pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        case 8:{
                            ServerItem item2(ItemType::HealthPot2, m_Floor+1);
                            pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        case 9:{
                            ServerItem item2(ItemType::ManaPot2, m_Floor+1);
                            pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        case 10:{
                            ServerItem item2(ItemType::EnergyPot2, m_Floor+1);
                            pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        case 11:{
                            ServerItem item2(ItemType::BoostAttack2, m_Floor+1);
                            pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        case 12:{
                            ServerItem item2(ItemType::BoostMana2, m_Floor+1);
                            pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        case 13:{
                            ServerItem item2(ItemType::BoostXP2, m_Floor+1);
                            pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        case 14:{
                            ServerItem item2(ItemType::BoostDefense2, m_Floor+1);
                            pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        case 15:{
                            ServerItem item2(ItemType::BoostHP2, m_Floor+1);
                            pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        case 16:{
                            ServerItem item2(ItemType::HealthPot3, m_Floor+1);
                            pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        case 17:{
                            ServerItem item2(ItemType::ManaPot3, m_Floor+1);
                            pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        case 18:{
                            ServerItem item2(ItemType::EnergyPot3, m_Floor+1);
                            pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        case 19:{
                            ServerItem item2(ItemType::BoostAttack3, m_Floor+1);
                            pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        case 20:{
                            ServerItem item2(ItemType::BoostMana3, m_Floor+1);
                            pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        case 21:{
                            ServerItem item2(ItemType::BoostXP3, m_Floor+1);
                            pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        case 22:{
                            ServerItem item2(ItemType::BoostDefense3, m_Floor+1);
                            pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        case 23:{
                            ServerItem item2(ItemType::BoostHP3, m_Floor+1);
                            pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }

                        default:
                            break;
                    }

                    if (pos != -1)
                    {
                        broadcast(prop.second.createUpdateItemPacket(InventorySlotType::Cargo, false, pos));
                    }     
                }

                int randomType = rand() % EQUIPMENT_TYPE;
                int randomTier;
                if(m_Floor < 5)
                {
                    randomTier = rand() % TIER1; // 5 first tier
                }
                else if (m_Floor >= 5 && m_Floor < 15)
                {
                    randomTier = rand() % TIER2; // 10 first tier
                }
                else if (m_Floor >= 15)
                {
                    randomTier = rand() % TIER3; // all tier
                }  

                switch (randomType)
                {
                case 0:{
                    switch (randomTier)
                    {
                    case 0:{
                        ServerItem item2(ItemType::Sword1, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 1:{
                        ServerItem item2(ItemType::Sword2, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 2:{
                        ServerItem item2(ItemType::Sword3, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 3:{
                        ServerItem item2(ItemType::Sword4, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 4:{
                        ServerItem item2(ItemType::Sword5, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 5:{
                        ServerItem item2(ItemType::Sword6, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 6:{
                        ServerItem item2(ItemType::Sword7, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 7:{
                        ServerItem item2(ItemType::Sword8, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 8:{
                        ServerItem item2(ItemType::Sword9, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 9:{
                        ServerItem item2(ItemType::Sword10, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 10:{
                        ServerItem item2(ItemType::Sword11, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 11:{
                        ServerItem item2(ItemType::Sword12, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 12:{
                        ServerItem item2(ItemType::Sword13, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 13:{
                        ServerItem item2(ItemType::Sword14, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 15:{
                        ServerItem item2(ItemType::Sword15, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    default:
                        break;
                    }
                    if (pos != -1)
                    {
                        broadcast(prop.second.createUpdateItemPacket(InventorySlotType::Cargo, false, pos));
                    }
                    break;
                }
                case 1:{
                    switch (randomTier)
                    {
                    case 0:{
                        ServerItem item2(ItemType::Staff1, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 1:{
                        ServerItem item2(ItemType::Staff2, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 2:{
                        ServerItem item2(ItemType::Staff3, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 3:{
                        ServerItem item2(ItemType::Staff4, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 4:{
                        ServerItem item2(ItemType::Staff5, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 5:{
                        ServerItem item2(ItemType::Staff6, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 6:{
                        ServerItem item2(ItemType::Staff7, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 7:{
                        ServerItem item2(ItemType::Staff8, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 8:{
                        ServerItem item2(ItemType::Staff9, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 9:{
                        ServerItem item2(ItemType::Staff10, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 10:{
                        ServerItem item2(ItemType::Staff11, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 11:{
                        ServerItem item2(ItemType::Staff12, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 12:{
                        ServerItem item2(ItemType::Staff13, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 13:{
                        ServerItem item2(ItemType::Staff14, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 15:{
                        ServerItem item2(ItemType::Staff15, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    default:
                        break;
                    }
                    if (pos != -1)
                    {
                        broadcast(prop.second.createUpdateItemPacket(InventorySlotType::Cargo, false, pos));
                    }
                    break;
                }
                case 2:{
                    switch (randomTier)
                    {
                    case 0:{
                        ServerItem item2(ItemType::Bow1, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 1:{
                        ServerItem item2(ItemType::Bow2, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 2:{
                        ServerItem item2(ItemType::Bow3, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 3:{
                        ServerItem item2(ItemType::Bow4, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 4:{
                        ServerItem item2(ItemType::Bow5, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 5:{
                        ServerItem item2(ItemType::Bow6, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 6:{
                        ServerItem item2(ItemType::Bow7, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 7:{
                        ServerItem item2(ItemType::Bow8, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 8:{
                        ServerItem item2(ItemType::Bow9, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 9:{
                        ServerItem item2(ItemType::Bow10, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 10:{
                        ServerItem item2(ItemType::Bow11, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 11:{
                        ServerItem item2(ItemType::Bow12, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 12:{
                        ServerItem item2(ItemType::Bow13, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 13:{
                        ServerItem item2(ItemType::Bow14, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 15:{
                        ServerItem item2(ItemType::Bow15, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    default:
                        break;
                    }
                    if (pos != -1)
                    {
                        broadcast(prop.second.createUpdateItemPacket(InventorySlotType::Cargo, false, pos));
                    }
                    break;
                }
                case 3:{
                    switch (randomTier)
                    {
                    case 0:{
                        ServerItem item2(ItemType::SpellBook1, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 1:{
                        ServerItem item2(ItemType::SpellBook2, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 2:{
                        ServerItem item2(ItemType::SpellBook3, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 3:{
                        ServerItem item2(ItemType::SpellBook4, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 4:{
                        ServerItem item2(ItemType::SpellBook5, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 5:{
                        ServerItem item2(ItemType::SpellBook6, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 6:{
                        ServerItem item2(ItemType::SpellBook7, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 7:{
                        ServerItem item2(ItemType::SpellBook8, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 8:{
                        ServerItem item2(ItemType::SpellBook9, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 9:{
                        ServerItem item2(ItemType::SpellBook10, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 10:{
                        ServerItem item2(ItemType::SpellBook11, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 11:{
                        ServerItem item2(ItemType::SpellBook12, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 12:{
                        ServerItem item2(ItemType::SpellBook13, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 13:{
                        ServerItem item2(ItemType::SpellBook14, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 15:{
                        ServerItem item2(ItemType::SpellBook15, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    default:
                        break;
                    }
                    if (pos != -1)
                    {
                        broadcast(prop.second.createUpdateItemPacket(InventorySlotType::Cargo, false, pos));
                    }
                    break;
                }
                case 4:{
                    switch (randomTier)
                    {
                    case 0:{
                        ServerItem item2(ItemType::Shield1, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 1:{
                        ServerItem item2(ItemType::Shield2, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 2:{
                        ServerItem item2(ItemType::Shield3, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 3:{
                        ServerItem item2(ItemType::Shield4, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 4:{
                        ServerItem item2(ItemType::Shield5, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 5:{
                        ServerItem item2(ItemType::Shield6, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 6:{
                        ServerItem item2(ItemType::Shield7, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 7:{
                        ServerItem item2(ItemType::Shield8, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 8:{
                        ServerItem item2(ItemType::Shield9, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 9:{
                        ServerItem item2(ItemType::Shield10, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 10:{
                        ServerItem item2(ItemType::Shield11, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 11:{
                        ServerItem item2(ItemType::Shield12, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 12:{
                        ServerItem item2(ItemType::Shield13, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 13:{
                        ServerItem item2(ItemType::Shield14, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 15:{
                        ServerItem item2(ItemType::Shield15, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    default:
                        break;
                    }
                    if (pos != -1)
                    {
                        broadcast(prop.second.createUpdateItemPacket(InventorySlotType::Cargo, false, pos));
                    }
                    break;
                }
                case 5:{
                    switch (randomTier)
                    {
                    case 0:{
                        ServerItem item2(ItemType::Helmet1, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 1:{
                        ServerItem item2(ItemType::Helmet2, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 2:{
                        ServerItem item2(ItemType::Helmet3, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 3:{
                        ServerItem item2(ItemType::Helmet4, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 4:{
                        ServerItem item2(ItemType::Helmet5, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 5:{
                        ServerItem item2(ItemType::Helmet6, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 6:{
                        ServerItem item2(ItemType::Helmet7, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 7:{
                        ServerItem item2(ItemType::Helmet8, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 8:{
                        ServerItem item2(ItemType::Helmet9, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 9:{
                        ServerItem item2(ItemType::Helmet5, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 10:{
                        ServerItem item2(ItemType::Helmet11, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 11:{
                        ServerItem item2(ItemType::Helmet12, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 12:{
                        ServerItem item2(ItemType::Helmet13, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 13:{
                        ServerItem item2(ItemType::Helmet14, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 15:{
                        ServerItem item2(ItemType::Helmet15, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    default:
                        break;
                    }
                    if (pos != -1)
                    {
                        broadcast(prop.second.createUpdateItemPacket(InventorySlotType::Cargo, false, pos));
                    }
                    break;
                }
                case 6:{
                    switch (randomTier)
                    {
                    case 0:{
                        ServerItem item2(ItemType::Chesplate1, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 1:{
                        ServerItem item2(ItemType::Chesplate2, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 2:{
                        ServerItem item2(ItemType::Chesplate3, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 3:{
                        ServerItem item2(ItemType::Chesplate4, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 4:{
                        ServerItem item2(ItemType::Chesplate5, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 5:{
                        ServerItem item2(ItemType::Chesplate6, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 6:{
                        ServerItem item2(ItemType::Chesplate7, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 7:{
                        ServerItem item2(ItemType::Chesplate8, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 8:{
                        ServerItem item2(ItemType::Chesplate9, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 9:{
                        ServerItem item2(ItemType::Chesplate10, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 10:{
                        ServerItem item2(ItemType::Chesplate11, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 11:{
                        ServerItem item2(ItemType::Chesplate12, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 12:{
                        ServerItem item2(ItemType::Chesplate13, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 13:{
                        ServerItem item2(ItemType::Chesplate14, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 15:{
                        ServerItem item2(ItemType::Chesplate15, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    default:
                        break;
                    }
                    if (pos != -1)
                    {
                        broadcast(prop.second.createUpdateItemPacket(InventorySlotType::Cargo, false, pos));
                    }
                    break;
                }
                case 7:{
                    switch (randomTier)
                    {
                    case 0:{
                        ServerItem item2(ItemType::Legging1, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 1:{
                        ServerItem item2(ItemType::Legging2, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 2:{
                        ServerItem item2(ItemType::Legging3, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 3:{
                        ServerItem item2(ItemType::Legging4, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 4:{
                        ServerItem item2(ItemType::Legging5, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 5:{
                        ServerItem item2(ItemType::Legging6, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 6:{
                        ServerItem item2(ItemType::Legging7, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 7:{
                        ServerItem item2(ItemType::Legging8, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 8:{
                        ServerItem item2(ItemType::Legging9, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 9:{
                        ServerItem item2(ItemType::Legging10, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 10:{
                        ServerItem item2(ItemType::Legging11, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 11:{
                        ServerItem item2(ItemType::Legging12, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 12:{
                        ServerItem item2(ItemType::Legging13, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 13:{
                        ServerItem item2(ItemType::Legging14, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 15:{
                        ServerItem item2(ItemType::Legging15, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    default:
                        break;
                    }
                    if (pos != -1)
                    {
                        broadcast(prop.second.createUpdateItemPacket(InventorySlotType::Cargo, false, pos));
                    }
                    break;
                }
                case 8:{
                    switch (randomTier)
                    {
                    case 0:{
                        ServerItem item2(ItemType::Boot1, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 1:{
                        ServerItem item2(ItemType::Boot2, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 2:{
                        ServerItem item2(ItemType::Boot3, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 3:{
                        ServerItem item2(ItemType::Boot4, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 4:{
                        ServerItem item2(ItemType::Boot5, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 5:{
                        ServerItem item2(ItemType::Boot6, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 6:{
                        ServerItem item2(ItemType::Boot7, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 7:{
                        ServerItem item2(ItemType::Boot8, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 8:{
                        ServerItem item2(ItemType::Boot9, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 9:{
                        ServerItem item2(ItemType::Boot10, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 10:{
                        ServerItem item2(ItemType::Boot11, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 11:{
                        ServerItem item2(ItemType::Boot12, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 12:{
                        ServerItem item2(ItemType::Boot13, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 13:{
                        ServerItem item2(ItemType::Boot14, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    case 15:{
                        ServerItem item2(ItemType::Boot15, m_Floor+1);
                        pos = prop.second.getInventory().addItemRandom(InventorySlotType::Cargo, std::move(item2));
                        break;
                    }
                    default:
                        break;
                    }
                    if (pos != -1)
                    {
                        broadcast(prop.second.createUpdateItemPacket(InventorySlotType::Cargo, false, pos));
                    }
                    break;
                }
                default:
                    break;
                }
            }
        }
    }

    void RedsquareInstance::doRemovePlayer(ServerPlayer& player)
    {
        Player *player2 = getPlayer(player.id);
        assert(player2 != nullptr);

        bool wasHisTurn = player2->m_PlayerTurn;

        m_Players.erase(player.id);

        if (getPlayersCount() > 0)
        {
            RedsquareServerDeleteEntity packet;
            packet.entityType = EntityType::Player;
            packet.id = player.id;
            broadcast(packet);

            if (wasHisTurn)
            {
                goNextTurn();
            }
        }
    }
}
