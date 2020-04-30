/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include <random>
#include <vector>
#include <string>
#include <tuple>

#include "csv.h"
#include "const.h"
#include "util.h"
#include <chrono>
#include <sstream>
#include <iomanip>
#include <iostream>

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
    if (value.empty())
    {
        return false;
    } 
    else return true;
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
        //std::cout << rec[3] << "," << rec[4] << "," << rec[5] << std::endl;
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

std::tuple<bool, int> TarakoUtil::IsMultipleNode(std::vector<std::vector<std::tuple<int, bool>>> g_boxes, int target_node_id)
{
    const int SINGLE = 1;
    bool is_multiple_node = false;
    int target_index = 0;
    for (auto& g_box: g_boxes)
    {
        for(auto& sensor: g_box)
        {
            int node_id = std::get<0>(sensor);
            if (node_id == target_node_id)
            {
                if (g_box.size() <= SINGLE)
                {
                    break;
                }
                else 
                {
                    is_multiple_node = true;
                    break;
                }
            }
        }
        if(is_multiple_node) break;
        target_index++;
    }
    return {is_multiple_node, target_index};
}

std::string TarakoUtil::GetCurrentTimeStamp()
{
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(localtime(&now_c), "%Y%m%d_%H%M%S");
    return ss.str();
}

}
}