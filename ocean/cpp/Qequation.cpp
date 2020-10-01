#include <math.h> 
#include <Qequation.h>
#include <Utility.h>

using namespace dann5::ocean;

/*** QuboEquation ***/

Qequation::Qequation(const Qequation& right)
	:mResult(right.mResult), mArguments(right.mArguments), mExpression(right.mExpression), mCorrector(*this)
{
	mCorrector.init();
}

Qequation::Qequation(const Qvar& result, const Qexpression& equation)
	: mResult(result), mExpression(equation), mCorrector(*this)
{
	if (mExpression.rows() > mResult.symbol().rows())
	{
		mResult.extendTo(mExpression.rows());
	}
	mCorrector.init();
}

Qequation::Qequation(const Qvar& result, const Qexpression& equation, const QvarList& arguments)
	: mResult(result), mExpression(equation), mArguments(arguments), mCorrector(*this)
{
	mCorrector.init();
}

Qequation::~Qequation()
{

}

void Qequation::addArgument(const Qvar& argument)
{
	mArguments.push_back(argument);
}

Qequation& Qequation::operator<<(const Qvar& argument)
{
	addArgument(argument);
	return(*this);
}

BQM Qequation::bqm(bool ready) const
{
	BQM rawBqm;
	for (Index at = 0; at < size(); at++)
		rawBqm += mExpression(at)->bqm();

	BQM bqm;	// Updated BQM
	//	for (const auto &at : bqm)
	for (BQM::iterator at = rawBqm.begin(); at != rawBqm.end(); at++)
	{
		// remove all quadratic elements with bias 0
		if (((*at).first.first != (*at).first.second) && ((*at).second == 0))
			continue;

		BQKey key = mCorrector.correct((*at).first, ready);
		if(key != Corrector::cSkip)
		{
			// Using a correct key add bias for the buinary quadratic element in updated BQM 
			bqm[key] += (*at).second;
		}
	}
	return(bqm);	// return updated BQM
}


std::ostream& dann5::ocean::operator<<(std::ostream& out, const Qequation& qe)
{
	for (Index at = 0; at < qe.size(); at++)
	{
		string exprStr = qe.mExpression(at)->toString();
		for(Index innerAt = at - 1; innerAt >= 0; innerAt--)
		{
			string from = Qaddition::CarrySymbol(qe.mExpression(innerAt)->toString());
			string to = Qaddition::CarrySymbol(qe.mResult.symbol()(innerAt));
			replaceAll(exprStr, from, to);
		}
		out << "| " << qe.mResult.symbol()(at) << " = " << int(qe.mResult.value()(at)) << " |" << " = " << exprStr << std::endl;
	}
	return out;
}


const BQKey Qequation::Corrector::cSkip("skip", "skip");

Qequation::Corrector::Corrector(Qequation& equation)
	:mEquation(equation)
{
}

void Qequation::Corrector::init()
{
	string symbol(""), expression("");

	for (Index at = 0; at < mEquation.size(); at++)
	{
/*		if (at > 0)
		{
			// correct identifiers of carry forward operands in expression
			mEquation.mExpression(at)->rename(expression, symbol);
		}
*/
		// capture result expression and correction value
		expression = mEquation.mExpression(at)->toString();
		symbol = mEquation.mResult.symbol()(at);
		Qbit value = mEquation.mResult.value()(at);
		Correction correction(symbol, value);
		mCorrections[expression] = correction;

		// capture expression of carry forward operands in expression
		expression = Qaddition::CarrySymbol(expression);
		symbol = Qaddition::CarrySymbol(symbol);
		value = Qint::cSuperposition;
		Correction carryCorrect(symbol, value);
		mCorrections[expression] = carryCorrect;
	}

	// capture corrections for arguments with values different from cSuperposition (i.e. 0 or 1)
	for (QvarList::const_iterator atArg = mEquation.mArguments.cbegin(); atArg != mEquation.mArguments.cend(); atArg++)
	{
		for (Index at = 0; at < (*atArg).symbol().rows(); at++)
		{
			Qbit value = (*atArg).value()(at);
			if (value < 2)
			{
				string symbol = (*atArg).symbol()(at);
				Correction correction(symbol, value);
				mCorrections[symbol] = correction;
			}
		}
	}
}

BQKey Qequation::Corrector::correct(const BQKey& original, bool ready) const
{
	BQKey key = original;
	Corrections::const_iterator found1stAt = mCorrections.find(original.first);
	Corrections::const_iterator found2ndAt = mCorrections.find(original.second);
	Corrections::const_iterator end = mCorrections.cend();
	if (found1stAt != end)
	{
		// The 1st half of a key of a quadratic element  should be updated with replacement symbol
		Correction correct1st = (*found1stAt).second;
		if (found2ndAt != end)
		{
			// The 2nd half of a key of a quadratic element should be updated with replacement symbol
			Correction correct2nd = (*found2ndAt).second;
			if (!ready || (correct1st.second == Qint::cSuperposition && correct2nd.second == Qint::cSuperposition))
			{
				// Always just replace 1st and 2nd symbols without applying condition
				// or replace 1st and 2nd symbols in binaryquadratic element when condition values are not defined (not 0 or 1)
				key = BQKey(correct1st.first, correct2nd.first);
			}
			else if (correct1st.second == Qint::cSuperposition && correct2nd.second == 1)
			{
				// otherwise replace with a linear element using 1st replacement when its condition value is 1
				key = BQKey(correct1st.first, correct1st.first);
			}
			else if (correct1st.second == 1 && correct2nd.second == Qint::cSuperposition)
			{
				// otherwise replace with a linear element using 2nd replacement when its condition value is 1
				key = BQKey(correct2nd.first, correct2nd.first);
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
				key = BQKey(correct1st.first, original.second);
			}
			else if (correct1st.second == 1)
			{
				// otherwise replace with a linear element using 2nd half of a key when condition value is 1
				key = BQKey(original.second, original.second);
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
		Correction correction = (*found2ndAt).second;
		if (!ready || correction.second == Qint::cSuperposition)
		{
			key = BQKey(original.first, correction.first);
		}
		else if (correction.second == 1)
		{
			// otherwise replace with a linear element using 1st half of a key when condition value is 1
			key = BQKey(original.first, original.first);
		}
		else
		{
			// all other combinations should be removed as result is 0
			key = cSkip;
		}
	}
	return key;
}