#include <math.h> 
#include <Qequation.h>
#include <Utility.h>
#include <Qroutine.h>

#include <Logger.h>

using namespace dann5::ocean;

/*** Quantum Equation ***/


Qequation::Qequation()
	: Qstatement(), mResult(), mNoResult(true)
{
	reduct(createReduct());
	_lct(toString());
}

Qequation::Qequation(const Qequation& right)
	: Qstatement(right), mResult(right.mResult), mNoResult(right.mNoResult)
{
	reduct(createReduct());
	reduct().init();
	_lct(toString());
}

Qequation::Qequation(const Qvar& result)
	: Qstatement(result.nobs()), mResult(result), mNoResult(false)
{
	reduct(createReduct());
	reduct().init();
	_lct(toString());
}

Qequation::Qequation(const Qvar& result, const Qexpression& equation, const Qvars& arguments)
	: Qstatement(equation, arguments), mResult(result), mNoResult(false)
{
	// ensure result Q variable and equation Q expression have same number-of-bits
	Index n(nobs()), rn(mResult.nobs());
	if (n != rn)
	{
		if (n < rn)
		{
			expression().resize(rn);
		}
		else
		{
			mResult.resize(n);
		}
	}
	reduct(createReduct());
	reduct().init();	// reduct the equation to prepare 
	_lct(toString());
}

Qequation::~Qequation()
{
	_ldt(toString());
}

Qequation& Qequation::operator=(const Qvar& right)
{
	arguments().clear();
	arguments().push_back(right);
	expression() = right.definition();
	if (nobs() > mResult.nobs())
	{
		mResult.resize(nobs(), 0);
	}
	reduct().init();	// prepare for qubo() call 
	return(*this);
}

Qequation& Qequation::operator=(const Qequation& right)
{
	if(mNoResult)
	{
		mResult = right.mResult;
		mNoResult = false;
	}
	if (right.nobs() > mResult.nobs())
	{
		mResult.resize(right.nobs(), 0);
	}
	Qstatement::operator=(right);
	return(*this);
}

Qequation Qequation::operator&(const Qvar& right) const
{
	Qequation result(*this);
	result &= right;
	return(result);
}

Qequation Qequation::operator&(const Qequation& right) const
{
	Qequation result(*this);
	result &= right;
	return(result);
}

Qequation& Qequation::operator&=(const Qvar& right)
{
	arguments().push_back(right);
	Index size = expression().nobs();
	if (size == 0)
	{
		expression() = Qexpression(right.definition());
	}
	else
	{
		expression() &= right.definition();
	}
	if (nobs() > mResult.nobs())
		mResult.resize(nobs(), 0);
	reduct().init();	// prepare for qubo() call 
	_lat("&=\n", toString(true));
	return(*this);
}

Qequation& Qequation::operator&=(const Qequation& right)
{
	if (mNoResult)
	{
		mResult = right.mResult;
		mNoResult = false;
	}
	else if (!right.mNoResult)
		mResult &= right.mResult;
	arguments().insert(arguments().end(), right.arguments().begin(), right.arguments().end());
	expression() &= right.expression();
	if (nobs() > mResult.nobs())
		mResult.resize(nobs(), 0);
	reduct().init();	// prepare for qubo() call 
	_lat("&= Reducted: \n", toString(true));
	return(*this);
}

Qequation Qequation::operator|(const Qvar& right) const
{
	Qequation result(*this);
	result |= right;
	return(result);
}

Qequation Qequation::operator|(const Qequation& right) const
{
	Qequation result(*this);
	result |= right;
	return(result);
}

Qequation& Qequation::operator|=(const Qvar& right)
{
	arguments().push_back(right);
	Index size = expression().nobs();
	if (size == 0)
	{
		expression() = Qexpression(right.definition());
	}
	else
	{
		expression() |= right.definition();
	}
	if (nobs() > mResult.nobs())
		mResult.resize(nobs(), 0);
	reduct().init();	// prepare for qubo() call 
	_lat("|=\n", toString(true));
	return(*this);
}

Qequation& Qequation::operator|=(const Qequation& right)
{
	if (mNoResult)
	{
		mResult = right.mResult;
		mNoResult = false;
	}
	else if (!right.mNoResult)
		mResult |= right.mResult;
	arguments().insert(arguments().end(), right.arguments().begin(), right.arguments().end());
	expression() |= right.expression();
	if (nobs() > mResult.nobs())
		mResult.resize(nobs(), 0);
	reduct().init();	// prepare for qubo() call 
	_lat("|= Reducted: \n", toString(true));
	return(*this);
}

Qequation Qequation::nand(const Qvar& right) const
{
	Qequation result(*this);
	result.nandMutable(right);
	return(result);
}

Qequation Qequation::nand(const Qequation& right) const
{
	Qequation result(*this);
	result.nandMutable(right);
	return(result);
}

