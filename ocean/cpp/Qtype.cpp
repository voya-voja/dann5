#include <math.h> 
#include <Qtype.h>

#include <Logger.h>

using namespace dann5::ocean;

/*** Qbit ***/

const string Qbit::cSuporpositionSign = "(S)";



/*** Qint ***/

const string Qint::cUnknownSign = "(U)";


Qint::Qint()
	:q_int(Qbit::cSuperposition)
{
	_lc;
}

Qint::Qint(const Qint& right)
	: q_int(right)
{
	_lc;
}

Qint::Qint(Index size)
	: q_int(size)
{
	for (Index at = 0; at < rows(); at++)
		(*this)(at) = Qbit::cSuperposition;
	_lc;
}


Qint::Qint(long value)
	: q_int(int(std::log(double(value))/std::log(2.0)) + 1)
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

Qint::~Qint()
{
	_ld;
}

Qint& Qint::operator =(const Qint& right)
{
	q_int::operator=(right);
	return *this;
}

QintInitializer Qint::operator <<(const Qbit& right)
{
	return QintInitializer(*static_cast<q_int*>(this), right);
}

Qint Qint::operator ~() const
{
	Qint result(*this);
	for (Index at = 0; at < result.rows(); at++)
	{
		Qbit rqb = result(at);
		if (rqb > 1)	// Qbit in superposition state
			result(at) = Qbit::cSuperposition;
		else            // Qbit in definite state
			result(at) = (result(at) + 1) % 2;
	}
	return result;
}

long Qint::value() const
{
	long value = 0;
	for (Index at = 0; at < rows(); at++)
	{
		Qbit qbit = (*this)(at);
		// Qbit greater than 1 is in superposition state
		if (qbit > 1) return cUnknown;
		value += long(qbit * std::pow(2, at));
	}
	return value;
}

bool Qint::isUnknown() const
{
	for (Index at = 0; at < rows(); at++)
	{
		// Qbit greater than 1 is in superposition state
		if ((*this)(at) > 1) return(true);
	}
	return(false);
}

Qint Qint::operator &(const Qint& right) const
{
	Qint result(*this);
	result &= right;
	return result;
}

Qint& Qint::operator &=(const Qint& right)
{
	Index rsize = right.rows();
	Index size = rows();
	if (size < rsize)
		resize(rsize);	// this Qint has to have same or more Qbits as right
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

Qint Qint::operator |(const Qint& right) const
{
	Qint result(*this);
	result |= right;
	return result;
}

Qint& Qint::operator |=(const Qint& right)
{
	Index rsize = right.rows();
	Index size = rows();
	if (size < rsize)
		resize(rsize);	// this Qint has to have same or more Qbits as right
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

Qint Qint::nand(const Qint& right) const
{
	Qint result(*this);
	result.nandMutable(right);
	return result;
}

Qint& Qint::nandMutable(const Qint& right)
{
	Index rsize = right.rows();
	Index size = rows();
	if (size < rsize)
		resize(rsize);	// this Qint has to have same or more Qbits as right
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

Qint Qint::nor(const Qint& right) const
{
	Qint result(*this);
	result.norMutable(right);
	return result;
}

Qint& Qint::norMutable(const Qint& right)
{
	Index rsize = right.rows();
	Index size = rows();
	if (size < rsize)
		resize(rsize);	// this Qint has to have same or more Qbits as right
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

Qint Qint::operator ^(const Qint& right) const
{
	Qint result(*this);
	result ^= right;
	return result;
}

Qint& Qint::operator ^=(const Qint& right)
{
	Index rsize = right.rows();
	Index size = rows();
	if (size < rsize)
		resize(rsize);	// this Qint has to have same or more Qbits as right
	for (Index at = 0; at < rows(); at++)
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

Qint Qint::operator +(const Qint& right) const
{
	Qint result(*this);
	result += right;
	return result;
}


Qint& Qint::operator +=(const Qint& right)
{
	if (rows() < right.rows())
		resize(right.rows());
	int carry = 0;
	bool superposition = false;
	Qint temp(rows() + 1);
	for (Index at = 0; at < rows(); at++)
	{
		Qbit t((*this)(at)), r(right(at));
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
		temp(rows()) = carry;
		*this = temp;
	}
	return *this;
}

Qint Qint::operator *(const Qint& right) const
{
	Qint result(*this);
	result *= right;
	return result;
}

Qint& Qint::operator *=(const Qint& right)
{
	QbitMatrix xMatrix = q_int(*this) * right.transpose();
	QbitMatrix reversedXMatrix = xMatrix.rowwise().reverse();

	// resize this vector to a # of diagonals + 1 of xMatrix
	Index size = reversedXMatrix.rows() + reversedXMatrix.cols();
	resize(size, NoChange);
	Index last = size - 1;

	int carry = 0;
	bool superpositon = false;
	// Value of 0 bit is the sum of right-most diagonal 
	Index atDiagonal = reversedXMatrix.cols() - 1;
	for (Index at = 0; at < rows() - 1; at++)
	{
		q_int diagonal = reversedXMatrix.diagonal(atDiagonal);
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
		(*this)(rows() - 1) = Qbit::cSuperposition;
	else
		(*this)(rows() - 1) = carry;
	return *this;
}

void Qint::resize(Index size, const Qbit& qBit)
{
	Index oSize = rows();
	Qint temp(*this);
	q_int::resize(size, NoChange);
	for (Index at = 0; at < size; at++)
		if (at < oSize)
			(*this)(at) = temp(at);
		else
			(*this)(at) = qBit;
}

string Qint::toString() const
{
	long v = value();
	if (v == cUnknown) return "(S)";
	return(to_string(v));
}
