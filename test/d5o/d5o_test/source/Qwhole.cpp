#include <Qwhole.h>
#include <Qop.h>
#include <Factory.h>

using namespace dann5::ocean;

Qwhole::operator unsigned long long()
{
	return(operator const unsigned long long());
}

Qwhole::operator const unsigned long long() const
{
	if (any(Qbit::cSuperposition))
		return Qwhole::cUnknown;
	unsigned long long ull = 0;
	size_t at = 0;
	for (auto qbit : value())
	{
		ull += qbit * 2 ^ at;
		at++;
	}
	return(ull);
}


Qwhole& Qwhole::operator+=(const Qwhole& right)
{
	Index tSize(nobs()), rSize(right.nobs());
	Index size(tSize < rSize ? rSize : tSize);
	if (tSize < size)
		resize(size);

	unsigned char carry = 0;
	bool superposition = false;
	Qbits& tQbits = value(), rQbits = right.value();
	for (Index at = 0; at < size; at++)
	{
		// if right is smaller apply + operator up to last right Q bit
		QbitV t(tQbits[at]), r(at < rSize ? rQbits[at] : 0);
		superposition = (t > 1) || (r > 1);
		if (superposition)
			tQbits[at].value(Qbit::cSuperposition);
		else
		{
			carry += t + r;
			tQbits[at].value(carry % 2);
			carry >>= 1;
		}
	}
	if (!superposition && carry == 1)
	{
		resize(size + 1);
		tQbits[size].value(carry);
	}
	return (*this);
}

Qwhole& Qwhole::operator*=(const Qwhole& right)
{
	QbitVector tVector(this->operator const dann5::ocean::QbitVector()), rVector(right.operator const dann5::ocean::QbitVector());
	QbitMatrix xMatrix = tVector * rVector.transpose();
	QbitMatrix reversedXMatrix = xMatrix.rowwise().reverse();

	// resize this vector to a # of diagonals + 1 of xMatrix
	Index size = reversedXMatrix.rows() + reversedXMatrix.cols();
	resize(size);
	Index last = size - 1;

	int carry = 0;
	bool superpositon = false;
	// Value of 0 bit is the sum of right-most diagonal 
	Index atDiagonal = reversedXMatrix.cols() - 1;
	Qbits& qbits = value();
	for (Index at = 0; at < last; at++)
	{
		QbitVector diagonal = reversedXMatrix.diagonal(atDiagonal);
		for (Index atD = 0; atD < diagonal.rows() && !superpositon; atD++)
		{
			QbitV qb = diagonal(atD);
			superpositon = qb == Qbit::cSuperposition;
			carry += qb;
		}
		if (superpositon)
			qbits[at].value(Qbit::cSuperposition);
		else
		{
			qbits[at].value(carry % 2);
			carry >>= 1;
		}
		--atDiagonal;
	}
	if (superpositon)
		qbits[last].value(Qbit::cSuperposition);
	else
		qbits[last].value(carry);
	return *this;
}

Qwhole& Qwhole::operator-=(const Qwhole& right)
{
	unsigned long long tValue(*this), rValue(right);
	if (rValue > tValue)
		throw invalid_argument("The right operand (" + to_string(rValue)
			+ ") has to be lower than value of the left operand ("
			+ to_string(tValue) + ")");

	unsigned long long result = tValue - rValue;
	size_t size = size_t(log(result) / log(2) + 1);
	resize(size);
	Qbits& qbits = value();
	for (size_t at = 0; at < size; at++)
	{
		qbits[at].value(result % 2);
		result >>= 1;
	}
	return *this;
}

Qwhole& Qwhole::operator/=(const Qwhole& right)
{
	unsigned long long tValue(*this), rValue(right);
	if (rValue > tValue)
		throw invalid_argument("The right operand (" + to_string(rValue)
			+ ") has to be lower than value of the left operand ("
			+ to_string(tValue) + ")");

	unsigned long long result = tValue / rValue;
	size_t size = size_t(log(result) / log(2) + 1);
	resize(size);
	Qbits & qbits = value();
	for (size_t at = 0; at < size; at++)
	{
		qbits[at].value(result % 2);
		result >>= 1;
	}
	return *this;
}

Qexpr<Qwhole> Qwhole::operator+(const Qwhole& right) const
{
	Qop::Sp pOp = Factory<string, Qop>::Instance().create(AdderQT::cMark);
	pOp->arguments({ clone(), right.clone() });

	Qexpr<Qwhole> expr(pOp);
	return expr;
}

Qexpr<Qwhole> Qwhole::operator*(const Qwhole& right) const
{
	Qop::Sp pOp = Factory<string, Qop>::Instance().create(Qmultiplication::cMark);
	pOp->arguments({ clone(), right.clone() });

	Qexpr<Qwhole> expr(pOp);
	return expr;
}

Qexpr<Qwhole> Qwhole::operator==(const Qwhole& right) const
{
	Qop::Sp pOp = Factory<string, Qop>::Instance().create(EqQT::cMark);
	pOp->arguments({ clone(), right.clone() });

	Qexpr<Qwhole> expr(pOp);
	return expr;
}

Qexpr<Qwhole> Qwhole::operator!=(const Qwhole& right) const
{
	Qop::Sp pOp = Factory<string, Qop>::Instance().create(NeqQT::cMark);
	pOp->arguments({ clone(), right.clone() });

	Qexpr<Qwhole> expr(pOp);
	return expr;
}

Qexpr<Qwhole> Qwhole::operator>(const Qwhole& right) const
{
	Qop::Sp pOp = Factory<string, Qop>::Instance().create(GtQT::cMark);
	pOp->arguments({ clone(), right.clone() });

	Qexpr<Qwhole> expr(pOp);
	return expr;
}

Qexpr<Qwhole> Qwhole::operator>=(const Qwhole& right) const
{
	Qop::Sp pOp = Factory<string, Qop>::Instance().create(GeQT::cMark);
	pOp->arguments({ clone(), right.clone() });

	Qexpr<Qwhole> expr(pOp);
	return expr;
}

Qexpr<Qwhole> Qwhole::operator<(const Qwhole& right) const
{
	Qop::Sp pOp = Factory<string, Qop>::Instance().create(LtQT::cMark);
	pOp->arguments({ clone(), right.clone() });

	Qexpr<Qwhole> expr(pOp);
	return expr;
}

Qexpr<Qwhole> Qwhole::operator<=(const Qwhole& right) const
{
	Qop::Sp pOp = Factory<string, Qop>::Instance().create(LeQT::cMark);
	pOp->arguments({ clone(), right.clone() });

	Qexpr<Qwhole> expr(pOp);
	return expr;
}

Qexpr<Qwhole> Qwhole::operator+(const Qexpr<Qwhole>& right) const
{
	Qop::Sp pOp = Factory<string, Qop>::Instance().create(AdderQT::cMark);
	pOp->arguments({ clone(), right.root() });

	Qexpr<Qwhole> expr(pOp);
	return expr;
}

Qexpr<Qwhole> Qwhole::operator*(const Qexpr<Qwhole>& right) const
{
	Qop::Sp pOp = Factory<string, Qop>::Instance().create(Qmultiplication::cMark);
	pOp->arguments({ clone(), right.root() });

	Qexpr<Qwhole> expr(pOp);
	return expr;
}