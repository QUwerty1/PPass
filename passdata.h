//
// Created by mrquw on 20.06.2024.
//

#ifndef PPASS_PASSDATA_H
#define PPASS_PASSDATA_H

#include <string>

using namespace std;

struct PassData {
    string login;
    string pass;
    string service;
    uint64_t timestamp;
    uint64_t duration;
};

void PassesToString(const vector<PassData> &passes, string &text) {
    text = "";
    for (const PassData &pass: passes) {
        text += pass.login
    }
}

// Делает число символов в строке кратным 64
string normalizeString(string p) {
    return p + string(64 - p.size(), '/');
}

// преобразует строку в вектор беззнаковых 32-битных чисел
vector<uint32_t> stringToVector(string s) {
    s = normalizeString(s);
    vector<uint32_t> data (s.size() >> 2);
    uint32_t *nPtr = reinterpret_cast<uint32_t*>(&*s.begin());

    for (uint32_t &i: data) {
        i = *nPtr;
        nPtr++;
    }
    return data;
}

// преобразует вектор 32-битных беззнаковых чисел в строку
string vectorToString(const vector<uint32_t> &data) {
    string s;
    char* cPtr = (char*)(&*data.begin());

    for (int i = 0; i < data.size() << 2; ++i) {
        s += *cPtr;
        cPtr++;
    }
    cPtr--;
    int cnt = 0;

    while (*cPtr == '/') {
        cPtr--;
        cnt++;
    }
    s = s.substr(0, s.size() - cnt);

    return s;
}

#endif //PPASS_PASSDATA_H
