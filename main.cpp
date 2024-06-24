#include <iostream>
#include <string>
#include "fileworker.h"

using namespace std;

//int main() {
//    setlocale(LC_ALL, "Russian");
//    string command;
//    string guide = "Список команд органайзера паролей:\n\n"
//    "guide       - показать список возможных команд\n\n"
//    "create_pass - записать новый пароль\n\n"
//    "get_pass    - получить пароль:\n"
//    "-s - получить пароль по сервису\n"
//    "-l - получить пароль по логину\n\n"
//    //"\n"
//    //"\n"
//    ;
//    while (command != "exit") {
//        cout << "Введите команду: ";
//        cin  >> command;
//        if (command == "guide")
//            cout << guide;
//        else if (command == "create_pass"){
//            //функция сохраниения нового пароля
//        }
//        else if (command == "get_pass") {
//            cout << "Введите параметр: ";
//            string parameter;
//            cin >> parameter;
//            if (parameter == "-l") {
//                //функция получения пароля по логину
//            }
//            else if (parameter == "-s") {
//                //функция получения пароля по сервису
//            }
//            else {
//                cout << "Параметр введён неверно";
//            }
//        }
//        else
//            cout << "Такой команды нет\n";
//    }
//    return 0;
//}

int main() {
    string keyStr = "qwerty12345";
    vector<PassData> passes {
        PassData("user1", "pass1", "service1", time(nullptr), 0),
        PassData("user2", "pass2", "service2", time(nullptr), 2254),
        PassData("user3", "pass3", "service3", time(nullptr), 23452),
        PassData("user4", "pass4", "service4", time(nullptr), 234523),
    };
    vector<uint32_t> key;
    stringToVector(keyStr, key);
    save_file(passes, key);

    return 0;
}