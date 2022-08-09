#include <iostream>
#include <windows.h>
#include <conio.h>

using namespace std;

int pid;

BOOL WINAPI ConsoleHandler(DWORD CEvent) {
	if (CEvent == CTRL_CLOSE_EVENT) {
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
		if (hProcess != NULL) {
			TerminateProcess(hProcess, 0);
			CloseHandle(hProcess);
		}
	}
	return TRUE;
}

int main(int argc, char* argv[]) {
	if (!argv[1]) {
		cout << "Usage: " << argv[0] << " <filename>";
		Sleep(2000);
		return 0;
	}
	else {
		cout << argv[1] << endl << endl;

		STARTUPINFO si;
		PROCESS_INFORMATION pi;
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));

		if (!CreateProcessA(NULL, argv[1], NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi)) {
			cout << "CreateProcess failed: " << GetLastError();
			Sleep(2000);
			return GetLastError();
		}
		else {
			SetConsoleCtrlHandler(ConsoleHandler, TRUE);
			pid = pi.dwProcessId;
			cout << "CreateProcess succeeded." << endl;
			cout << "Process handle is " << pi.hProcess << endl;
			cout << "Thread handle is " << pi.hThread << endl;
			cout << "Process ID is " << pi.dwProcessId << endl;
			cout << "Thread ID is " << pi.dwThreadId << endl;
		}
		
		cout << "\nPress any key to resume " << pi.dwProcessId << "..." << endl;
		_getch();
		ResumeThread(pi.hThread);
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		return 0;
	}
}