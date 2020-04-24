#include "ns3/lora-frame-header.h"
#include "ns3/lorawan-mac-header.h"
#include "ns3/trace-helper.h"
#include "ns3/file-helper.h"
#include "ns3/packet.h"

#include "string.h"
#include "logger.h"
#include "random"

namespace ns3 {
namespace tarako {

void TarakoLogger::WriteLog(std::unordered_map<int, NodeInfo> node_map, std::string log_path)
{
    // --- Init Energy Consumption ---
    AsciiTraceHelper ascii;
    // Col(0): Node DeviceAddr, Col(1): EnergyConsumption(mA)
    std::stringstream energy_consumption_file;
    energy_consumption_file << log_path << "energy_consumption.csv";
    Ptr<OutputStreamWrapper> e_stream = ascii.CreateFileStream(energy_consumption_file.str()); 
    
    std::stringstream node_packet_info_file;
    node_packet_info_file << log_path << "node_packet_info.csv";
    Ptr<OutputStreamWrapper> n_stream = ascii.CreateFileStream(node_packet_info_file.str()); // "Col(0): Node DeviceAddr, Col(1): EnergyConsumption(mA)

    for (auto itr = node_map.begin(); itr != node_map.end(); ++itr)
    {
        int nwk_addr = itr->first;
        NodeInfo node_info = itr->second;
        *e_stream->GetStream () << nwk_addr << "," << node_info.energy_consumption << std::endl;
        
        for (auto& packet: node_info.recieved_packets)
        {
            lorawan::LorawanMacHeader mHdr;
            lorawan::LoraFrameHeader fHdr;
            Ptr<Packet> myPacket = packet->Copy ();
            myPacket->RemoveHeader (mHdr);
            myPacket->RemoveHeader (fHdr);
            *n_stream->GetStream() << nwk_addr << ",";
            *n_stream->GetStream() << packet->GetUid() << ",";
            *n_stream->GetStream() << fHdr.GetFCnt() << std::endl;
        }
        std::cout << "[info] done: write " << node_packet_info_file.str() << std::endl;
    }
}

}
}