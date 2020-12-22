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

OperatorQT::OperatorQT()
	:QuboTable(QuboTable::Size(2))
{
}

QuboTable::Labels OperatorQT::format(const QuboTable::Labels& args) const
{
	if (args.rows() == 2) return args;

	QuboTable::Labels fArgs(2);
	fArgs << args(0), args(2);
	return(fArgs);
}

BinaryOpQT::BinaryOpQT()
	:QuboTable(QuboTable::Size(3))
{
}

QuboTable::Labels BinaryOpQT::format(const QuboTable::Labels& arg) const
{
	return(arg);
}

BinaryOp2OutQT::BinaryOp2OutQT()
	:QuboTable(QuboTable::Size(4))
{
}

QuboTable::Labels BinaryOp2OutQT::format(const QuboTable::Labels& args) const
{
	if (args.rows() == 4) return args;

	QuboTable::Labels fArgs(4);
	fArgs << args(0), args(1), args(2), Qaddition::Carry::Symbol(args(2));
	return(fArgs);
}


EqQT::EqQT()
	:OperatorQT()
{
	*this << "a", "r";
	*this <<  1,  -2,
			  0,   1;
}

NotQT::NotQT()
	:OperatorQT()
{
	*this << "a", "r";
	*this << -1,  2, 
			  0, -1;
}

LtQT::LtQT()
	:OperatorQT()
{
	*this << "a", "r";
	*this <<  2,   4,
			  0,  -6;
}

LeQT::LeQT()
	:OperatorQT()
{
	*this << "a", "r";
	*this <<  4,  -4,
			  0,   0;
}

GtQT::GtQT()
	:OperatorQT()
{
	*this << "a", "r";
	*this << -6,   4,
			  0,   2;
}

GeQT::GeQT()
	:OperatorQT()
{
	*this << "a", "r";
	*this <<  0,  -4,
			  0,   4;
}

AndQT::AndQT()
	:BinaryOpQT()
{
	*this << "a", "b", "r";
	*this <<  0,   1,  -2,
			  0,   0,  -2,
			  0,   0,   3;
}

NandQT::NandQT()
	:BinaryOp2OutQT()
{
	// assuming x = 1 - ab
	*this << "a", "b", "r", "x";
	*this <<  0,   5,   0,  -7,
			  0,   0,   0,  -8,
			  0,   0,  -5,  10,
			  0,   0,   0,   5;
}

OrQT::OrQT()
	:BinaryOpQT()
{
	*this << "a", "b", "r";
	*this <<  1, 1, -2,
			  0, 1, -2,
			  0, 0,  1;
}

NorQT::NorQT()
	:BinaryOp2OutQT()
{
	// assuming x = 1 - ab
	*this << "a", "b", "r", "x";
	*this << -1,   2,   2,  -2,
			  0,  -1,   2,  -2,
			  0,   0,  -1,  -1,
			  0,   0,   0,   3;
}

NotLeftOrRightQT::NotLeftOrRightQT()
	:BinaryOp2OutQT()
{
	*this << "a", "b", "r", "x";
	*this << -1,   1,   2,  -3,
			  0,   0,   0,  -2,
			  0,   0,  -1,  -2,
			  0,   0,   0,   5;
}


DwNotLeftOrRightQT::DwNotLeftOrRightQT()
	:BinaryOp2OutQT()
{
	*this << "a", "b", "r", "x";
	*this << -1,   4,   2,  -6,
			  0,   0,   0,  -6,
			  0,   0,  -1,  -2,
			  0,   0,   0,   9;
}

XorQuboTable::XorQuboTable()
	:BinaryOp2OutQT()
{
	*this << "a", "b", "r", "x";
	*this << 1, 2, -2, -4,
			 0, 1, -2, -4,
			 0, 0,  1,  4,
			 0, 0,  0,  4; 
}

NxorQuboTable::NxorQuboTable()
	:BinaryOp2OutQT()
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

Creator<string, QuboTable, EqQT> gEqualQuboCreator_s("=");
Creator<string, QuboTable, EqQT> gEqualQuboCreator("equal");

Creator<string, QuboTable, NotQT> gNotQuboCreator_s("~");
Creator<string, QuboTable, NotQT> gNotQuboCreator("not");

Creator<string, QuboTable, LtQT> gLessThanQuboCreator_s("<");
Creator<string, QuboTable, LtQT> gLessThanQuboCreator("lessthan");

Creator<string, QuboTable, LeQT> gLessEqualQuboCreator_s("<=");
Creator<string, QuboTable, LeQT> gLessEqualQuboCreator("lessequal");

Creator<string, QuboTable, GtQT> gGreaterThanQuboCreator_s(">");
Creator<string, QuboTable, GtQT> gGreaterThanQuboCreator("greaterthan");

Creator<string, QuboTable, GeQT> gGreaterEqualQuboCreator_s(">=");
Creator<string, QuboTable, GeQT> gGreaterEqualQuboCreator("greaterequal");

Creator<string, QuboTable, AndQT> gAndQuboCreator_s("&");
Creator<string, QuboTable, AndQT> gAndQuboCreator("and");

Creator<string, QuboTable, NandQT> gNandQuboCreator_s("~&");
Creator<string, QuboTable, NandQT> gNandQuboCreator("nand");

Creator<string, QuboTable, OrQT> gOrQuboCreator_s("|");
Creator<string, QuboTable, OrQT> gOrQuboCreator("or");

Creator<string, QuboTable, NorQT> gNorQuboCreator_s("~|");
Creator<string, QuboTable, NorQT> gNorQuboCreator("nor");

Creator<string, QuboTable, NotLeftOrRightQT> gNotLeftOrRightQuboCreator("notleftorright");

Creator<string, QuboTable, DwNotLeftOrRightQT> gNotLeftOrRightQuboCreator_dw("dwnotleftorright");

Creator<string, QuboTable, XorQuboTable> gXorQuboCreator_s("^");
Creator<string, QuboTable, XorQuboTable> gXorQuboCreator("xor");

Creator<string, QuboTable, Adder05QuboTable> gHalfAdderQuboCreator_s("+05");
Creator<string, QuboTable, Adder05QuboTable> gHalfAdderQuboCreator("halfadder");

Creator<string, QuboTable, NxorQuboTable> gNxorQuboCreator_s("~^");
Creator<string, QuboTable, NxorQuboTable> gNxorQuboCreator("nxor");

Creator<string, QuboTable, AdderQuboTable> gAdderQuboCreator_s("+");
Creator<string, QuboTable, AdderQuboTable> gAdderQuboCreator("adder");