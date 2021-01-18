#include <math.h> 
#include <Qfunction.h>
#include <Utility.h>
#include <Qequation.h>

#include <Logger.h>

using namespace dann5::ocean;

/*** QuboEquation ***/


Qfunction::Qfunction(const string& name)
	: Qvar(Qroutine(name))
{
	mpRutine = dynamic_pointer_cast<Qroutine>(definition());
	_lct(toString());
}

Qfunction::Qfunction(Index size, const string& name)
	: Qvar(Qroutine(size, name))
{
	mpRutine = dynamic_pointer_cast<Qroutine>(definition());
	_lct(toString());
}

Qfunction::Qfunction(const Qroutine& routine)
	: Qvar(routine)
{
	mpRutine = dynamic_pointer_cast<Qroutine>(definition());
	_lct(toString());
}

Qfunction::Qfunction(const Qfunction& right)
	:Qvar(right)
{
	mpRutine = dynamic_pointer_cast<Qroutine>(definition());
	_lct(toString());
}

Qfunction::~Qfunction()
{
	_lct(toString());
}

Qfunction& Qfunction::operator<<(const Qstatement& right)
{
	Index size(mpRutine->nobs()), rSize(right.nobs());
	if (size < rSize)
		resize(rSize);

	routine() << right;
	return(*this);
}

Qfunction& Qfunction::operator<<(const Qroutine& right)
{
	routine() << right;
	return(*this);
}


string Qfunction::toString(bool decomposed) const
{
	string funcStr(Qvar::toString(decomposed));
	funcStr += routine().toString(decomposed);
	return funcStr;
}

ostream& dann5::ocean::operator<<(std::ostream& out, const Qfunction& function)
{
	out << function.toString();
	return out;
}

Qmod::Qmod(const Qvar& dividend, const Qvar& divisor)
	:Qfunction("mod")
{

}

Qmod::Qmod(const Qvar& dividend, const Qwhole& divisor)
	: Qfunction("mod")
{

}

Qmod::Qmod(const Qdef& dividend, const Qwhole& divisor)
	: Qfunction("mod")
{

}

Qmod::Qmod(const Qdef& dividend, const Qvar& divisor)
	: Qfunction("mod")
{

}

Qpow::Qpow(const Qvar& base, int power)
	: Qfunction(base.definition().size(), base.definition().name() + "**" + to_string(power) + "_")
{
	initialize(base, power);
}

Qpow::Qpow(const Qdef& base, int power)
	: Qfunction(base.size(), base.name() + "**" + to_string(power) + "_")
{
	initialize(Qvar(base), power);
}

void Qpow::initialize(const Qvar& base, int power)
{
	Qequation pow(*this);
	pow = base;
	for (int time = 1; time < power; time++)
		pow *= base;
	(*this) << pow;
}

Qgcd::Qgcd(const Qvar& dividend, const Qvar& divisor)
	: Qfunction("gcd")
{

}

Qgcd::Qgcd(const Qvar& dividend, const Qdef& divisor)
	: Qfunction("gcd")
{

}

Qgcd::Qgcd(const Qdef& dividend, const Qvar& divisor)
	: Qfunction("gcd")
{

}

Qgcd::Qgcd(const Qdef& dividend, const Qdef& divisor)
	: Qfunction("gcd")
{

}