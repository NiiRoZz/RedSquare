#ifndef REDSQUARE_PROTOCOL_BYTES_PACKET_H
#define REDSQUARE_PROTOCOL_BYTES_PACKET_H

#include <gf/Id.h>
#include <gf/Packet.h>
#include <gf/Streams.h>
#include <gf/Serialization.h>
#include <gf/SerializationOps.h>

namespace redsquare
{
    struct ProtocolBytes
    {
        gf::Id type = gf::InvalidId;
        gf::Packet packet;

        gf::Id getType()
        {
            if (type != gf::InvalidId)
            {
                return type;
            }

            gf::BufferInputStream stream(&packet.bytes);
            gf::Deserializer deserializer(stream);
            deserializer | type;
            return type;
        }

        template<typename T>
        T as()
        {
            gf::BufferInputStream stream(&packet.bytes);
            gf::Deserializer deserializer(stream);

            T data;
            deserializer | type | data;
            assert(T::type == type);
            return data;
        }

        template<typename T>
        void is(const T& data)
        {
            packet.bytes.clear();
            type = T::type;
            gf::BufferOutputStream stream(&packet.bytes);
            gf::Serializer serializer(stream);
            serializer | type | const_cast<T&>(data);
        }
    };
}

#endif // REDSQUARE_PROTOCOL_BYTES_PACKET_H
