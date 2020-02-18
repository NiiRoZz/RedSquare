#ifndef REDSQUARE_SERVER_NETWORK_H
#define REDSQUARE_SERVER_NETWORK_H

#include <atomic>
#include <map>
#include <set>

#include <gf/Random.h>
#include <gf/Ref.h>
#include <gf/SocketSelector.h>
#include <gf/TcpListener.h>

#include "ServerLobby.h"
#include "ServerPlayer.h"
#include "GameFactory.h"

namespace redsquare 
{
    class ServerNetwork
    {
    public:
        ServerNetwork(gf::Random& random, GameFactory& factory);

        void run();

    private:
        static void signalHandler(int sig);

    private:
        gf::Ref<gf::Random> m_Random;
        gf::TcpListener m_Listener;
        gf::SocketSelector m_Selector;

        std::map<gf::Id, ServerPlayer> m_Players;
        ServerLobby m_Lobby;

        static std::atomic_bool g_running;
    };
}

#endif // REDSQUARE_SERVER_NETWORK_H
