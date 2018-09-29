#include "resource.h"
#include <Windows.h>
#include <string>

using namespace std;

/// <summary>
/// Get last error message
/// </summary>
/// <returns>Error message</returns>
static wstring GetErrorMessage(DWORD error)
{
	wstring ret;
	if (error != NO_ERROR)
	{
		LPWSTR buffer = nullptr;
		size_t size = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPWSTR>(&buffer), 0, nullptr);
		ret = wstring(buffer, size);
		LocalFree(buffer);
	}
	return ret;
}

/// <summary>
/// Main entry point
/// </summary>
/// <param name="hInstance">Instance handle</param>
/// <param name="hPrevInstance">Previous instance handle</param>
/// <param name="lpCmdLine">Command line</param>
/// <param name="nCmdShow">Command show</param>
/// <returns>Exit code</returns>
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int ret(0);
	HMODULE mh(GetModuleHandleW(L"kernel32.dll"));
	FARPROC pa;
	LPVOID ptr;
	PROCESS_INFORMATION process_info = { 0 };
	STARTUPINFOW startup_info = { 0 };
	HANDLE rt;
	wstring curr_dir, samp_dll, gta_sa_exe;
	curr_dir.resize(FILENAME_MAX);
	DWORD sz = GetCurrentDirectoryW(FILENAME_MAX, &(curr_dir[0]));
	curr_dir.resize(sz);
	samp_dll = curr_dir + L"\\samp.dll";
	gta_sa_exe = curr_dir + L"\\gta_sa.exe";
	if (mh)
	{
		pa = GetProcAddress(mh, "LoadLibraryW");
		if (pa)
		{
			if (CreateProcessW(gta_sa_exe.c_str(), GetCommandLineW(), nullptr, nullptr, false, DETACHED_PROCESS | CREATE_SUSPENDED, nullptr, nullptr, &startup_info, &process_info))
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
						{
							ret = GetLastError();
						}
					}
					else
					{
						ret = GetLastError();
					}
					VirtualFreeEx(process_info.hProcess, ptr, 0, MEM_RELEASE);
				}
				else
				{
					ret = GetLastError();
				}
				ResumeThread(process_info.hThread);
				CloseHandle(process_info.hProcess);
			}
			else
			{
				ret = GetLastError();
			}
		}
		else
		{
			ret = GetLastError();
		}
	}
	else
	{
		ret = GetLastError();
	}
	if (ret != NO_ERROR)
	{
		wstring error(GetErrorMessage(ret));
		MessageBoxW(nullptr, error.c_str(), L"Error", MB_ICONERROR | MB_OK);
	}
	return ret;
}
