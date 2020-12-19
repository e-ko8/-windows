#include "iostream"
#include <windows.h>
#include <tchar.h>
#include <vector>
#include <random>
#include <boost/align.hpp>
#include <C:\Libaa\new\hayai-master\src\hayai.hpp>
#include "C:\Libaa\new\hayai-master\sample\delivery_man.hpp"


using namespace std;

class Tester {
public:
    int random(int min=0, int max=0) {
        mt19937 prng(random_device{}());
        return uniform_int_distribution<>(min, max)(prng);
    }
    explicit Tester(LPCSTR path)
    {
        vector<char,boost::alignment::aligned_allocator<char, 512>>vec(c_BufferSize);
        hout = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 0);
        GetFileSizeEx(hout, &li);
        s = random(0, li.QuadPart / c_BufferSize);
        ReadFile(hout, &vec[0], c_BufferSize, (LPDWORD)&num, &ovr);
        ovr.Internal = (li.QuadPart / s) * 256;
    }

    Tester(const Tester& other) = delete;
    Tester(Tester&& other) = delete;

    Tester& operator=(const Tester& other) = delete;
    Tester& operator=(Tester&& other) = delete;

    bool check_status()
    {
        return hout;
    }

    ~Tester()
    {
        CloseHandle(hout);
    }


private:
    HANDLE hout;
    CONST LONGLONG c_BufferSize = 8192;
    LONGLONG num = 0, s = 1;
    OVERLAPPED ovr = { 0 };
    LARGE_INTEGER li = { 0 };
};
BENCHMARK(Tester, Tester, 10, 10000)
{
Tester("C:/Project1/test.txt").random();
}
int main()
{
    Tester tester("C:/Project1/test.txt");
    hayai::ConsoleOutputter consoleOutputter;

    hayai::Benchmarker::AddOutputter(consoleOutputter);
    hayai::Benchmarker::RunAllTests();
    
    return 0;
}