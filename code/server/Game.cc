#include "Game.h"
#include "../common/Singletons.h"
#include "../common/Constants.h"
#include "Message.h"

#include <gf/Random.h>
#include <gf/VectorOps.h>
#include <tuple>
#include <iostream>

namespace redsquare
{
    Game::Game()
    : m_PlayerSpawned(0)
    , m_Floor(0)
    {
        gMessageManager().registerHandler<UpdateEntityCharacteristic>(&Game::onUpdateEntityCharacteristic, this);

        generateGame(false);
    }

    void Game::generateGame(bool boss)
    {
        m_World.generateWorld(boss,*this); // generate map
        if(!boss){
            placeProps(); // place props
            addNewMonsters(10); // place monsters TODO: make the number of monsters depends on the floor
            m_World.putStair(*this); // put stair on map
            m_World.getSpawnPoint(*this); // place the spawn of player
        }
        m_World.prettyPrint();  // print the map in server console

        m_World.prettyPrint();
    }

    gf::Id Game::addNewPlayer(SocketTcp socket)
    {
        // Generate a new ID
        gf::Id id = generateId();
        std::map<gf::Id, Player>::iterator itNewPlayer;

        Packet packet;

        //Receive info of client, freeze until we receive them
        socket.receive(packet);
    
        // Create a new player
        std::tie(itNewPlayer, std::ignore) = m_Players.emplace(id, Player(std::move(socket), id, packet.playerInfoConnection.entitySubType, std::move(std::string(packet.playerInfoConnection.name))));
        itNewPlayer->second.playerSpawn(m_World,++m_PlayerSpawned);

        NewPlayer packetNewPlayer( m_World.m_World, id, m_Floor );
        itNewPlayer->second.sendPacket(packetNewPlayer);

        //sending fake move to all other players include himself
        packet.type = PacketType::SpawnEntity;
        packet.spawnEntity.entityID = id;
        packet.spawnEntity.typeEntity = EntityType::Player;
        packet.spawnEntity.typeOfEntity = itNewPlayer->second.m_EntitySubType;
        packet.spawnEntity.posX = itNewPlayer->second.m_Pos[0];
        packet.spawnEntity.posY = itNewPlayer->second.m_Pos[1];
        sendPacketToAllPlayers( packet );

        itNewPlayer->second.createCarPacket(packet);
        sendPacketToAllPlayers( packet );

        //fake a move of all players inside the game to make them apparear in the new client
        auto it = m_Players.begin();
        // Iterate over the map using Iterator till end.
        while (it != m_Players.end())
        {
            if ( it->first != id )
            {
                packet.type = PacketType::SpawnEntity;
                packet.spawnEntity.entityID = it->first;
                packet.spawnEntity.typeEntity = EntityType::Player;
                packet.spawnEntity.typeOfEntity = it->second.m_EntitySubType;
                packet.spawnEntity.posX = it->second.m_Pos[0];
                packet.spawnEntity.posY = it->second.m_Pos[1];
                itNewPlayer->second.sendPacket( packet );

                it->second.createCarPacket(packet);
                itNewPlayer->second.sendPacket( packet );
            }

            ++it;
        }

        //fake a move of all monsters inside the game to make them apparear in the new client
        auto it2 = m_Monsters.begin();
        // Iterate over the map using Iterator till end.
        while (it2 != m_Monsters.end())
        {
            packet.type = PacketType::SpawnEntity;
            packet.spawnEntity.entityID = it2->first;
            packet.spawnEntity.typeEntity = EntityType::Monster;
            packet.spawnEntity.typeOfEntity = it2->second.m_EntitySubType;
            packet.spawnEntity.posX = it2->second.m_Pos[0];
            packet.spawnEntity.posY = it2->second.m_Pos[1];
            itNewPlayer->second.sendPacket( packet );

            it2->second.createCarPacket(packet);
            itNewPlayer->second.sendPacket( packet );

            ++it2;
        }

        //fake a move of all props inside the game to make them apparear in the new client
        auto it3 = m_Props.begin();
        // Iterate over the map using Iterator till end.
        while (it3 != m_Props.end())
        {
            packet.type = PacketType::SpawnEntity;
            packet.spawnEntity.entityID = it3->first;
            packet.spawnEntity.typeEntity = EntityType::Prop;
            packet.spawnEntity.typeOfEntity = it3->second.m_EntitySubType;
            packet.spawnEntity.posX = it3->second.m_Pos[0];
            packet.spawnEntity.posY = it3->second.m_Pos[1];
            itNewPlayer->second.sendPacket( packet );

            ++it3;
        }

        itNewPlayer->second.sendUpdateOfSpells();

        itNewPlayer->second.defaultInventoryStuff();

        return id;
    }

