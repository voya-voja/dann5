#include <math.h> 
#include <Qequation.h>
#include <Utility.h>

#include <Logger.h>

using namespace dann5::ocean;

/*** QuboEquation ***/


Qequation::Qequation()
	: mResult(), mExpression(0), mReduct(*this), mNoResult(true)
{
	_lct(toString());
}

Qequation::Qequation(const Qequation& right)
	:mResult(right.mResult), mArguments(right.mArguments), 
	mExpression(right.mExpression), mReduct(*this), mNoResult(right.mNoResult)
{
	_lct(toString());
}

Qequation::Qequation(const Qvar& result)
	: mResult(result), mExpression(result.symbol().size()), mReduct(*this), mNoResult(false)
{
	_lct(toString());
}

Qequation::Qequation(const Qvar& result, const Qexpression& equation, const Qvars& arguments)
	: mResult(result), mExpression(equation), mArguments(arguments), mReduct(*this), mNoResult(false)
{
	Index size = mExpression.rows();
	if (size > mResult.symbol().rows())
		mResult.resize(size);
	mReduct();
	_lct(toString());
}

Qequation::~Qequation()
{
	_ldt(toString());
}

Qequation& Qequation::operator=(const Qequation& right)
{
	if(mNoResult)
	{
		mResult = right.mResult;
		mNoResult = false;
	}
	mArguments.clear();
	mArguments.insert(mArguments.end(), right.mArguments.begin(), right.mArguments.end());
	mExpression = right.mExpression;
	Index size = mExpression.rows();
	if (size > mResult.symbol().size())
		mResult.resize(size, 0);
	mReduct();
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
	mArguments.push_back(right);
	Index size = mExpression.rows();
	if (size == 0)
	{
		mExpression = Qexpression(right.symbol());
	}
	else
	{	
		mExpression += right.symbol();
	}
	size = mExpression.rows();
	if (size > mResult.symbol().size())
		mResult.resize(size, 0);
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
	mArguments.insert(mArguments.end(), right.mArguments.begin(), right.mArguments.end());
	mExpression += right.mExpression;
	Index size = mExpression.rows();
	if (size > mResult.symbol().size())
		mResult.resize(size, 0);
	_lat("+=\n", toString(true));
	mReduct();
	_lat("+= Reducted: \n", toString(true));
	return(*this);
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
	mArguments.push_back(right);
	Index size = mExpression.rows();
	if (size == 0)
	{
		mExpression = Qexpression(right.symbol());
	}
	else
	{
		mExpression *= right.symbol();
	}
	size = mExpression.rows();
	if (size > mResult.symbol().size())
		mResult.resize(size, 0);
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
	mArguments.insert(mArguments.end(), right.mArguments.begin(), right.mArguments.end());
	mExpression *= right.mExpression;
	Index size = mExpression.rows();
	if (size > mResult.symbol().size())
		mResult.resize(size, 0);
	_lat("+=\n", toString(true));
	mReduct();
	_lat("+= Reducted: \n", toString(true));
	return(*this);
}

Qubo Qequation::qubo(bool ready) const
{
	Qubo rawBqm;
	for (Index at = 0; at < size(); at++)
		rawBqm += mExpression(at)->qubo();

	Qubo qubo;	// Updated Qubo
	for (auto at = rawBqm.begin(); at != rawBqm.end(); at++)
	{
		// remove all quadratic elements with bias 0
		if (((*at).first.first != (*at).first.second) && ((*at).second == 0))
			continue;

		QuboKey key = mReduct((*at).first, ready);
		if(key != Reduct::cSkip)
		{
			// Using a correct key add bias for the buinary quadratic element in updated Qubo 
			qubo[key] += (*at).second;
		}
	}
	return(qubo);	// return updated Qubo
}

string Qequation::toString(bool decomposed) const
{
	string sEquation = "";
	vector<pair<string, string> > reduces;
	for (Index at = 0; at < size(); at++)
	{
		Qoperand::Sp pOperand = mExpression(at);
		string exprStr = "";
		Qop::Sp pOp = nullptr;
		if(decomposed) 
			pOp = dynamic_pointer_cast<Qop>(pOperand);
		// if the operand is an operation, create its and its operands' labels
		if (pOp != nullptr) 
			pOp->labelFor(to_string(at));

		if(pOperand != nullptr)
			exprStr = pOperand->toString(decomposed);

		if (pOp != nullptr) 
			pOp->unlabel();	// remove label as formula can change

		// reduce all previous carry expressions
		for(auto reduce : reduces)
		{
			replaceAll(exprStr, reduce.first, reduce.second);
		}
		// prepare a reduce of the cary expression symbol with carry result symbol for a current Qbit
		string resultS = mResult.symbol()(at)->identity();
		string carryExpression = Qaddition::Carry::Symbol(exprStr);
		string carryResultSymbol = Qaddition::Carry::Symbol(resultS);
		reduces.push_back(pair<string, string>(carryExpression, carryResultSymbol));

		// add a string representing an equation for the current Qbit
		string resultV = to_string(mResult.value()(at));
		sEquation += "| " + resultS + " = " + resultV + " |" + " = " + exprStr + "\n";
	}
	return sEquation;
}

