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

    static std::vector<std::string> GetPairGarbageBox(
        std::string csv_file,
        std::string belong_to
    );

    static std::string GetFirstLeader(
        std::vector<std::tuple<int, std::string>> group_addrs,
        int my_lora_addr, 
        std::string my_ble_addr
    );

    static std::string GetNextGroupLeader(std::vector<std::tuple<std::string, double>> nodes);

    static bool IsMultipleNode(tarako::GarbageBox g_box);

    static std::string GetCurrentTimeStamp();
};

}
}
#endif