#include "pch.h"
#include <iostream>
#include <Windows.h>
#include <tchar.h>
#include <iostream>
#include <chrono>
#include <string>

using namespace std;

int main()
{
    setlocale(LC_ALL, "Russian");
    int size = 262144000; // выделеный буфер, зависит от кол-во ОЗУ на ПК, а так же от кол-во свободной памяти, в данном случае мой максимум, далее переполнение кучи и естественно ошибка
    int *buff = new int[size]; // буффер динамической памяти для хранения читаемых данных
    DWORD dwCounter; // начальная точка чтения
    HANDLE hndFileOut = CreateFile(L"test.mp4", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING, 0); // установить вместо FILE_FLAG_NO_BUFFERING для нормального чтения файла FILE_ATTRIBUTE_NORMAL
    if (hndFileOut == INVALID_HANDLE_VALUE) { // проверка правильности файла
        cout << "Ошибка открытия файла\n";
        system("pause");
        exit(1);
    }
    const auto start = chrono::high_resolution_clock::now(); // старт таймера
    ReadFile(hndFileOut, buff, size, &dwCounter, NULL); // чтение файла
    const auto end = chrono::high_resolution_clock::now() - start; // конец таймера
    std::cout << chrono::duration_cast<chrono::nanoseconds>(end).count() << " ns" << std::endl; // вывод кол-во времени потраченного на чтение, в данном случае в НаноСекундах
    CloseHandle(hndFileOut); // закрываем файл
    delete [] buff; // чистим память
    return 0;
}

///////////////////////ТЕСТЫ ФАЙЛА НА 17ГБ//////////////////////////
///////////////////////БЕЗ БУФФЕРИЗАЦИИ/////////////////////////////
//SSD//147300//
//HDD//184100//
//USB Flash//502900//
///////////////////////С БУФФЕРИЗАЦИЕЙ//////////////////////////////
//SSD//295500//
//HDD//253100//
//USB Flash//783800//
//////ДАННЫЕ НА ДРУГИХ ПК ПРИ ДРУГИХ УСЛОВИЯХ БУДУТ ОТЛИЧАТЬСЯ//////
