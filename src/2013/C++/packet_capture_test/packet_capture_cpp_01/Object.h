#pragma once

#include "String.h"

class String;

class Object
{
public:
	Object();
	~Object();

	virtual const String ToString() const;
};

