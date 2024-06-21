//
// Created by User on 19.06.2024.
//

#ifndef FILEWORKER_H
#define FILEWORKER_H

#include <fstream>
#include <string>
#include <cstdint>
#include <vector>

using namespace std;

void input_file() {
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
            string str = vectotToString(buffer);

            // Перевод в строковый вектор
            vector<string> text(fileSize / sizeof(string));
            string line;
            for (int i = 0, j = 0, k = 0; str[i] != '\0'; i++, j++)  {
                if(str[i] != '\n') {
                    line[j] = str[i];
                }
                else {
                    j = 0;
                    text[k] = line;
                    line = "";
                    k++;
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

#endif //FILEWORKER_H
