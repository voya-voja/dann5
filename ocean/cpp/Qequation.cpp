#include <math.h> 
#include <Qequation.h>
#include <Utility.h>
#include <Qroutine.h>

#include <Logger.h>

using namespace dann5::ocean;

/*** Quantum Equation ***/


Qequation::Qequation()
	: Qstatement(), mResult(), mReduct(*this), mNoResult(true)
{
	_lct(toString());
}

Qequation::Qequation(const Qequation& right)
	: Qstatement(right), mResult(right.mResult), mReduct(*this), mNoResult(right.mNoResult)
{
	mReduct();
	_lct(toString());
}

Qequation::Qequation(const Qvar& result)
	: Qstatement(result.nobs()), mResult(result), mReduct(*this), mNoResult(false)
{
	_lct(toString());
}

Qequation::Qequation(const Qvar& result, const Qexpression& equation, const Qvars& arguments)
	: Qstatement(equation, arguments), mResult(result), mReduct(*this), mNoResult(false)
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
	mReduct();	// reduct the equation to prepare 
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
		mResult.resize(nobs(), 0);
	mReduct();	// prepare for qubo() call 
	return(*this);
}

Qequation& Qequation::operator=(const Qequation& right)
{
	if(mNoResult)
	{
		mResult = right.mResult;
		mNoResult = false;
	}
	arguments().clear();
	arguments().insert(arguments().end(), right.arguments().begin(), right.arguments().end());
	expression() = right.expression();
	if (nobs() > mResult.nobs())
		mResult.resize(nobs(), 0);
	mReduct();	// prepare for qubo() call 
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
	mReduct();	// prepare for qubo() call 
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
	mReduct();	// prepare for qubo() call 
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
	mReduct();	// prepare for qubo() call 
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
	mReduct();	// prepare for qubo() call 
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
	mReduct();	// prepare for qubo() call 
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
	mReduct();	// prepare for qubo() call 
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
	mReduct();	// prepare for qubo() call 
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
	mReduct();	// prepare for qubo() call 
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
	mReduct();	// prepare for qubo() call 
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
	mReduct();	// prepare for qubo() call 
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
	mReduct();	// prepare for qubo() call 
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
	mReduct();	// prepare for qubo() call 
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
	mReduct();	// prepare for qubo() call 
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
	mReduct();	// prepare for qubo() call 
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

Qubo Qequation::qubo(bool finalized, Index level) const
{
	bool iterateAll = level == Eigen::Infinity;
	Index last = (iterateAll ? nobs() - 1 : level);
	Qubo rawQubo;
	for (Index at = 0; at <= last; at++)
	{
		if(iterateAll || (!iterateAll && at == level))
		{
			Qubo qubo = expression()(at)->qubo(finalized);
			rawQubo += qubo;
//			cout << endl << expression()(at)->toString();
//			cout << endl << qubo << endl << rawQubo << endl;
		}
	}

	Qubo qubo;	// Updated Qubo
	for (auto at = rawQubo.begin(); at != rawQubo.end(); at++)
	{
		// remove all quadratic elements with bias 0
		if (((*at).first.first != (*at).first.second) && ((*at).second == 0))
			continue;

		QuboKey key = mReduct((*at).first, finalized);
		if(key != Reduct::cSkip)
		{
			// Using a correct key add bias for the buinary quadratic element in updated Qubo 
			qubo[key] += (*at).second;
		}
	}
	return(qubo);	// return updated Qubo
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
		string resultV = to_string(q_bit(mResult.value()(at)));
		if(iterateAll || (!iterateAll && at == level))
			sEquation += "| " + resultS + " = " + resultV + " |" + " = " + exprStr + "\n";
	}
	return sEquation;
}

void Qequation::add(Sample& sample)
{
	mSolutions.push_back(sample);
}

void Qequation::set(Samples& samples)
{
	_lat("solutions", to_string(samples.size()) + " of samples");
	mSolutions = samples;
}

string Qequation::solutions() const
{
	string values("");
	for (auto sample : mSolutions)
	{
		const Qvars& args = arguments();
		for (auto arg : args)
		{
			Qdef definition = as_const(arg).definition();
			Qwhole value(as_const(arg).value().nobs());
			int atBit = 0;
			for (auto bitSymbol : definition)
				value[atBit++] = sample[bitSymbol->identity()];
			values += definition.name() + " = " + value.toString() + "; ";
		}
		values += "\n";
	}
	_lat("solutions", values);
	return(values);
}

