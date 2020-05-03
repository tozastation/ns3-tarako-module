/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#include "group_node.h"

namespace ns3 {
namespace tarako {
    TarakoNodeData::TarakoNodeData(){};

    std::string TarakoNodeData::ToString()
    {
        std::stringstream ss;
        ss << "    -------------------------------------------------    " << std::endl;
        ss << "id                               : " << this->id << std::endl;
        ss << "lora network addr                : " << this->lora_network_addr << std::endl;
        ss << "ble network addr                 : " << this->ble_network_addr << std::endl;
        ss << "belong to                        : " << this->belong_to << std::endl;
        ss << "position                         : " << this->position << std::endl;
        ss << "current status                   : " << this->current_status << std::endl;
        ss << "activate time                    : " << this->activate_time.GetSeconds() << "s" << std::endl;
        ss << "connection interval              : " << this->conn_interval.GetSeconds() << "s" << std::endl;
        ss << "total energy consumption         : " << this->total_energy_consumption << "A" << std::endl;
        ss << "lorawan energy consumption       : " << this->lora_energy_consumption << "A" << std::endl;
        ss << "ble energy consumption           : " << this->ble_energy_consumption << "A" << std::endl;
        ss << "sent packets by lora number      : " << this->sent_packets_by_lora.size() << std::endl;
        ss << "received packets by lora number  : " << this->received_packets_by_lora.size() << std::endl;
        ss << "sent packets by ble number       : " << this->sent_packets_by_ble.size() << std::endl;
        ss << "received packets by ble number   : " << this->received_packets_by_ble.size() << std::endl;
        ss << "group leader node addr           : " << this->leader_node_addr << std::endl;
        for (auto& node_addr: this->group_node_addrs)
        {
            ss << "lora: " << std::get<0>(node_addr) << ", ";
            ss << "ble: " << std::get<1>(node_addr) << std::endl;
        }
        ss << "    -------------------------------------------------    " << std::endl;
        return ss.str();
    };
}
}