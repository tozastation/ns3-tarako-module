#ifndef INCLUDED_CSV_HPP
#define INCLUDED_CSV_HPP
#include <string>
#include <vector>

namespace ns3 {
namespace tarako {

class Csv
{
    std::string csv_file;

public:
    Csv(std::string);
    bool getCsv(std::vector<std::vector<std::string>>&, const char delim = ',');
};

}
}
#endif