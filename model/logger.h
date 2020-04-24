#ifndef TARAKO_LOGGER_HPP
#define TARAKO_LOGGER_HPP

#include "node_info.h"
#include <unordered_map> 

namespace ns3 {
namespace tarako {

class TarakoLogger
{
    public:
    void WriteLog(
        std::unordered_map<int, NodeInfo> node_map,
        std::string log_path
    );
};

}
}
#endif