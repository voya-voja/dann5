#include <BQM.h>

using namespace dann5::ocean;

BQM::BQM()
{

}


BQM::BQM(const BQM& right)
	:BQDictionary(right)
{

}

BQM& BQM::operator=(const BQM& right)
{
	BQDictionary::operator=(right);
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
	BQM::const_iterator at = right.cbegin();
	for (; at != right.cend(); at++)
	{
		BQKey key = (*at).first;
		BQM::iterator item = find(key);
		if (item != end())
			(*item).second += (*at).second;
		else
			(*this)[key] = (*at).second;
	}
	return *this;
}

BQDictionary& BQM::dictionary()
{
	return(*this);
}

std::ostream& dann5::ocean::operator<< (std::ostream& stream, const BQM& right)
{
	BQM::const_iterator at(right.cbegin());
	for (; at != right.cend(); at++)
		stream << "(" << at->first.first << ", " << at->first.second << "): " << at->second << "; ";
	return stream;
}