    void Game::fillChest()
    {
        for(auto &prop: m_Props)
        {
            if (prop.second.m_EntitySubType == EntitySubType::Chest)
            {
                if(m_Floor <= 5){
                    // at least 1 piece of equipment and 2 potion
                    int randomType = rand() % EQUIPMENT_TYPE;
                    int randomTier = rand() % 5; // 5 first tier 
                    ssize_t pos;
                    switch (randomType)
                    {
                    case 0:{
                        switch (randomTier)
                        {
                        case 0:{
                            ServerItem item2(ItemType::Sword1, m_Floor+1);
                            pos = prop.second.getInventory().addItem(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        case 1:{
                            ServerItem item2(ItemType::Sword2, m_Floor+1);
                            pos = prop.second.getInventory().addItem(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        case 2:{
                            ServerItem item2(ItemType::Sword3, m_Floor+1);
                            pos = prop.second.getInventory().addItem(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        case 3:{
                            ServerItem item2(ItemType::Sword4, m_Floor+1);
                            pos = prop.second.getInventory().addItem(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        case 4:{
                            ServerItem item2(ItemType::Sword5, m_Floor+1);
                            pos = prop.second.getInventory().addItem(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        default:
                            break;
                        }
                        if (pos != -1)
                        {
                            Packet packet = prop.second.createUpdateItemPacket(InventorySlotType::Cargo, false, pos);
                            sendPacketToAllPlayers(packet);
                        }
                        break;
                    }
                    case 1:{
                        switch (randomTier)
                        {
                        case 0:{
                            ServerItem item2(ItemType::Staff1, m_Floor+1);
                            pos = prop.second.getInventory().addItem(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        case 1:{
                            ServerItem item2(ItemType::Staff2, m_Floor+1);
                            pos = prop.second.getInventory().addItem(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        case 2:{
                            ServerItem item2(ItemType::Staff3, m_Floor+1);
                            pos = prop.second.getInventory().addItem(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        case 3:{
                            ServerItem item2(ItemType::Staff4, m_Floor+1);
                            pos = prop.second.getInventory().addItem(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        case 4:{
                            ServerItem item2(ItemType::Staff5, m_Floor+1);
                            pos = prop.second.getInventory().addItem(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        default:
                            break;
                        }
                        if (pos != -1)
                        {
                            Packet packet = prop.second.createUpdateItemPacket(InventorySlotType::Cargo, false, pos);
                            sendPacketToAllPlayers(packet);
                        }
                        break;
                    }
                    case 2:{
                        switch (randomTier)
                        {
                        case 0:{
                            ServerItem item2(ItemType::Bow1, m_Floor+1);
                            pos = prop.second.getInventory().addItem(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        case 1:{
                            ServerItem item2(ItemType::Bow2, m_Floor+1);
                            pos = prop.second.getInventory().addItem(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        case 2:{
                            ServerItem item2(ItemType::Bow3, m_Floor+1);
                            pos = prop.second.getInventory().addItem(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        case 3:{
                            ServerItem item2(ItemType::Bow4, m_Floor+1);
                            pos = prop.second.getInventory().addItem(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        case 4:{
                            ServerItem item2(ItemType::Bow5, m_Floor+1);
                            pos = prop.second.getInventory().addItem(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        default:
                            break;
                        }
                        if (pos != -1)
                        {
                            Packet packet = prop.second.createUpdateItemPacket(InventorySlotType::Cargo, false, pos);
                            sendPacketToAllPlayers(packet);
                        }
                        break;
                    }
                    case 3:{
                        switch (randomTier)
                        {
                        case 0:{
                            ServerItem item2(ItemType::SpellBook1, m_Floor+1);
                            pos = prop.second.getInventory().addItem(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        case 1:{
                            ServerItem item2(ItemType::SpellBook2, m_Floor+1);
                            pos = prop.second.getInventory().addItem(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        case 2:{
                            ServerItem item2(ItemType::SpellBook3, m_Floor+1);
                            pos = prop.second.getInventory().addItem(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        case 3:{
                            ServerItem item2(ItemType::SpellBook4, m_Floor+1);
                            pos = prop.second.getInventory().addItem(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        case 4:{
                            ServerItem item2(ItemType::SpellBook5, m_Floor+1);
                            pos = prop.second.getInventory().addItem(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        default:
                            break;
                        }
                        if (pos != -1)
                        {
                            Packet packet = prop.second.createUpdateItemPacket(InventorySlotType::Cargo, false, pos);
                            sendPacketToAllPlayers(packet);
                        }
                        break;
                    }
                    case 4:{
                        switch (randomTier)
                        {
                        case 0:{
                            ServerItem item2(ItemType::Shield1, m_Floor+1);
                            pos = prop.second.getInventory().addItem(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        case 1:{
                            ServerItem item2(ItemType::Shield2, m_Floor+1);
                            pos = prop.second.getInventory().addItem(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        case 2:{
                            ServerItem item2(ItemType::Shield3, m_Floor+1);
                            pos = prop.second.getInventory().addItem(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        case 3:{
                            ServerItem item2(ItemType::Shield4, m_Floor+1);
                            pos = prop.second.getInventory().addItem(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        case 4:{
                            ServerItem item2(ItemType::Shield5, m_Floor+1);
                            pos = prop.second.getInventory().addItem(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        default:
                            break;
                        }
                        if (pos != -1)
                        {
                            Packet packet = prop.second.createUpdateItemPacket(InventorySlotType::Cargo, false, pos);
                            sendPacketToAllPlayers(packet);
                        }
                        break;
                    }
                    case 5:{
                        switch (randomTier)
                        {
                        case 0:{
                            ServerItem item2(ItemType::Helmet1, m_Floor+1);
                            pos = prop.second.getInventory().addItem(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        case 1:{
                            ServerItem item2(ItemType::Helmet2, m_Floor+1);
                            pos = prop.second.getInventory().addItem(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        case 2:{
                            ServerItem item2(ItemType::Helmet3, m_Floor+1);
                            pos = prop.second.getInventory().addItem(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        case 3:{
                            ServerItem item2(ItemType::Helmet4, m_Floor+1);
                            pos = prop.second.getInventory().addItem(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        case 4:{
                            ServerItem item2(ItemType::Helmet5, m_Floor+1);
                            pos = prop.second.getInventory().addItem(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        default:
                            break;
                        }
                        if (pos != -1)
                        {
                            Packet packet = prop.second.createUpdateItemPacket(InventorySlotType::Cargo, false, pos);
                            sendPacketToAllPlayers(packet);
                        }
                        break;
                    }
                    case 6:{
                        switch (randomTier)
                        {
                        case 0:{
                            ServerItem item2(ItemType::Chesplate1, m_Floor+1);
                            pos = prop.second.getInventory().addItem(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        case 1:{
                            ServerItem item2(ItemType::Chesplate2, m_Floor+1);
                            pos = prop.second.getInventory().addItem(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        case 2:{
                            ServerItem item2(ItemType::Chesplate3, m_Floor+1);
                            pos = prop.second.getInventory().addItem(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        case 3:{
                            ServerItem item2(ItemType::Chesplate4, m_Floor+1);
                            pos = prop.second.getInventory().addItem(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        case 4:{
                            ServerItem item2(ItemType::Chesplate5, m_Floor+1);
                            pos = prop.second.getInventory().addItem(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        default:
                            break;
                        }
                        if (pos != -1)
                        {
                            Packet packet = prop.second.createUpdateItemPacket(InventorySlotType::Cargo, false, pos);
                            sendPacketToAllPlayers(packet);
                        }
                        break;
                    }
                    case 7:{
                        switch (randomTier)
                        {
                        case 0:{
                            ServerItem item2(ItemType::Legging1, m_Floor+1);
                            pos = prop.second.getInventory().addItem(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        case 1:{
                            ServerItem item2(ItemType::Legging2, m_Floor+1);
                            pos = prop.second.getInventory().addItem(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        case 2:{
                            ServerItem item2(ItemType::Legging3, m_Floor+1);
                            pos = prop.second.getInventory().addItem(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        case 3:{
                            ServerItem item2(ItemType::Legging4, m_Floor+1);
                            pos = prop.second.getInventory().addItem(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        case 4:{
                            ServerItem item2(ItemType::Legging5, m_Floor+1);
                            pos = prop.second.getInventory().addItem(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        default:
                            break;
                        }
                        if (pos != -1)
                        {
                            Packet packet = prop.second.createUpdateItemPacket(InventorySlotType::Cargo, false, pos);
                            sendPacketToAllPlayers(packet);
                        }
                        break;
                    }
                    case 8:{
                        switch (randomTier)
                        {
                        case 0:{
                            ServerItem item2(ItemType::Boot1, m_Floor+1);
                            pos = prop.second.getInventory().addItem(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        case 1:{
                            ServerItem item2(ItemType::Boot2, m_Floor+1);
                            pos = prop.second.getInventory().addItem(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        case 2:{
                            ServerItem item2(ItemType::Boot3, m_Floor+1);
                            pos = prop.second.getInventory().addItem(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        case 3:{
                            ServerItem item2(ItemType::Boot4, m_Floor+1);
                            pos = prop.second.getInventory().addItem(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        case 4:{
                            ServerItem item2(ItemType::Boot5, m_Floor+1);
                            pos = prop.second.getInventory().addItem(InventorySlotType::Cargo, std::move(item2));
                            break;
                        }
                        default:
                            break;
                        }
                        if (pos != -1)
                        {
                            Packet packet = prop.second.createUpdateItemPacket(InventorySlotType::Cargo, false, pos);
                            sendPacketToAllPlayers(packet);
                        }
                        break;
                    }
                    default:
                        break;
                    }
                }
            }
        }
    }
    void Game::addNewMonsters(int nbMonster)
    {
        for(int i = 0; i < nbMonster ; ++i)
        {   
            // Generate a new ID
            gf::Id id = generateId();
            std::map<gf::Id, Monster>::iterator itNewMonster;

            // Create a new monster
            std::tie(itNewMonster, std::ignore) = m_Monsters.emplace(id, Monster(id));
            m_World.monsterSpawn(itNewMonster->second, m_Floor);
        }
    }
    
    void Game::placeProps()
    {
        for(gf::Vector4u currentRoom : m_World.TabRoom) // for every room
        { 
            int randChest = rand() % 5; // rand if the room must contains a chest 
            int roomType = rand() % 13;

            gf::Id id; // if of the props
            std::map<gf::Id, Prop>::iterator itNewProp; // map of props

            if(randChest != 6){ // true chest
                id = generateId();
                std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Chest));
                m_World.spawnProps(itNewProp->second,*this,currentRoom);
            }

            switch (roomType)
                {
                case 0 : // SHELF ROOM 

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::BookShelf));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::BlankShelf));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::BrokenShelf));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Stool));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Stool));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::PotShelf));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::BlankShelf));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Torch));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);


                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Torch));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::ExtinctTorch));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    break;
                case 1 : // PILLAR ROOM 
                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::BrownBrokenPillar));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyBrokenPillar));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyPillar1));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyPillar2));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyPillar1));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyPillar2));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::BrownBrokenPillar));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyBrokenPillar));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyBrokenPillar));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyBrokenPillar));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::BrownBrokenPillar));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::BrownPillar2));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);
                    break;
                case 2 : // ROCK ROOM 

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Rock));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Rock));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Rock));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Rock));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Rock));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::BrownDualRock));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::BrownQuadRock));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::BrownRock));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyQuadRock));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyDualRock));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::BrownDualRock));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    break;
                case 3 : // BOXS /POTS 

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::PotShelf));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::BrokenPots));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::LitllePots));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::PotShelf));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Pot));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::PotShelf));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::BrokenPots));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::LitllePots));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::PotShelf));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Pot));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Pot));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);


                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::BrokenPots));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::LitllePots));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);
                    break;
                case 4 : // ARMORY 

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::WeaponShelf1));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::WeaponShelf2));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::WeaponShelf1));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::WeaponShelf2));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::WeaponShelf1));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::WeaponShelf2));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::EmptyStand));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::EmptyStand));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Torch));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Torch));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::ExtinctTorch));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::BlankShelf));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    break;
                case 5 : // THRONE 

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Throne));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Throne));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyPillar1));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyPillar1));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyPillar1));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyPillar1));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyPillar1));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyPillar1));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    break; 
                case 6 : // TRIVIA 

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::OpenedChest));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Box1));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::EmptyStand));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::EmptyStand));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::PotShelf));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::BrokenPots));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::LitllePots));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Torch));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::DualBox));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::PileWood));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Pot));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::BrokenPots));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Pot));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::BrokenPots));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    break; 
                case 7 : // TABLE ROOM 

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Table));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Stool));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Stool));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Stool));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::DualBox));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Box1));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Box2));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Box2));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);
                    break;
                case 8 : // WAREHOUSE 
                    
                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::DualBox));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Box1));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Box2));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::DualBox));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Box1));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Box2));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::DualBox));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Box1));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Box2));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);
                    break;  
                 case 10: // PRISON

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::LeftChain));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::RightChain));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::LeftChain));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::RightChain));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Box1));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Box2));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Box2));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Box2));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Box2));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Box2));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Stool));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Stool));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    break;
                case 11: // ROCK AND PILLAR

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Rock));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyDualRock));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyDualRock));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyBrokenPillar));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyBrokenPillar));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Throne));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyDualRock));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyQuadRock));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyPillar2));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyPillar1));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyPillar1));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyPillar1));
                    m_World.spawnProps(itNewProp->second,*this,currentRoom);

                    break;
                case 12: // EMPTY
                    break;
                default:
                    break;
            }
        }
    }

    gf::Id Game::generateId() const
    {
        uint64_t min = std::numeric_limits<uint64_t>::min();
        uint64_t max = std::numeric_limits<uint64_t>::max();
        uint64_t number = gRandom().computeUniformInteger(min, max);

        return number;
    }

    void Game::processPackets( Packet &packet )
    {
        switch ( packet.type )
        {
            case PacketType::RequestMove:
            {
                Player *player = getPlayer( packet.requestMove.playerID );
                if ( player != nullptr )
                {
                    bool moved = player->applyMove( packet.requestMove.dirX,packet.requestMove.dirY, m_World );

                    if ( moved && !player->m_MovedInRound )
                    {
                        player->m_PointInRound = 0;
                        player->m_MovedInRound = true;

                        Packet sendPacket;
                        
                        //If player is on the stair, regenerate the map
                        if (player->m_Pos == m_World.m_StairPosition)
                        {
                            for (auto it = m_Monsters.begin(); it != m_Monsters.end(); ++it)
                            {
                                sendPacket.type = PacketType::EntityDisconnected;
                                sendPacket.entityDisconnected.typeEntity = EntityType::Monster;
                                sendPacket.entityDisconnected.entityID = it->first;

                                sendPacketToAllPlayers( sendPacket );
                            }

                            for (auto it2 = m_Props.begin(); it2 != m_Props.end(); ++it2)
                            {
                                sendPacket.type = PacketType::EntityDisconnected;
                                sendPacket.entityDisconnected.typeEntity = EntityType::Prop;
                                sendPacket.entityDisconnected.entityID = it2->first;

                                sendPacketToAllPlayers( sendPacket );
                            }

                            m_Monsters.clear();
                            m_Props.clear();

                            sendPacket.type = PacketType::NewMap;
                            sendPacketToAllPlayers( sendPacket );

                            m_Floor++;
                            if(m_Floor%4 == 0){ // boss room every 4 floors 
                                generateGame(true);
                            }else{
                                generateGame(false);
                            }
                            m_PlayerSpawned = 0;

                            for (auto it3 = m_Players.begin(); it3 != m_Players.end(); ++it3)
                            {
                                it3->second.playerSpawn(m_World,++m_PlayerSpawned);

                                NewPlayer packetNewPlayer( m_World.m_World, it3->first, m_Floor );
                                it3->second.sendPacket(packetNewPlayer);

                                //fake a move of all monsters inside the game to make them apparear in the new client
                                auto itMonster = m_Monsters.begin();
                                // Iterate over the map using Iterator till end.
                                while (itMonster != m_Monsters.end())
                                {
                                    packet.type = PacketType::SpawnEntity;
                                    packet.spawnEntity.entityID = itMonster->first;
                                    packet.spawnEntity.typeEntity = EntityType::Monster;
                                    packet.spawnEntity.typeOfEntity = itMonster->second.m_EntitySubType;
                                    packet.spawnEntity.posX = itMonster->second.m_Pos[0];
                                    packet.spawnEntity.posY = itMonster->second.m_Pos[1];
                                    it3->second.sendPacket( packet );

                                    itMonster->second.createCarPacket(packet);
                                    it3->second.sendPacket( packet );

                                    ++itMonster;
                                }

                                //fake a move of all props inside the game to make them apparear in the new client
                                auto itProp = m_Props.begin();
                                // Iterate over the map using Iterator till end.
                                while (itProp != m_Props.end())
                                {
                                    packet.type = PacketType::SpawnEntity;
                                    packet.spawnEntity.entityID = itProp->first;
                                    packet.spawnEntity.typeEntity = EntityType::Prop;
                                    packet.spawnEntity.typeOfEntity = itProp->second.m_EntitySubType;
                                    packet.spawnEntity.posX = itProp->second.m_Pos[0];
                                    packet.spawnEntity.posY = itProp->second.m_Pos[1];
                                    it3->second.sendPacket( packet );

                                    ++itProp;
                                }
                            }

                            for (auto it4 = m_Players.begin(); it4 != m_Players.end(); ++it4)
                            {
                                sendPacket.type = PacketType::ReceiveMove;
                                sendPacket.receiveMove.entityID = it4->first;
                                sendPacket.receiveMove.typeEntity = EntityType::Player;
                                sendPacket.receiveMove.posX = it4->second.m_Pos[0];
                                sendPacket.receiveMove.posY = it4->second.m_Pos[1];

                                sendPacketToAllPlayers( sendPacket );
                            }
                            
                            fillChest();
                        }
                        else
                        {
                            sendPacket.type = PacketType::ReceiveMove;
                            sendPacket.receiveMove.entityID = packet.requestMove.playerID;
                            sendPacket.receiveMove.typeEntity = EntityType::Player;
                            sendPacket.receiveMove.posX = player->m_Pos[0];
                            sendPacket.receiveMove.posY = player->m_Pos[1];
                            sendPacketToAllPlayers( sendPacket );

                            //Detect if the player walked on an item holder
                            ItemHolder *itemHolder = getItemHolder(player->m_Pos);
                            if ( itemHolder != nullptr )
                            {
                                ServerItem item(itemHolder->getItemTypeHolding(), itemHolder->getItemHoldingBaseFloor());
                                ssize_t pos = player->getInventory().addItem(InventorySlotType::Cargo, std::move(item));
                                if (pos != -1)
                                {
                                    sendPacket.type = PacketType::EntityDisconnected;
                                    sendPacket.entityDisconnected.typeEntity = EntityType::ItemHolder;
                                    sendPacket.entityDisconnected.entityID = itemHolder->getEntityID();
                                    sendPacketToAllPlayers( sendPacket );

                                    sendPacket = player->createUpdateItemPacket(InventorySlotType::Cargo, false, pos);
                                    sendPacketToAllPlayers( sendPacket );
                                }
                            }
                        }
                    }
                }
                break;
            }

            case PacketType::RequestAttack:
            {
                Player *player = getPlayer( packet.requestMove.playerID );
                if ( player != nullptr )
                {
                    gf::Vector2i posTarget({packet.requestAttack.posX, packet.requestAttack.posY});
                    ServerEntity *targetServerEntity;

                    Monster *targetMonster = getMonster(posTarget);
                    Player *targetPlayer = getPlayer(posTarget);
                    Prop *targetProp = getProp(posTarget);

                    if(targetPlayer != nullptr){

                        targetServerEntity = dynamic_cast<ServerEntity*>(targetPlayer);
                        int level = player->m_Level;
                        Packet sendPacket;

                        if( packet.requestAttack.spellType == SpellType::LightningStrike ){ // TODO fix the bug where you use 2 time this attack and it only deal the damage once
                            std::vector<Monster*> allPacket = player->attack(packet.requestAttack.spellType, targetServerEntity, m_Monsters);
                            for(auto currentMonster : allPacket){
                                if( currentMonster->m_LifePoint <= 0 ){
                                    sendPacket.type = PacketType::EntityDisconnected;
                                    sendPacket.entityDisconnected.typeEntity = EntityType::Monster;
                                    sendPacket.entityDisconnected.entityID = currentMonster->getEntityID();
                                    m_World.setWalkableFromEntity( currentMonster, true);
                                    m_World.setTransparentFromEntity( currentMonster, true );
                                    m_Monsters.erase(currentMonster->getEntityID());
                                }else{
                                    currentMonster->createCarPacket(sendPacket);
                                }
                                sendPacketToAllPlayers( sendPacket);
                            }

                            if(player->m_Level != level){
                                player->createCarPacket(sendPacket);
                                sendPacketToAllPlayers( sendPacket );
                            }else{
                                player->createCarPacket(sendPacket);
                                sendPacketToAllPlayers( sendPacket );
                            }
                        }else{
                            targetServerEntity = dynamic_cast<ServerEntity*>(targetPlayer);
                            player->attack(packet.requestAttack.spellType, targetServerEntity);
                        }
                        
                        player->createCarPacket(sendPacket);
                        sendPacketToAllPlayers( sendPacket );

                    }else if(targetMonster != nullptr){
                        
                        targetServerEntity = dynamic_cast<ServerEntity*>(targetMonster);
                        int level = player->m_Level;
                        Packet sendPacket;

                        if( packet.requestAttack.spellType == SpellType::Reaper ){
                            std::vector<Monster*> allPacket = player->attack(packet.requestAttack.spellType, targetServerEntity, m_Monsters);
                            for(auto currentMonster : allPacket){
                                if( currentMonster->m_LifePoint <= 0 ){
                                    sendPacket.type = PacketType::EntityDisconnected;
                                    sendPacket.entityDisconnected.typeEntity = EntityType::Monster;
                                    sendPacket.entityDisconnected.entityID = currentMonster->getEntityID();
                                    m_World.setWalkableFromEntity( currentMonster, true);
                                    m_World.setTransparentFromEntity( currentMonster, true );
                                    m_Monsters.erase(currentMonster->getEntityID());
                                }else{
                                    currentMonster->createCarPacket(sendPacket);
                                }
                                sendPacketToAllPlayers( sendPacket);
                            }

                            player->createCarPacket(sendPacket);
                            sendPacketToAllPlayers( sendPacket );
                            
                        }else{
                            player->attack(packet.requestAttack.spellType, targetServerEntity);
                            Packet sendPacket;
                            if ( targetMonster->m_LifePoint > 0 ){
                                targetMonster->createCarPacket(sendPacket);
                            }else{   
                                sendPacket.type = PacketType::EntityDisconnected;
                                sendPacket.entityDisconnected.typeEntity = EntityType::Monster;
                                sendPacket.entityDisconnected.entityID = targetMonster->getEntityID();

                                m_World.setWalkableFromEntity(static_cast<redsquare::Entity*>(targetMonster), true);
                                m_World.setTransparentFromEntity( static_cast<redsquare::Entity*>(targetMonster), true );

                                m_Monsters.erase(targetMonster->getEntityID());
                            }

                            sendPacketToAllPlayers( sendPacket ); // monster packet

                            player->createCarPacket(sendPacket); // player packet
                            sendPacketToAllPlayers( sendPacket );
                        }

                    }else if(targetProp != nullptr){
                        Packet sendPacket;
                        sendPacket.type = PacketType::EntityDisconnected;
                        sendPacket.entityDisconnected.typeEntity = EntityType::Prop;
                        sendPacket.entityDisconnected.entityID = targetProp->getEntityID();

                        m_World.setWalkableFromEntity(static_cast<redsquare::Entity*>(targetProp), true);
                        m_World.setTransparentFromEntity( static_cast<redsquare::Entity*>(targetProp), true );

                        m_Props.erase(targetProp->getEntityID());

                        sendPacketToAllPlayers( sendPacket );
                    }
                }

                break;
            }

            case PacketType::PassTurn:
            {
                Player *player = getPlayer( packet.passTurn.playerID );
                if ( player != nullptr )
                {
                    player->m_PointInRound = 0;
                }
                break;
            }

            case PacketType::MoveItem:
            {
                ServerEntity *oldEntity = nullptr;
                ServerEntity *newEntity = nullptr;

                switch (packet.moveItem.oldEntityType)
                {
                    case EntityType::Player:
                    {
                        oldEntity = getPlayer( packet.moveItem.oldEntityID );
                        break;
                    }

                    case EntityType::Monster:
                    {
                        oldEntity = getMonster( packet.moveItem.oldEntityID );
                        break;
                    }

                    case EntityType::Prop:
                    {
                        oldEntity = getProp( packet.moveItem.oldEntityID );
                        break;
                    }
                }
                assert(oldEntity != nullptr);

                switch (packet.moveItem.newEntityType)
                {
                    case EntityType::Player:
                    {
                        newEntity = getPlayer( packet.moveItem.newEntityID );
                        break;
                    }

                    case EntityType::Monster:
                    {
                        newEntity = getMonster( packet.moveItem.newEntityID );
                        break;
                    }

                    case EntityType::Prop:
                    {
                        newEntity = getProp( packet.moveItem.newEntityID );
                        break;
                    }
                }
                assert(newEntity != nullptr);

                if (oldEntity == newEntity)
                {
                    if ( newEntity->getInventory().moveItem(packet.moveItem) )
                    {
                        sendPacketToAllPlayers(packet);
                    }
                }
                else
                {
                    ServerItem oldItem = oldEntity->getInventory().removeItem(packet.moveItem.oldSlotType, packet.moveItem.oldPos);
                    Packet updatePacket = oldEntity->createUpdateItemPacket(packet.moveItem.oldSlotType, true, packet.moveItem.oldPos);
                    sendPacketToAllPlayers(updatePacket);

                    if (oldItem.getType() != ItemType::Unknow)
                    {
                        newEntity->getInventory().addItem(packet.moveItem.newSlotType, std::move(oldItem), packet.moveItem.newPos);
                        updatePacket = newEntity->createUpdateItemPacket(packet.moveItem.newSlotType, false, packet.moveItem.newPos);
                        sendPacketToAllPlayers(updatePacket);
                    }
                }

                break;
            }

            case PacketType::UpdateSpells:
                break;

            case PacketType::RequestUse:
            {
                ServerEntity *entity = nullptr;

                switch (packet.requestUse.entityType)
                {
                    case EntityType::Player:
                    {
                        entity = getPlayer( packet.requestUse.entityID );
                        break;
                    }

                    case EntityType::Monster:
                    {
                        entity = getMonster( packet.requestUse.entityID );
                        break;
                    }

                    case EntityType::Prop:
                    {
                        entity = getProp( packet.requestUse.entityID );
                        break;
                    }
                }
                assert(entity != nullptr);

                Player *player = getPlayer( packet.requestUse.playerID );
                assert(player != nullptr);

                ServerItem *item = entity->getInventory().getItem(packet.requestUse.slotType, packet.requestUse.pos);
                if (item != nullptr && item->isUseable())
                {
                    player->UseItem(item->getType());

                    entity->getInventory().removeItem(packet.requestUse.slotType, packet.requestUse.pos);
                    Packet updatePacket = entity->createUpdateItemPacket(packet.requestUse.slotType, true, packet.requestUse.pos);
                    sendPacketToAllPlayers(updatePacket);

                    Packet updateCarPacket;
                    player->createCarPacket(updateCarPacket);
                    sendPacketToAllPlayers(updateCarPacket);
                }
                break;
            }

            case PacketType::RequestDrop:
            {
                ServerEntity *entity = nullptr;

                switch (packet.requestDrop.entityType)
                {
                    case EntityType::Player:
                    {
                        entity = getPlayer( packet.requestDrop.entityID );
                        break;
                    }

                    case EntityType::Monster:
                    {
                        entity = getMonster( packet.requestDrop.entityID );
                        break;
                    }

                    case EntityType::Prop:
                    {
                        entity = getProp( packet.requestDrop.entityID );
                        break;
                    }
                }
                assert(entity != nullptr);

                Player *player = getPlayer( packet.requestDrop.playerID );
                assert(player != nullptr);

                ServerItem *item = entity->getInventory().getItem(packet.requestDrop.slotType, packet.requestDrop.pos);
                if (item != nullptr && getItemHolder(player->m_Pos) == nullptr)
                {
                    // Generate a new ID and Create a new itemHolder
                    gf::Id id = generateId();
                    m_ItemHolders.emplace(id, ItemHolder(id, item->getType(), item->getBaseFloor(), player->m_Pos));

                    Packet packetSpawnEntity;
                    packetSpawnEntity.type = PacketType::SpawnEntity;
                    packetSpawnEntity.spawnEntity.entityID = id;
                    packetSpawnEntity.spawnEntity.typeEntity = EntityType::ItemHolder;
                    packetSpawnEntity.spawnEntity.holdingItem = item->getType();
                    packetSpawnEntity.spawnEntity.posX = player->m_Pos[0];
                    packetSpawnEntity.spawnEntity.posY = player->m_Pos[1];
                    sendPacketToAllPlayers(packetSpawnEntity);

                    InventorySlotType slotType = packet.requestDrop.slotType;

                    entity->getInventory().removeItem(packet.requestDrop.slotType, packet.requestDrop.pos);
                    Packet updatePacket = entity->createUpdateItemPacket(packet.requestDrop.slotType, true, packet.requestDrop.pos);
                    sendPacketToAllPlayers(updatePacket);
                }
                break;
            }


            default :
                break;

           /* case PacketType::Message:{
                
            }*/

        }
    }

    void Game::sendPacketToAllPlayers( Packet &packet )
    {
        auto it = m_Players.begin();
 
        // Iterate over the map using Iterator till end.
        while ( it != m_Players.end() )
        {
            it->second.sendPacket( packet );

            ++it;
        }
    }

    Player* Game::getPlayer( gf::Id playerID )
    {
        auto player = m_Players.find( playerID );

        if ( player != m_Players.end() )
        {
            return &player->second;
        }

        return nullptr;
    }

    Player* Game::getPlayer( gf::Vector2i pos )
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

    Monster* Game::getMonster( gf::Id monsterID )
    {
        auto monster = m_Monsters.find( monsterID );

        if ( monster != m_Monsters.end() )
        {
            return &monster->second;
        }

        return nullptr;
    }

    Monster* Game::getMonster( gf::Vector2i pos )
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

    Prop* Game::getProp( gf::Id propID )
    {
        auto prop = m_Props.find( propID );

        if ( prop != m_Props.end() )
        {
            return &prop->second;
        }

        return nullptr;
    }

    Prop* Game::getProp( gf::Vector2i pos )
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

    ItemHolder* Game::getItemHolder( gf::Id itemHolderID )
    {
        auto itemHolder = m_ItemHolders.find( itemHolderID );

        if ( itemHolder != m_ItemHolders.end() )
        {
            return &itemHolder->second;
        }

        return nullptr;
    }

    ItemHolder* Game::getItemHolder( gf::Vector2i pos )
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

    bool Game::canAttack(Monster &monster, gf::Vector2i targetPos)
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

        auto it = m_Players.begin();
 
        while ( it != m_Players.end() )
        {
            if ( it->second.isInsideMe( targetPos ) )
            {
                return true;
            }

            ++it;
        }

        return false;
    }

    gf::MessageStatus Game::onUpdateEntityCharacteristic(gf::Id id, gf::Message *msg)
    {
        assert(id == UpdateEntityCharacteristic::type);
        
        UpdateEntityCharacteristic *message = static_cast<UpdateEntityCharacteristic*>(msg);

        if (message && message->entity)
        {
            Packet packet;
            message->entity->createCarPacket(packet);
            sendPacketToAllPlayers(packet);
        }

        return gf::MessageStatus::Keep;
    }
}
