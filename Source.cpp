#include "iostream"
#include <windows.h>
#include <tchar.h>
#include <vector>
#include <ctime>
#include <malloc.h>
#include <random>

using namespace std;

int main()
{
    const unsigned long size = 8192;
    HANDLE hEvent;
    DWORD rfs;
    BOOL RF;
    DWORD dwTotalBytesRead = 0;
    HANDLE hout = CreateFile(_T("test.txt"), GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hout == INVALID_HANDLE_VALUE) {
        cout << "Error: " << GetLastError() << endl;
        exit(0);
    }
    vector<char> buff(size, (char)(LPBYTE)4096);
    buff.reserve(size);
    LARGE_INTEGER* fsize;
    fsize = new LARGE_INTEGER;
    rfs = GetFileSizeEx(hout, fsize);
    DWORD nRead;
    OVERLAPPED ovr = { 0 };
    LARGE_INTEGER li = { 0 };
    default_random_engine generator;
    uniform_int_distribution<LONGLONG> distribution(0, fsize->LowPart - 1);
    LONGLONG rndnum = distribution(generator);
    li.QuadPart = rndnum;
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
    for (int i = 0; i < size; i++)
    {
        cout << buff[i];
    }
    CloseHandle(hout);
    delete fsize;
    return 0;
}



/*
int main()
{
    const unsigned long size = 8193;
    void* rtdf = _aligned_malloc(size, 4096);
    HANDLE hEvent;
    DWORD rfs;
    BOOL RF;
    LONGLONG dwTotalBytesRead = 0;
    HANDLE hout = CreateFile(_T("test.txt"), GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED | FILE_FLAG_NO_BUFFERING, NULL);
    if (hout == INVALID_HANDLE_VALUE)
        cout << "Error: " << GetLastError() << endl;
    vector<char> buff(size, 0);
    buff.reserve((LONGLONG)rtdf);
    LARGE_INTEGER* fsize;
    fsize = new LARGE_INTEGER;
    rfs = GetFileSizeEx(hout, fsize);
    DWORD nRead = 0;
    OVERLAPPED ovr = { 0 };
    LARGE_INTEGER li = { 0 };
    ZeroMemory(&ovr, sizeof(ovr));
    default_random_engine generator;
    uniform_int_distribution<LONGLONG> distribution(0, fsize->LowPart - 1);
    LONGLONG rndnum = distribution(generator);
    void* rtd = _aligned_malloc(rndnum, 4096);
    li.QuadPart = (LONGLONG)rtd;
    ovr.Offset = li.LowPart;
    ovr.OffsetHigh = li.HighPart;
    hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (hEvent == NULL)
        cout << GetLastError() << " event error" << endl;
    while (fsize->QuadPart > dwTotalBytesRead)
    {
        RF = ReadFile(hout, &buff[0], (LONGLONG)rtdf, &nRead, &ovr);
        if ((RF == 0) && GetLastError() == 997)
        {
            while (!GetOverlappedResult(hout, &ovr, &nRead, FALSE))
            {
                if (GetLastError() == 996)
                {
                    /*cout << "Working..." << endl;*//*
                }
                else if (GetLastError() == 38)
                {
                    cout << GetLastError() << endl;
                    break;
                }
                else
                {
                    cout << GetLastError() << endl;
                    break;
                }

            }
        }
        else if ((RF == 0) && GetLastError() != 997)
        {
            cout << GetLastError() << endl;
            return 0;
        }
        dwTotalBytesRead = dwTotalBytesRead + ovr.InternalHigh;
        ovr.Offset = dwTotalBytesRead;
        cout << dwTotalBytesRead << " was reading!" << endl;
    }
    cout << dwTotalBytesRead << " was reading!" << endl;
    for (int i = 0; i < buff.size(); i++)
    {
        cout << buff[i];
    }
    delete fsize;
    return 0;
}*/