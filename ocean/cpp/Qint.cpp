#include <math.h> 
#include <Qint.h>


using namespace dann5::ocean;

/*** QBitVector ***/

Qint::Qint()
	:qbit_int(cSuperposition)
{
}

Qint::Qint(const Qint& right)
	:qbit_int(right)
{
}

Qint::Qint(Index size)
	: qbit_int(size)
{
	for (Index at = 0; at < rows(); at++)
		(*this)(at) = cSuperposition;
}


Qint::Qint(long value)
	: qbit_int(int(std::log(double(value))/std::log(2.0)) + 1)
{
	long v = value;
	int bitIndex = 0;
	while( v > 0)
	{
		(*this)(bitIndex++) = v % 2;
		v >>= 1;
	}
}

Qint::~Qint()
{
}

Qint& Qint::operator =(const Qint& right)
{
	qbit_int::operator=(right);
	return *this;
}

Qint Qint::operator ~() const
{
	Qint result(*this);
	for (Index at = 0; at < result.rows(); at++)
	{
		Qbit rqb = result(at);
		if (rqb > 1)
			result(at) = cSuperposition;
		else
			result(at) = (result(at) + 1) % 2;
	}
	return result;
}

QintInitializer Qint::operator <<(const bool& right)
{
	return QintInitializer(*static_cast<qbit_int*>(this), right);
}

long Qint::value() const
{
	long value = 0;
	for (Index at = 0; at < rows(); at++)
	{
		Qbit qbit = (*this)(at);
		if (qbit > 1) return -1;
		value += long(qbit * std::pow(2, at));
	}
	return value;
}

Qint Qint::operator &(const Qint& right) const
{
	Qint result(*this);
	result &= right;
	return result;
}

Qint& Qint::operator &=(const Qint& right)
{
	if (rows() < right.rows())
		extendTo(right.rows());
	for (Index at = 0; at < rows(); at++)
	{
		Qbit tqb = (*this)(at), rqb = right(at);
		if (tqb > 1 || rqb > 1)
			(*this)(at) = cSuperposition;
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
	if (rows() < right.rows())
		extendTo(right.rows());
	for (Index at = 0; at < rows(); at++)
	{
		Qbit tqb = (*this)(at), rqb = right(at);
		if (tqb > 1 || rqb > 1)
			(*this)(at) = cSuperposition;
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
	if (rows() < right.rows())
		extendTo(right.rows());
	for (Index at = 0; at < rows(); at++)
	{
		Qbit tqb = (*this)(at), rqb = right(at);
		if (tqb > 1 || rqb > 1)
			(*this)(at) = cSuperposition;
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
		extendTo(right.rows());
	int carry = 0;
	Qint temp(rows() + 1);
	for (Index at = 0; at < rows(); at++)
	{
		carry = (*this)(at) + right(at) + carry;
		(*this)(at) = carry % 2;
		temp(at) = (*this)(at);
		carry >>= 1;
	}
	if (carry)
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
	Xmatrix xMatrix = qbit_int(*this) * right.transpose();
	Xmatrix reversedXMatrix = xMatrix.rowwise().reverse();

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
		qbit_int diagonal = reversedXMatrix.diagonal(atDiagonal);
		for (Index atD = 0; atD < diagonal.rows() && !superpositon; atD++)
		{
			Qbit qb = diagonal(atD);
			superpositon = qb == cSuperposition;
			carry += qb;
		}
		if (superpositon)
			(*this)(at) = cSuperposition;
		else
			(*this)(at) = carry % 2;
		carry >>= 1;
		--atDiagonal;
	}
	if (superpositon)
		(*this)(rows() - 1) = cSuperposition;
	else
		(*this)(rows() - 1) = carry;
	return *this;
}

void Qint::extendTo(Index size)
{
	Index oSize = rows();
	Qint temp(*this);
	resize(size, NoChange);
	for (Index at = 0; at < size; at++)
		if (at < oSize)
			(*this)(at) = temp(at);
		else
			(*this)(at) = 0;
}