#include "resource.h"
#include <Windows.h>
#include <locale>
#include <codecvt>
#include <string>

using namespace std;

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int ret(0);
	HMODULE mh(GetModuleHandleW(L"kernel32.dll"));
	FARPROC pa;
	LPVOID ptr;
	PROCESS_INFORMATION process_info;
	STARTUPINFOW startup_info;
	HANDLE rt;
	wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
	wstring curr_dir, samp_dll, gta_sa_exe, params(converter.from_bytes(lpCmdLine));
	memset(&process_info, 0, sizeof(PROCESS_INFORMATION));
	memset(&startup_info, 0, sizeof(STARTUPINFOW));
	wchar_t cd[MAX_PATH + 1];
	memset(cd, 0, (MAX_PATH + 1) * sizeof(wchar_t));
	GetCurrentDirectoryW(MAX_PATH, cd);
	curr_dir = cd;
	samp_dll = curr_dir + L"\\samp.dll";
	gta_sa_exe = curr_dir + L"\\gta_sa.exe";
	if (mh)
	{
		pa = GetProcAddress(mh, "LoadLibraryW");
		if (pa)
		{
			if (CreateProcessW(gta_sa_exe.c_str(), const_cast<LPWSTR>(params.c_str()), nullptr, nullptr, false, DETACHED_PROCESS | CREATE_SUSPENDED, nullptr, nullptr, &startup_info, &process_info))
			{
				ptr = VirtualAllocEx(process_info.hProcess, nullptr, (samp_dll.length() + 1) * sizeof(wchar_t), MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
				if (ptr)
				{
					if (WriteProcessMemory(process_info.hProcess, ptr, samp_dll.c_str(), (samp_dll.length() + 1) * sizeof(wchar_t), nullptr))
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