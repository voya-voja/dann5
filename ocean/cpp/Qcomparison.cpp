#include <math.h> 
#include <Qcomparison.h>
#include <Factory.h>
#include <Qubo.h>

#include <Logger.h>

using namespace dann5::ocean;

/*** Q comparison ***/


Qcomparison::Qcomparison()
	: Qstatement()
{
	_lct(toString());
}


Qcomparison::Qcomparison(const Qcomparison& right)
	: Qstatement(right)
{
	_lct(toString());
}

Qcomparison::Qcomparison(const Qexpression& equation, const Qvars& arguments)
	: Qstatement(equation, arguments)
{
	_lct(toString());
}

Qcomparison::~Qcomparison()
{
	_ldt(toString());
}

Qcomparison& Qcomparison::operator=(const Qcomparison& right)
{
	Qstatement::operator=(right);
	return(*this);
}

Qcomparison& Qcomparison::operator~()
{
	return(*this);
}

Qcomparison Qcomparison::operator&(const Qcomparison& right) const
{
	Qcomparison result(*this);
	result &= right;
	return(result);
}

Qcomparison& Qcomparison::operator&=(const Qcomparison& right)
{
	_lat("+=\n", toString(true));
	return(*this);
}

Qcomparison Qcomparison::operator|(const Qcomparison& right) const
{
	Qcomparison result(*this);
	result |= right;
	return(result);
}

Qcomparison& Qcomparison::operator|=(const Qcomparison& right)
{
	_lat("+=\n", toString(true));
	return(*this);
}

Qcomparison Qcomparison::operator^(const Qcomparison& right) const
{
	Qcomparison result(*this);
	result |= right;
	return(result);
}

Qcomparison& Qcomparison::operator^=(const Qcomparison& right)
{
	_lat("+=\n", toString(true));
	return(*this);
}

Qcomparison Qcomparison::nand(const Qcomparison& right) const
{
	Qcomparison result(*this);
	result |= right;
	return(result);
}

Qcomparison& Qcomparison::nand(const Qcomparison& right)
{
	_lat("+=\n", toString(true));
	return(*this);
}

Qcomparison Qcomparison::nor(const Qcomparison& right) const
{
	Qcomparison result(*this);
	result |= right;
	return(result);
}

Qcomparison& Qcomparison::nor(const Qcomparison& right)
{
	_lat("+=\n", toString(true));
	return(*this);
}

Qubo Qcomparison::createRawQubo(bool finalized, Index level) const
{
/*
	bool iterateAll = level == Eigen::Infinity;
	Index last = (iterateAll ? nobs() - 1 : level);
	Qoperand::Sp pOperand = nullptr;
	const Qexpression& expr = expression();
	if (iterateAll || (!iterateAll && 0 == level))
	{
		pOperand = expression()(0);
	}
	for (Index at = 1; at <= last; at++)
	{
		if (iterateAll || (!iterateAll && at == level))
		{
			Qop::Sp pOp = Factory<string, Qop>::Instance().create(XorQT::cMark);
			Qoperands args;
			args.push_back(pOperand);
			args.push_back(expr(at));
			pOp->arguments(args);
			pOperand = pOp;
		}
	}
	Qubo rawQubo;
	if(pOperand != nullptr)
		rawQubo = pOperand->qubo(finalized);
	return rawQubo;
*/	return Qstatement::createRawQubo(finalized, level);
}

string Qcomparison::toString(bool decomposed, Index level) const
{
	bool iterateAll = level == Eigen::Infinity;
	Index last = (iterateAll ? nobs() - 1 : level);
	string sCondition = "";
	vector<pair<string, string> > reduces;
	for (Index at = 0; at <=  last; at++)
	{
		Qoperand::Sp pOperand = expression()(at);
		string exprStr = "";
		Qop::Sp pOp = nullptr;
		if (decomposed)
			pOp = dynamic_pointer_cast<Qop>(pOperand);
		// if the operand is an operation, create its and its operands' labels
		if (pOp != nullptr)
			pOp->labelFor(to_string(at));

		if (pOperand != nullptr)
			exprStr = pOperand->toString(decomposed);

		if (pOp != nullptr)
			pOp->unlabel();	// remove label as formula can change
		// add a string representing an condition for the current Qbit
		if (iterateAll || (!iterateAll && at == level))
		{
			if (at > 0) 
				sCondition += "; ";
			sCondition += exprStr;
		}
	}
	return sCondition;
}


ostream& dann5::ocean::operator<<(std::ostream& out, const Qcomparison& statement)
{
	out << statement.toString();
	return out;
}