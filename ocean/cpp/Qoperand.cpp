#include <math.h> 
#include <Qoperand.h>

#include <Logger.h>

using namespace dann5::ocean;


/*** Operand ***/

Qoperand::Qoperand(const Qoperand& right)
	:mIdentity(right.mIdentity)
{
	_lct(toString());
}

Qoperand::Qoperand(const string& identity)
	: mIdentity(identity)
{
	_lct(toString());
}

Qoperand::~Qoperand()
{
	_ldt(toString());
}

string Qoperand::toString(bool reduced) const
{
	return mIdentity;
}

bool Qoperand::doesExist(const Qoperand::Sp& pRight) const
{
	string tStr(toString()), rStr(pRight->toString());
	return(tStr.find(rStr) != string::npos);
};

Qubo Qoperand::qubo() const
{
	return Qubo();
}

std::ostream& dann5::ocean::operator << (std::ostream& output, const Qoperand& right)
{
	if(&right != nullptr)
		output << right.toString();
	return output;
}
