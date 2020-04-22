#ifndef TARAKO_PAYLOAD_HPP
#define TARAKO_PAYLOAD_HPP

#include "garbage_station.h"

namespace ns3 {
namespace tarako {

struct LoRaWANPayload 
{
    GarbageBoxStatus c;
};

}
}
#endif