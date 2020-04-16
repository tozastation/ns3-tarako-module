#ifndef GARBAGE_STATION_HPP
#define GARBAGE_STATION_HPP

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

}
}
#endif