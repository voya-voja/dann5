#include <Qbool.h>
#include <Qop.h>
#include <Factory.h>

using namespace dann5::ocean;

Qbool& Qbool::operator=(const Qbool& right)
{
	mValue = right.mValue;
	id(right.id());
	return(*this);
};

Qbool& Qbool::operator&=(const Qbool& right)
{
	if (mValue == cSuperposition || right.mValue == cSuperposition)
		mValue = cSuperposition;
	else if (mValue == cTrue && right.mValue == cTrue)
		mValue = cTrue;
	else
		mValue = cFalse;

	return(*this);
};

Qbool& Qbool::operator|=(const Qbool& right)
{
	if (mValue == cSuperposition || right.mValue == cSuperposition)
		mValue = cSuperposition;
	else if (mValue == cTrue || right.mValue == cTrue)
		mValue = cTrue;
	else
		mValue = cFalse;
	return(*this);
};

Qbool& Qbool::operator^=(const Qbool& right)
{
	if (mValue == cSuperposition || right.mValue == cSuperposition)
		mValue = cSuperposition;
	else if (mValue == right.mValue)
		mValue = cTrue;
	else
		mValue = cFalse;
	return(*this);
};

Qexpr<Qbool> Qbool::operator~()
{
	Qbool inverted("~" + id());
	if (mValue == cTrue)
		inverted.mValue = cFalse;
	else if (mValue == cFalse)
		inverted.mValue = cTrue;

	Qexpr<Qbool> expr = inverted != (*this);
	return expr;
}

Qexpr<Qbool> Qbool::operator&(const Qbool& right) const
{
	Qop::Sp pOp = Factory<string, Qop>::Instance().create(AndQT::cMark);
	pOp->arguments({ clone(), right.clone() });

	Qexpr<Qbool> expr(pOp);
	return expr;
}

Qexpr<Qbool> Qbool::operator|(const Qbool& right) const
{
	Qop::Sp pOp = Factory<string, Qop>::Instance().create(OrQT::cMark);
	pOp->arguments({ clone(), right.clone() });

	Qexpr<Qbool> expr(pOp);
	return expr;
}

Qexpr<Qbool> Qbool::operator^(const Qbool& right) const
{
	Qop::Sp pOp = Factory<string, Qop>::Instance().create(XorQT::cMark);
	pOp->arguments({ clone(), right.clone() });

	Qexpr<Qbool> expr(pOp);
	return expr;
}

Qexpr<Qbool> Qbool::operator==(const Qbool& right) const
{
	Qop::Sp pOp = Factory<string, Qop>::Instance().create(EqQT::cMark);
	pOp->arguments({ clone(), right.clone() });

	Qexpr<Qbool> expr(pOp);
	return expr;
}

Qexpr<Qbool> Qbool::operator!=(const Qbool& right) const
{
	Qop::Sp pOp = Factory<string, Qop>::Instance().create(NeqQT::cMark);
	pOp->arguments({ clone(), right.clone() });

	Qexpr<Qbool> expr(pOp);
	return expr;
}

Qexpr<Qbool> Qbool::operator&(const Qexpr<Qbool>& right) const
{
	Qop::Sp pOp = Factory<string, Qop>::Instance().create(AndQT::cMark);
	pOp->arguments({ clone(), right.root() });

	Qexpr<Qbool> expr(pOp);
	return expr;
}

Qexpr<Qbool> Qbool::operator|(const Qexpr<Qbool>& right) const
{
	Qop::Sp pOp = Factory<string, Qop>::Instance().create(OrQT::cMark);
	pOp->arguments({ clone(), right.root() });

	Qexpr<Qbool> expr(pOp);
	return expr;
}

Qexpr<Qbool> Qbool::operator^(const Qexpr<Qbool>& right) const
{
	Qop::Sp pOp = Factory<string, Qop>::Instance().create(XorQT::cMark);
	pOp->arguments({ clone(), right.root() });

	Qexpr<Qbool> expr(pOp);
	return expr;
}