#ifndef REDSQUARE_CLIENT_CLIENTENTITY_H
#define REDSQUARE_CLIENT_CLIENTENTITY_H

#include "../common/Entity.h"
#include "Inventory.h"
#include <gf/Entity.h>
#include <gf/Texture.h>
#include <gf/Animation.h>

namespace redsquare
{
    class ClientEntity: public redsquare::Entity
    {
    public:
        static gf::Texture* loadTexture(const EntitySubType entitySubType);

    public:
        ClientEntity( gf::Id entityID, const EntityType entityType, const EntitySubType entitySubType );

        Inventory& getInventory();

        gf::Texture* getEntityTexture() const;

        void loadCharacteristic(EntityCharacteristicData &characteristics);

        void loadAnimation( gf::Path pathTextureAnimated, int line, int startFramePos, int nmbFrames, float frameDuration);

    protected:
        Inventory m_Inventory;
        gf::Texture* m_EntityTexture;
        bool m_Animated;
        gf::Animation m_Animation;
    };
}

#endif