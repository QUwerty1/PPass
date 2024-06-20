//
// Created by User on 19.06.2024.
//
#include <fstream>
#include <string>
#include <cstdint>
#include <vector>

using namespace std;

#ifndef FILEWORKER_H
#define FILEWORKER_H

void input_file (string pass) {
    //если файла нет, то создать его
    if(ifstream fin(ppas.txt, app)) {
        fin >> pass;
    }
    else {
        ofstream fout;
        fout.open(ppas.txt);
        fout.close(ppas.txt);
        void input_file (pass);
    }

}
void output_file () {
    ifstream file("testo.txt", ios::in);
    string s = string(48, '\0');
    getline(file, s);
    file.close();
    cout << s << '\n';

    vector<uint32_t> text(s.size() >> 2, 0);
    char *num = &*s.begin();

    for (int i = 0; i < s.size() >> 2; ++i) {
        text[i] = *num;
        for (int j = 0; j < 3; ++j) {
            num++;
            text[i] = (text[i] << 1) + *num;
        }
        num++;
    }

    xtea_decrypt(text, vector<uint32_t >{1, 2, 3, 4});

    for (uint32_t el: text) {
        cout << el << ' ';
    }
    cout << '\n' << text.size() << '\n' << s.size();
}

#endif //FILEWORKER_H
