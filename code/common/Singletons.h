#ifndef REDSQUARE_COMMON_SINGLETONS_H
#define REDSQUARE_COMMON_SINGLETONS_H

#include <gf/AssetManager.h>
#include <gf/Random.h>
#include <gf/Singleton.h>

namespace redsquare
{
  //extern gf::Singleton<gf::AssetManager> gAssetManager;
  extern gf::Singleton<gf::Random> gRandom;
}

#endif // REDSQUARE_COMMON_SINGLETONS_H
