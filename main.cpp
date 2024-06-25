#include <iostream>
#include <random>
#include "fileworker.h"

#define pathToPass "notPasswords.bin"

using namespace std;

int main() {
    setlocale(LC_ALL, "Russian");
    string command;
    string help = "Список команд органайзера паролей:\n\n"
                  "help          - показать список возможных команд\n\n"
                  "create_pass   - записать новый пароль\n\n"
                  "get_pass      - получить пароль:\n"
                  "-s - получить пароль по сервису\n"
                  "-l - получить пароль по логину\n\n"
                  "update_pass   - обновить пароль\n\n"
                  "list_pass     - вывести список всех паролей\n\n"
                  "list_outdated - показать список паролей с истекшем сроком действия\n\n";
    vector<PassData> passes;
    string keyStr;
    bool fileExists;

    ifstream file(pathToPass);
    fileExists = !!file;
    file.close();

    if (fileExists) {
        int status;
        do {
            cout << "Введите мастер-ключ: ";
            cin >> keyStr;
            status = openFile(keyStr, passes);
            if (status == 2)
                cout << "Введен неверный ключ\n";
        } while (status != 0);
    } else {
        string tmpKey;
        do {

            cout << "Введите мастер-ключ программы: ";
            cin >> keyStr;
            cout << "Подтвердите мастер ключ: ";
            cin >> tmpKey;
        } while (tmpKey != keyStr || keyStr.empty());
    }

    while (command != "exit") {
        cout << "Введите команду: ";
        cin >> command;
        if (command == "help")
            cout << help;

        else if (command == "create_pass") {
            string line;
            PassData newPass = PassData();

            newPass.timestamp = time(nullptr);

            cout << "Введите логин: ";
            cin >> newPass.login;

            char ans;
            cout << "Вы хотите, чтобы пароль был сгенерирован автоматически? (y/N): ";
            cin >> ans;
            if (ans != 'y' && ans != 'Y') {
                cout << "Введите пароль: ";
                cin >> newPass.pass;

            } else {
                uint32_t num;

                random_device randomDevice;
                default_random_engine randomEngine(randomDevice());
                uniform_int_distribution<char> uniformDist(33, 127);

                cout << "Введите длину ключа (16): ";
                cin.ignore();
                getline(cin, line);

                num = line.empty() ? 16 : stoul(line);
                newPass.pass = string(num, 0);
                for (char &i: newPass.pass) {
                    auto a = uniformDist(randomEngine);
                    i = a;
                }
            }

            cout << "Введите название сервиса: ";
            cin >> newPass.service;

            cout << "Введите срок действия пароля в днях (1 год): ";
            cin.ignore();
            getline(cin, line);
            newPass.duration = line.empty() ? 31536000 : stoll(line) * 60 * 60 * 24;

            bool isExists = false;
            for (const PassData &pass: passes) {
                if (pass.login == newPass.login && pass.service == newPass.service) {
                    isExists = true;
                    break;
                }
            }
            if (isExists)
                cout << "Учетная запись с таким логином и названием сервиса уже существует\n";
            else
                passes.push_back(newPass);

        } else if (command == "get_pass") {
            cout << "Введите параметр: ";
            string parameter;
            cin >> parameter;
            if (parameter == "-l") {
                //функция получения пароля по логину
                string login;
                vector<PassData> findings{};
                findings.reserve(10);

                cout << "Введите логин: ";
                cin >> login;

                uint32_t cnt = 0;
                for (auto & pass : passes) {
                    if (pass.login.contains(login)) {
                        cnt++;
                        findings.push_back(pass);
                        cout << "\t" << cnt << ". Логин: " << pass.login
                             << "\tСервис: " << pass.service << '\n';
                    }
                }

                uint32_t num;
                cout << "Введите номер учетной записи пароль от которой вам нужен: ";
                cin >> num;
                num--;
                if (num >= findings.size())
                    cout << "Введен неверный номер";
                else
                    cout << "\tЛогин: " << findings[num].login
                         << "\n\tПароль: " << findings[num].pass
                         << "\n\tСервис: " << findings[num].service << '\n';

            } else if (parameter == "-s") {
                //функция получения пароля по сервису
                string service;
                vector<PassData> findings{};
                findings.reserve(10);

                cout << "Введите название сервиса: ";
                cin >> service;

                uint32_t cnt = 0;
                for (const PassData &pass: passes) {
                    if (pass.service.contains(service)) {
                        cnt++;
                        findings.push_back(pass);
                        cout << "\t" << cnt << ". Логин: " << pass.login
                             << "\tСервис: " << pass.service << '\n';
                    }
                }

                uint32_t num;
                cout << "Введите номер учетной записи пароль от которой вам нужен: ";
                cin >> num;
                num--;
                if (num >= findings.size())
                    cout << "Введен неверный номер";
                else
                    cout << "\tЛогин: " << findings[num].login
                         << "\n\tПароль: " << findings[num].pass
                         << "\n\tСервис: " << findings[num].service << '\n';

            } else {
                cout << "Параметр введён неверно\n";
            }
        } else if (command == "update_pass") {
            string login;
            string service;

            cout << "Введите логин: ";
            cin >> login;

            cout << "Введите название сервиса: ";
            cin >> service;

            bool wasFound = false;
            for (PassData &pass: passes) {
                if (pass.login == login && pass.service == service) {
                    pass.timestamp = time(nullptr);
                    wasFound = true;
                    string line;

                    cout << "Введите новый логин (" << pass.login << "): ";
                    cin.ignore();
                    getline(cin, line);
                    if (!line.empty())
                        pass.login = line;

                    cout << "Введите новый пароль (" << pass.pass << "): ";
                    getline(cin, line);
                    if (!line.empty())
                        pass.pass = line;

                    cout << "Введите новое название сервиса (" << pass.service << "): ";
                    getline(cin, line);
                    if (!line.empty())
                        pass.service = line;

                    cout << "Введите новый срок действия пароля в днях ("
                         << pass.duration / 60 / 60 / 24 << "): ";
                    getline(cin, line);
                    if (!line.empty())
                        pass.duration = stoull(line) * 60 * 60 * 24;

                    break;
                }
            }

            if (!wasFound)
                cout << "Учетной записи с таким логином и паролем не обнаружено\n";

        } else if (command == "list_pass") {
            for (const PassData &pass: passes) {
                cout << "\tЛогин: " << pass.login << "\tСервис: " << pass.service << '\n';
            }
        } else if (command == "list_outdated") {
            uint64_t timeNow = time(nullptr);
            bool wasFound = false;

            for (const PassData &pass: passes) {
                if (pass.timestamp + pass.duration <= timeNow) {
                    wasFound = true;
                    cout << "\tЛогин: " << pass.login << "\tСервис: " << pass.service
                         << "\n\t\tБыл просрочен "
                         << (timeNow - (pass.timestamp + pass.duration)) / 60 / 60 / 24
                         << " дней(день) назад\n";
                }
            }
            if (!wasFound)
                cout << "У вас не просроченных паролей\n";

        } else if (command == "exit") {
            saveFile(passes, keyStr);
        } else
            cout << "Такой команды нет\n";
    }
    return 0;
}