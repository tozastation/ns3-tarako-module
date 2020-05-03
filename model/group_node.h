#ifndef GROUP_NODE_HPP
#define GROUP_NODE_HPP

#include <string>
#include <vector>

#include "ns3/packet.h"
#include "ns3/ptr.h"
#include "ns3/timer.h"
#include "ns3/position-allocator.h"
#include "ns3/ble-net-device.h"
#include "ns3/lora-net-device.h"
#include "garbage_station.h"
#include <ns3/lr-wpan-net-device.h>

namespace ns3 {
namespace tarako {
    enum TarakoNodeStatus: int {
        group_leader = 1,
        group_member = 2,
        only_lorawan = 3
    };

    class TarakoNodeData
    {
        public:
        // Basic Info
        int id;
        std::string belong_to;
        
        ns3::Vector3D position;
        ns3::Time activate_time;
        ns3::Time conn_interval;
        double total_energy_consumption;
        // Group Info
        TarakoNodeStatus current_status;
        std::string leader_node_addr;
        std::vector<std::tuple<int, std::string>> group_node_addrs;
        bool EnableGrouping;
        bool EnableEqualization; 
        // LoRaWAN
        int lora_network_addr;
        double lora_energy_consumption;
        std::vector<Ptr<Packet>> sent_packets_by_lora;
        std::vector<Ptr<Packet>> received_packets_by_lora;
        // BLE
        std::string ble_network_addr;
        double ble_energy_consumption;
        std::vector<Ptr<Packet>> sent_packets_by_ble;
        std::vector<Ptr<Packet>> received_packets_by_ble;
        std::vector<std::tuple<std::string, Ptr<Packet>>> buffered_packets;
        // Net Device Object
        Ptr<lorawan::LoraNetDevice> lora_net_device;
        Ptr<ns3::LrWpanNetDevice> lr_wpan_net_device;
        Ptr<ns3::BleNetDevice> ble_net_device;
        // Sensor
        tarako::GarbageBoxSensor sensor;
        TarakoNodeData();
        std::string ToString();
    };

    struct TarakoGroupLeaderPayload {
        std::vector<std::tuple<std::string, GarbageBoxStatus>> node_infos;
    };

    struct TarakoGroupMemberPayload {
        GarbageBoxStatus status;
        double lora_energy_consumption;
        int cnt_ble;
    };

    struct TarakoGroupDownlink {
        ns3::Mac16Address next_leader_id;
    };
}
}
#endif