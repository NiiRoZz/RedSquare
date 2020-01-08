#include "Game.h"
#include "../common/Singletons.h"

#include <gf/Random.h>
#include <gf/VectorOps.h>
#include <iostream>

namespace redsquare
{
    Game::Game()
    : m_PlayerSpawned(0)
    , m_Floor(0)
    {
        m_World.generateWorld(); // generate map
        placeProps(5); // place props
        addNewMonsters(5); // place monsters
        m_World.putStair(m_Props); // put stair on map
        m_World.prettyPrint();  // print the map in server console
        m_World.getSpawnPoint(m_Props,m_Monsters); // place the spawn of player

        m_World.prettyPrint();
    }

    void Game::addNewPlayer(SocketTcp socket)
    {
        // Generate a new ID
        gf::Id id = generateId();
        std::map<gf::Id, Player>::iterator itNewPlayer;

        Packet packet;

        //Receive info of client, freeze until we receive them
        socket.receive(packet);
    

        // Create a new player
        std::tie(itNewPlayer, std::ignore) = m_Players.emplace(id, Player(std::move(socket), id, packet.playerInfoConnection.entitySubType));
        itNewPlayer->second.playerSpawn(m_World,++m_PlayerSpawned);

        NewPlayer packetNewPlayer( m_World.m_World, id, m_Floor );
        itNewPlayer->second.sendPacket(packetNewPlayer);

        //HACKY, too, sending fake move to all other players INCLUDE HIMSELF!!! Should be reworked
        packet.type = PacketType::SpawnEntity;
        packet.spawnEntity.entityID = id;
        packet.spawnEntity.typeEntity = EntityType::Player;
        packet.spawnEntity.typeOfEntity = itNewPlayer->second.m_TypeOfEntity;
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
                packet.spawnEntity.typeOfEntity = it->second.m_TypeOfEntity;
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
            packet.spawnEntity.typeOfEntity = it2->second.m_TypeOfEntity;
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
            packet.spawnEntity.typeOfEntity = it3->second.m_TypeOfEntity;
            packet.spawnEntity.posX = it3->second.m_Pos[0];
            packet.spawnEntity.posY = it3->second.m_Pos[1];
            itNewPlayer->second.sendPacket( packet );

            ++it3;
        }

