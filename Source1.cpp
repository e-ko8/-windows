#include "iostream"
#include <windows.h>
#include <tchar.h>
#include <chrono>
#include <vector>

using namespace std;

int main() 
{
    HANDLE hout = CreateFile(_T("H:/test.txt"), GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); // FILE_FLAG_OVERLAPPED | FILE_FLAG_NO_BUFFERING FILE_ATTRIBUTE_NORMAL

    const unsigned long size = 14760846895; // 17820    1291264000    14760846895   1875945007   1291264000
    vector<char> buff(size, 0);                                                     
    buff.reserve(size);
    DWORD nRead;
    OVERLAPPED ovr = { 0 };
    LARGE_INTEGER li = { 0 };
    ovr.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (!ovr.hEvent)
    {
        _tprintf_s(TEXT("Error creating I/O event for reading second line\n"));
        return 1;
    }
    li.QuadPart = 0;
    ovr.Offset = li.LowPart;
    ovr.OffsetHigh = li.HighPart;
    
    if (hout == INVALID_HANDLE_VALUE)
    {
        cout << "error";
    }
    const auto start = chrono::high_resolution_clock::now(); // старт таймера
    if (!ReadFile(hout, &buff[0], size, &nRead, &ovr))
        return GetLastError();// чтение файла
    if (ovr.Internal == -1 && GetLastError())
        return 2;
    const auto end = chrono::high_resolution_clock::now() - start; // конец таймера

    CloseHandle(hout);
    cout << chrono::duration_cast<chrono::milliseconds>(end).count() << " milliseconds" << std::endl;
    cout << nRead << endl;
    /*for (int i = 0; i < size; i++) {
        cout << buff[i];
    }*/
    cin.get();
	return 0;
}