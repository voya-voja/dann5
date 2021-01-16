#include <math.h> 
#include <Qcondition.h>
#include <Utility.h>

#include <Logger.h>

using namespace dann5::ocean;

/*** QuboEquation ***/


Qcondition::Qcondition()
	: Qstatement(), mAction("")
{
	_lct(toString());
}


Qcondition::Qcondition(const Qcondition& right)
	: Qstatement(right), mAction(right.mAction)
{
	_lct(toString());
}

Qcondition::Qcondition(const Qrutine& action)
	: Qstatement(), mAction(action)
{
	_lct(toString());
}

Qcondition::Qcondition(const Qrutine& action, const Qexpression& equation, const Qvars& arguments)
	: Qstatement(equation, arguments), mAction(action)
{
	_lct(toString());
}

Qcondition::~Qcondition()
{
	_ldt(toString());
}

Qcondition& Qcondition::operator=(const Qcondition& right)
{
	mAction = right.mAction;
	arguments().clear();
	arguments().insert(arguments().end(), right.arguments().begin(), right.arguments().end());
	expression() = right.expression();
	Index size = expression().rows();
	return(*this);
}

Qcondition& Qcondition::operator<<(const Qstatement& right)
{
	mAction << right;
	return(*this);
}

Qcondition& Qcondition::operator<<(const Qrutine& right)
{
	mAction << right;
	return(*this);
}

Qcondition& Qcondition::operator~()
{
	return(*this);
}

Qcondition Qcondition::operator&(const Qcondition& right) const
{
	Qcondition result(*this);
	result &= right;
	return(result);
}

Qcondition& Qcondition::operator&=(const Qcondition& right)
{
	_lat("+=\n", toString(true));
	return(*this);
}

Qcondition Qcondition::operator|(const Qcondition& right) const
{
	Qcondition result(*this);
	result |= right;
	return(result);
}

Qcondition& Qcondition::operator|=(const Qcondition& right)
{
	_lat("+=\n", toString(true));
	return(*this);
}

Qcondition Qcondition::operator^(const Qcondition& right) const
{
	Qcondition result(*this);
	result |= right;
	return(result);
}

Qcondition& Qcondition::operator^=(const Qcondition& right)
{
	_lat("+=\n", toString(true));
	return(*this);
}

Qcondition Qcondition::nand(const Qcondition& right) const
{
	Qcondition result(*this);
	result |= right;
	return(result);
}

Qcondition& Qcondition::nand(const Qcondition& right)
{
	_lat("+=\n", toString(true));
	return(*this);
}

Qcondition Qcondition::nor(const Qcondition& right) const
{
	Qcondition result(*this);
	result |= right;
	return(result);
}

Qcondition& Qcondition::nor(const Qcondition& right)
{
	_lat("+=\n", toString(true));
	return(*this);
}

Qubo Qcondition::qubo(bool finalized, Index level) const
{
	bool iterateAll = level == Eigen::Infinity;
	Index last = (iterateAll ? size() - 1 : level);
	Qubo rawBqm;
	for (Index at = 0; at <= last; at++)
		if (iterateAll || (!iterateAll && at == level))
			rawBqm += expression()(at)->qubo(finalized);

	Qubo qubo;	// Updated Qubo
	for (auto at = rawBqm.begin(); at != rawBqm.end(); at++)
	{
		// remove all quadratic elements with bias 0
		if (((*at).first.first != (*at).first.second) && ((*at).second == 0))
			continue;

/*		QuboKey key = mReduct((*at).first, ready);
		if (key != Reduct::cSkip)
		{
			// Using a correct key add bias for the buinary quadratic element in updated Qubo 
			qubo[key] += (*at).second;
		}
*/	}
	return(qubo);	// return updated Qubo
}

string Qcondition::toString(bool decomposed, Index level) const
{
	bool iterateAll = level == Eigen::Infinity;
	Index last = (iterateAll ? size() - 1 : level);
	string sEquation = "";
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

		// reduce all previous carry expressions
		for (auto reduce : reduces)
		{
			replaceAll(exprStr, reduce.first, reduce.second);
		}
		// prepare a reduce of the cary expression definition with carry result definition for a current Qbit
/*		string resultS = mResult.definition()(at)->identity();
		string carryExpression = Qaddition::Carry::Symbol(exprStr);
		string carryResultSymbol = Qaddition::Carry::Symbol(resultS);
		reduces.push_back(pair<string, string>(carryExpression, carryResultSymbol));

		// add a string representing an equation for the current Qbit
		string resultV = to_string(q_bit(mResult.value()(at)));
		if(iterateAll || (!iterateAll && at == level))
			sEquation += "| " + resultS + " = " + resultV + " |" + " = " + exprStr + "\n";
*/
	}
	return sEquation;
}


ostream& dann5::ocean::operator<<(std::ostream& out, const Qcondition& statement)
{
	out << statement.toString();
	return out;
}