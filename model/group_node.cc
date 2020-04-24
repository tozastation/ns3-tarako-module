/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#include "group_node.h"

namespace ns3 {
namespace tarako {
    TarakoNodeData::TarakoNodeData(){};

    std::string TarakoNodeData::ToString()
    {
        std::stringstream ss;
        ss << "    -------------    " << std::endl;
        ss << "id: " << this->id << std::endl;
        ss << "lora network addr: " << this->lora_network_addr << std::endl;
        ss << "ble network addr: " << this->ble_network_addr << std::endl;
        ss << "position: " << this->position << std::endl;
        ss << "current status: " << this->current_status << std::endl;
        ss << "connection interval: " << this->conn_interval << std::endl;
        ss << "total energy consumption: " << this->total_energy_consumption << std::endl;
        ss << "lorawan energy consumption: " << this->lora_energy_consumption << std::endl;
        ss << "ble energy consumption: " << this->ble_energy_consumption << std::endl;
        ss << "sent packets number: " << this->sent_packets.size() << std::endl;
        ss << "received packets number: " << std::endl;
        for (auto& node_addr: this->group_node_addrs)
        {
            ss << "lora: " << std::get<0>(node_addr) << ", ";
            ss << "ble: " << std::get<1>(node_addr) << std::endl;
        }
        ss << "    -------------    " << std::endl;
        return ss.str();
    };
}
}