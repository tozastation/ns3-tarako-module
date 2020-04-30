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
        // --- Free --- //
        delete []buffer;
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
        
        lorawan::LorawanMacHeader mHdr = {};
        lorawan::LoraFrameHeader fHdr = {};
        Ptr<Packet> lora_packet = packet->Copy ();
        lora_packet->RemoveHeader (mHdr);
        lora_packet->RemoveHeader (fHdr);
        
        int nwk_addr = int(fHdr.GetAddress().GetNwkAddr());
        node_data_map->at(nwk_addr).received_packets_by_lora.push_back(packet->Copy());
        // --- Print Payload from Recieved Packet ---
        uint8_t *packet_received_buffer = new uint8_t[lora_packet->GetSize()];
        lora_packet->CopyData(packet_received_buffer, lora_packet->GetSize());
        tarako::TarakoNodeStatus node_status = node_data_map->at(nwk_addr).current_status;
        NS_LOG_INFO("---  OnPacketRecievedAtNetworkServerForGroup ---");
        NS_LOG_INFO("(id)                 : " << node_data_map->at(nwk_addr).id);
        NS_LOG_INFO("(lora net addr)      : " << node_data_map->at(nwk_addr).lora_network_addr);
        if (node_status == tarako::TarakoNodeStatus::group_member)
        {
            NS_LOG_INFO("(mode)   : " << "grouping");
            tarako::TarakoGroupLeaderPayload gl_payload = {};
            memcpy(&gl_payload, packet_received_buffer, sizeof(gl_payload));
            for (auto& gm: gl_payload.node_infos)
            {
                NS_LOG_INFO("(from)   : " << std::get<0>(gm));
                NS_LOG_INFO("(status) : " << std::get<1>(gm));
            }
        }
        else
        {
            NS_LOG_INFO("(mode)   : " << "only lorawan");
            tarako::LoRaWANPayload lorawan_payload = {};
            memcpy(&lorawan_payload, packet_received_buffer, sizeof(lorawan_payload));
            NS_LOG_INFO("(status) : " << lorawan_payload.c);
        }
        NS_LOG_INFO("------------------------------------------------");
        // --- Free --- //
        delete []packet_received_buffer;
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
                LoRaWANPayload lorawan_payload = LoRaWANPayload{status};
                Ptr<Packet> new_packet = Create<Packet>((uint8_t *)&lorawan_payload, sizeof(lorawan_payload));
                node_data->lora_net_device->Send(new_packet);
                node_data->sent_packets_by_lora.push_back(new_packet);
                NS_LOG_INFO("---  OnActivateNodeForGroup::tarako::TarakoNodeStatus::only_lorawan ---");
                NS_LOG_INFO("(id)                 : " << node_data->id);
                NS_LOG_INFO("(lora net addr)      : " << node_data->lora_network_addr);
                NS_LOG_INFO("(garbage box status) : " << lorawan_payload.c);
                NS_LOG_INFO("-----------------------------------------------------------------------");
                Simulator::Schedule(node_data->conn_interval, &OnActivateNodeForGroup, node_data);
                break;
            }
            case tarako::TarakoNodeStatus::group_leader:
            {
                NS_LOG_INFO("---  OnActivateNodeForGroup::tarako::TarakoNodeStatus::group_leader ---");
                NS_LOG_INFO("(id)                 : " << node_data->id);
                NS_LOG_INFO("(lora net addr)      : " << node_data->lora_network_addr);
                NS_LOG_INFO("-----------------------------------------------------------------------");
                break;
            }
            case tarako::TarakoNodeStatus::group_member:
            {
                TarakoGroupMemberPayload gm_payload = TarakoGroupMemberPayload{
                    status,
                    node_data->lora_energy_consumption,
                    static_cast<int>(node_data->sent_packets_by_ble.size())
                };
                
                Ptr<Packet> new_packet = Create<Packet>((uint8_t *)&gm_payload, sizeof(gm_payload));
                McpsDataRequestParams params;
                params.m_dstPanId = 0;
                params.m_srcAddrMode = SHORT_ADDR;
                params.m_dstAddrMode = SHORT_ADDR;
                params.m_dstAddr = Mac16Address (node_data->leader_node_addr.c_str());
                node_data->lr_wpan_net_device->GetMac()->McpsDataRequest(params, new_packet);
                
                node_data->sent_packets_by_ble.push_back(new_packet);
                
                NS_LOG_INFO("---  OnActivateNodeForGroup::tarako::TarakoNodeStatus::group_member ---");
                NS_LOG_INFO("(id)                      : " << node_data->id);
                NS_LOG_INFO("(lora net addr)           : " << node_data->lora_network_addr);
                NS_LOG_INFO("(ble net addr)            : " << node_data->ble_network_addr);
                NS_LOG_INFO("(leader net addr)         : " << node_data->leader_node_addr);
                NS_LOG_INFO("(garbage box status)      : " << gm_payload.status);
                NS_LOG_INFO("(lora energy consumption) : " << gm_payload.lora_energy_consumption);
                NS_LOG_INFO("(ble count)               : " << gm_payload.cnt_ble);
                NS_LOG_INFO("(number of sent)          : " << node_data->sent_packets_by_lora.size());
                NS_LOG_INFO("-----------------------------------------------------------------------");
                Simulator::Schedule(node_data->conn_interval, &OnActivateNodeForGroup, node_data);
                break;
            }
            default:
            {
                // std::cout << "[STATUS] error" << std::endl;
                break;
            }
        }
    }

    void DataIndication (tarako::TarakoNodeData* node_data , McpsDataIndicationParams params, Ptr<Packet> packet)
    {
        std::ostringstream ble_src_addr;
        ble_src_addr << params.m_srcAddr;
        
        auto node_status = node_data->current_status;
        if (node_status == tarako::TarakoNodeStatus::group_leader)
        {
            int group_member_n              = node_data->group_node_addrs.size();
            int buffered_packet_n           = node_data->buffered_packets.size();
            double total_energy_consumption = 0.0;
            std::vector<std::tuple<std::string, double>> node_energy_consumptions;
            
            if (group_member_n == buffered_packet_n)
            {
                tarako::TarakoGroupLeaderPayload gl_payload = {};
                for (auto bp: node_data->buffered_packets)
                {
                    std::string sent_from = std::get<0>(bp);
                    Ptr<Packet> data_sent = std::get<1>(bp);
                    tarako::TarakoGroupMemberPayload gm_payload = {};
                    uint8_t *data_sent_buffer = new uint8_t[data_sent->GetSize()];
                    data_sent->CopyData(data_sent_buffer, data_sent->GetSize());
                    memcpy(&gm_payload, data_sent_buffer, sizeof(gm_payload));
                    gl_payload.node_infos.push_back({sent_from, gm_payload.status});
                    
                    total_energy_consumption = gm_payload.lora_energy_consumption + (gm_payload.cnt_ble * 0.003);
                    node_energy_consumptions.push_back({sent_from, total_energy_consumption});
                    delete []data_sent_buffer;
                }
                // My Info
                total_energy_consumption = node_data->lora_energy_consumption + (node_data->sent_packets_by_ble.size() * 0.003);
                node_energy_consumptions.push_back({ble_src_addr.str(), total_energy_consumption});
                // Send Packet
                Ptr<Packet> new_packet = Create<Packet>((uint8_t *)&gl_payload, sizeof(gl_payload));
                // TODO: Garbage Box Sensor
                node_data->lora_net_device->Send(new_packet);
                node_data->sent_packets_by_lora.push_back(new_packet);
                NS_LOG_INFO("----  DataIndication.SendAggregatePayload()  ----");
                NS_LOG_INFO("(id)                : " << node_data->id);
                NS_LOG_INFO("(lora net addr)     : " << node_data->lora_network_addr);
                NS_LOG_INFO("(ble net addr)      : " << node_data->ble_network_addr);
                NS_LOG_INFO("(data size)         : " << sizeof(gl_payload));
                NS_LOG_INFO("(aggregate num)     : " << gl_payload.node_infos.size());
                NS_LOG_INFO("(group num)         : " << node_data->group_node_addrs.size() + 1);
                NS_LOG_INFO("-------------------------------------------------");
                // Judge Next Leader
                std::string next_leader_id = tarako::TarakoUtil::GetNextGroupLeader(node_energy_consumptions);
                if (!next_leader_id.empty() && next_leader_id != node_data->leader_node_addr) 
                { 
                    for (auto node: node_data->group_node_addrs)
                    {
                        tarako::TarakoGroupDownlink downlink = tarako::TarakoGroupDownlink{next_leader_id};
                        Ptr<Packet> new_packet = Create<Packet>((uint8_t *)&downlink, sizeof(downlink));
                        McpsDataRequestParams params = {};
                        params.m_dstPanId    = 0;
                        params.m_srcAddrMode = SHORT_ADDR;
                        params.m_dstAddrMode = SHORT_ADDR;
                        params.m_dstAddr     = Mac16Address (std::get<1>(node).c_str());
                        node_data->lr_wpan_net_device->GetMac()->McpsDataRequest(params, new_packet);
                        NS_LOG_INFO("----  DataIndication.JudgeNextLeader()  ----");
                        NS_LOG_INFO("(id)                : " << node_data->id);
                        NS_LOG_INFO("(lora net addr)     : " << node_data->lora_network_addr);
                        NS_LOG_INFO("(ble net addr)      : " << node_data->ble_network_addr);
                        NS_LOG_INFO("(next leader)       : " << next_leader_id);
                        NS_LOG_INFO("(notify to)         : " << std::get<1>(node));
                        NS_LOG_INFO("--------------------------------------------");
                    }
                    // Update My Node Status -> Leader to Member
                    node_data->current_status   = tarako::TarakoNodeStatus::group_member;
                    node_data->leader_node_addr = next_leader_id;
                    Simulator::Schedule(node_data->conn_interval, &OnActivateNodeForGroup, node_data);
                }
            }
            else
            {
                node_data->buffered_packets.push_back({ble_src_addr.str(), packet});
                Ptr<Packet> received_packet = packet->Copy ();
                uint8_t *from_group_member_buffer = new uint8_t[received_packet->GetSize()];
                received_packet->CopyData(from_group_member_buffer, received_packet->GetSize());
                tarako::TarakoGroupMemberPayload gm_payload = {};
                memcpy(&gm_payload, from_group_member_buffer, sizeof(gm_payload));
            
                NS_LOG_INFO("----  DataIndication.PushToBufferdPackets()  ----");
                NS_LOG_INFO("(id)                : " << node_data->id);
                NS_LOG_INFO("(lora net addr)     : " << node_data->lora_network_addr);
                NS_LOG_INFO("(ble net addr)      : " << node_data->ble_network_addr);
                NS_LOG_INFO("(payload.status)    : " << gm_payload.status);
                NS_LOG_INFO("(from)              : " << ble_src_addr.str());
                NS_LOG_INFO("-------------------------------------------------");
                delete []from_group_member_buffer;
            }
        }
        else if(node_status == tarako::TarakoNodeStatus::group_member)
        {
            node_data->received_packets_by_ble.push_back(packet->Copy());
            tarako::TarakoGroupDownlink down_link = {};

            packet->Print(std::cout);
            Ptr<Packet> received_packet       = packet->Copy ();
            uint8_t *from_gl = new uint8_t[received_packet->GetSize()];
            received_packet->CopyData(from_gl, received_packet->GetSize());
            memcpy(&down_link, from_gl, sizeof(down_link));
            
            if (!down_link.next_leader_id.empty())
            {
                if (node_data->ble_network_addr == down_link.next_leader_id)
                {
                    node_data->current_status = tarako::TarakoNodeStatus::group_leader;
                    node_data->leader_node_addr = node_data->ble_network_addr;
                    NS_LOG_INFO("[UPDATE] Topology");
                }
                else if(down_link.next_leader_id.length() == 5)
                {
                    node_data->leader_node_addr = down_link.next_leader_id;
                    NS_LOG_INFO("[UPDATE] Topology");
                }
                else
                {
                    NS_LOG_INFO("[Error] invalid payload");
                }
            }
            NS_LOG_INFO("----  DataIndication.ReceivedFromGL()  ----");
            NS_LOG_INFO("(id)                : " << node_data->id);
            NS_LOG_INFO("(lora net addr)     : " << node_data->lora_network_addr);
            NS_LOG_INFO("(ble net addr)      : " << node_data->ble_network_addr);
            NS_LOG_INFO("(from)              : " << ble_src_addr.str());
            NS_LOG_INFO("(next leader)       : " << down_link.next_leader_id);
            NS_LOG_INFO("(current status)    : " << node_data->current_status);
            NS_LOG_INFO("(tx time)           : " << node_data->sent_packets_by_ble.size());
            NS_LOG_INFO("(rx time)           : " << node_data->received_packets_by_ble.size());
            NS_LOG_INFO("--------------------------------------------");
            delete []from_gl;
        }
    }
    
    void DataConfirm (McpsDataConfirmParams params){}

}
}