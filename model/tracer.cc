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
        unsigned int random_value = tarako::TarakoUtil::CreaterRandomUnsignedInt(
            tarako::TarakoConst::RANDOM_VALUE_RANGE_BEGIN,
            tarako::TarakoConst::RANDOM_VALUE_RANGE_END
        );
        GarbageBoxStatus status =  tarako::TarakoUtil::GetGarbageBoxStatus(
            gs, 
            random_value,
            tarako::TarakoConst::GARBAGE_BOX_VOLUME
            );
        LoRaWANPayload payload = LoRaWANPayload{status};
        Ptr<Packet> new_packet = Create<Packet>((uint8_t *)&payload, sizeof(payload));
        device->Send(new_packet);
        node->sent_packets.push_back(new_packet);
        Simulator::Schedule(Seconds(10), &OnActivateNode, device, gs, node, Seconds(10));
    }
}
}