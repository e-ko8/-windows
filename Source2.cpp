#include "windows.h"
#include "stdlib.h"
#include <windows.h>
#include <string>
#include <winbase.h>
#include <iostream>
#include <tchar.h>
#include <chrono>
using namespace std;
#include <psapi.h>
#pragma comment(lib,"psapi.lib")
#define BUF_SIZE 8192*100*100 // BuffSize for read file

int main()
{
    HANDLE hIFile = CreateFile(_T("H:/test.txt"), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED | FILE_FLAG_NO_BUFFERING, NULL); 

    if (INVALID_HANDLE_VALUE == hIFile)
    {
        return 1;
    }

    OVERLAPPED iAIO;
    ZeroMemory(&iAIO, sizeof(iAIO));

    DWORD dwBytesRead = 0;
    DWORD dwTotalBytesRead = 0;
    BOOL RF;
    DWORD rfs;
    LARGE_INTEGER* fsize;
    fsize = new LARGE_INTEGER;
    rfs = GetFileSizeEx(hIFile, fsize);
    LONGLONG BuffSize = 0;
    HANDLE hEvent;
    hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (hEvent)
    {
        iAIO.hEvent = hEvent;
    }
    else
    {
        cout << GetLastError() << endl;;
    }

    if (fsize->QuadPart == 0)
    {
        cout << GetLastError() << endl;;
        return 1;
    }
    else
    {
        if (fsize->QuadPart > BUF_SIZE)
        {
            BuffSize = BUF_SIZE;
        }
        else
        {
            BuffSize = fsize->QuadPart;
        }
    }
    wchar_t* IBuffer = (wchar_t*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, BuffSize);

    const auto start = chrono::high_resolution_clock::now(); 
   
    while (fsize->QuadPart > dwTotalBytesRead)
    {

        SetLastError(0);

        RF = ReadFile(hIFile, IBuffer, BuffSize, NULL, &iAIO); 

        if ((RF == 0) && GetLastError() == 997)     
        {
            

            while (!GetOverlappedResult(hIFile, &iAIO, &dwBytesRead, FALSE))
            {
                if (GetLastError() == 996)
                {
                    //working
                }
                else
                {
                    cout << GetLastError() << endl;
                    break;
                }

            }
        }
        else
        {
            cout << GetLastError() << endl;
            return 0;
        }



        dwTotalBytesRead = dwTotalBytesRead + iAIO.InternalHigh;
        iAIO.Offset = dwTotalBytesRead;
    }
    cout << dwTotalBytesRead << " bytes" << endl;
    const auto end = chrono::high_resolution_clock::now() - start; 

    cout << chrono::duration_cast<chrono::milliseconds>(end).count() << " milliseconds" << endl;

    ResetEvent(iAIO.hEvent);
    HeapFree(GetProcessHeap(), 0, IBuffer);
    CloseHandle(hEvent);

    CloseHandle(hIFile);
    return 0;
}
