#include <Qbinary.h>
#include <Qop.h>
#include <Factory.h>

using namespace dann5::ocean;

Qbinary::operator QbitVector()
{
	return(operator const QbitVector());
}

Qbinary::operator const QbitVector() const
{
	QbitVector vector(nobs());
	size_t at = 0;
	for (auto qbit : value())
	{
		vector(at) = qbit;
		at++;
	}
	return(vector);
}

Qbinary::Qbinary(const string& id)
	:Qtype(id), mValue(8)
{
	for (size_t at = 0; at < 8; at++)
	{
		mValue[at] = Qbit(id + to_string(at), 0);
	}
}

Qbinary::Qbinary(size_t size, const string& id)
	:Qtype(id), mValue(size)
{
	for (size_t at = 0; at < size; at++)
	{
		mValue[at] = Qbit(id + to_string(at));
	}
}

Qbinary::Qbinary(const string& id, const Qbits& value)
	:Qtype(id), mValue(value)
{

}

Qbinary::Qbinary(const string& id, const Bits& value, bool asis)
	: Qtype(id), mValue(value.size())
{
	size_t size = asis ? value.size() : size_t(log(value.to_ullong()) / log(2)) + 1;
	for (size_t at = 0; at < size; at++)
	{
		mValue[at] = Qbit(id + to_string(at), value[at]);
	}
}

void Qbinary::resize(size_t size, QbitV value)
{
	size_t oSize = nobs();
	mValue.resize(size);
	for (size_t at = oSize; at < size; at++)
		mValue[at] = Qbit(id() + to_string(at), value);
}

bool Qbinary::all(QbitV value) const
{
	size_t at = 0, size = mValue.size();
	while(at < size && mValue[at++] == value);
	return(at == size);
}

bool Qbinary::any(QbitV value) const
{
	size_t at = 0, size = mValue.size();
	while (at < size && mValue[at++] != value);
	return(at < size);
}

bool Qbinary::none(QbitV value) const
{
	size_t at = 0, size = mValue.size();
	while (at < size && mValue[at++] != value);
	return(at == size);
}

size_t Qbinary::count(QbitV value) const
{
	size_t count = 0;
	for (auto v : mValue)
		if (v == value) count++;
	return count;
}

Qbinary Qbinary::operator<<(size_t pos) const
{
	Qbinary result(*this);
	result <<= pos;
	return result;
}

Qbinary& Qbinary::operator<<=(size_t pos)
{
	size_t size = nobs();
	for (size_t at = size; at > 0; at--)
	{
		if (at - 1 >= pos)
			mValue[at - 1].value(mValue[at - pos - 1]);
		else
			mValue[at - 1].value(0);
	}
	return (*this);
}

Qbinary Qbinary::operator>>(size_t pos) const
{
	Qbinary result(*this);
	result >>= pos;
	return result;
}

Qbinary& Qbinary::operator>>=(size_t pos)
{
	size_t size = mValue.size();
	for (size_t at = 0; at < size; at++)
	{
		if (at > size - pos)
			mValue[at].value(mValue[at + pos]);
		else
			mValue[at].value(0);
	}
	return (*this);
}

Qbinary Qbinary::operator<<(QbitV value) const
{
	Qbinary result(*this);
	result <<= value;
	return result;
}

Qbinary& Qbinary::operator<<=(QbitV value)
{
	(*this) <<= size_t(1);
	mValue[0].value(value);
	return (*this);
}

Qbinary Qbinary::operator>>(QbitV value) const
{
	Qbinary result(*this);
	result >>= value;
	return result;
}

Qbinary& Qbinary::operator>>=(QbitV value)
{
	(*this) >>= size_t(1);
	mValue[mValue.size() - 1].value(value);
	return (*this);
}

Qbinary& Qbinary::operator=(const Qbinary& right)
{
	mValue = right.mValue;
	id(right.id());
	return(*this);
}

