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
    void* rtdf = _aligned_malloc(size, 4096);
    HANDLE hEvent;
    DWORD rfs;
    BOOL RF;
    DWORD dwTotalBytesRead = 0;
    HANDLE hout = CreateFile(_T("test.txt"), GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED | FILE_FLAG_NO_BUFFERING, NULL);
    if (hout == INVALID_HANDLE_VALUE)
        cout << "Error: " << GetLastError() << endl;
    vector<char> buff(size, 0);
    buff.reserve((LONGLONG)rtdf);
    LARGE_INTEGER* fsize;
    fsize = new LARGE_INTEGER;
    rfs = GetFileSizeEx(hout, fsize);
    DWORD nRead;
    OVERLAPPED ovr = { 0 };
    LARGE_INTEGER li = { 0 };
    void *rtd = _aligned_malloc(rand() % fsize->LowPart - 1, 4096);
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
                    cout << "Working..." << endl;
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
    }
    cout << dwTotalBytesRead << " was reading!" << endl;
    delete fsize;
    return 0;
}