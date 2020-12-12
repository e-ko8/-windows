#include "iostream"
#include <windows.h>
#include <tchar.h>
#include <vector>
#include <ctime>
#include <malloc.h>
#include <random>

const LONGLONG c_BufferSize = 8192*8192;

using namespace std;

int main()
{
    HANDLE hout = CreateFile(L"test1.mp4", GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING, 0);
    char32_t* buf = new char32_t[c_BufferSize];
    int num;
    LONGLONG bytesRead = 0;
    LARGE_INTEGER* fsize;
    fsize = new LARGE_INTEGER;
    DWORD rfs = GetFileSizeEx(hout, fsize);
    OVERLAPPED ovr = { 0 };
    LARGE_INTEGER li = { 0 };
    ovr.Offset = li.LowPart;
    ovr.OffsetHigh = li.HighPart;
    while (fsize->QuadPart > bytesRead) {
        BOOL hRead = ReadFile(hout, buf, c_BufferSize, (LPDWORD)&num, &ovr);
        if (hRead == 0)
        {
            int errCode = GetLastError();
            cout << errCode << endl;
        }
        else
        {
            bytesRead += num;
            ovr.Offset = bytesRead;
            cout << bytesRead << " bytes read" << endl;
        }
    }
    delete[] buf;
    CloseHandle(hout);
    return 0;
}