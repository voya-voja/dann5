#include <Qwhole.h>
#include <Qop.h>
#include <Factory.h>

using namespace dann5::ocean;


Qwhole& Qwhole::operator+=(const Qwhole& right)
{
	size_t size = nobs(), rSize = right.nobs();
	if (rSize > size)
	{ // resize this object if right has more bits
		resize(rSize);
	}
	// if right is smaller apply | operator up to last right Q bit
	for (size_t at = 0; at < rSize; at++)
		value()[at] += right.value()[at];
	return (*this);
}

Qwhole& Qwhole::operator*=(const Qwhole& right)
{
	size_t size = nobs(), rSize = right.nobs();
	if (rSize > size)
	{ // resize this object if right has more bits
		resize(rSize);
		size = nobs();
	}
	for (size_t at = 0; at < size; at++)
		if (at < rSize)
			value()[at] *= right.value()[at];
		else // when right is smaller all Qbits are 0
			value()[at] *= Qbit("", 0);
	return (*this);
}

Qwhole& Qwhole::operator-=(const Qwhole& right)
{
	size_t size = nobs(), rSize = right.nobs();
	if (rSize > size)
	{ // resize this object if right has more bits
		resize(rSize);
	}
	// if right is smaller apply | operator up to last right Q bit
	for (size_t at = 0; at < rSize; at++)
		value()[at] -= right.value()[at];
	return (*this);
}

Qwhole& Qwhole::operator/=(const Qwhole& right)
{
	size_t size = nobs(), rSize = right.nobs();
	if (rSize > size)
	{ // resize this object if right has more bits
		resize(rSize);
		size = nobs();
	}
	for (size_t at = 0; at < size; at++)
		if (at < rSize)
			value()[at] /= right.value()[at];
		else // when right is smaller all Qbits are 0
			value()[at] /= Qbit("", 0);
	return (*this);
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
	Qop::Sp pOp = Factory<string, Qop>::Instance().create(MultiplyQT::cMark);
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
	Qop::Sp pOp = Factory<string, Qop>::Instance().create(MultiplyQT::cMark);
	pOp->arguments({ clone(), right.root() });

	Qexpr<Qwhole> expr(pOp);
	return expr;
}