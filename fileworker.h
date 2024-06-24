//
// Created by User on 19.06.2024.
//
#ifndef FILEWORKER_H
#define FILEWORKER_H

#include <fstream>
#include <string>
#include <cstdint>
#include <vector>
#include "passdata.h"
#include <iostream>
#include "xtea.h"

using namespace std;

void openFile(const vector<uint32_t> &key, vector<PassData> &passes) {

    // Открываем файл в бинарном режиме
    string filename = "notPasswords.bin";
    ifstream fin(filename, ios::binary);

    if (fin) {
        // Определяем размер файла
        fin.seekg(0, ios::end);
        std::streampos fileSize = fin.tellg();
        fin.seekg(0, ios::beg);

        // Проверяем, что размер файла кратен размеру uint32_t
        if (fileSize % sizeof(uint32_t) != 0) {
            cerr << "Размер файла не кратен" << endl;
            return;
        }

        // Создаем вектор нужного размера
        vector<uint32_t> buffer(fileSize / sizeof(uint32_t));

        // Читаем данные в вектор
        if (fin.read(reinterpret_cast<char *>(buffer.data()), fileSize)) {
            // Обрабатываем данные
            string str = vectorToString(buffer);

            string fileKey = "";
            uint32_t size = 0;

            string line;
            for (int i = 0, k = -2; str[i] != '\0'; i++)  {
                if(str[i] != '\n' && str[i] != ';') {
                    line += str[i];
                }
                else if (str[i] != ';'){
                    if(k == -2) {
                        fileKey = line;
                        k++;
                        line = "";
                        //проверка ключа
                    }
                    if(k == -1) {
                        passes.resize(stoi(line) + 1);
                        passes[1].pass = line;
                        line = "";
                        k++;
                    }
                    else {
                        if(k == 0) {
                            passes[2].login = line;
                            line = "";
                            k++;
                        }
                        else if(k == 1) {
                            passes[2].pass = line;
                            line = "";
                            k++;
                        }
                        else if(k == 2){
                            passes[2].service = line;
                            line = "";
                            k++;
                        }
                        else{
                            int toInt  = stoi(line);
                            passes[2].duration = static_cast<uint64_t>(toInt);
                            line = "";
                            k = 0;
                        }
                    }
                }
            }
        } else {
            cerr << "Ошибка при чтении файла" << endl;
        }
    } else {
        //создание файла
        ofstream fout;
        fout.open(filename);
        fout.close();
        openFile(key, passes);
    }
    fin.close();
}

// сохраняет файл
void saveFile(const vector<PassData> &passes, const vector<uint32_t> &key) {
    string keyStr = vectorToString(key);
    string dataStr = keyStr + ';' + to_string(passes.size()) + '\n';
    for (const PassData &pass: passes) {
        dataStr += pass.login + ';' + pass.pass + ';' + pass.service + ';'
                   + to_string(pass.timestamp) + to_string(pass.duration) + '\n';
    }
    vector<uint32_t> data;
    stringToVector(dataStr, data);
    xtea_encrypt(data, key);
    ofstream file("notPasswords.bin", ios::out);

    char *cPtr = reinterpret_cast<char *>(&*data.begin());
    file.write(cPtr, (uint32_t) data.size() * 4);

    file.close();
}

#endif //FILEWORKER_H
