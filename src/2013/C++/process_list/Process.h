
#ifndef PROCESS_dfee32729aa44eb7bcbe5d9161f6362e
#define PROCESS_dfee32729aa44eb7bcbe5d9161f6362e

#include <functional>
#include <regex>
#include <vector>
#include <Windows.h>
#include <TlHelp32.h>
#include <boost/shared_ptr.hpp>
#include "my_tchar.h"


class Process
{
private:
	HANDLE hProcess;
	DWORD id;
	DWORD parentId;
	DWORD threadCount;
	tstring name;
	LONG basePriority;

	Process()
	{
	}

public:
	typedef boost::shared_ptr<Process> ProcessElement;
	typedef std::vector<ProcessElement> ProcessContainer;

	//static const Process& Start(tstring fileName)
	//{
	//	Process proc;
	//	proc.hProcess = OpenProcess(
	//	return proc;
	//}

	static const ProcessContainer GetProcesses(std::function<bool(PROCESSENTRY32&)> filter);
	static const ProcessContainer GetProcesses();
	static const ProcessContainer GetProcessesByName(tstring name);
	static const ProcessContainer GetProcessById(DWORD id);

	const DWORD& GetId() const
	{
		return id;
	}

	const DWORD& GetParentId() const
	{
		return parentId;
	}

	LONG GetBasePriority() const
	{
		return basePriority;
	}

	DWORD GetThreadCount() const
	{
		return threadCount;
	}

	const tstring& GetName() const
	{
		return name;
	}
};

#endif