/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#include "ns3/lora-frame-header.h"
#include "ns3/lorawan-mac-header.h"
#include "ns3/packet.h"

#include "payload.h"
#include "util.h"
#include "const.h"
#include "tracer.h"

#include <unordered_map> 
#include "random"

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
        unsigned int random_value = tarako::TarakoUtil::CreaterRandomInt(
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
        node_data_map->at(nwk_addr).received_packets.push_back(packet->Copy());
        // --- Print Payload from Recieved Packet ---
        uint8_t *buffer = new uint8_t[myPacket->GetSize()];
        myPacket->CopyData(buffer, myPacket->GetSize());
        LoRaWANPayload payload;
        memcpy(&payload, buffer, sizeof(payload));
    }

    void OnActivateNodeForGroup (
        tarako::GarbageBoxSensor* gs, 
        tarako::TarakoNodeData* node_data
    ){
        // std::cout << "[INFO] target node: " << node_data->id << std::endl;
        unsigned int random_value = tarako::TarakoUtil::CreaterRandomInt(
            tarako::TarakoConst::RANDOM_VALUE_RANGE_BEGIN,
            tarako::TarakoConst::RANDOM_VALUE_RANGE_END
        );
        // std::cout << "[INFO] add random value to garbage box: " << random_value << std::endl;
        GarbageBoxStatus status =  tarako::TarakoUtil::GetGarbageBoxStatus(
            gs, 
            random_value,
            tarako::TarakoConst::GARBAGE_BOX_VOLUME
            );
        // std::cout << "[INFO] garbage box status: " << status << std::endl;
        LoRaWANPayload payload = LoRaWANPayload{status};
        Ptr<Packet> new_packet = Create<Packet>((uint8_t *)&payload, sizeof(payload));
        node_data->lora_net_device->Send(new_packet);
        node_data->sent_packets.push_back(new_packet);
        // std::cout << node_data->ToString() << std::endl;
        Simulator::Schedule(node_data->conn_interval, &OnActivateNodeForGroup, gs, node_data);
    }
}
}