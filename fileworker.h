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

void openFile(string &key, vector<PassData> &passes) {

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

            //перевод строки в вектор структур
            string line;
            //прогоняем строчку
            for (int i = 0, k = -2, j = 0; str[i] != '\0'; i++)  {
                if(str[i] != '\n' && str[i] != ';') {
                    line += str[i];
                }
                //считывание слов
                else if (str[i] != ';'){
                    if(k == -2) {
                        //проверка ключа
                        if (key != line) {
                            cout << "Неверный ключ\n";
                            return;
                        }
                        line = "";
                        k++;
                    }
                    if(k == -1) {
                        passes.resize(stoi(line));
                        line = "";
                        k++;
                    }
                    //переход на новую подстроку
                    else {
                        if(k == 0) {
                            passes[j].login = line;
                            line = "";
                            //сдвиг счётчика слов в подстроке
                            k++;
                        }
                        else if(k == 1) {
                            passes[j].pass = line;
                            line = "";
                            //сдвиг счётчика слов в подстроке
                            k++;
                        }
                        else if(k == 2){
                            passes[j].service = line;
                            line = "";
                            //сдвиг счётчика слов в подстроке
                            k++;
                        }
                        else{
                            int toInt  = stoi(line);
                            passes[j].duration = static_cast<uint64_t>(toInt);
                            line = "";
                            //обнуление счётчика слов в подстроке
                            k = 0;
                            //сдвиг на следующий элемент вектора
                            j++;
                        }
                    }
                }
            }
        } else {
            cerr << "Ошибка при чтении файла" << endl;
        }
    } else {
        //возвращаем пустой массив
        cerr << "Ошибка при чтении файла" << endl;
        return;
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
