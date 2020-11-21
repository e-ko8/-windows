#include "iostream"
#include <windows.h>
#include <tchar.h>
#include <vector>
#include <ctime>
#include <malloc.h>

using namespace std;

int main()
{
    srand(time(NULL));
    const unsigned long size = 8192;
    HANDLE hEvent;
    DWORD rfs;
    BOOL RF;
    DWORD dwTotalBytesRead = 0;
    HANDLE hout = CreateFile(_T("test.txt"), GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hout == INVALID_HANDLE_VALUE)
        cout << "Error: " << GetLastError() << endl;
    vector<char> buff(size, (char)(LPBYTE)4096);
    buff.reserve(size);
    LARGE_INTEGER* fsize;
    fsize = new LARGE_INTEGER;
    rfs = GetFileSizeEx(hout, fsize);
    DWORD nRead;
    OVERLAPPED ovr = { 0 };
    LARGE_INTEGER li = { 0 };
    li.QuadPart = rand() % fsize->LowPart - 1;
    ovr.Offset = li.LowPart;
    ovr.OffsetHigh = li.HighPart;
    hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (hEvent == NULL)
        cout << GetLastError() << " event error" << endl;
    if (!ReadFile(hout, &buff[0], size, &nRead, &ovr))
        return GetLastError();// чтение файла
    if (ovr.Internal == -1 && GetLastError())
        return 2;
    cout << nRead << endl;
    delete fsize;
    return 0;
}