#include "Game.h"

#include <iostream>
#include <utility>

#include <gf/VectorOps.h>
#include <gf/Sprite.h>
#include <gf/RenderTarget.h>

namespace redsquare
{
    Game::Game( char* hostname, char *port, gf::ExtendView &view )
    : m_ThreadCom(hostname, port, m_ComQueue)
    , m_View(view)
    , m_CanPlay( false )
    , m_dirX(0)
    , m_dirY(0)
    , m_AttackX(0)
    , m_AttackY(0)
    , m_PassTurn(false)
    {
        nextPosTexture.loadFromFile( "data/redsquare/img/redsquare.png" );

        //TEMP
        m_AnimatedEntities.emplace(gf::Id(10), AnimatedEntity(10, {1,1}, "data/redsquare/img/TileSet3.png", 0, 0, 4, 0.1f));
    }

    void Game::startThreadCom()
    {
        m_ThreadCom.start();
    }

    void Game::sendInfoConnection(EntityClass type, char *name)
    {
        Packet sendPacket;
        sendPacket.type = PacketType::PlayerInfoConnection;
        sendPacket.playerInfoConnection.entityClass = type;
        strncpy(sendPacket.playerInfoConnection.name, name, 20);

        m_ThreadCom.sendPacket(sendPacket);
    }

    void Game::receiveWorld()
    {
        NewPlayer newPlayerData;

        m_ThreadCom.receivePacket( newPlayerData );

        m_PlayerID = newPlayerData.playerID;

        m_World.generateWorld( std::move( newPlayerData.world ) );
    }

    void Game::render(gf::RenderTarget& target, const gf::RenderStates& states)
    {
        m_World.render( target, states );

        auto it1 = m_TempMove.begin();
 
        // Iterate over the map using Iterator till end.
        while (it1 != m_TempMove.end())
        {
            //create sprite here
            gf::Sprite sprite;

            sprite.setPosition( (*it1) * World::TileSize );
            sprite.setScale( 1 );
            sprite.setTexture( nextPosTexture );
            target.draw( sprite, states );

            ++it1;
        }
        
        auto it2 = m_Players.begin();
 
        // Iterate over the map using Iterator till end.
        while (it2 != m_Players.end())
        {
            it2->second.render( target, states );

            ++it2;
        }

        auto it3 = m_Monsters.begin();
 
        // Iterate over the map using Iterator till end.
        while (it3 != m_Monsters.end())
        {
            it3->second.render( target, states );

            ++it3;
        }

        auto it4 = m_AnimatedEntities.begin();
 
        // Iterate over the map using Iterator till end.
        while (it4 != m_AnimatedEntities.end())
        {
            //std::cout << "m_AnimatedEntities render 1" << std::endl;
            //std::cout << "AnimatedEntity::render m_Animation : " << &it4->second.m_Animation << " m_Animation.getCurrentBounds().getTopLeft()[0] : " << it4->second.m_Animation.getCurrentBounds().getTopLeft()[0] << " m_Animation.getCurrentTexture().getSize()[0] : " << it4->second.m_Animation.getCurrentTexture().getSize()[0] << std::endl;
            it4->second.render( target, states );
            //std::cout << "AnimatedEntity::render m_Animation : " << &it4->second.m_Animation << " m_Animation.getCurrentBounds().getTopLeft()[0] : " << it4->second.m_Animation.getCurrentBounds().getTopLeft()[0] << " m_Animation.getCurrentTexture().getSize()[0] : " << it4->second.m_Animation.getCurrentTexture().getSize()[0] << std::endl;
            //std::cout << "m_AnimatedEntities render 2" << std::endl;

            ++it4;
        }

        auto it5 = m_Props.begin();
 
        // Iterate over the map using Iterator till end.
        while (it5 != m_Props.end())
        {
            it5->second.render( target, states );

            ++it5;
        }
    }

    void Game::update(gf::Time time)
    {
        //Checking if we have received a message from server
        processPackets();

        m_World.update( time );

        //We update only the player we are controlling
        Player* player = getPlayer( m_PlayerID );

        if ( player != nullptr )
        {
            player->update( time );

            m_View.setCenter( { (float)(player->m_Pos[0] * World::TileSize), (float)(player->m_Pos[1] * World::TileSize )} );

            //Do all actions stuff here
            doAction();
        }

        auto it = m_AnimatedEntities.begin();
 
        // Iterate over the map using Iterator till end.
        while (it != m_AnimatedEntities.end())
        {
            it->second.update( time );

            ++it;
        }
    }