string Qequation::solutions() const
{
	string values("");
	for (auto sample : mSolutions)
	{
		for (auto arg : mArguments)
		{
			Qdef symbol = arg.symbol();
			Qint value(arg.value().rows());
			int atBit = 0;
			for (auto bitSymbol : symbol)
				value[atBit++] = sample[bitSymbol->identity()];
			values += symbol.name() + " = " + value.toString() + "; ";
		}
		values += "\n";
	}
	_lat("solutions", values);
	return(values);
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

std::ostream& dann5::ocean::operator<<(std::ostream& out, const Qequation& qe)
{
	out << qe.toString();
	return out;
}


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
	string symbol(""), expression("");

	for (Index at = 0; at < mEquation.size(); at++)
	{
		// capture result expression and reduction
		Qoperand::Sp pOperand = mEquation.mExpression(at);
		if (pOperand != nullptr)
		{
			expression = pOperand->toString();
			symbol = mEquation.mResult.symbol()(at)->identity();
			Qbit value = mEquation.mResult.value()(at);
			Reduction reduction(symbol, value);
			mReductions[expression] = reduction;

			// capture expression of carry forward operands in expression
			expression = Qaddition::Carry::Symbol(expression);
			symbol = Qaddition::Carry::Symbol(symbol);
			value = Qint::cSuperposition;
			Reduction carryCorrect(symbol, value);
			mReductions[expression] = carryCorrect;
		}
	}

	// capture reductions for arguments with values different from cSuperposition (i.e. 0 or 1)
	for (auto atArg = mEquation.mArguments.cbegin(); atArg != mEquation.mArguments.cend(); atArg++)
	{
		for (Index at = 0; at < (*atArg).symbol().rows(); at++)
		{
			Qbit value = (*atArg).value()(at);
			if (value < 2)
			{
				string symbol = (*atArg).symbol()(at)->identity();
				Reduction reduction(symbol, value);
				mReductions[symbol] = reduction;
			}
		}
	}
}

QuboKey Qequation::Reduct::operator() (const QuboKey& original, bool ready) const
{
	QuboKey key = original;
	Reductions::const_iterator found1stAt = mReductions.find(original.first);
	Reductions::const_iterator found2ndAt = mReductions.find(original.second);
	Reductions::const_iterator end = mReductions.cend();
	if (found1stAt != end)
	{
		// The 1st half of a key of a quadratic element  should be updated with replacement symbol
		Reduction correct1st = (*found1stAt).second;
		if (found2ndAt != end)
		{
			// The 2nd half of a key of a quadratic element should be updated with replacement symbol
			Reduction correct2nd = (*found2ndAt).second;
			if (!ready || (correct1st.second == Qint::cSuperposition && correct2nd.second == Qint::cSuperposition))
			{
				// Always just replace 1st and 2nd symbols without applying condition
				// or replace 1st and 2nd symbols in binaryquadratic element when condition values are not defined (not 0 or 1)
				key = QuboKey(correct1st.first, correct2nd.first);
			}
			else if (correct1st.second == Qint::cSuperposition && correct2nd.second == 1)
			{
				// otherwise replace with a linear element using 1st replacement when its condition value is 1
				key = QuboKey(correct1st.first, correct1st.first);
			}
			else if (correct1st.second == 1 && correct2nd.second == Qint::cSuperposition)
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
			// Just 1st half of a key of a quadratic element should be updated with replacement symbol
			if (!ready || correct1st.second == Qint::cSuperposition)
			{
				// Always just replace 1st symbol without applying condition
				// or replace 1st symbol in binaryquadratic element when condition value is not defined (not 0 or 1)
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
		// Just 2st half of a key of a quadratic element should be updated with replacement symbol
		Reduction reduction = (*found2ndAt).second;
		if (!ready || reduction.second == Qint::cSuperposition)
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