#include "StdAfx.h"
#include "Object.h"

Object::Object()
{
}


Object::~Object()
{
}

const String Object::ToString() const
{
	String str(_T("Object"));
	return str;
}
