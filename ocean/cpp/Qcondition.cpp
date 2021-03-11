#include <math.h> 
#include <Qcondition.h>
#include <Utility.h>

#include <Logger.h>

using namespace dann5::ocean;

/*** QuboEquation ***/


Qcondition::Qcondition()
	: Qcomparison(), mAction("")
{
	_lct(toString());
}

Qcondition::Qcondition(const Qcomparison& right)
	: Qcomparison(right), mAction("")
{
	_lct(toString());
}

Qcondition::Qcondition(const Qcondition& right)
	: Qcomparison(right), mAction(right.mAction)
{
	_lct(toString());
}

Qcondition::Qcondition(const Qexpression& equation, const Qvars& arguments)
	: Qcomparison(equation, arguments), mAction("")
{
	_lct(toString());
}

Qcondition::Qcondition(const Qexpression& equation, const Qvars& arguments, const Qroutine& action)
	: Qcomparison(equation, arguments), mAction(action)
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
	Qcomparison::operator&(right);
	return(*this);
}

Qroutine& Qcondition::operator<<(const Qstatement& right)
{
	mAction << right;
	return(mAction);
}

Qroutine& Qcondition::operator<<(const Qroutine& right)
{
	mAction << right;
	return(mAction);
}

string Qcondition::toString(bool decomposed, Index level) const
{
	string sCondition = Qcomparison::toString(decomposed, level);
	sCondition += mAction.toString(decomposed);
	return sCondition;
}

void Qcondition::add(Qsolver::Sample& sample)
{
	Qcomparison::add(sample);
	mAction.add(sample);
}

void Qcondition::set(Qsolver::Samples& samples)
{
	Qcomparison::set(samples);
	mAction.set(samples);
}

string Qcondition::solutions() const
{
	string values = Qcomparison::solutions();
	values += mAction.solutions();
	return(values);
}

ostream& dann5::ocean::operator<<(std::ostream& out, const Qcondition& statement)
{
	out << statement.toString();
	return out;
}