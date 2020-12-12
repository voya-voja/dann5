#include <Qubo.h>
#include <Factory.h>
#include <Qops.h>

using namespace dann5::ocean;


std::ostream& dann5::ocean::operator<< (std::ostream& stream, const Qubo& right)
{
	for (auto at = right.cbegin(); at != right.cend(); at++)
		stream << "(" << at->first.first << ", " << at->first.second << "): " << at->second << "; ";
	return stream;
}

Qubo& dann5::ocean::operator+=(Qubo& left, const Qubo& right)
{
	for (auto at = right.cbegin(); at != right.cend(); at++)
	{
		QuboKey key = (*at).first;
		Qubo::iterator item = left.find(key);
		if (item != left.end())
			(*item).second += (*at).second;
		else
			left[key] = (*at).second;
	}
	return left;
}

QuboTable::QuboTable(Size size)
	:mVariables(size.nCols), mElements(size.nRows, size.nCols)
{
	_lc;
}

QuboTable::~QuboTable() 
{
	_ld;
}

inline QuboTable::VariableComma QuboTable::operator << (const string& variable)
{
	return mVariables << variable;
}

QuboTable& QuboTable::operator << (const Labels& variables)
{
	mVariables = variables;
	return *this;
}

inline QuboTable::ElementComma QuboTable::operator << (const double& element)
{
	return mElements << element;
}

QuboTable& QuboTable::operator << (const VariableRow& row)
{
	int c, r = -1;
	while (mVariables(++r) != row.first && r < mVariables.rows());

	if (r == mVariables.rows())
		throw invalid_argument("Row assignment incorect variable: '" + row.first + "'!");

	for (c = 0; c < mElements.rows() && r < mElements.rows(); c++)
		mElements(r, c) = row.second(c);

	return *this;
}

Qubo QuboTable::qubo() const
{
	int c, r;
	QuboKey key;
	Qubo dict;

	for (r = 0; r < mElements.rows(); r++)
	{
		key.first = mVariables(r);
		for (c = r; c < mElements.cols(); c++)
		{
			key.second = mVariables(c);
			dict[key] = mElements(r, c);
		}
	}
	return dict;
}

Qubo QuboTable::qubo(const Labels& arguments) const
{
	int c, r;
	QuboKey key;
	Qubo dict;

	Labels args = format(arguments);
	Labels variables(mVariables);
	for (Index at = 0; at < args.rows(); at++)
		variables(at) = args(at);
	for (r = 0; r < mElements.rows(); r++)
	{
		key.first = variables(r);
		for (c = 0; c < mElements.cols(); c++)
		{
			key.second = variables(c);
			dict[key] = mElements(r, c);
		}
	}
	return dict;
}

QuboTable::Labels QuboTable::format(const QuboTable::Labels& arguments) const
{
	return(arguments);
}

std::ostream& dann5::ocean::operator<< (std::ostream& stream, const QuboTable& right)
{
	stream << "  " << right.mVariables.transpose() << std::endl;
	stream << right.mElements;
	return(stream);
}

QuboTableInOut::QuboTableInOut()
	:QuboTable(QuboTable::Size(2))
{
}

QuboTable::Labels QuboTableInOut::format(const QuboTable::Labels& args) const
{
	if (args.rows() == 2) return args;

	QuboTable::Labels fArgs(2);
	fArgs << args(0), args(2);
	return(fArgs);
}

QuboTable2in1out::QuboTable2in1out()
	:QuboTable(QuboTable::Size(3))
{
}

QuboTable::Labels QuboTable2in1out::format(const QuboTable::Labels& arg) const
{
	return(arg);
}

QuboTable2in2out::QuboTable2in2out()
	:QuboTable(QuboTable::Size(4))
{
}

QuboTable::Labels QuboTable2in2out::format(const QuboTable::Labels& args) const
{
	if (args.rows() == 4) return args;

	QuboTable::Labels fArgs(4);
	fArgs << args(0), args(1), args(2), Qaddition::Carry::Symbol(args(2));
	return(fArgs);
}


EqQuboTable::EqQuboTable()
	:QuboTableInOut()
{
	*this << "a", "r";
	*this <<  1,  -2,
			  0,   1;
}

NotQuboTable::NotQuboTable()
	:QuboTableInOut()
{
	*this << "a", "r";
	*this << -1,  2, 
			  0, -1;
}

LtQuboTable::LtQuboTable()
	:QuboTableInOut()
{
	*this << "a", "r";
	*this <<  2,   4,
			  0,  -6;
}

LeQuboTable::LeQuboTable()
	:QuboTableInOut()
{
	*this << "a", "r";
	*this <<  4,  -4,
			  0,   0;
}

GtQuboTable::GtQuboTable()
	:QuboTableInOut()
{
	*this << "a", "r";
	*this << -6,   4,
			  0,   2;
}

GeQuboTable::GeQuboTable()
	:QuboTableInOut()
{
	*this << "a", "r";
	*this <<  0,  -4,
			  0,   4;
}

AndQuboTable::AndQuboTable()
	:QuboTable2in1out()
{
	*this << "a", "b", "r";
	*this <<  0,   1,  -2,
			  0,   0,  -2,
			  0,   0,   3;
}

NandQuboTable::NandQuboTable()
	:QuboTable2in2out()
{
	// assuming x = 1 - ab
	*this << "a", "b", "r", "x";
	*this <<  0,   5,   0,  -7,
			  0,   0,   0,  -8,
			  0,   0,  -5,  10,
			  0,   0,   0,   5;
}