        itNewPlayer->second.sendUpdateOfSpells();
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
            m_World.monsterSpawn(itNewMonster->second,m_Monsters,m_Floor);
        }
    }
    
    void Game::placeProps(int nbProps){
        
        int roomType;
        for(gf::Vector4u currentRoom : m_World.TabRoom){ // for every room

            roomType = rand() % 10;

            gf::Id id;
            std::map<gf::Id, Prop>::iterator itNewProp;

            switch (roomType)
                {
                case 0 : // SHELF ROOM 
                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::BookShelf));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::BlankShelf));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::BrokenShelf));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Stool));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Stool));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::PotShelf));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::BlankShelf));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Torch));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);


                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Torch));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::ExtinctTorch));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    break;
                case 1 : // PILLAR ROOM 
                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::BrownBrokenPillar));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyBrokenPillar));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyPillar1));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyPillar2));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyPillar1));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyPillar2));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::BrownBrokenPillar));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyBrokenPillar));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyBrokenPillar));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyBrokenPillar));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::BrownBrokenPillar));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::BrownPillar2));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);
                    break;
                case 2 : // ROCK ROOM 

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Rock));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Rock));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Rock));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Rock));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Rock));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::BrownDualRock));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::BrownQuadRock));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::BrownRock));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyQuadRock));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyDualRock));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::BrownDualRock));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    break;
                case 3 : // BOXS /POTS 

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::PotShelf));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::BrokenPots));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::LitllePots));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::PotShelf));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Pot));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::PotShelf));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::BrokenPots));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::LitllePots));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::PotShelf));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Pot));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Pot));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);


                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::BrokenPots));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::LitllePots));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);
                    break;
                case 4 : // ARMORY 

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::WeaponShelf1));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::WeaponShelf2));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::WeaponShelf1));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::WeaponShelf2));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::WeaponShelf1));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::WeaponShelf2));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::EmptyStand));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::EmptyStand));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Torch));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Torch));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::ExtinctTorch));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::BlankShelf));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    break;
                case 5 : // THRONE 

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Throne));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Throne));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyPillar1));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyPillar1));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyPillar1));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyPillar1));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyPillar1));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::GreyPillar1));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    break; 
                case 6 : // TRIVIA 

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::OpenedChest));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Box1));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::EmptyStand));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::EmptyStand));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Chest));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::PotShelf));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::BrokenPots));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::LitllePots));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Torch));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::DualBox));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::PileWood));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Pot));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::BrokenPots));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Pot));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::BrokenPots));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    break; 
                case 7 : // TABLE ROOM 

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Table));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Stool));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Stool));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Stool));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::DualBox));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Box1));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Box2));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Box2));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);
                    break;
                case 8 : // WAREHOUSE 
                    
                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::DualBox));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Box1));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Box2));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::DualBox));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Box1));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Box2));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::DualBox));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Box1));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);

                    id = generateId();
                    std::tie(itNewProp, std::ignore) = m_Props.emplace(id, Prop(id, EntitySubType::Box2));
                    m_World.spawnProps(itNewProp->second,m_Props,currentRoom);
                    break;  
                 case 9: // EMPTY
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
                        //TODO: TEMP
                        player->m_PointInRound -= player->m_PointInRound;
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
                            m_World.generateWorld();
                            placeProps(5);
                            addNewMonsters(5);
                            m_World.putStair(m_Props);
                            m_World.prettyPrint();  
                            m_World.getSpawnPoint(m_Props,m_Monsters);
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
                                    packet.spawnEntity.typeOfEntity = itMonster->second.m_TypeOfEntity;
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
                                    packet.spawnEntity.typeOfEntity = itProp->second.m_TypeOfEntity;
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
                        }
                        else
                        {
                            sendPacket.type = PacketType::ReceiveMove;
                            sendPacket.receiveMove.entityID = packet.requestMove.playerID;
                            sendPacket.receiveMove.typeEntity = EntityType::Player;
                            sendPacket.receiveMove.posX = player->m_Pos[0];
                            sendPacket.receiveMove.posY = player->m_Pos[1];

                            sendPacketToAllPlayers( sendPacket );
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
                    targetServerEntity = dynamic_cast<ServerEntity*>(targetMonster);
                    if ( targetMonster != nullptr && targetServerEntity != nullptr )
                    {
                        int level = player->m_Level;
                        player->attack(packet.requestAttack.spellType, targetServerEntity);

                        Packet sendPacket;
                        if ( targetMonster->m_LifePoint > 0 )
                        {
                            targetMonster->createCarPacket(sendPacket);
                        }
                        else
                        {   
                            sendPacket.type = PacketType::EntityDisconnected;
                            sendPacket.entityDisconnected.typeEntity = EntityType::Monster;
                            sendPacket.entityDisconnected.entityID = targetMonster->m_EntityID;

                            m_World.setWalkableFromEntity(static_cast<redsquare::Entity*>(targetMonster), true);
                            m_World.m_SquareWorld.setTransparent(targetMonster->m_Pos, true);

                            m_Monsters.erase(targetMonster->m_EntityID);
                        }
                        sendPacketToAllPlayers( sendPacket );

                        if(player->m_Level != level)
                        {
                            player->createCarPacket(sendPacket);
                            sendPacketToAllPlayers( sendPacket );
                        }
                    }
                    else
                    {
                        Prop *targetProp = getProp(posTarget);

                        if ( targetProp != nullptr )
                        {
                            Packet sendPacket;
                            sendPacket.type = PacketType::EntityDisconnected;
                            sendPacket.entityDisconnected.typeEntity = EntityType::Prop;
                            sendPacket.entityDisconnected.entityID = targetProp->m_EntityID;

                            m_World.setWalkableFromEntity(static_cast<redsquare::Entity*>(targetProp), true);
                            m_World.m_SquareWorld.setTransparent(targetProp->m_Pos);

                            m_Props.erase(targetProp->m_EntityID);

                            sendPacketToAllPlayers( sendPacket );
                        }
                    }
                }
                break;
            }

            case PacketType::PassTurn:
            {
                Player *player = getPlayer( packet.passTurn.playerID );
                if ( player != nullptr )
                {
                    player->m_PointInRound -= player->m_PointInRound;
                }
                break;
            }

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
}