    void Game::doAction()
    {
        if ( !m_CanPlay )
        {
            return;
        }

        //Player want to move
        if ( m_dirX != 0 || m_dirY != 0 )
        {
            //don't forget to call m_CanPlay false when sent action
            m_CanPlay = false;

            Packet packet;
            packet.type = PacketType::RequestMove;
            packet.requestMove.playerID = m_PlayerID;
            packet.requestMove.dirX = m_dirX;
            packet.requestMove.dirY = m_dirY;

            //If it's move with key
            if (m_dirX == 0 || m_dirY == 0)
            {
                m_TempMove.clear();
            }

            m_dirX = 0;
            m_dirY = 0;

            m_ThreadCom.sendPacket( packet );
        }
        else if ( m_AttackX != 0 || m_AttackY != 0 )
        {
            //don't forget to call m_CanPlay false when sent action
            m_CanPlay = false;
            
            Packet packet;
            packet.type = PacketType::RequestAttack;
            packet.requestAttack.playerID = m_PlayerID;
            packet.requestAttack.posX = m_AttackX;
            packet.requestAttack.posY = m_AttackY;

            m_AttackX = 0;
            m_AttackY = 0;
            m_TempMove.clear();

            m_ThreadCom.sendPacket( packet );
        }
        else if ( m_PassTurn )
        {
            //don't forget to call m_CanPlay false when sent action
            m_CanPlay = false;

            Packet packet;
            packet.type = PacketType::PassTurn;
            packet.passTurn.playerID = m_PlayerID;

            m_PassTurn = false;
            m_TempMove.clear();

            m_ThreadCom.sendPacket( packet );
        }
    }

    void Game::processPackets()
    {
        Packet packet;
        while ( m_ComQueue.poll(packet) )
        {
            switch (packet.type)
            {   
                case PacketType::ReceiveMove:
                {
                    if (packet.receiveMove.typeEntity == EntityType::Player)
                    {
                        Player* player = getPlayer( packet.receiveMove.entityID );
                        assert( player != nullptr );
                        
                        player->m_Pos[0] = packet.receiveMove.posX;
                        player->m_Pos[1] = packet.receiveMove.posY;
                    }
                    else if (packet.receiveMove.typeEntity == EntityType::Monster)
                    {
                        Monster* monster = getMonster( packet.receiveMove.entityID );
                        assert( monster != nullptr );
                        
                        monster->m_Pos[0] = packet.receiveMove.posX;
                        monster->m_Pos[1] = packet.receiveMove.posY;
                    }
                    break;
                }

                case PacketType::EntityDisconnected:
                {
                    switch (packet.entityDisconnected.typeEntity)
                    {
                        case EntityType::Player:
                        {
                            m_Players.erase( packet.entityDisconnected.entityID );
                            break;
                        }

                        case EntityType::Monster:
                        {
                            m_Monsters.erase( packet.entityDisconnected.entityID );
                            break;
                        }

                        case EntityType::Prop:
                        {
                            m_Props.erase( packet.entityDisconnected.entityID );
                            break;
                        }
                    }

                    break;
                }

                case PacketType::PlayerTurn:
                {
                    if ( m_TempMove.size() > 0 && !monsterNear() && !playerNear() )
                    {
                        gf::Vector2i pos = m_TempMove.front();
                        m_TempMove.erase(m_TempMove.begin());

                        if ( getPlayer(pos) == nullptr && getMonster(pos) == nullptr )
                        {
                            Packet packet;
                            packet.type = PacketType::RequestMove;
                            packet.requestMove.playerID = m_PlayerID;
                            packet.requestMove.dirX = pos[0];
                            packet.requestMove.dirY = pos[1];

                            m_ThreadCom.sendPacket( packet );

                            packet.type = PacketType::PassTurn;
                            packet.passTurn.playerID = m_PlayerID;

                            m_ThreadCom.sendPacket( packet );
                            break;
                        }
                    }

                    m_TempMove.clear();
                    
                    m_CanPlay = packet.playerTurn.playerTurn;
                    std::cout << "It's your turn!!!" << std::endl;

                    break;
                }

                case PacketType::SpawnEntity:
                {
                    switch (packet.spawnEntity.typeEntity)
                    {
                        case EntityType::Player:
                        {
                            auto it = m_Players.insert( std::make_pair( packet.spawnEntity.entityID, Player( packet.spawnEntity.entityID, packet.spawnEntity.typeOfEntity, gf::Vector2i(packet.spawnEntity.posX, packet.spawnEntity.posY) ) ) );
                            assert( it.second );
                            break;
                        }

                        case EntityType::Monster:
                        {
                            auto it = m_Monsters.insert( std::make_pair( packet.spawnEntity.entityID, Monster( packet.spawnEntity.entityID, packet.spawnEntity.typeOfEntity, gf::Vector2i(packet.spawnEntity.posX, packet.spawnEntity.posY) ) ) );
                            assert( it.second );
                            break;
                        }

                        case EntityType::Prop:
                        {
                            auto it = m_Props.insert( std::make_pair( packet.spawnEntity.entityID, Prop( packet.spawnEntity.entityID, packet.spawnEntity.typeOfEntity, gf::Vector2i(packet.spawnEntity.posX, packet.spawnEntity.posY) ) ) );
                            assert( it.second );
                            break;
                        }
                    }
                    break;
                }

                case PacketType::EntityCar:
                {
                    if ( packet.entityCar.entityType == EntityType::Player )
                    {
                        Player* player = getPlayer(packet.entityCar.entityID);
                        assert(player != nullptr);

                        player->m_LifePoint = packet.entityCar.m_LifePoint;
                        player->m_MaxLifePoint = packet.entityCar.m_MaxLifePoint;
                        
                        player->m_ManaPoint = packet.entityCar.m_ManaPoint;
                        player->m_MaxManaPoint = packet.entityCar.m_MaxManaPoint;

                        player->m_AttackPoint = packet.entityCar.m_AttackPoint;
                        player->m_DefensePoint = packet.entityCar.m_DefensePoint;

                        player->m_MaxAttackPoint = packet.entityCar.m_MaxAttackPoint;
                        player->m_MaxDefensePoint = packet.entityCar.m_MaxDefensePoint;

                        player->m_Range = packet.entityCar.m_Range;

                        player->m_XP = packet.entityCar.m_XP;
                        player->m_Max_XP = packet.entityCar.m_MaxXP;

                        player->m_Level = packet.entityCar.m_Level;
                    }
                    else if ( packet.entityCar.entityType == EntityType::Monster )
                    {
                        Monster* monster = getMonster(packet.entityCar.entityID);
                        assert(monster != nullptr);

                        monster->m_LifePoint = packet.entityCar.m_LifePoint;
                        monster->m_MaxLifePoint = packet.entityCar.m_MaxLifePoint;

                        monster->m_AttackPoint = packet.entityCar.m_AttackPoint;
                        monster->m_DefensePoint = packet.entityCar.m_DefensePoint;

                        monster->m_MaxAttackPoint = packet.entityCar.m_MaxAttackPoint;
                        monster->m_MaxDefensePoint = packet.entityCar.m_MaxDefensePoint;

                        monster->m_Range = packet.entityCar.m_Range;

                        monster->m_Level = packet.entityCar.m_Level;
                    }
                    
                    break;
                }
            }
        }
    }

