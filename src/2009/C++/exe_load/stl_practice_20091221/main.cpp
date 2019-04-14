
#include <windows.h>
#include <imagehlp.h>
#pragma comment(lib, "imagehlp.lib")

#include <ctime>
#include <iostream>

using namespace std;

PLOADED_IMAGE LoadImageFile(const char* filename)
{
	PLOADED_IMAGE pImg = ImageLoad(filename, NULL);
	if(pImg == NULL)
	{
		cout << "load error." << endl;
		exit(-1);
		return NULL;
	}
	return pImg;
}

void DisplayMachineType(const WORD& val)
{
	cout << "MachineType          : ";
	switch(val)
	{
	case IMAGE_FILE_MACHINE_UNKNOWN:
		cout << "Unknown";
		break;
	case IMAGE_FILE_MACHINE_I386:
		cout << "Intel 386";
		break;
	case IMAGE_FILE_MACHINE_IA64:
		cout << "Intel 64";
		break;
	case IMAGE_FILE_MACHINE_AMD64:
		cout << "AMD 64(K8)";
		break;
	}
	cout << "(" << val << ")" << endl;
}

void DisplaySubsystem(const WORD& val)
{
	cout << "Subsystem            : ";
	switch(val)
	{
	case IMAGE_SUBSYSTEM_NATIVE:
		cout << "Native";
		break;
	case IMAGE_SUBSYSTEM_WINDOWS_CUI:
		cout << "WindowsCUI";
		break;
	case IMAGE_SUBSYSTEM_WINDOWS_GUI:
		cout << "WindowsGUI";
		break;
	}
	cout << "(" << val << ")" << endl;
}

void DisplayFileHeader(const IMAGE_FILE_HEADER& header)
{
	time_t create_datetime = (time_t)header.TimeDateStamp;
	char *p = ctime(&create_datetime); // ‰½ŒÌ‚©––”ö‚É\n‚ª•t‚¢‚Ä‚½
	cout << "TimeDateStamp        : " << p;

	DisplayMachineType(header.Machine);
}

void DisplaySection(const IMAGE_SECTION_HEADER& section)
{
	cout << "Name                 : " << section.Name                 << endl;
	cout << "Characteristics      : " << section.Characteristics      << endl;
	cout << "Misc" << endl;
	cout << "    PhysicalAddress  : " << section.Misc.PhysicalAddress << endl;
	cout << "    VirtualSize      : " << section.Misc.VirtualSize     << endl;
	cout << "NumberOfLinenumbers  : " << section.NumberOfLinenumbers  << endl;
	cout << "NumberOfRelocations  : " << section.NumberOfRelocations  << endl;
	cout << "PointerToLinenumbers : " << section.PointerToLinenumbers << endl;
	cout << "PointerToRawData     : " << section.PointerToRawData     << endl;
	cout << "PointerToRelocations : " << section.PointerToRelocations << endl;
	cout << "SizeOfRawData        : " << section.SizeOfRawData        << endl;
	cout << "VirtualAddress       : " << section.VirtualAddress       << endl;
	cout << "-------------------------------------------------------" << endl;
}

int main()
{
	const char* filename = "c:\\windows\\notepad.exe";

	PLOADED_IMAGE pImg = LoadImageFile(filename);

	cout << "[File Header]" << endl;
	DisplayFileHeader(pImg->FileHeader->FileHeader);

	DisplaySubsystem(pImg->FileHeader->OptionalHeader.Subsystem);

	cout << "[Sections]" << endl;
	for(ULONG i=0; i < pImg->NumberOfSections; i++)
	{
		DisplaySection(pImg->Sections[i]);
	}

	ImageUnload(pImg);
}