#ifndef REDSQUARE_COMMON_SINGLETONS_H
#define REDSQUARE_COMMON_SINGLETONS_H

#include <gf/MessageManager.h>
#include <gf/Random.h>
#include <gf/Singleton.h>
#include <gf/ResourceManager.h>

namespace redsquare
{
  extern gf::Singleton<gf::MessageManager> gMessageManager;
  extern gf::Singleton<gf::ResourceManager> gResourceManager;
  extern gf::Singleton<gf::Random> gRandom;
}

#endif // REDSQUARE_COMMON_SINGLETONS_H
