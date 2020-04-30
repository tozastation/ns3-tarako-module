#ifndef TARAKO_TRACER_HPP
#define TARAKO_TRACER_HPP

#include "garbage_station.h"
#include "node_info.h"

#include "ns3/lora-net-device.h"
#include "ns3/group_node.h"
#include "ns3/ble-net-device.h"

#include <unordered_map> 

namespace ns3 {
namespace tarako {

    extern void OnLoRaWANEnergyConsumptionChange (
        NodeInfo* node, 
        double oldEnergy, 
        double newEnergy
    );
    extern void OnPacketRecievedAtNetworkServer (
        std::unordered_map<int, tarako::NodeInfo>* node_map, 
        Ptr<Packet const> packet
    );
    extern void OnActivateNode (
        Ptr<lorawan::LoraNetDevice> device, 
        GarbageBoxSensor* gs, 
        NodeInfo* node,
        Time interval
    );

    // --- For Grouping --- //
    extern void OnLoRaWANEnergyConsumptionChangeForGroup(
        tarako::TarakoNodeData* node_data, 
        double oldValue, 
        double newValue
    );
    extern void OnPacketRecievedAtNetworkServerForGroup (
        std::unordered_map<int, tarako::TarakoNodeData>* node_data_map, 
        Ptr<Packet const> packet
    );
    extern void OnActivateNodeForGroup ( 
        tarako::TarakoNodeData* node_data
    );

    extern void DataIndication (
        tarako::TarakoNodeData* node_data, 
        McpsDataIndicationParams params, 
        Ptr<Packet> packet
    );

    extern void DataConfirm (McpsDataConfirmParams params);

}
}
#endif