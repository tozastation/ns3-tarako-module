#ifndef NODE_INFO_HPP
#define NODE_INFO_HPP

#include "ns3/packet.h"
#include "ns3/ptr.h"

namespace ns3 {
namespace tarako {

struct NodeInfo {
    std::string id;
    double energy_consumption;
    std::vector<Ptr<Packet>> sent_packets;
    std::vector<Ptr<Packet>> recieved_packets;
};

}
}
#endif