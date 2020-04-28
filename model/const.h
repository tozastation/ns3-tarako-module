#ifndef TARAKO_CONST_HPP
#define TARAKO_CONST_HPP

#include "ns3/timer.h"

namespace ns3 {
namespace tarako {

class TarakoConst
{
    public:
    // Garbage Box Option
    static const unsigned int INTERVAL = 10;
    static const unsigned int GARBAGE_BOX_VOLUME = 70; // UNIT: L
    static const int RANDOM_VALUE_RANGE_BEGIN = 1;
    static const int RANDOM_VALUE_RANGE_END = 6;
    // CSV Option
    static const int ID = 0;
    static const int LONGITUDE = 1;
    static const int LATITUDE = 2;
    static const int BURNABLE = 3;
    static const int INCOMBUSTIBLE = 4;
    static const int RESOURCE = 5;
    // Group Option
    static const bool EnableGrouping = false;
    static const bool EnableEqualization = true;
    static const int SIMURATION_TIME = 24; // Hours
    static const int ACTIVATION_INTERVAL = 10; // Seconds
};

}
}
#endif