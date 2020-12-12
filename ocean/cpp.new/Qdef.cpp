#include <math.h> 
#include <Qdef.h>
#include <Qexpression.h>

#include <Logger.h>

using namespace dann5::ocean;
/*** Qdef ***/

const string Qdef::cDefaultName = "v";
Index Qdef::gNameIndex = 0;

string Qdef::AutoName()
{
	string id = to_string(gNameIndex++);
	return(cDefaultName + id + "_");
}


Qdef::Qdef()
	:StringVector(), mName(Qdef::AutoName())
{
	_lct(mName);
}

Qdef::Qdef(const Qdef& right)
	:StringVector(right), mName(right.mName)
{
	_lct(mName);
}

Qdef::Qdef(Index size)
	: StringVector(size), mName(Qdef::AutoName())
{
	for (Index at = 0; at < size; at++)
	{
		(*this)(at) = mName + to_string(at);
	}
	_lct(mName);
}

Qdef::Qdef(Index size, const string& name)
	: StringVector(size), mName(name)
{
	for (Index at = 0; at < size; at++)
	{
		(*this)(at) = name + to_string(at);
	}
	_lct(mName);
}

Qdef::~Qdef()
{
	_ldt(mName);
}

Qexpression Qdef::operator *(const Qdef& right) const
{
	Qexpression result(*this);
	result *= right;
	return result;
}

Qexpression Qdef::operator +(const Qdef& right) const
{
	Qexpression result(*this);
	result += right;
	return result;
}

void Qdef::extendTo(Index size)
{
	Index oSize = rows();
	Qdef temp(*this);
	resize(size, NoChange);
	for (Index at = 0; at < size; at++)
		if (at < oSize)
			(*this)(at) = temp(at);
		else
			(*this)(at) = mName + to_string(at);
}