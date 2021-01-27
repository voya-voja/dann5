#include <math.h> 
#include <Qtype.h>

#include <Logger.h>

using namespace dann5::ocean;

/*** Qbit ***/

const string Qbit::cSuporpositionSign = "(S)";



/*** Qwhole ***/

const string Qwhole::cUnknownSign = "(U)";


Qwhole::Qwhole()
	:q_binary(Qbit::cSuperposition)
{
	_lc;
}

Qwhole::Qwhole(const Qwhole& right)
	: q_binary(right)
{
	_lc;
}

Qwhole::Qwhole(Index size)
	: q_binary(size)
{
	for (Index at = 0; at < nobs(); at++)
		(*this)(at) = Qbit::cSuperposition;
	_lc;
}


Qwhole::Qwhole(long value)
	: q_binary(int(std::log(double(value))/std::log(2.0)) + 1)
{
	long v = value;
	int bitIndex = 0;
	while( v > 0)
	{
		(*this)(bitIndex++) = v % 2;
		v >>= 1;
	}
	_lc;
}

Qwhole::~Qwhole()
{
	_ld;
}

Qwhole& Qwhole::operator =(const Qwhole& right)
{
	q_binary::operator=(right);
	return *this;
}

QintInitializer Qwhole::operator <<(const Qbit& right)
{
	return QintInitializer(*static_cast<q_binary*>(this), right);
}

Qwhole Qwhole::operator ~() const
{
	Qwhole result(*this);
	for (Index at = 0; at < result.nobs(); at++)
	{
		Qbit rqb = result(at);
		if (rqb > 1)	// Qbit in superposition state
			result(at) = Qbit::cSuperposition;
		else            // Qbit in definite state
			result(at) = (result(at) + 1) % 2;
	}
	return result;
}

long Qwhole::value() const
{
	long value = 0;
	for (Index at = 0; at < nobs(); at++)
	{
		Qbit qbit = (*this)(at);
		// Qbit greater than 1 is in superposition state
		if (qbit > 1) return cUnknown;
		value += long(qbit * std::pow(2, at));
	}
	return value;
}

bool Qwhole::isUnknown() const
{
	for (Index at = 0; at < nobs(); at++)
	{
		// Qbit greater than 1 is in superposition state
		if ((*this)(at) > 1) return(true);
	}
	return(false);
}

Qwhole Qwhole::operator &(const Qwhole& right) const
{
	Qwhole result(*this);
	result &= right;
	return result;
}

Qwhole& Qwhole::operator &=(const Qwhole& right)
{
	Index rsize = right.nobs();
	Index size = nobs();
	if (size < rsize)
		resize(rsize);	// this Qwhole has to have same or more Qbits as right
	for (Index at = 0; at < size; at++)
	{
		Qbit tqb = (*this)(at), rqb = 0;
		if(at < rsize)
			rqb = right(at);
		if (tqb > 1 || rqb > 1)	// if either bits is in superposition
			(*this)(at) = Qbit::cSuperposition;
		else
			(*this)(at) &= rqb;
	}
	return *this;
}

Qwhole Qwhole::operator |(const Qwhole& right) const
{
	Qwhole result(*this);
	result |= right;
	return result;
}

Qwhole& Qwhole::operator |=(const Qwhole& right)
{
	Index rsize = right.nobs();
	Index size = nobs();
	if (size < rsize)
		resize(rsize);	// this Qwhole has to have same or more Qbits as right
	for (Index at = 0; at < size; at++)
	{
		Qbit tqb = (*this)(at), rqb = 0;
		if (at < rsize)
			rqb = right(at);
		if (tqb > 1 || rqb > 1)	// if either bits is in superposition
			(*this)(at) = Qbit::cSuperposition;
		else
			(*this)(at) |= rqb;
	}
	return *this;
}

Qwhole Qwhole::nand(const Qwhole& right) const
{
	Qwhole result(*this);
	result.nandMutable(right);
	return result;
}

Qwhole& Qwhole::nandMutable(const Qwhole& right)
{
	Index rsize = right.nobs();
	Index size = nobs();
	if (size < rsize)
		resize(rsize);	// this Qwhole has to have same or more Qbits as right
	for (Index at = 0; at < size; at++)
	{
		Qbit tqb = (*this)(at), rqb = 0;
		if (at < rsize)
			rqb = right(at);
		if (tqb > 1 || rqb > 1)	// if either bits is in superposition
			(*this)(at) = Qbit::cSuperposition;
		else
			(*this)(at) &= rqb;
	}
	return *this;
}

