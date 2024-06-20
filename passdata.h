//
// Created by mrquw on 20.06.2024.
//

#ifndef PPASS_PASSDATA_H
#define PPASS_PASSDATA_H

#include <string>

using namespace std;

string normalizeString(string p) {
    return p + string(64 - p.size(), '/');
}

vector<uint32_t> stringToInt(string &s) {
    vector<uint32_t> data (s.size() >> 2);
    uint32_t *nPtr = reinterpret_cast<uint32_t*>(&*s.begin());

    for (uint32_t &i: data) {
        i = *nPtr;
        nPtr++;
    }
    return data;
}

string intToString(const vector<uint32_t> &data) {
    string s;
    char* cPtr = (char*)(&*data.begin());

    for (int i = 0; i < data.size() << 2; ++i) {
        s += *cPtr;
        cPtr++;
    }

    return s;
}

#endif //PPASS_PASSDATA_H
