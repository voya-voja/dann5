#include <Qubo.h>
#include <Factory.h>
#include <Qops.h>

using namespace dann5::ocean;


Qubo::Qubo(Size size)
	:mVariables(size.nCols), mElements(size.nRows, size.nCols)
{

}

Qubo::~Qubo() {}

inline Qubo::VariableComma Qubo::operator << (const string& variable)
{
	return mVariables << variable;
}

Qubo& Qubo::operator << (const Labels& variables)
{
	mVariables = variables;
	return *this;
}

inline Qubo::ElementComma Qubo::operator << (const double& element)
{
	return mElements << element;
}

Qubo& Qubo::operator << (const VariableRow& row)
{
	int c, r = -1;
	while (mVariables(++r) != row.first && r < mVariables.rows());

	if (r == mVariables.rows())
		throw string("Row assignment incorect variable: '" + row.first + "'!");

	for (c = 0; c < mElements.rows() && r < mElements.rows(); c++)
		mElements(r, c) = row.second(c);

	return *this;
}

BQM Qubo::bqm() const
{
	int c, r;
	BQKey key;
	BQM dict;

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

BQM Qubo::bqm(const Labels& arguments) const
{
	int c, r;
	BQKey key;
	BQM dict;

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

Qubo::Labels Qubo::format(const Qubo::Labels& arguments) const
{
	return(arguments);
}

std::ostream& dann5::ocean::operator<< (std::ostream& stream, const Qubo& right)
{
	stream << "  " << right.mVariables.transpose() << std::endl;
	stream << right.mElements;
	return(stream);
}

QuboInOut::QuboInOut()
	:Qubo(Qubo::Size(2))
{
}

Qubo::Labels QuboInOut::format(const Qubo::Labels& args) const
{
	if (args.rows() == 2) return args;

	Qubo::Labels fArgs(2);
	fArgs << args(0), args(2);
	return(fArgs);
}

Qubo2in1out::Qubo2in1out()
	:Qubo(Qubo::Size(3))
{
}

Qubo::Labels Qubo2in1out::format(const Qubo::Labels& arg) const
{
	return(arg);
}

Qubo2in2out::Qubo2in2out()
	:Qubo(Qubo::Size(4))
{
}

Qubo::Labels Qubo2in2out::format(const Qubo::Labels& args) const
{
	if (args.rows() == 4) return args;

	Qubo::Labels fArgs(4);
	fArgs << args(0), args(1), args(2), Qcarry::Symbol(args(2));
	return(fArgs);
}


EqualQubo::EqualQubo()
	:QuboInOut()
{
	*this << "a", "r";
	*this <<  1,  -2,
			  0,   1;
}

NotQubo::NotQubo()
	:QuboInOut()
{
	*this << "a", "r";
	*this << -1,  2, 
			  0, -1;
}

LessThanQubo::LessThanQubo()
	:QuboInOut()
{
	*this << "a", "r";
	*this <<  2,   4,
			  0,  -6;
}

LessEqualQubo::LessEqualQubo()
	:QuboInOut()
{
	*this << "a", "r";
	*this <<  4,  -4,
			  0,   0;
}

GreaterThanQubo::GreaterThanQubo()
	:QuboInOut()
{
	*this << "a", "r";
	*this << -6,   4,
			  0,   2;
}

GreaterEqualQubo::GreaterEqualQubo()
	:QuboInOut()
{
	*this << "a", "r";
	*this <<  0,  -4,
			  0,   4;
}

AndQubo::AndQubo()
	:Qubo2in1out()
{
	*this << "a", "b", "r";
	*this <<  0,   1,  -2,
			  0,   0,  -2,
			  0,   0,   3;
}

NandQubo::NandQubo()
	:Qubo2in2out()
{
	// assuming x = 1 - ab
	*this << "a", "b", "r", "x";
	*this <<  0,   5,   0,  -7,
			  0,   0,   0,  -8,
			  0,   0,  -5,  10,
			  0,   0,   0,   5;
}

OrQubo::OrQubo()
	:Qubo2in1out()
{
	*this << "a", "b", "r";
	*this <<  1, 1, -2,
			  0, 1, -2,
			  0, 0,  1;
}

NorQubo::NorQubo()
	:Qubo2in2out()
{
	// assuming x = 1 - ab
	*this << "a", "b", "r", "x";
	*this << -1,   2,   2,  -2,
			  0,  -1,   2,  -2,
			  0,   0,  -1,  -1,
			  0,   0,   0,   3;
}

NotLeftOrRightQubo::NotLeftOrRightQubo()
	:Qubo2in2out()
{
	*this << "a", "b", "r", "x";
	*this << -1,   1,   2,  -3,
			  0,   0,   0,  -2,
			  0,   0,  -1,  -2,
			  0,   0,   0,   5;
}


DwNotLeftOrRightQubo::DwNotLeftOrRightQubo()
	:Qubo2in2out()
{
	*this << "a", "b", "r", "x";
	*this << -1,   4,   2,  -6,
			  0,   0,   0,  -6,
			  0,   0,  -1,  -2,
			  0,   0,   0,   9;
}

XorQubo::XorQubo()
	:Qubo2in2out()
{
	*this << "a", "b", "r", "x";
	*this << 1, 2, -2, -4,
			 0, 1, -2, -4,
			 0, 0,  1,  4,
			 0, 0,  0,  4; 
}

NxorQubo::NxorQubo()
	:Qubo2in2out()
{
	*this << "a", "b","r", "x";
	*this << -1,  2, -4,  2,
			  0, -1, -4,  2,
			  0,  0,  8, -4,
			  0,  0,  0, -1;
}

AdderQubo::AdderQubo()
	:Qubo(Qubo::Size(5))
{
	*this << "a", "b", "c", "s", "x";
	*this << 1, 2, 2, -2, -4,
			 0, 1, 2, -2, -4,
			 0, 0, 1, -2, -4,
			 0, 0, 0,  1,  4,
			 0, 0, 0,  0,  4;
}

Qubo::Labels AdderQubo::format(const Qubo::Labels& args) const
{
	if (args.rows() == 5) return args;

	Qubo::Labels fAdderArgs(5);
	if (args.rows() == 4)
		fAdderArgs << args(0), args(1), args(2), args(3), Qcarry::Symbol(args(3));
	else
		fAdderArgs << args(0), args(1), Qcarry::Symbol(args(0)), args(2), Qcarry::Symbol(args(2));

	return(fAdderArgs);
}

Factory<string, Qubo> Factory<string, Qubo>::gFactory;

Creator<string, Qubo, EqualQubo> gEqualQuboCreator_s("=");
Creator<string, Qubo, EqualQubo> gEqualQuboCreator("equal");

Creator<string, Qubo, NotQubo> gNotQuboCreator_s("~");
Creator<string, Qubo, NotQubo> gNotQuboCreator("not");

Creator<string, Qubo, LessThanQubo> gLessThanQuboCreator_s("<");
Creator<string, Qubo, LessThanQubo> gLessThanQuboCreator("lessthan");

Creator<string, Qubo, LessEqualQubo> gLessEqualQuboCreator_s("<=");
Creator<string, Qubo, LessEqualQubo> gLessEqualQuboCreator("lessequal");

Creator<string, Qubo, GreaterThanQubo> gGreaterThanQuboCreator_s(">");
Creator<string, Qubo, GreaterThanQubo> gGreaterThanQuboCreator("greaterthan");

Creator<string, Qubo, GreaterEqualQubo> gGreaterEqualQuboCreator_s(">=");
Creator<string, Qubo, GreaterEqualQubo> gGreaterEqualQuboCreator("greaterequal");

Creator<string, Qubo, AndQubo> gAndQuboCreator_s("&");
Creator<string, Qubo, AndQubo> gAndQuboCreator("and");

Creator<string, Qubo, NandQubo> gNandQuboCreator_s("~&");
Creator<string, Qubo, NandQubo> gNandQuboCreator("nand");

Creator<string, Qubo, OrQubo> gOrQuboCreator_s("|");
Creator<string, Qubo, OrQubo> gOrQuboCreator("or");

Creator<string, Qubo, NorQubo> gNorQuboCreator_s("~|");
Creator<string, Qubo, NorQubo> gNorQuboCreator("nor");

Creator<string, Qubo, NotLeftOrRightQubo> gNotLeftOrRightQuboCreator("notleftorright");

Creator<string, Qubo, DwNotLeftOrRightQubo> gNotLeftOrRightQuboCreator_dw("dwnotleftorright");

Creator<string, Qubo, XorQubo> gXorQuboCreator_s("^");
Creator<string, Qubo, XorQubo> gXorQuboCreator("xor");


Creator<string, Qubo, HalfAdderQubo> gHalfAdderQuboCreator_s("#");
Creator<string, Qubo, HalfAdderQubo> gHalfAdderQuboCreator("halfadder");

Creator<string, Qubo, NxorQubo> gNxorQuboCreator_s("~^");
Creator<string, Qubo, NxorQubo> gNxorQuboCreator("nxor");

Creator<string, Qubo, AdderQubo> gAdderQuboCreator_s("+");
Creator<string, Qubo, AdderQubo> gAdderQuboCreator("adder");