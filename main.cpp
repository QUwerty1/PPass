#include <iostream>
#include <string>
using namespace std;

int main() {
    setlocale(LC_ALL, "Russian");
    string command;
    string guide = "������ ������ ����������� �������:\n\n"
    "guide       - �������� ������ ��������� ������\n\n"
    "create_pass - �������� ����� ������\n\n"
    "get_pass    - �������� ������:\n"
    "-s - �������� ������ �� �������\n"
    "-l - �������� ������ �� ������\n\n"
    //"\n"
    //"\n"
    ;
    while (command != "exit") {
        cout << "������� �������: ";
        cin  >> command;
        if (command == "guide")
            cout << guide;
        else if (command == "create_pass"){
            //������� ����������� ������ ������
        }
        else if (command == "get_pass") {
            cout << "������� ��������: ";
            string parameter;
            cin >> parameter;
            if (parameter == "-l") {
                //������� ��������� ������  �� ������
            }
            else if (parameter == "-s") {
                //������� ��������� ������ �� �������
            }
            else {
                cout << "�������� ����� �������";
            }
        }
        else
            cout << "����� ������� ���\n";
    }
    return 0;
}