Qwhole Qwhole::nor(const Qwhole& right) const
{
	Qwhole result(*this);
	result.norMutable(right);
	return result;
}

Qwhole& Qwhole::norMutable(const Qwhole& right)
{
	Index rsize = right.nobs();
	Index size = nobs();
	if (size < rsize)
		resize(rsize);	// this Qwhole has to have same or more Qbits as right
	for (Index at = 0; at < size; at++)
	{
		Qbit tqb = (*this)(at), rqb = 0;
		if (at < rsize)
			rqb = right(at);
		if (tqb > 1 || rqb > 1)	// if either bits is in superposition
			(*this)(at) = Qbit::cSuperposition;
		else
			(*this)(at) |= rqb;
	}
	return *this;
}

Qwhole Qwhole::operator ^(const Qwhole& right) const
{
	Qwhole result(*this);
	result ^= right;
	return result;
}

Qwhole& Qwhole::operator ^=(const Qwhole& right)
{
	Index rsize = right.nobs();
	Index size = nobs();
	if (size < rsize)
		resize(rsize);	// this Qwhole has to have same or more Qbits as right
	for (Index at = 0; at < nobs(); at++)
	{
		Qbit tqb = (*this)(at), rqb = 0;
		if (at < rsize)
			rqb = right(at);
		if (tqb > 1 || rqb > 1)	// if either bits is in superposition
			(*this)(at) = Qbit::cSuperposition;
		else
			(*this)(at) ^= rqb;
	}
	return *this;
}

Qwhole Qwhole::operator +(const Qwhole& right) const
{
	Qwhole result(*this);
	result += right;
	return result;
}


Qwhole& Qwhole::operator +=(const Qwhole& right)
{
	Index tSize(nobs()), rSize(right.nobs());
	Index size(tSize < rSize ? rSize : tSize);
	if (tSize < size)
		resize(size);
	int carry = 0;
	bool superposition = false;
	Qwhole temp(nobs() + 1);
	for (Index at = 0; at < size; at++)
	{
		Qbit t((*this)(at)), r(at < rSize ? right(at) : 0);
		superposition = (t > 1) || (r > 1);
		if (superposition)
			(*this)(at) = Qbit::cSuperposition;
		else
		{
			carry = t + r + carry;
			(*this)(at) = carry % 2;
			temp(at) = (*this)(at);
			carry >>= 1;
		}
	}
	if (!superposition && carry == 1)
	{
		temp(nobs()) = carry;
		*this = temp;
	}
	return *this;
}

Qwhole Qwhole::operator *(const Qwhole& right) const
{
	Qwhole result(*this);
	result *= right;
	return result;
}

Qwhole& Qwhole::operator *=(const Qwhole& right)
{
	QbitMatrix xMatrix = q_binary(*this) * right.transpose();
	QbitMatrix reversedXMatrix = xMatrix.rowwise().reverse();

	// resize this vector to a # of diagonals + 1 of xMatrix
	Index size = reversedXMatrix.rows() + reversedXMatrix.cols();
	resize(size, NoChange);
	Index last = size - 1;

	int carry = 0;
	bool superpositon = false;
	// Value of 0 bit is the sum of right-most diagonal 
	Index atDiagonal = reversedXMatrix.cols() - 1;
	for (Index at = 0; at < nobs() - 1; at++)
	{
		q_binary diagonal = reversedXMatrix.diagonal(atDiagonal);
		for (Index atD = 0; atD < diagonal.rows() && !superpositon; atD++)
		{
			Qbit qb = diagonal(atD);
			superpositon = qb > 1;
			carry += qb;
		}
		if (superpositon)
			(*this)(at) = Qbit::cSuperposition;
		else
		{
			(*this)(at) = carry % 2;
			carry >>= 1;
		}
		--atDiagonal;
	}
	if (superpositon)
		(*this)(nobs() - 1) = Qbit::cSuperposition;
	else
		(*this)(nobs() - 1) = carry;
	return *this;
}

void Qwhole::resize(Index size, const Qbit& qBit)
{
	Index oSize = nobs();
	Qwhole temp(*this);
	q_binary::resize(size, NoChange);
	for (Index at = 0; at < size; at++)
		if (at < oSize)
			(*this)(at) = temp(at);
		else
			(*this)(at) = qBit;
}

string Qwhole::toString() const
{
	long v = value();
	if (v == cUnknown) return cUnknownSign;
	return(to_string(v));
}
