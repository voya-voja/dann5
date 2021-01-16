#include <math.h> 
#include <Qstatement.h>
#include <Utility.h>

#include <Logger.h>

using namespace dann5::ocean;

/*** QuboEquation ***/


Qstatement::Qstatement()
	:mExpression(0)
{
	_lc;
}

Qstatement::Qstatement(const Qstatement& right)
	: mArguments(right.mArguments),
	mExpression(right.mExpression)
{
	_lc;
}

Qstatement::Qstatement(Index size)
	:mExpression(size)
{
	_lc;
}

Qstatement::Qstatement(const Qexpression& equation, const Qvars& arguments)
	: mExpression(equation), mArguments(arguments)
{
	_lc;
}

Qstatement::~Qstatement()
{
	_ld;
}
