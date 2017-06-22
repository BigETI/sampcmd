#include "resource.h"
#include <Windows.h>
#include <string>

using namespace std;

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int ret(0);
	HMODULE mh(GetModuleHandleA("kernel32.dll"));
	FARPROC pa;
	LPVOID ptr;
	PROCESS_INFORMATION process_info;
	STARTUPINFOA startup_info;
	HANDLE rt;
	string curr_dir, samp_dll, gta_sa_exe;
	memset(&process_info, 0, sizeof(PROCESS_INFORMATION));
	memset(&startup_info, 0, sizeof(STARTUPINFOA));
	char cd[MAX_PATH + 1];
	memset(cd, 0, (MAX_PATH + 1) * sizeof(char));
	GetCurrentDirectoryA(MAX_PATH, cd);
	curr_dir = cd;
	samp_dll = curr_dir + "\\samp.dll";
	gta_sa_exe = curr_dir + "\\gta_sa.exe";
	if (mh)
	{
		pa = GetProcAddress(mh, "LoadLibraryA");
		if (pa)
		{
			if (CreateProcessA(gta_sa_exe.c_str(), lpCmdLine, nullptr, nullptr, false, DETACHED_PROCESS | CREATE_SUSPENDED, nullptr, nullptr, &startup_info, &process_info))
			{
				ptr = VirtualAllocEx(process_info.hProcess, nullptr, (samp_dll.length() + 1) * sizeof(char), MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
				if (ptr)
				{
					if (WriteProcessMemory(process_info.hProcess, ptr, samp_dll.c_str(), (samp_dll.length() + 1) * sizeof(char), nullptr))
					{
						rt = CreateRemoteThread(process_info.hProcess, nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(pa), ptr, CREATE_SUSPENDED, nullptr);
						if (rt)
						{
							ResumeThread(rt);
							WaitForSingleObject(rt, INFINITE);
						}
						else
							ret = GetLastError();
					}
					else
						ret = GetLastError();
					VirtualFreeEx(process_info.hProcess, ptr, 0, MEM_RELEASE);
				}
				else
					ret = GetLastError();
				ResumeThread(process_info.hThread);
				CloseHandle(process_info.hProcess);
			}
			else
				ret = GetLastError();
		}
		else
			ret = GetLastError();
	}
	else
		ret = GetLastError();
	return ret;
}