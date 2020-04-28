/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#include "ns3/lora-frame-header.h"
#include "ns3/lorawan-mac-header.h"
#include "ns3/packet.h"
#include "ns3/ble-phy.h"
#include "ns3/log.h"
#include "ns3/ble-link-manager.h"
#include "ns3/lr-wpan-mac.h"

#include "payload.h"
#include "util.h"
#include "const.h"
#include "tracer.h"

#include <unordered_map> 
#include "random"

NS_LOG_COMPONENT_DEFINE ("TarakoTracer");

namespace ns3 {
namespace tarako {
    void OnLoRaWANEnergyConsumptionChange (NodeInfo* node,  double oldEnergy, double newEnergy)
    {
        std::ostringstream oss;
        node->energy_consumption = newEnergy;
    }

    void OnPacketRecievedAtNetworkServer (std::unordered_map<int, NodeInfo>* node_map, Ptr<Packet const> packet)
    {
        // --- Remove Header From Wrapper Packet ---
        lorawan::LorawanMacHeader mHdr;
        lorawan::LoraFrameHeader fHdr;
        Ptr<Packet> myPacket = packet->Copy ();
        myPacket->RemoveHeader (mHdr);
        myPacket->RemoveHeader (fHdr);
        node_map->at(int(fHdr.GetAddress().GetNwkAddr())).recieved_packets.push_back(packet->Copy());
        // --- Print Payload from Recieved Packet ---
        uint8_t *buffer = new uint8_t[myPacket->GetSize()];
        myPacket->CopyData(buffer, myPacket->GetSize());
        LoRaWANPayload payload;
        memcpy(&payload, buffer, sizeof(payload));
    }

    void OnActivateNode (Ptr<lorawan::LoraNetDevice> device, GarbageBoxSensor* gs, NodeInfo* node, Time interval)
    {
        std::cout << "[INFO] target node: " << node->id << std::endl;
        unsigned int random_value = tarako::TarakoUtil::CreateRandomInt(
            tarako::TarakoConst::RANDOM_VALUE_RANGE_BEGIN,
            tarako::TarakoConst::RANDOM_VALUE_RANGE_END
        );
        std::cout << "[INFO] add random value to garbage box: " << random_value << std::endl;
        GarbageBoxStatus status =  tarako::TarakoUtil::GetGarbageBoxStatus(
            gs, 
            random_value,
            tarako::TarakoConst::GARBAGE_BOX_VOLUME
            );
        std::cout << "[INFO] garbage box status: " << status << std::endl;
        LoRaWANPayload payload = LoRaWANPayload{status};
        Ptr<Packet> new_packet = Create<Packet>((uint8_t *)&payload, sizeof(payload));
        device->Send(new_packet);
        node->sent_packets.push_back(new_packet);
        std::cout << "[INFO] send packet" << std::endl;
        Simulator::Schedule(Seconds(10), &OnActivateNode, device, gs, node, Seconds(10));
    }

    void OnLoRaWANEnergyConsumptionChangeForGroup(tarako::TarakoNodeData* node_data, double oldValue, double newValue)
    {
        std::ostringstream oss;
        node_data->lora_energy_consumption = newValue;
    }

    void OnPacketRecievedAtNetworkServerForGroup (std::unordered_map<int, tarako::TarakoNodeData>* node_data_map, Ptr<Packet const> packet)
    {
        // --- Remove Header From Wrapper Packet ---
        lorawan::LorawanMacHeader mHdr;
        lorawan::LoraFrameHeader fHdr;
        Ptr<Packet> myPacket = packet->Copy ();
        myPacket->RemoveHeader (mHdr);
        myPacket->RemoveHeader (fHdr);
        int nwk_addr = int(fHdr.GetAddress().GetNwkAddr());
        node_data_map->at(nwk_addr).received_packets_by_lora.push_back(packet->Copy());
        // --- Print Payload from Recieved Packet ---
        uint8_t *buffer = new uint8_t[myPacket->GetSize()];
        myPacket->CopyData(buffer, myPacket->GetSize());
        LoRaWANPayload payload;
        memcpy(&payload, buffer, sizeof(payload));
    }

    void OnActivateNodeForGroup (
        tarako::TarakoNodeData* node_data
    ){
        // std::cout << "[INFO] target node: " << node_data->id << std::endl;
        unsigned int random_value = tarako::TarakoUtil::CreateRandomInt(
            tarako::TarakoConst::RANDOM_VALUE_RANGE_BEGIN,
            tarako::TarakoConst::RANDOM_VALUE_RANGE_END
        );
        // std::cout << "[INFO] add random value to garbage box: " << random_value << std::endl;
        GarbageBoxStatus status =  tarako::TarakoUtil::GetGarbageBoxStatus(
            &node_data->sensor, 
            random_value,
            tarako::TarakoConst::GARBAGE_BOX_VOLUME
        );
        switch (node_data->current_status)
        {
            case tarako::TarakoNodeStatus::only_lorawan:
            {
                LoRaWANPayload payload = LoRaWANPayload{status};
                Ptr<Packet> new_packet = Create<Packet>((uint8_t *)&payload, sizeof(payload));
                node_data->lora_net_device->Send(new_packet);
                node_data->sent_packets_by_lora.push_back(new_packet);
                NS_LOG_INFO("---  tarako::TarakoNodeStatus::only_lorawan ---");
                NS_LOG_INFO("(i am)               : " << node_data->id);
                NS_LOG_INFO("(garbage box status) : " << payload.c);
                NS_LOG_INFO("(sent num)           : " << node_data->sent_packets_by_lora.size());
                NS_LOG_INFO("-----------------------------------------------");
                break;
            }
            case tarako::TarakoNodeStatus::group_leader:
            {
                if (node_data->leader_node_addr == node_data->ble_network_addr)
                {
                    // std::cout << "[INFO] I'm Leader (Receiver) : " << node_data->ble_network_addr << std::endl;
                }
                break;
            }
            case tarako::TarakoNodeStatus::group_member:
            {
                // [INFO] Prepare Send Data to Group Leader
                TarakoGroupMemberPayload payload = TarakoGroupMemberPayload{
                    status,
                    node_data->lora_energy_consumption,
                    static_cast<int>(node_data->sent_packets_by_ble.size())
                };
                Ptr<Packet> new_packet = Create<Packet>((uint8_t *)&payload, sizeof(payload));
                McpsDataRequestParams params;
                params.m_dstPanId = 0;
                params.m_srcAddrMode = SHORT_ADDR;
                params.m_dstAddrMode = SHORT_ADDR;
                params.m_dstAddr = Mac16Address (node_data->leader_node_addr.c_str());
                // [INFO] Sent
                node_data->lr_wpan_net_device->GetMac()->McpsDataRequest(params, new_packet);
                node_data->sent_packets_by_ble.push_back(new_packet);
                break;
            }
            default:
            {
                // std::cout << "[STATUS] error" << std::endl;
                break;
            }
        }
        Simulator::Schedule(node_data->conn_interval, &OnActivateNodeForGroup, node_data);
    }
}
}