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
void openFile() {
    setlocale(LC_ALL, "Russian");

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

            // Перевод вектор структур
            vector<PassData> PassDataVec(1);
            string line;
            for (int i = 0, k = -2; str[i] != '\0'; i++)  {
                if(str[i] != '\n' || str[i] != ';') {
                    line += str[i];
                }
                else if (str[i] != ';'){
                    if(k == -2) {
                        PassDataVec[1].login = line;
                        k++;
                        line = "";
                        //проверка ключа
                    }
                    if(k == -1) {
                        PassDataVec.resize(stoi(line) + 1);
                        PassDataVec[1].pass = line;
                        line = "";
                        k++;
                    }
                    else {
                        if(k == 0) {
                            PassDataVec[2].login = line;
                            line = "";
                            k++;
                        }
                        else if(k == 1) {
                            PassDataVec[2].pass = line;
                            line = "";
                            k++;
                        }
                        else if(k == 2){
                            PassDataVec[2].service = line;
                            line = "";
                            k++;
                        }
                        else{
                            int toInt  = stoi(line);
                            PassDataVec[2].duration = static_cast<uint64_t>(toInt);
                            line = "";
                            k = 0;
                        }
                    }
                }
                else {
                    line = "";
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
        void input_file();
    }
    fin.close();
}

void saveFile(const vector<string> &passData, const vector<uint32_t> &key) {
    string keyStr = vectorToString(key);
    string dataStr = keyStr + '\n';
    for (const string &i: passData) {
        dataStr += i + '\n';
    }
    vector<uint32_t> data = stringToVector(dataStr);
    xtea_encrypt(data, key);
    ofstream file("notPasswords.bin", ios::out);

    char* cPtr = reinterpret_cast<char*>(&*data.begin());
    file.write(cPtr, (uint32_t)data.size() * 4);

    file.close();
}

#endif //FILEWORKER_H
