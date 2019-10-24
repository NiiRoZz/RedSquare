#include "Game.h"

#include <iostream>
#include <utility>

namespace redsquare
{
    Game::Game( char* hostname, char *port, gf::ExtendView &view )
    : m_ThreadCom(hostname, port, m_ComQueue)
    , m_View(&view)
    , m_CanPlay( false )
    , m_dirX(0)
    , m_dirY(0)
    , m_AttackX(0)
    , m_AttackY(0)
    {
    }

    void Game::startThreadCom()
    {
        m_ThreadCom.start();
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
        
        auto it = m_Players.begin();
 
        // Iterate over the map using Iterator till end.
        while (it != m_Players.end())
        {
            it->second.render( target, states );

            ++it;
        }

        auto it2 = m_Monsters.begin();
 
        // Iterate over the map using Iterator till end.
        while (it2 != m_Monsters.end())
        {
            it2->second.render( target, states );

            ++it2;
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

            m_View->setCenter( { (float)(player->m_Pos[0] * World::TileSize), (float)(player->m_Pos[1] * World::TileSize )} );

            //Do all actions stuff here
            doAction();
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
                    Player* player = getPlayer( packet.receiveMove.playerID );
                    assert( player != nullptr );
                    
                    player->m_Pos[0] = packet.receiveMove.posX;
                    player->m_Pos[1] = packet.receiveMove.posY;
                    break;
                }

                case PacketType::PlayerDisconnected:
                {
                    Player* player = getPlayer( packet.playerDisconnected.playerID );
                    assert( player != nullptr );
                    
                    m_Players.erase( packet.playerDisconnected.playerID );
                    break;
                }

                case PacketType::PlayerTurn:
                {
                    m_CanPlay = packet.playerTurn.playerTurn;
                    std::cout << "It's your turn!!!" << std::endl;
                    break;
                }

                case PacketType::SpawnEntity:
                {
                    if ( packet.spawnEntity.typeEntity == EntityType::Player )
                    {
                        auto it = m_Players.insert( std::make_pair( packet.spawnEntity.entityID, Player( gf::Vector2i(packet.spawnEntity.posX, packet.spawnEntity.posY), packet.spawnEntity.typeOfEntity ) ) );
                        assert( it.second );
                    }
                    else if ( packet.spawnEntity.typeEntity == EntityType::Monster )
                    {
                        auto it = m_Monsters.insert( std::make_pair( packet.spawnEntity.entityID, Monster( gf::Vector2i(packet.spawnEntity.posX, packet.spawnEntity.posY), packet.spawnEntity.typeOfEntity ) ) );
                        assert( it.second );
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

                        player->m_MovePoint = packet.entityCar.m_MovePoint;
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

                        monster->m_MovePoint = packet.entityCar.m_MovePoint;
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
        
        m_dirX=dirX;
        m_dirY=dirY;
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
}
