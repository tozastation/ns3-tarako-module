#ifndef GARBAGE_STATION_HPP
#define GARBAGE_STATION_HPP

#include <string>

namespace ns3 {
namespace tarako {

struct GarbageStation {
    std::string id;
    double longitude;
    double latitude;
    // Garbage Category
    bool burnable;
    bool incombustible;
    bool resource;
};

enum GarbageBoxStatus: unsigned int
{
    EMPTY = 0, 
    FILLED = 1, 
    FULL = 2
};

struct GarbageBox {
    std::string id;
    double longitude;
    double latitude;
    // Garbage Category
    bool burnable;
    bool incombustible;
    bool resource;
};

struct GarbageBoxSensor
{
    unsigned int current_volume;
};

}
}
#endif