Qbinary& Qbinary::operator&=(const Qbinary& right)
{
	size_t size = nobs(), rSize = right.nobs();
	if (rSize > size)
	{ // resize this object if right has more bits
		resize(rSize);
		size = nobs();
	}
	for (size_t at = 0; at < size; at++)
		if (at < rSize)
			mValue[at] &= right.mValue[at];
		else // when right is smaller all Qbits are 0
			mValue[at].value(0);
	return (*this);
}

Qbinary& Qbinary::operator|=(const Qbinary& right)
{
	size_t size = nobs(), rSize = right.nobs();
	if (rSize > size)
	{ // resize this object if right has more bits
		resize(rSize);
	}
	// if right is smaller apply | operator up to last right Q bit
	for (size_t at = 0; at < rSize; at++)
		mValue[at] |= right.mValue[at];
	return (*this);
}

Qbinary& Qbinary::operator^=(const Qbinary& right)
{
	size_t size = nobs(), rSize = right.nobs();
	if (rSize > size)
	{ // resize this object if right has more bits
		resize(rSize);
		size = nobs();
	}
	for (size_t at = 0; at < size; at++)
		if (at < rSize)
			mValue[at] ^= right.mValue[at];
		else // when right is smaller all Qbits are 0
			mValue[at] ^= Qbit("", 0);
	return (*this);
}

Qexpr<Qbinary> Qbinary::operator~()
{
	Qbinary inverted(*this);
	inverted.id("~" + id());
	size_t size = inverted.nobs();
	for (size_t at = 0; at < size; at++) ~inverted[at];

	Qexpr<Qbinary> expr(inverted != (*this));
	return expr;
}

Qexpr<Qbinary> Qbinary::operator&(const Qbinary& right) const
{
	Qop::Sp pOp = Factory<string, Qop>::Instance().create(AndQT::cMark);
	pOp->arguments({ clone(), right.clone() });

	Qexpr<Qbinary> expr(pOp);
	return expr;
}

Qexpr<Qbinary> Qbinary::operator|(const Qbinary& right) const
{
	Qop::Sp pOp = Factory<string, Qop>::Instance().create(OrQT::cMark);
	pOp->arguments({ clone(), right.clone() });

	Qexpr<Qbinary> expr(pOp);
	return expr;
}

Qexpr<Qbinary> Qbinary::operator^(const Qbinary& right) const
{
	Qop::Sp pOp = Factory<string, Qop>::Instance().create(XorQT::cMark);
	pOp->arguments({ clone(), right.clone() });

	Qexpr<Qbinary> expr(pOp);
	return expr;
}

Qexpr<Qbinary> Qbinary::operator==(const Qbinary& right) const
{
	Qop::Sp pOp = Factory<string, Qop>::Instance().create(EqQT::cMark);
	pOp->arguments({ clone(), right.clone() });

	Qexpr<Qbinary> expr(pOp);
	return expr;
}

Qexpr<Qbinary> Qbinary::operator!=(const Qbinary& right) const
{
	Qop::Sp pOp = Factory<string, Qop>::Instance().create(NeqQT::cMark);
	pOp->arguments({ clone(), right.clone() });

	Qexpr<Qbinary> expr(pOp);
	return expr;
}

Qexpr<Qbinary> Qbinary::operator&(const Qexpr<Qbinary>& right) const
{
	Qop::Sp pOp = Factory<string, Qop>::Instance().create(AndQT::cMark);
	pOp->arguments({ clone(), right.root() });

	Qexpr<Qbinary> expr(pOp);
	return expr;
}

Qexpr<Qbinary> Qbinary::operator|(const Qexpr<Qbinary>& right) const
{
	Qop::Sp pOp = Factory<string, Qop>::Instance().create(OrQT::cMark);
	pOp->arguments({ clone(), right.root() });

	Qexpr<Qbinary> expr(pOp);
	return expr;
}

Qexpr<Qbinary> Qbinary::operator^(const Qexpr<Qbinary>& right) const
{
	Qop::Sp pOp = Factory<string, Qop>::Instance().create(XorQT::cMark);
	pOp->arguments({ clone(), right.root() });

	Qexpr<Qbinary> expr(pOp);
	return expr;
}