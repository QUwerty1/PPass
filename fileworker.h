//
// Created by User on 19.06.2024.
//
#ifndef FILEWORKER_H
#define FILEWORKER_H

#include <fstream>
#include "passdata.h"
#include "xtea.h"

using namespace std;

int openFile(string &key, vector<PassData> &passes) {

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
            return -1;
        }

        // Создаем вектор нужного размера
        vector<uint32_t> buffer(fileSize / sizeof(uint32_t));

        // Читаем данные в вектор
        if (fin.read(reinterpret_cast<char *>(buffer.data()), fileSize)) {
            fin.close();
            bool isValidKey = false;

            // Обрабатываем данные
            vector<uint32_t> byteKey;
            stringToVector(key, byteKey);
            xtea_decrypt(buffer, byteKey);
            string str = vectorToString(buffer);

            //перевод строки в вектор структур
            string line;
            //прогоняем строчку
            for (int i = 0, k = -2, j = 0; str[i] != '\0'; i++) {
                if (str[i] != '\n' && str[i] != ';') {
                    line += str[i];
                }
                    //считывание слов
                else if (str[i] == ';' || str[i] == '\n') {
                    if (k == -2) {
                        //проверка ключа
                        if (key == line) {
                            isValidKey = true;
                        }
                        line = "";
                        k++;
                    } else if (k == -1) {
                        uint64_t size = stoull(line);
                        passes.reserve(size + 20);
                        passes.resize(size);
                        line = "";
                        k++;
                    }
                        //переход на новую подстроку
                    else {
                        if (k == 0) {
                            passes[j].login = line;
                            line = "";
                            //сдвиг счётчика слов в подстроке
                            k++;
                        } else if (k == 1) {
                            passes[j].pass = line;
                            line = "";
                            //сдвиг счётчика слов в подстроке
                            k++;
                        } else if (k == 2) {
                            passes[j].service = line;
                            line = "";
                            //сдвиг счётчика слов в подстроке
                            k++;
                        } else if (k == 3) {
                            passes[j].timestamp = stoull(line);
                            line = "";
                            //сдвиг счётчика слов в подстроке
                            k++;
                        } else {
                            passes[j].duration = stoull(line);
                            line = "";
                            //обнуление счётчика слов в подстроке
                            k = 0;
                            //сдвиг на следующий элемент вектора
                            j++;
                        }
                    }
                }
            }
            if (isValidKey)
                return 0;
            else
                return 2;
        } else {
            fin.close();
            cerr << "Ошибка при чтении файла" << endl;
            return -1;
        }
    } else {
        //возвращаем пустой массив
        cerr << "Ошибка при чтении файла" << endl;
        return -1;
    }
}

// сохраняет файл
void saveFile(const vector<PassData> &passes, const string &keyStr) {
    vector<uint32_t> key;
    stringToVector(keyStr, key);
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
