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

// Делает число символов в строке кратным 64, добавляя /
string normalizeString(string &p) {
    return p + string(p.size() % 64, '/');
}

// преобразует строку в вектор беззнаковых 32-битных чисел
void stringToVector(string s, vector<uint32_t> &data) {
    s = normalizeString(s);
    data = vector<uint32_t>(s.size() >> 2);
    uint32_t *nPtr = reinterpret_cast<uint32_t *>(&*s.begin());

    for (uint32_t &i: data) {
        i = *nPtr;
        nPtr++;
    }
}

// преобразует вектор 32-битных беззнаковых чисел в строку
string vectorToString(const vector<uint32_t> &data) {
    string s;
    char *cPtr = (char *) (&*data.begin());

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
