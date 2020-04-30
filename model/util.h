#ifndef TARAKO_UTIL_HPP
#define TARAKO_UTIL_HPP

#include "garbage_station.h"
#include <vector>

namespace ns3 {
namespace tarako {

class TarakoUtil
{
    public:
    static GarbageBoxStatus GetGarbageBoxStatus(
        GarbageBoxSensor* gs, 
        unsigned int add, 
        unsigned int max_volume
        );
    
    static int CreateRandomInt(int begin, int end);

    static double ConvertStringToDouble(std::string value);

    static bool IsSupportedGarbageBoxType(std::string value);

    static std::vector<GarbageBox> GetGarbageBox(
        std::string csv_file
    );

    static std::string GetNextGroupLeader(std::vector<std::tuple<std::string, double>> nodes);

    static std::tuple<bool, int> IsMultipleNode(std::vector<std::vector<std::tuple<int, bool>>> g_boxes, int target_node_id);

    static std::string GetCurrentTimeStamp();
};

}
}
#endif