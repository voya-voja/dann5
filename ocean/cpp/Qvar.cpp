#include <math.h> 
#include <Qequation.h>

#include <Logger.h>

using namespace dann5::ocean;

/*** Qvar ***/


Qvar::Qvar()
	:mSymbol(), mValue()
{
	_lct(mSymbol.name() + " = " + mValue.toString());
}

Qvar::Qvar(const Qvar& right)
	:mSymbol(right.mSymbol), mValue(right.mValue)
{
	_lct(mSymbol.name() + " = " + mValue.toString());
}

Qvar::Qvar(Index size, const string& symbol)
	: mSymbol(size, symbol), mValue(size)
{
	_lct(mSymbol.name() + " = " + mValue.toString());
}

Qvar::Qvar(const string& symbol, long value)
	: mSymbol(Index(log(value) / log(2)) + 1, symbol), mValue(value)
{
	_lct(mSymbol.name() + " = " + mValue.toString());
}

Qvar::Qvar(const Qdef& symbol)
	: mSymbol(symbol), mValue()
{
	_lct(mSymbol.name() + " = " + mValue.toString());
}

Qvar::Qvar(const Qdef& symbol, const Qint& value)
	: mSymbol(symbol), mValue(value)
{
	_lct(mSymbol.name() + " = " + mValue.toString());
}

Qvar::~Qvar()
{
	_ldt(mSymbol.name() + " = " + mValue.toString());
}

Qvar& Qvar::operator+=(const Qvar& right)
{
	mSymbol += right.mSymbol;
	mValue += right.mValue;
	return(*this);
}

Qequation Qvar::operator+(const Qvar& right) const
{
	Qequation result;
	result += (*this);
	result += right;
	return result;
}

Qequation Qvar::operator+(const Qequation& right) const
{
	Qequation result(right);
	result += *this;
	return result;
}

Qvar& Qvar::operator*=(const Qvar& right)
{
	mSymbol *= right.mSymbol;
	mValue *= right.mValue;
	return(*this);
}
Qequation Qvar::operator*(const Qvar& right) const
{
	Qequation result;
	result *= (*this);
	result *= right;
	return result;
}


Qequation Qvar::operator*(const Qequation& right) const
{
	Qequation result(right);
	result *= *this;
	return result;
}

void Qvar::resize(Index size, const Qbit& qBit)
{
	mSymbol.resize(size);
	mValue.resize(size, qBit);
}

string Qvar::toString(bool bitFormat) const
{
	string vStr = "";
	if (bitFormat)
	{
		for (Index at = 0; at < mSymbol.rows(); at++)
		{
			vStr += mSymbol(at)->identity() + " = ";
			Qbit v = mValue(at);
			if (v > 1)	// supperposition
				vStr += Qint::cUnknownSign;
			else
				vStr += to_string(v);
			vStr += "\n";
		}
	}
	else
	{
		vStr = mSymbol.name() + " = " + mValue.toString();
	}
	return(vStr);
}

std::ostream& dann5::ocean::operator<<(std::ostream& out, const Qvar& bv)
{
	out << bv.toString();
	return out;
}