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
	:qbit_def_vector(), mSymbol(Qdef::AutoName())
{
	_lct(mSymbol);
}

Qdef::Qdef(const Qdef& right)
	:qbit_def_vector(right), mSymbol(right.mSymbol)
{
	_lct(mSymbol);
}

Qdef::Qdef(Index size)
	: qbit_def_vector(size), mSymbol(Qdef::AutoName())
{
	for (Index at = 0; at < size; at++)
	{
		(*this)(at) = Qoperand::Sp(new Qoperand(mSymbol + to_string(at)));
	}
	_lct(mSymbol);
}

Qdef::Qdef(Index size, const string& name)
	: qbit_def_vector(size), mSymbol(name)
{
	for (Index at = 0; at < size; at++)
	{
		(*this)(at) = Qoperand::Sp(new Qoperand(name + to_string(at)));
	}
	_lct(mSymbol);
}

Qdef::~Qdef()
{
	_ldt(mSymbol);
}

void Qdef::concatenate(const Qdef& right, const string& sign)
{
	Index size(rows()), rSize(right.rows());
	if (size < rSize) resize(rSize);
	mSymbol += sign + right.mSymbol;
	for (Index at = 0; at < size; at++)
	{
		string id = (*this)(at)->identity();
		string rId = "";
		if (at < rSize)
		{
			rId = (right)(at)->identity();
		}
		else
		{
			rId = right.mSymbol + to_string(at);

		}
		(*this)(at)->identity(id + sign + rId);
	}
}

Qdef& Qdef::operator+=(const Qdef& right)
{
	concatenate(right, "+");
	return(*this);
}

Qexpression Qdef::operator +(const Qdef& right) const
{
	Qexpression result(*this);
	result += right;
	return result;
}

Qexpression Qdef::operator +(const Qexpression& right) const
{
	Qexpression result(*this);
	result += right;
	return result;
}

Qdef& Qdef::operator*=(const Qdef& right)
{
	concatenate(right, "*");
	return(*this);
}

Qexpression Qdef::operator *(const Qdef& right) const
{
	Qexpression result(*this);
	result *= right;
	return result;
}

Qexpression Qdef::operator *(const Qexpression& right) const
{
	Qexpression result(*this);
	result *= right;
	return result;
}

void Qdef::resize(Index size)
{
	Index oSize = rows();
	Qdef temp(*this);
	qbit_def_vector::resize(size, NoChange);
	for (Index at = 0; at < size; at++)
		if (at < oSize)
			(*this)(at) = temp(at);
		else
			(*this)(at) = Qoperand::Sp(new Qoperand(mSymbol + to_string(at)));
}