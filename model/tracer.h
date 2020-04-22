#ifndef TARAKO_TRACER_HPP
#define TARAKO_TRACER_HPP

#include "garbage_station.h"
#include "node_info.h"
#include <unordered_map> 
#include "ns3/lora-net-device.h"

namespace ns3 {
namespace tarako {

    void OnLoRaWANEnergyConsumptionChange (
        NodeInfo* node, 
        double oldEnergy, 
        double newEnergy
    );

    void OnPacketRecievedAtNetworkServer (
        std::unordered_map<int, NodeInfo>* node_map, 
        Ptr<Packet const> packet
    );
    
    void OnActivateNode (
        Ptr<lorawan::LoraNetDevice> device, 
        GarbageBoxSensor* gs, 
        NodeInfo* node,
        Time interval
    );

}
}
#endif