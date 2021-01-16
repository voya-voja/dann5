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
	:qbit_def_vector(), mDefinition(Qdef::AutoName())
{
	_lct(mDefinition);
}

Qdef::Qdef(const Qdef& right)
	:qbit_def_vector(right), mDefinition(right.mDefinition)
{
	_lct(mDefinition);
}

Qdef::Qdef(Index size)
	: qbit_def_vector(size), mDefinition(Qdef::AutoName())
{
	initialize(size);
}

Qdef::Qdef(Index size, const string& name)
	: qbit_def_vector(size), mDefinition(name)
{
	initialize(size);
}

void Qdef::initialize(Index size)
{
	for (Index at = 0; at < size; at++)
	{
		(*this)(at) = operand(at);
	}
	_lct(mDefinition);
}


Qoperand::Sp Qdef::operand(Index level)
{
	string identifier = mDefinition + to_string(level);
	return Qoperand::Sp(new Qoperand(identifier));
}

Qdef::~Qdef()
{
	_ldt(mDefinition);
}

void Qdef::concatenate(const Qdef& right, const string& sign)
{
	Index size(rows()), rSize(right.rows());
	if (size < rSize) resize(rSize);
	mDefinition += sign + right.mDefinition;
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
			rId = right.mDefinition + to_string(at);

		}
		(*this)(at)->identity(id + sign + rId);
	}
}

Qdef& Qdef::operator~()
{
	string sign = "~";
	Index size(rows());
	mDefinition = sign + mDefinition;
	for (Index at = 0; at < size; at++)
	{
		string id = (*this)(at)->identity();
		(*this)(at)->identity(sign + id);
	}
	return(*this);
}

Qdef& Qdef::operator&=(const Qdef& right)
{
	concatenate(right, "&");
	return(*this);
}

Qexpression Qdef::operator &(const Qdef& right) const
{
	Qexpression result(*this);
	result &= right;
	return result;
}

Qexpression Qdef::operator &(const Qexpression& right) const
{
	Qexpression result(*this);
	result &= right;
	return result;
}

Qdef& Qdef::operator|=(const Qdef& right)
{
	concatenate(right, "|");
	return(*this);
}

Qexpression Qdef::operator |(const Qdef& right) const
{
	Qexpression result(*this);
	result |= right;
	return result;
}

Qexpression Qdef::operator |(const Qexpression& right) const
{
	Qexpression result(*this);
	result |= right;
	return result;
}

Qdef& Qdef::nandMutable(const Qdef& right)
{
	concatenate(right, "~&");
	return(*this);
}

Qexpression Qdef::nand(const Qdef& right) const
{
	Qexpression result(*this);
	result.nandMutable(right);
	return result;
}

Qexpression Qdef::nand(const Qexpression& right) const
{
	Qexpression result(*this);
	result.nandMutable(right);
	return result;
}

Qdef& Qdef::norMutable(const Qdef& right)
{
	concatenate(right, "~|");
	return(*this);
}

Qexpression Qdef::nor(const Qdef& right) const
{
	Qexpression result(*this);
	result.norMutable(right);
	return result;
}

Qexpression Qdef::nor(const Qexpression& right) const
{
	Qexpression result(*this);
	result.norMutable(right);
	return result;
}

Qdef& Qdef::operator^=(const Qdef& right)
{
	concatenate(right, "^");
	return(*this);
}

Qexpression Qdef::operator ^(const Qdef& right) const
{
	Qexpression result(*this);
	result ^= right;
	return result;
}

Qexpression Qdef::operator ^(const Qexpression& right) const
{
	Qexpression result(*this);
	result ^= right;
	return result;
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
			(*this)(at) = operand(at);
}