OrQuboTable::OrQuboTable()
	:QuboTable2in1out()
{
	*this << "a", "b", "r";
	*this <<  1, 1, -2,
			  0, 1, -2,
			  0, 0,  1;
}

NorQuboTable::NorQuboTable()
	:QuboTable2in2out()
{
	// assuming x = 1 - ab
	*this << "a", "b", "r", "x";
	*this << -1,   2,   2,  -2,
			  0,  -1,   2,  -2,
			  0,   0,  -1,  -1,
			  0,   0,   0,   3;
}

NotLeftOrRightQuboTable::NotLeftOrRightQuboTable()
	:QuboTable2in2out()
{
	*this << "a", "b", "r", "x";
	*this << -1,   1,   2,  -3,
			  0,   0,   0,  -2,
			  0,   0,  -1,  -2,
			  0,   0,   0,   5;
}


DwNotLeftOrRightQuboTable::DwNotLeftOrRightQuboTable()
	:QuboTable2in2out()
{
	*this << "a", "b", "r", "x";
	*this << -1,   4,   2,  -6,
			  0,   0,   0,  -6,
			  0,   0,  -1,  -2,
			  0,   0,   0,   9;
}

XorQuboTable::XorQuboTable()
	:QuboTable2in2out()
{
	*this << "a", "b", "r", "x";
	*this << 1, 2, -2, -4,
			 0, 1, -2, -4,
			 0, 0,  1,  4,
			 0, 0,  0,  4; 
}

NxorQuboTable::NxorQuboTable()
	:QuboTable2in2out()
{
	*this << "a", "b","r", "x";
	*this << -1,  2, -4,  2,
			  0, -1, -4,  2,
			  0,  0,  8, -4,
			  0,  0,  0, -1;
}

AdderQuboTable::AdderQuboTable()
	:QuboTable(QuboTable::Size(5))
{
	*this << "a", "b", "c", "s", "x";
	*this << 1, 2, 2, -2, -4,
			 0, 1, 2, -2, -4,
			 0, 0, 1, -2, -4,
			 0, 0, 0,  1,  4,
			 0, 0, 0,  0,  4;
}

QuboTable::Labels AdderQuboTable::format(const QuboTable::Labels& args) const
{
	if (args.rows() == 5) return args;

	QuboTable::Labels fAdderArgs(5);
	if (args.rows() == 4)
		fAdderArgs << args(0), args(1), args(2), args(3), Qaddition::Carry::Symbol(args(3));
	else
		fAdderArgs << args(0), args(1), Qaddition::Carry::Symbol(args(0)), args(2), Qaddition::Carry::Symbol(args(2));

	return(fAdderArgs);
}

Factory<string, QuboTable> Factory<string, QuboTable>::gFactory;

Creator<string, QuboTable, EqQuboTable> gEqualQuboCreator_s("=");
Creator<string, QuboTable, EqQuboTable> gEqualQuboCreator("equal");

Creator<string, QuboTable, NotQuboTable> gNotQuboCreator_s("~");
Creator<string, QuboTable, NotQuboTable> gNotQuboCreator("not");

Creator<string, QuboTable, LtQuboTable> gLessThanQuboCreator_s("<");
Creator<string, QuboTable, LtQuboTable> gLessThanQuboCreator("lessthan");

Creator<string, QuboTable, LeQuboTable> gLessEqualQuboCreator_s("<=");
Creator<string, QuboTable, LeQuboTable> gLessEqualQuboCreator("lessequal");

Creator<string, QuboTable, GtQuboTable> gGreaterThanQuboCreator_s(">");
Creator<string, QuboTable, GtQuboTable> gGreaterThanQuboCreator("greaterthan");

Creator<string, QuboTable, GeQuboTable> gGreaterEqualQuboCreator_s(">=");
Creator<string, QuboTable, GeQuboTable> gGreaterEqualQuboCreator("greaterequal");

Creator<string, QuboTable, AndQuboTable> gAndQuboCreator_s("&");
Creator<string, QuboTable, AndQuboTable> gAndQuboCreator("and");

Creator<string, QuboTable, NandQuboTable> gNandQuboCreator_s("~&");
Creator<string, QuboTable, NandQuboTable> gNandQuboCreator("nand");

Creator<string, QuboTable, OrQuboTable> gOrQuboCreator_s("|");
Creator<string, QuboTable, OrQuboTable> gOrQuboCreator("or");

Creator<string, QuboTable, NorQuboTable> gNorQuboCreator_s("~|");
Creator<string, QuboTable, NorQuboTable> gNorQuboCreator("nor");

Creator<string, QuboTable, NotLeftOrRightQuboTable> gNotLeftOrRightQuboCreator("notleftorright");

Creator<string, QuboTable, DwNotLeftOrRightQuboTable> gNotLeftOrRightQuboCreator_dw("dwnotleftorright");

Creator<string, QuboTable, XorQuboTable> gXorQuboCreator_s("^");
Creator<string, QuboTable, XorQuboTable> gXorQuboCreator("xor");

Creator<string, QuboTable, Adder05QuboTable> gHalfAdderQuboCreator_s("+05");
Creator<string, QuboTable, Adder05QuboTable> gHalfAdderQuboCreator("halfadder");

Creator<string, QuboTable, NxorQuboTable> gNxorQuboCreator_s("~^");
Creator<string, QuboTable, NxorQuboTable> gNxorQuboCreator("nxor");

Creator<string, QuboTable, AdderQuboTable> gAdderQuboCreator_s("+");
Creator<string, QuboTable, AdderQuboTable> gAdderQuboCreator("adder");