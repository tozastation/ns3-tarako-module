/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include <random>
#include <vector>
#include <string>
#include <tuple>

#include "csv.h"
#include "const.h"
#include "util.h"

namespace ns3 {
namespace tarako {


GarbageBoxStatus TarakoUtil::GetGarbageBoxStatus(GarbageBoxSensor* gs, unsigned int add, unsigned int max_volume)
{
    if (gs->current_volume == 0) 
    {
        gs->current_volume = gs->current_volume + add;
        return GarbageBoxStatus::EMPTY;
    }
    else if (0 < gs->current_volume && gs->current_volume < max_volume) 
    {
        gs->current_volume = gs->current_volume + add;
        return GarbageBoxStatus::FILLED;
    } 
    else
    {
        gs->current_volume = 0;
        return GarbageBoxStatus::FULL;
    } 
}

int TarakoUtil::CreateRandomInt(int begin, int end)
{
    std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> dice(begin,end);
    return dice(mt);
}

double TarakoUtil::ConvertStringToDouble(std::string value)
{
    return stod(value);
}

bool TarakoUtil::IsSupportedGarbageBoxType(std::string value)
{
    if (value == "○") return true;
    else return false;
}

std::vector<GarbageBox> TarakoUtil::GetGarbageBox(std::string csv_file)
{
    std::vector<std::vector<std::string>> data;
    std::vector<GarbageBox> g_boxes;
    Csv objCsv(csv_file);
    if (!objCsv.getCsv(data)) {
        throw 1;
    }
    for (unsigned int row = 1; row < data.size(); row++) {
        std::vector<std::string> rec = data[row];
        GarbageBox g_box;

        g_box.id = rec[TarakoConst::ID];
        g_box.latitude = stod(rec[TarakoConst::LATITUDE]);
        g_box.longitude = stod(rec[TarakoConst::LONGITUDE]);
        g_box.burnable = TarakoUtil::IsSupportedGarbageBoxType(rec[TarakoConst::BURNABLE]);
        g_box.incombustible = TarakoUtil::IsSupportedGarbageBoxType(rec[TarakoConst::INCOMBUSTIBLE]);
        g_box.resource = TarakoUtil::IsSupportedGarbageBoxType(rec[TarakoConst::RESOURCE]);
 
        g_boxes.push_back(g_box);
    }
    return g_boxes;
}

std::string TarakoUtil::GetNextGroupLeader(std::vector<std::tuple<std::string, double>> nodes)
{
    std::string next_group_leader = std::get<0>(nodes.at(0));
    double lowest_energy_consumption = std::get<1>(nodes.at(0));
    for (auto& node: nodes)
    {
        double e_v = std::get<1>(node);
        if (lowest_energy_consumption > e_v)
        {
            lowest_energy_consumption = e_v;
            next_group_leader = std::get<0>(node);
        }
    }
    return next_group_leader;
}
}
}