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

namespace ns3 {
namespace tarako {
    enum TarakoNodeStatus {
        group_leader = 1,
        group_member = 2,
        only_lorawan = 3
    };

    class TarakoNodeData
    {
        public:
        int id;
        int lora_network_addr;
        std::string ble_network_addr;
        ns3::Vector3D position;
        TarakoNodeStatus current_status;
        ns3::Time conn_interval;

        double total_energy_consumption;
        double lora_energy_consumption;
        double ble_energy_consumption;

        std::vector<std::tuple<int, std::string>> group_node_addrs;
        std::vector<Ptr<Packet>> sent_packets;
        std::vector<Ptr<Packet>> received_packets;

        Ptr<lorawan::LoraNetDevice> lora_net_device;
        Ptr<ns3::BleNetDevice> ble_net_device;
        TarakoNodeData();
        std::string ToString();
    };

}
}
#endif