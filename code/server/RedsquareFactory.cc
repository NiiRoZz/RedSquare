#include "RedsquareFactory.h"

#include "RedsquareInstance.h"

namespace redsquare
{
    std::unique_ptr<GameInstance> RedsquareFactory::createInstance()
    {
      return std::make_unique<RedsquareInstance>();
    }
}
