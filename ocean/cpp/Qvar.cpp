#include <math.h> 
#include <Qequation.h>
#include <Qcondition.h>

#include <Logger.h>

using namespace dann5::ocean;

/*** Qvar ***/


Qvar::Qvar()
	:mpDefinition(new Qdef()), mValue()
{
	_lc;
}

Qvar::Qvar(const Qvar& right)
	:mpDefinition(right.mpDefinition), mValue(right.mValue)
{
	_lct(mpDefinition->name() + " = " + mValue.toString());
}

Qvar::Qvar(Index size, const string& definition)
	: mpDefinition(new Qdef(size, definition)), mValue(size)
{
	_lct(mpDefinition->name() + " = " + mValue.toString());
}

Qvar::Qvar(const string& definition, long value)
	: mpDefinition(new Qdef(Index(log(value) / log(2)) + 1, definition)), mValue(value)
{
	_lct(mpDefinition->name() + " = " + mValue.toString());
}

Qvar::Qvar(const Qdef& definition)
	: mpDefinition(definition.clone()), mValue()
{
	_lct(mpDefinition->name() + " = " + mValue.toString());
}

Qvar::Qvar(const Qdef& definition, const Qnni& value)
	: mpDefinition(definition.clone()), mValue(value)
{
	Index vn(mValue.nobs()), dn(mpDefinition->nobs());
	if (vn != dn)
	{
		if (vn < dn) 
			mValue.resize(dn);
		else 
			mpDefinition->resize(vn);
	}
	_lct(mpDefinition->name() + " = " + mValue.toString());
}

Qvar::~Qvar()
{
	_ldt(mpDefinition->name() + " = " + mValue.toString());
}

Qvar Qvar::operator~() const
{
	Qvar result(*this);
	~(*result.mpDefinition);
	~result.mValue;
	return result;
}

Qcondition Qvar::operator==(const Qvar& right) const
{
	Qcondition c;
	return(c);
}

Qcondition Qvar::operator!=(const Qvar& right) const
{
	Qcondition c;
	return(c);
}

Qcondition Qvar::operator>(const Qvar& right) const
{
	Qcondition c;
	return(c);
}

Qcondition Qvar::operator>=(const Qvar& right) const
{
	Qcondition c;
	return(c);
}

Qcondition Qvar::operator<(const Qvar& right) const
{
	Qcondition c;
	return(c);
}

Qcondition Qvar::operator<=(const Qvar& right) const
{
	Qcondition c;
	return(c);
}

Qvar& Qvar::operator&=(const Qvar& right)
{
	(*mpDefinition) &= (*right.mpDefinition);
	mValue &= right.mValue;
	return(*this);
}


Qequation Qvar::operator&(const Qvar& right) const
{
	Qequation result;
	result &= (*this);
	result &= right;
	return result;
}

Qequation Qvar::operator&(const Qequation& right) const
{
	Qequation result(right);
	result &= *this;
	return result;
}

Qvar& Qvar::operator|=(const Qvar& right)
{
	(*mpDefinition) |= (*right.mpDefinition);
	mValue |= right.mValue;
	return(*this);
}

Qequation Qvar::operator|(const Qvar& right) const
{
	Qequation result;
	result |= (*this);
	result |= right;
	return result;
}

Qequation Qvar::operator|(const Qequation& right) const
{
	Qequation result(right);
	result |= *this;
	return result;
}

Qvar& Qvar::operator^=(const Qvar& right)
{
	(*mpDefinition) ^= (*right.mpDefinition);
	mValue ^= right.mValue;
	return(*this);
}

Qequation Qvar::operator^(const Qvar& right) const
{
	Qequation result;
	result ^= (*this);
	result ^= right;
	return result;
}

Qequation Qvar::operator^(const Qequation& right) const
{
	Qequation result(right);
	result ^= *this;
	return result;
}

Qvar& Qvar::nandMutable(const Qvar& right)
{
	mpDefinition->nandMutable((*right.mpDefinition));
	mValue.nandMutable(right.mValue);
	return(*this);
}

Qequation Qvar::nand(const Qvar& right) const
{
	Qequation result;
	result.nandMutable(*this);
	result.nandMutable(right);
	return result;
}

Qequation Qvar::nand(const Qequation& right) const
{
	Qequation result(right);
	result.nandMutable(*this);
	return result;
}

Qvar& Qvar::norMutable(const Qvar& right)
{
	mpDefinition->norMutable((*right.mpDefinition));
	mValue.norMutable(right.mValue);
	return(*this);
}

Qequation Qvar::nor(const Qvar& right) const
{
	Qequation result;
	result.norMutable(*this);
	result.norMutable(right);
	return result;
}

Qequation Qvar::nor(const Qequation& right) const
{
	Qequation result(right);
	result.norMutable(*this);
	return result;
}

Qvar& Qvar::operator+=(const Qvar& right)
{
	(*mpDefinition) += (*right.mpDefinition);
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

Qequation Qvar::operator-(const Qvar& subtrahend) const
{
	// define difference variable with the same size as subtrahend argument
	Qvar difference(subtrahend.definition().size(), "dfrnc");

	// for this as minuend and subtrahend argment: difference = this - subtrahend 
	Qequation minuend(*this);
	minuend = difference + subtrahend;
	// return Q equation: minuend = difference + subtrahend
	return minuend;
}

Qvar& Qvar::operator*=(const Qvar& right)
{
	(*mpDefinition) *= (*right.mpDefinition);
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

Qequation Qvar::operator/(const Qvar& divisor) const
{
	// define quatient variable with same size as divisor 
	Qvar quotient(divisor.definition().size(), "qtnt");

	// for this as dividend and divisor argment: quotient = this / divisor 
	Qequation dividend(*this);
	dividend = quotient * divisor;
	// return Q equation: dividend = quotient * divisor
	return dividend;
}

void Qvar::resize(Index size, const Qbit& qBit)
{
	mpDefinition->resize(size);
	mValue.resize(size, qBit);
}

string Qvar::toString(bool bitFormat) const
{
	string vStr = "";
	if (bitFormat)
	{
		for (Index at = 0; at < mpDefinition->nobs(); at++)
		{
			vStr += (*mpDefinition)(at)->identity() + " = ";
			Qbit v = mValue(at);
			if (v > 1)	// supperposition
				vStr += Qnni::cUnknownSign;
			else
				vStr += to_string(v);
			vStr += "\n";
		}
	}
	else
	{
		vStr = mpDefinition->name() + " = " + mValue.toString();
	}
	return(vStr);
}

std::ostream& dann5::ocean::operator<<(std::ostream& out, const Qvar& bv)
{
	out << bv.toString();
	return out;
}