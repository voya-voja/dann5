#include <math.h> 
#include <Qequation.h>

#include <Logger.h>

using namespace dann5::ocean;

/*** Qvar ***/

Qvar::Qvar(const Qvar& right)
	:mSymbol(right.mSymbol), mValue(right.mValue)
{
	_lct(mSymbol.name());
}

Qvar::Qvar(Index size, const string& lable)
	: mSymbol(size, lable), mValue(size)
{
	_lct(mSymbol.name());
}

Qvar::Qvar(const string& lable, long value)
	: mSymbol(Index(log(value) / log(2)) + 1, lable), mValue(value)
{
	_lct(mSymbol.name());
}

Qvar::Qvar(const Qdef& symbol)
	: mSymbol(symbol), mValue()
{
	_lct(mSymbol.name());
}

Qvar::Qvar(const Qdef& symbol, const Qint& value)
	: mSymbol(symbol), mValue(value)
{
	_lct(mSymbol.name());
}

Qvar::~Qvar()
{
	_ldt(mSymbol.name());
}

void Qvar::extendTo(Index size)
{
	mSymbol.extendTo(size);
	mValue.extendTo(size);
}

std::ostream& dann5::ocean::operator<<(std::ostream& out, const Qvar& bv)
{
	for (Eigen::Index at = 0; at < bv.mSymbol.rows(); at++)
		out << bv.mSymbol(at) << " = " << int(bv.mValue(at)) << std::endl;
	return out;
}