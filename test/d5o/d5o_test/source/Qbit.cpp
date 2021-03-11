#include <Qbit.h>
#include <Qop.h>
#include <Factory.h>

using namespace dann5::ocean;

Qbit& Qbit::operator=(const Qbit& right)
{
	mValue = right.mValue;
	id(right.id());
	return(*this);
};

Qbit& Qbit::operator&=(const Qbit& right)
{
	if (mValue == cSuperposition || right.mValue == cSuperposition)
		mValue = cSuperposition;
	else
		mValue &= right.mValue;
	return(*this);
};

Qbit& Qbit::operator|=(const Qbit& right)
{
	if (mValue == cSuperposition || right.mValue == cSuperposition)
		mValue = cSuperposition;
	else
		mValue |= right.mValue;
	return(*this);
};

Qbit& Qbit::operator^=(const Qbit& right)
{
	if (mValue == cSuperposition || right.mValue == cSuperposition)
		mValue = cSuperposition;
	else
		mValue ^= right.mValue;
	return(*this);
};


Qexpr<Qbit> Qbit::operator~()
{
	Qbit inverted("~" + id());
	if (mValue != cSuperposition)
		inverted.mValue = ~mValue;

	Qexpr<Qbit> expr = inverted != (*this);
	return expr;
}

Qexpr<Qbit> Qbit::operator&(const Qbit& right) const
{
	Qop::Sp pOp = Factory<string, Qop>::Instance().create(AndQT::cMark);
	pOp->arguments({ clone(), right.clone() });

	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::operator|(const Qbit& right) const
{
	Qop::Sp pOp = Factory<string, Qop>::Instance().create(OrQT::cMark);
	pOp->arguments({ clone(), right.clone() });

	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::operator^(const Qbit& right) const
{
	Qop::Sp pOp = Factory<string, Qop>::Instance().create(XorQT::cMark);
	pOp->arguments({ clone(), right.clone() });

	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::operator==(const Qbit& right) const
{
	Qop::Sp pOp = Factory<string, Qop>::Instance().create(EqQT::cMark);
	pOp->arguments({ clone(), right.clone() });

	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::operator!=(const Qbit& right) const
{
	Qop::Sp pOp = Factory<string, Qop>::Instance().create(NeqQT::cMark);
	pOp->arguments({ clone(), right.clone() });

	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::operator>(const Qbit& right) const
{
	Qop::Sp pOp = Factory<string, Qop>::Instance().create(GtQT::cMark);
	pOp->arguments({ clone(), right.clone() });

	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::operator>=(const Qbit& right) const
{
	Qop::Sp pOp = Factory<string, Qop>::Instance().create(GeQT::cMark);
	pOp->arguments({ clone(), right.clone() });

	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::operator<(const Qbit& right) const
{
	Qop::Sp pOp = Factory<string, Qop>::Instance().create(LtQT::cMark);
	pOp->arguments({ clone(), right.clone() });

	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::operator<=(const Qbit& right) const
{
	Qop::Sp pOp = Factory<string, Qop>::Instance().create(LeQT::cMark);
	pOp->arguments({ clone(), right.clone() });

	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::operator&(const Qexpr<Qbit>& right) const
{
	Qop::Sp pOp = Factory<string, Qop>::Instance().create(AndQT::cMark);
	pOp->arguments({ clone(), right.root() });

	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::operator|(const Qexpr<Qbit>& right) const
{
	Qop::Sp pOp = Factory<string, Qop>::Instance().create(OrQT::cMark);
	pOp->arguments({ clone(), right.root() });

	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::operator^(const Qexpr<Qbit>& right) const
{
	Qop::Sp pOp = Factory<string, Qop>::Instance().create(XorQT::cMark);
	pOp->arguments({ clone(), right.root() });

	Qexpr<Qbit> expr(pOp);
	return expr;
}
/*
Qexpr<Qbit> Qbit::operator==(const Qexpr<Qbit>& right) const
{

}

Qexpr<Qbit> Qbit::operator!=(const Qexpr<Qbit>& right) const
{

}

Qexpr<Qbit> Qbit::operator>(const Qexpr<Qbit>& right) const
{

}

Qexpr<Qbit> Qbit::operator>=(const Qexpr<Qbit>& right) const
{

}

Qexpr<Qbit> Qbit::operator<(const Qexpr<Qbit>& right) const
{

}

Qexpr<Qbit> Qbit::operator<=(const Qexpr<Qbit>& right) const
{

}
*/