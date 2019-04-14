
#include "stdafx.h"
#include "Privilege.cpp"

void Test1();

int _tmain(int argc, _TCHAR* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_tsetlocale(LC_ALL, _T(""));

	Test1();

	return EXIT_SUCCESS;
}



void Test1()
{
	tcout << _T("----- before -----") << std::endl;

	auto privs = Privilege::GetPrivileges();
	for each(auto priv in privs)
	{
		auto ptr = priv.get();
		tcout
			<< tformat(L"%-4s %-64s %-128s") 
				% ptr->GetStatusText()
				% ptr->GetName()
				% ptr->GetDisplayName()
			<< std::endl;
	}

	tcout << _T("----- “ÁŒ ‘€ì -----") << std::endl;

	auto pred = 
		[](const boost::shared_ptr<Privilege> &p) {
			return (p->GetName() == SE_SHUTDOWN_NAME);
		};
	auto targetPriv = std::find_if(privs.begin(), privs.end(), pred);
	tcout << targetPriv->get()->GetName() << std::endl;
	targetPriv->get()->SetStatus(SE_PRIVILEGE_ENABLED);

	tcout << _T("----- after -----") << std::endl;

	privs = Privilege::GetPrivileges();
	for each(auto priv in privs)
	{
		auto ptr = priv.get();
		tcout
			<< tformat(L"%-4s %-64s %-128s") 
				% ptr->GetStatusText()
				% ptr->GetName()
				% ptr->GetDisplayName()
			<< std::endl;
	}
}

