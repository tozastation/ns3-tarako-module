#ifndef NODE_PAYLOAD_HPP
#define NODE_PAYLOAD_HPP

#include <unordered_map> 

namespace ns3 {
namespace tarako {

struct GroupNodePayload {
    std::unordered_map<int, int> payloads;
};

}
}
#endif