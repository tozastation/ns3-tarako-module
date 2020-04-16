#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "ns3/csv.h"

using namespace std;

namespace ns3 {
namespace tarako {

Csv::Csv(string csv_file)
{
    this->csv_file = csv_file;
}

bool Csv::getCsv(vector<vector<string>>& data, const char delim)
{
    // ファイルOPEN
    ifstream ifs(csv_file);
    if (!ifs.is_open()) return false;  // 読み込み失敗

    // ファイルREAD
    string buf;                 // 1行分バッファ
    while (getline(ifs, buf)) {
        vector<string> rec;     // 1行分ベクタ
        istringstream iss(buf); // 文字列ストリーム
        string field;           // 1列分文字列

        // 1列分文字列を1行分ベクタに追加
        while (getline(iss, field, delim)) rec.push_back(field);

        // １行分ベクタを data ベクタに追加
        if (rec.size() != 0) data.push_back(rec);
    }

    return true;  // 読み込み成功
}

}
}