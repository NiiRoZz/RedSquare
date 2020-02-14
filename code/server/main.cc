#include "RedsquareFactory.h"
#include "ServerNetwork.h"
#include "../common/Singletons.h"

#include <gf/Random.h>

using namespace redsquare;

int main()
{
	gf::SingletonStorage<gf::MessageManager> storageForMessageManager(redsquare::gMessageManager);

	RedsquareFactory factory;

	srand(time(nullptr));

	gf::Random random;
	ServerNetwork network(random, factory);
	network.run();
	return EXIT_SUCCESS;
}
