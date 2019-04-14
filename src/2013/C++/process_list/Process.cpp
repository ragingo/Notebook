#include "stdafx.h"

const Process::ProcessContainer Process::GetProcesses(std::function<bool(PROCESSENTRY32&)> filter)
{
	ProcessContainer processes;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);

	if (hSnapshot != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 pe;
		pe.dwSize = sizeof(PROCESSENTRY32);
		if (Process32First(hSnapshot, &pe))
		{
			do {
				if (filter(pe))
				{
					ProcessElement p(new Process);
					p->id = pe.th32ProcessID;
					p->parentId = pe.th32ParentProcessID;
					p->name = tstring(pe.szExeFile);
					p->threadCount = pe.cntThreads;
					p->basePriority = pe.pcPriClassBase;
					processes.push_back(p);
				}
			} while(Process32Next(hSnapshot, &pe));

		}
		CloseHandle(hSnapshot);
	}

	return processes;
}

const Process::ProcessContainer Process::GetProcesses()
{
	return GetProcesses([](PROCESSENTRY32& pe) { return true; });
}

const Process::ProcessContainer Process::GetProcessesByName(tstring name)
{
	return GetProcesses([=](PROCESSENTRY32& pe) { return std::regex_match(tstring(pe.szExeFile), tregex(name)); });
}

const Process::ProcessContainer Process::GetProcessById(DWORD id)
{
	return GetProcesses([=](PROCESSENTRY32& pe) { return id == pe.th32ProcessID; });
}