Qequation& Qequation::nandMutable(const Qvar& right)
{
	arguments().push_back(right);
	Index size = expression().nobs();
	if (size == 0)
	{
		expression() = Qexpression(right.definition());
	}
	else
	{
		expression().nandMutable(right.definition());
	}
	if (nobs() > mResult.nobs())
		mResult.resize(nobs(), 0);
	reduct().init();	// prepare for qubo() call 
	_lat("~&=\n", toString(true));
	return(*this);
}

Qequation& Qequation::nandMutable(const Qequation& right)
{
	if (mNoResult)
	{
		mResult = right.mResult;
		mNoResult = false;
	}
	else if (!right.mNoResult)
		mResult.nandMutable(right.mResult);
	arguments().insert(arguments().end(), right.arguments().begin(), right.arguments().end());
	expression().nandMutable(right.expression());
	if (nobs() > mResult.nobs())
		mResult.resize(nobs(), 0);
	reduct().init();	// prepare for qubo() call 
	_lat("~&= Reducted: \n", toString(true));
	return(*this);
}

Qequation Qequation::nor(const Qvar& right) const
{
	Qequation result(*this);
	result.norMutable(right);
	return(result);
}

Qequation Qequation::nor(const Qequation& right) const
{
	Qequation result(*this);
	result.norMutable(right);
	return(result);
}

Qequation& Qequation::norMutable(const Qvar& right)
{
	arguments().push_back(right);
	Index size = expression().nobs();
	if (size == 0)
	{
		expression() = Qexpression(right.definition());
	}
	else
	{
		expression().norMutable(right.definition());
	}
	if (nobs() > mResult.nobs())
		mResult.resize(nobs(), 0);
	reduct().init();	// prepare for qubo() call 
	_lat("~|=\n", toString(true));
	return(*this);
}

Qequation& Qequation::norMutable(const Qequation& right)
{
	if (mNoResult)
	{
		mResult = right.mResult;
		mNoResult = false;
	}
	else if (!right.mNoResult)
		mResult.norMutable(right.mResult);
	arguments().insert(arguments().end(), right.arguments().begin(), right.arguments().end());
	expression().norMutable(right.expression());
	if (nobs() > mResult.nobs())
		mResult.resize(nobs(), 0);
	reduct().init();	// prepare for qubo() call 
	_lat("~|= Reducted: \n", toString(true));
	return(*this);
}

Qequation Qequation::operator^(const Qvar& right) const
{
	Qequation result(*this);
	result ^= right;
	return(result);
}

Qequation Qequation::operator^(const Qequation& right) const
{
	Qequation result(*this);
	result ^= right;
	return(result);
}

Qequation& Qequation::operator^=(const Qvar& right)
{
	arguments().push_back(right);
	Index size = expression().nobs();
	if (size == 0)
	{
		expression() = Qexpression(right.definition());
	}
	else
	{
		expression() ^= right.definition();
	}
	if (nobs() > mResult.nobs())
		mResult.resize(nobs(), 0);
	reduct().init();	// prepare for qubo() call 
	_lat("^=\n", toString(true));
	return(*this);
}

Qequation& Qequation::operator^=(const Qequation& right)
{
	if (mNoResult)
	{
		mResult = right.mResult;
		mNoResult = false;
	}
	else if (!right.mNoResult)
		mResult ^= right.mResult;
	arguments().insert(arguments().end(), right.arguments().begin(), right.arguments().end());
	expression() ^= right.expression();
	if (nobs() > mResult.nobs())
		mResult.resize(nobs(), 0);
	reduct().init();	// prepare for qubo() call 
	_lat("^= Reducted: \n", toString(true));
	return(*this);
}

Qequation Qequation::operator+(const Qvar& right) const
{
	Qequation result(*this);
	result += right;
	return(result);
}

Qequation Qequation::operator+(const Qequation& right) const
{
	Qequation result(*this);
	result += right;
	return(result);
}

Qequation& Qequation::operator+=(const Qvar& right)
{
	arguments().push_back(right);
	Index size = expression().nobs();
	if (size == 0)
	{
		expression() = Qexpression(right.definition());
	}
	else
	{	
		expression() += right.definition();
	}
	if (nobs() > mResult.nobs())
		mResult.resize(nobs(), 0);
	reduct().init();	// prepare for qubo() call 
	_lat( "+=\n", toString(true) );
	return(*this);
}

Qequation& Qequation::operator+=(const Qequation& right)
{
	if (mNoResult)
	{
		mResult = right.mResult;
		mNoResult = false;
	}
	else if (!right.mNoResult)
		mResult += right.mResult;
	arguments().insert(arguments().end(), right.arguments().begin(), right.arguments().end());
	expression() += right.expression();
	if (nobs() > mResult.nobs())
		mResult.resize(nobs(), 0);
	reduct().init();	// prepare for qubo() call 
	_lat("+= Reducted: \n", toString(true));
	return(*this);
}

