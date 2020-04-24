#ifndef TARAKO_TRACER_HPP
#define TARAKO_TRACER_HPP

#include "garbage_station.h"
#include "node_info.h"
#include <unordered_map> 
#include "ns3/lora-net-device.h"
#include "ns3/group_node.h"
#include "ns3/ble-net-device.h"

namespace ns3 {
namespace tarako {

    void OnLoRaWANEnergyConsumptionChange (
        NodeInfo* node, 
        double oldEnergy, 
        double newEnergy
    );


    void OnLoRaWANEnergyConsumptionChangeForGroup(
        tarako::TarakoNodeData* node_data, 
        double oldValue, 
        double newValue
    );

    void OnPacketRecievedAtNetworkServer (
        std::unordered_map<int, tarako::NodeInfo>* node_map, 
        Ptr<Packet const> packet
    );

    void OnPacketRecievedAtNetworkServerForGroup (
        std::unordered_map<int, tarako::TarakoNodeData>* node_data_map, 
        Ptr<Packet const> packet
    );
    
    void OnActivateNode (
        Ptr<lorawan::LoraNetDevice> device, 
        GarbageBoxSensor* gs, 
        NodeInfo* node,
        Time interval
    );

    void OnActivateNodeForGroup (
        tarako::GarbageBoxSensor* gs, 
        tarako::TarakoNodeData* node_data
    );

}
}
#endif