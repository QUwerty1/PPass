//
// Created by mrquw on 19.06.2024.
//

#ifndef PPASS_XTEA_H
#define PPASS_XTEA_H

#include <cstdint>
#include <vector>

using namespace std;

// Шифрует вектор беззнаковых 32-битных чисел text при помощи ключа key
// key должен состоять из 4-х элементов
// text должен состоять из кол-ва элементов кратного 2-ум
void xtea_encrypt(vector<uint32_t> &text, const vector<uint32_t> &key) {
    uint32_t delta = 0x9E3779B9;
    for (int i = 0; i < text.size(); i += 2) {
        uint32_t sum = 0;
        uint32_t v0 = text[i], v1 = text[i + 1];

        for (uint32_t j = 0; j < 32; ++j) {
            v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + key[sum & 3]);
            sum += delta;
            v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + key[(sum >> 11) & 3]);
        }
        text[i] = v0;
        text[i + 1] = v1;
    }
}

// Дешифрует вектор беззнаковых 32-битных чисел text при помощи ключа key
// key должен состоять из 4-х элементов
// text должен состоять из кол-ва элементов кратного 2-ум
void xtea_decrypt(vector<uint32_t> &text, const vector<uint32_t> &key) {
    uint32_t delta = 0x9E3779B9;
    for (int i = 0; i < text.size(); i += 2) {
        uint32_t sum = delta * 32;
        uint32_t v0 = text[i];
        uint32_t v1 = text[i + 1];

        for (uint32_t j = 0; j < 32; ++j) {
            v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + key[(sum >> 11) & 3]);
            sum -= delta;
            v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + key[sum & 3]);
        }
        text[i] = v0;
        text[i + 1] = v1;
    }
}

#endif //PPASS_XTEA_H