    void Game::movePlayer( int dirX, int dirY )
    {
        if ( !m_CanPlay )
        {
            return;
        }

        Player* myPlayer = getPlayer(m_PlayerID);

        if ( myPlayer == nullptr )
        {
            return;
        }

        //used mouse clic
        if ( dirX != 0 && dirY != 0 )
        {
            m_TempMove.clear();

            std::vector<gf::Vector2i> allPos = m_World.m_SquareMap.computeRoute(myPlayer->m_Pos, {dirX, dirY}, 0.0);

            m_TempMove.insert(m_TempMove.end(), ++(++allPos.begin()), allPos.end());

            m_dirX = allPos[1][0];
            m_dirY = allPos[1][1];
        }
        else
        {
            m_dirX = dirX;
            m_dirY = dirY;
        }
    }

    void Game::attackPos( int posX, int posY )
    {
        if ( !m_CanPlay )
        {
            return;
        }
        
        m_AttackX = posX;
        m_AttackY = posY;
    }

    void Game::passTurn()
    {
        if ( !m_CanPlay )
        {
            return;
        }

        m_PassTurn = true;
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
            if ( it->second.m_Pos[0] == pos[0] && it->second.m_Pos[1] == pos[1] )
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
            if ( it->second.m_Pos[0] == pos[0] && it->second.m_Pos[1] == pos[1] )
            {
                return &it->second;
            }

            ++it;
        }

        return nullptr;
    }

    bool Game::playerNear()
    {
        Player* myPlayer = getPlayer(m_PlayerID);

        if ( myPlayer == nullptr )
        {
            return false;
        }

        gf::Distance2<int> distFn = gf::manhattanDistance<int, 2>;

        auto it = m_Players.begin();
 
        while ( it != m_Players.end() )
        {
            if ( (it->first != m_PlayerID) && distFn(myPlayer->m_Pos, it->second.m_Pos) <= 1 )
            {
                return true;
            }

            ++it;
        }

        return false;
    }
    
    bool Game::monsterNear()
    {
        Player* myPlayer = getPlayer(m_PlayerID);

        if ( myPlayer == nullptr )
        {
            return false;
        }

        gf::Distance2<int> distFn = gf::manhattanDistance<int, 2>;

        auto it = m_Monsters.begin();
 
        while ( it != m_Monsters.end() )
        {
            if ( distFn(myPlayer->m_Pos, it->second.m_Pos) <= 1 )
            {
                return true;
            }

            ++it;
        }

        return false;
    }
}