ostream& dann5::ocean::operator<<(std::ostream& out, const Qequation& statement)
{
	out << statement.toString();
	return out;
}


/*** Qequation::Reduct code***/
const QuboKey Qequation::Reduct::cSkip("skip", "skip");

Qequation::Reduct::Reduct(Qequation& equation)
	:mEquation(equation)
{
	_lc;
}

Qequation::Reduct::~Reduct()
{
	_ld;
}

void Qequation::Reduct::operator() ()
{
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
			mReductions[expression] = reduction;

			// capture expression of carry forward operands in expression
			expression = Qaddition::Carry::Symbol(expression);
			definition = Qaddition::Carry::Symbol(definition);
			value = Qbit::cSuperposition;
			Reduction carryCorrect(definition, value);
			mReductions[expression] = carryCorrect;
		}
	}

	// capture reductions for arguments with values different from cSuperposition (i.e. 0 or 1)
	for (auto atArg = mEquation.arguments().cbegin(); atArg != mEquation.arguments().cend(); atArg++)
	{
		for (Index at = 0; at < (*atArg).definition().nobs(); at++)
		{
			Qbit value = (*atArg).value()(at);
			if (value < 2)
			{
				string definition = (*atArg).definition()(at)->identity();
				Reduction reduction(definition, value);
				mReductions[definition] = reduction;
			}
		}
	}
}

QuboKey Qequation::Reduct::operator() (const QuboKey& original, bool finalized) const
{
	QuboKey key = original;
	Reductions::const_iterator found1stAt = mReductions.find(original.first);
	Reductions::const_iterator found2ndAt = mReductions.find(original.second);
	Reductions::const_iterator end = mReductions.cend();
	if (found1stAt != end)
	{
		// The 1st half of a key of a quadratic element  should be updated with replacement definition
		Reduction correct1st = (*found1stAt).second;
		if (found2ndAt != end)
		{
			// The 2nd half of a key of a quadratic element should be updated with replacement definition
			Reduction correct2nd = (*found2ndAt).second;
			if (!finalized || (correct1st.second == Qbit::cSuperposition && correct2nd.second == Qbit::cSuperposition))
			{
				// Always just replace 1st and 2nd symbols without applying condition
				// or replace 1st and 2nd symbols in binaryquadratic element when condition values are not defined (not 0 or 1)
				key = QuboKey(correct1st.first, correct2nd.first);
			}
			else if (correct1st.second == Qbit::cSuperposition && correct2nd.second == 1)
			{
				// otherwise replace with a linear element using 1st replacement when its condition value is 1
				key = QuboKey(correct1st.first, correct1st.first);
			}
			else if (correct1st.second == 1 && correct2nd.second == Qbit::cSuperposition)
			{
				// otherwise replace with a linear element using 2nd replacement when its condition value is 1
				key = QuboKey(correct2nd.first, correct2nd.first);
			}
			else
			{
				// all other combinations should be skipped as result is 0 or 1 constant
				key = cSkip;
			}
		}
		else
		{
			// Just 1st half of a key of a quadratic element should be updated with replacement definition
			if (!finalized || correct1st.second == Qbit::cSuperposition)
			{
				// Always just replace 1st definition without applying condition
				// or replace 1st definition in binaryquadratic element when condition value is not defined (not 0 or 1)
				key = QuboKey(correct1st.first, original.second);
			}
			else if (correct1st.second == 1)
			{
				// otherwise replace with a linear element using 2nd half of a key when condition value is 1
				key = QuboKey(original.second, original.second);
			}
			else
			{
				// all other combinations should be removed as result is 0
				key = cSkip;
			}
		}
	}
	else if (found2ndAt != end)
	{
		// Just 2st half of a key of a quadratic element should be updated with replacement definition
		Reduction reduction = (*found2ndAt).second;
		if (!finalized || reduction.second == Qbit::cSuperposition)
		{
			key = QuboKey(original.first, reduction.first);
		}
		else if (reduction.second == 1)
		{
			// otherwise replace with a linear element using 1st half of a key when condition value is 1
			key = QuboKey(original.first, original.first);
		}
		else
		{
			// all other combinations should be removed as result is 0
			key = cSkip;
		}
	}
	return key;
}