Qroutine Qequation::operator-(const Qvar& subtrahend) const
{
	// define difference variable with the same size as subtrahend argument
	Qvar difference(subtrahend.definition().size(), "dfrnc");

	// for this.result as minuend and subtrahend argment: difference = minuend - subtrahend
	// create Q equation: minuend = difference + subtrahend
	Qequation minuend(mResult);
	minuend = difference + subtrahend;

	// create subtraction routine
	string rName = "dfrnc = " + mResult.definition().name() + " - " + subtrahend.definition().name();
	Qroutine subtraction(nobs(), rName);
	subtraction << (*this) << minuend;
	return subtraction;
}

Qequation Qequation::operator*(const Qvar& right) const
{
	Qequation result(*this);
	result *= right;
	return(result);
}

Qequation Qequation::operator*(const Qequation& right) const
{
	Qequation result(*this);
	result *= right;
	return(result);
}

Qequation& Qequation::operator*=(const Qvar& right)
{
	arguments().push_back(right);
	Index size = expression().nobs();
	if (size == 0)
	{
		expression() = Qexpression(right.definition());
	}
	else
	{
		expression() *= right.definition();
	}
	if (nobs() > mResult.nobs())
		mResult.resize(nobs(), 0);
	reduct().init();	// prepare for qubo() call 
	_lat("+=\n", toString(true));
	return(*this);
}

Qequation& Qequation::operator*=(const Qequation& right)
{
	if (mNoResult)
	{
		mResult = right.mResult;
		mNoResult = false;
	}
	else if (!right.mNoResult)
		mResult *= right.mResult;
	arguments().insert(arguments().end(), right.arguments().begin(), right.arguments().end());
	expression() *= right.expression();
	if (nobs() > mResult.nobs())
		mResult.resize(nobs(), 0);
	reduct().init();	// prepare for qubo() call 
	_lat("+= Reducted: \n", toString(true));
	return(*this);
}

Qroutine Qequation::operator/(const Qvar& divisor) const
{
	// define quatient variable with same size as divisor 
	Qvar quotient(divisor.definition().size(), "qtnt");

	// for this.result as dividend and divisor argment: quotient = dividend / divisor
	// create Q equation: dividend = quotient * divisor
	Qequation dividend(mResult);
	dividend = quotient * divisor;

	// create multiplication routine
	string rName = "qtnt = " + mResult.definition().name() + " / " + divisor.definition().name();
	Qroutine multiplication(nobs(), rName);
	multiplication << (*this) << dividend;
	return multiplication;
}

string Qequation::toString(bool decomposed, Index level) const
{
	bool iterateAll = level == Eigen::Infinity;
	Index last = (iterateAll ? nobs() - 1 : level);
	string sEquation = "";
	vector<pair<string, string> > reduces;
	for (Index at = 0; at <= last; at++)
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
		string resultS = mResult.definition()(at)->identity();
		string carryExpression = Qaddition::Carry::Symbol(exprStr);
		string carryResultSymbol = Qaddition::Carry::Symbol(resultS);
		reduces.push_back(pair<string, string>(carryExpression, carryResultSymbol));

		// add a string representing an equation for the current Qbit
		string resultV = to_string(QbitV(mResult.value()(at)));
		if(iterateAll || (!iterateAll && at == level))
			sEquation += "| " + resultS + " = " + resultV + " |" + " = " + exprStr + "\n";
	}
	return sEquation;
}

ostream& dann5::ocean::operator<<(std::ostream& out, const Qequation& statement)
{
	out << statement.toString();
	return out;
}


/*** Qequation::Reduct code***/
Qequation::Reduct::Reduct(Qequation& equation)
	:Qstatement::Reduct(equation), mEquation(equation)
{
	_lc;
}

void Qequation::Reduct::init()
{
	Qstatement::Reduct::init();

	string definition(""), expression("");
	for (Index at = 0; at < mEquation.nobs(); at++)
	{
		// capture result expression and reduction
		Qoperand::Sp pOperand = mEquation.expression()(at);
		if (pOperand != nullptr)
		{
			expression = pOperand->toString(false);
			definition = as_const(mEquation.mResult).definition()(at)->identity();
			Qbit value = as_const(mEquation.mResult).value()(at);
			Reduction reduction(definition, value);
			reductions()[expression] = reduction;

			// capture expression of carry forward operands in expression
			expression = Qaddition::Carry::Symbol(expression);
			definition = Qaddition::Carry::Symbol(definition);
			value = Qbit::cSuperposition;
			Reduction carryCorrect(definition, value);
			reductions()[expression] = carryCorrect;
		}
	}
}
