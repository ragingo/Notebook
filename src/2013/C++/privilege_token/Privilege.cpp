#include "stdafx.h"

class Privilege
{
private:
	LUID_AND_ATTRIBUTES luidAndAttrs;
	tstring name;
	tstring displayName;

public:
	static const int PRIV_NAME_MAX_LENGTH = 64;
	static const int PRIV_DISPNAME_MAX_LENGTH = 128;
	typedef std::vector<boost::shared_ptr<Privilege>> PrivilegeContainer;

	Privilege(LUID_AND_ATTRIBUTES luidAndAttrs)
	{
		this->luidAndAttrs = luidAndAttrs;
	}

	static PrivilegeContainer GetPrivileges()
	{
		PrivilegeContainer result;
		HANDLE hToken;

		if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
		{
			tcout << "OpenProcessToken Ž¸”s" << std::endl;
			return result;
		}

		DWORD length;
		GetTokenInformation(hToken, TokenPrivileges, NULL, 0, &length);
		boost::scoped_array<TOKEN_PRIVILEGES> privs(new TOKEN_PRIVILEGES[length]);
		GetTokenInformation(hToken, TokenPrivileges, privs.get(), length, &length);


		for (DWORD i=0; i<privs.get()->PrivilegeCount; i++)
		{
			auto luidAndAttrs = privs.get()->Privileges[i];
			boost::shared_ptr<Privilege> priv(new Privilege(luidAndAttrs));
			result.push_back(priv);
		}

		CloseHandle(hToken);

		return result;
	}

	tstring GetName()
	{
		if (this->name.empty())
		{
			DWORD cchName = PRIV_NAME_MAX_LENGTH;
			TCHAR name[PRIV_NAME_MAX_LENGTH+1] = {0};

			LookupPrivilegeName(NULL, &this->luidAndAttrs.Luid, name, &cchName);
			this->name = tstring(name);
		}
		return this->name;
	}

	tstring GetDisplayName()
	{
		if (this->displayName.empty())
		{
			DWORD cchDispName = PRIV_DISPNAME_MAX_LENGTH;
			TCHAR dispname[PRIV_DISPNAME_MAX_LENGTH+1] = {0};
			DWORD langID = 0;

			LookupPrivilegeDisplayName(NULL, GetName().c_str(), dispname, &cchDispName, &langID);
			this->displayName = tstring(dispname);
		}
		return this->displayName;
	}

	DWORD GetStatus() const
	{
		return this->luidAndAttrs.Attributes;
	}

	tstring GetStatusText() const
	{
		tstring text;
		if (this->luidAndAttrs.Attributes & SE_PRIVILEGE_ENABLED)
		{
			text.append(_T("—LŒø"));
		}
		else
		{
			text.append(_T("–³Œø"));
		}
		return text;
	}

	void SetStatus(DWORD status)
	{
		HANDLE hToken;

		if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES, &hToken))
		{
			tcout << "OpenProcessToken Ž¸”s" << std::endl;
			return;
		}
		TOKEN_PRIVILEGES privs;
		privs.PrivilegeCount = 1;
		privs.Privileges[0].Attributes = status;
		privs.Privileges[0].Luid = this->luidAndAttrs.Luid;
		if (!AdjustTokenPrivileges(hToken, FALSE, &privs, 0, NULL, NULL))
		{
			tcout << "AdjustTokenPrivileges Ž¸”s" << std::endl;
			CloseHandle(hToken);
			return;
		}

		this->luidAndAttrs.Attributes = status;
		CloseHandle(hToken);
	}
};
