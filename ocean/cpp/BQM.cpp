#include <BQM.h>

#include <Logger.h>

using namespace dann5::ocean;

BQM::BQM()
{
	_lc;
}

BQM::BQM(const BQM& right)
{
	_lc;
}

BQM::~BQM()
{
	_ld;
}

BQM& BQM::operator=(const BQM& right)
{
	// TBA
	return(*this);
}

BQM BQM::operator+(const BQM&) const
{
	BQM result(*this);
	result += (const BQM&)right;
	return(result);
}

BQM& BQM::operator+=(const BQM& right)
{
	// TBA
	return *this;
}

std::ostream& dann5::ocean::operator<< (std::ostream& stream, const BQM& right)
{
	// TBA
	return stream;
}