#include "ServerNetwork.h"

#include <csignal>
#include <cinttypes>
#include <stdexcept>

#include <gf/Log.h>

#include "../common/Protocol.h"

namespace redsquare
{
    ServerNetwork::ServerNetwork(gf::Random& random, GameFactory& factory)
    : m_Random(random)
    , m_Listener(Service)
    , m_Lobby(random, factory)
    {
        if (!m_Listener)
        {
            throw std::runtime_error("Can not start listener.");
        }

        std::signal(SIGINT, &ServerNetwork::signalHandler);
        m_Selector.addSocket(m_Listener);

        gf::Log::info("(SERVER) Waiting for connections on port %s...\n", Service);
    }

    void ServerNetwork::run()
    {
        static constexpr gf::Time Timeout = gf::seconds(1); // wake up the server every seconds to see if something happened

        while (g_running)
        {
            auto status = m_Selector.wait(Timeout);

            if (status == gf::SocketSelectorStatus::Timeout)
            {
                continue;
            }

            if (status == gf::SocketSelectorStatus::Error)
            {
                gf::Log::error("(SERVER) An error occurred in the selector. Stopping the server...\n");
                break;
            }

            std::vector<gf::Id> purgatory;

            for (auto& kv : m_Players)
            {
                auto& player = kv.second;

                if (m_Selector.isReady(player.socket))
                {
                    ProtocolBytes bytes;

                    switch (player.socket.recvPacket(bytes.packet))
                    {
                        case gf::SocketStatus::Data:
                        {
                            switch (bytes.getType())
                            {
                                case ClientDisconnect::type:
                                    gf::Log::info("(SERVER) {%" PRIX64 "} Disconnected.\n", player.id);
                                    purgatory.push_back(player.id);
                                break;

                                default:
                                    m_Lobby.update(player, bytes);
                                break;
                            }

                            break;
                        }
                        
                        case gf::SocketStatus::Error:
                            gf::Log::error("(SERVER) {%" PRIX64 "} Error receiving data.\n", player.id);
                            // fallthrough
                            
                        case gf::SocketStatus::Close:
                            gf::Log::info("(SERVER) {%" PRIX64 "} Socket closed.\n", player.id);
                            purgatory.push_back(player.id);
                        break;
                        
                        case gf::SocketStatus::Block:
                            assert(false);
                        break;
                    }
                }
            }

            if (m_Selector.isReady(m_Listener))
            {
                gf::TcpSocket socket = m_Listener.accept();

                if (!socket)
                {
                    continue;
                }

                gf::Id id = m_Random.get().computeId(); // assume it's unique

                ServerPlayer playerInstance;
                playerInstance.id = id;
                playerInstance.name = ""; // no name yet
                playerInstance.socket = std::move(socket);

                auto res = m_Players.emplace(id, std::move(playerInstance));
                assert(res.second);
                auto& player = res.first->second;
                m_Selector.addSocket(player.socket);
                m_Lobby.addPlayer(player);

                gf::Log::info("(SERVER) {%" PRIX64 "} Connected.\n", player.id);
            }

            if (!purgatory.empty())
            {
                for (auto id : purgatory)
                {
                    auto it = m_Players.find(id);
                    assert(it != m_Players.end());
                    auto& player = it->second;
                    m_Selector.removeSocket(player.socket);
                    m_Lobby.removePlayer(player);
                    m_Players.erase(it);
                }
            }
        }
    }

    void ServerNetwork::signalHandler(int sig)
    {
        assert(sig == SIGINT);
        g_running = false;
    }

    std::atomic_bool ServerNetwork::g_running(true);
}

