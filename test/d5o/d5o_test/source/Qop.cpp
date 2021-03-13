#include <math.h> 
#include <Qop.h>
#include <Factory.h>

#include <Qubo.h>

#include <Logger.h>

using namespace dann5::ocean;


/*** Operation ***/

Qop::Qop(const string& id, size_t noArgs)
	:Qdef(id), mNoArguments(noArgs)
{
}

Qop::Qop(const Qop& right)
	: Qdef(right), mNoArguments(right.mNoArguments)
{
	for (auto& pOp : right.mArguments)
		mArguments.push_back(pOp);
}

Qop::~Qop()
{
}

void Qop::releaseArguments()
{
	while (mArguments.size() > 0)
		mArguments.pop_back();
}

void Qop::arguments(const Qdefs& operands)
{
	if (operands.size() != mNoArguments)
		throw invalid_argument("Arguments number is " + to_string(operands.size())
			+ " instead of " + to_string(argsNumber()));
	for (auto& pOp : operands)
		mArguments.push_back(pOp);
}

void Qop::add(Qdef::Sp argument)
{
	if (mArguments.size() == mNoArguments)
		throw invalid_argument("Arguments number will exceed defined size of "
			+ to_string(argsNumber()));
	mArguments.push_back(argument);
}

void Qop::labelFor(const string& id)
{
	return labelFor(id, -1);
}

void Qop::labelFor(const string& anID, Index index)
{
	if (index >= 0)
		mLabel = id() + "|" + anID + to_string(index) + "|";
	size_t size = mArguments.size();
	for (size_t at = 0; at < size; at++)
	{
		Qop::Sp pOp = dynamic_pointer_cast<Qop>(mArguments[at]);
		if (pOp != nullptr)
			pOp->labelFor(anID + to_string(at), index + 1);
	}
}

string Qop::toString(bool decomposed) const
{
	Qdefs args = arguments();
	size_t size = args.size();
	string rStr(""), rest("");
	if (decomposed && isLabeled())
		rStr += "\n\t" + label() + " = ";
	for (size_t atArg = 0; atArg < size; atArg++)
	{
		Qdef::Sp arg = args[atArg];
		string aStr("?");
		if (arg != nullptr)
		{
			aStr = arg->toString(decomposed);
			if (decomposed)
			{
				// if operand is a sub-peration
				size_t at = aStr.find(" = ");
				if (at != string::npos && aStr[0] != Qaddition::Carry::cSymbol[0])
				{
					rest += aStr;
					aStr = aStr.substr(2, at - 2); // extract sub-operation label
				}
			}
		}
		rStr += aStr;
		if (atArg != size - 1)
			rStr += " " + id() + " ";
	}
	if (decomposed)
		rStr += rest;
	return rStr;
}

// return Qubo presentation of this Qdef
Qubo Qop::qubo(bool finalized) const
{
	Qdefs args = arguments();
	size_t size = args.size();
	Qubo argsQubo;
	QuboTable::Labels qubo_arguments(size + 1);
	vector<Qkey> opKeys;
	for (size_t atArg = 0; atArg < size; atArg++)
	{
		// add sub-qubo from argument operand
		argsQubo += args[atArg]->qubo(finalized);

		// use names of argument and this operands as unique string describing arguments
		Qdef::Sp pOperand = arguments()[atArg];
		string aName = pOperand->toString();
		Qop::Sp pOp = dynamic_pointer_cast<Qop>(pOperand);
		if (pOp != nullptr)
		{
			Qkey key(aName, aName);
			opKeys.push_back(key);
		}
		qubo_arguments[atArg] = aName;

	}
	string tName = toString();
	qubo_arguments[size] = tName;
	// create QuboTable rule object for this operand
	QuboTable::Sp pQubo = Factory<string, QuboTable>::Instance().create(id());
	Qubo qubo = pQubo->qubo(qubo_arguments);
	qubo += argsQubo;
	auto quboEnd = qubo.end();
	for (auto opKey : opKeys)
	{
		auto opElement = qubo.find(opKey);
		if (opElement != quboEnd && opElement->second == 0)
			qubo.erase(opKey);
	}
	return(qubo);
}

/*** Addition ***/

Qaddition::Qaddition(const string& id, size_t noArgs)
	:Qop(id, noArgs)
{
	mpCarry = Carry::Sp(new Carry(this));
}

Qaddition::Qaddition(const Qaddition& right)
	: Qop(right)
{
	mpCarry = Carry::Sp(new Carry(this));
}

Qaddition::~Qaddition()
{}

Qaddition::Sp Qaddition::assign(const Qdefs& operands)
{
	arguments(operands);
	return nullptr;
}

Qaddition::Carry::Sp Qaddition::carry()
{
	if (mpCarry == nullptr)
		mpCarry = Carry::Sp(new Carry(this));
	return(mpCarry);
}

void Qaddition::carry(const Carry::Sp& pCarry)
{
	mpCarry = pCarry;
	if (pCarry != nullptr)
		mpCarry->addition(this);
};


/*** Carry Operation ***/

const string Qaddition::Carry::cSymbol = "#";

string Qaddition::Carry::Symbol(const string& operand)
{
	return(cSymbol + "|" + operand + "|");
}

Qaddition::Carry::Carry(Qaddition* pAddition)
	: Qdef(cSymbol), mpAddition(pAddition)
{
}

Qaddition::Carry::Carry(const Carry& right)
	: Qdef(right), mpAddition(right.mpAddition)
{}

Qaddition::Carry::~Carry()
{}

string Qaddition::Carry::toString(bool decomposed) const
{
	if (decomposed)
	{
		if (mpAddition->isLabeled())
			return Symbol(mpAddition->label());
	}
	return Symbol(mpAddition->toString(decomposed));
}

void Qaddition::Carry::addition(Qaddition* pAddition)
{
	mpAddition = pAddition;
}

/*** Xor Operation ***/


Qaddition::Sp Qxor::assign(const Qdefs& operands)
{
	if (operands[0]->id() == XorQT::cMark || operands[1]->id() == XorQT::cMark)
	{
		// identify xor operand
		Qxor::Sp pXor = nullptr;
		Qdef::Sp pOperand = nullptr;
		pXor = dynamic_pointer_cast<Qxor>(operands[0]);
		if (pXor != nullptr)
		{
			pOperand = operands[1];
		}
		else
		{
			pOperand = operands[0];
			pXor = dynamic_pointer_cast<Qxor>(operands[1]);
		}

		// replace xor binary-operation with adder trinary-operation
		Qadder::Sp pAdder = dynamic_pointer_cast<Qadder>(Factory<string, Qop>::Instance().create(AdderQT::cMark));
		pAdder->carry(pXor->carry());
		pXor->carry(nullptr);

		// push adder's tri arguments
		pAdder->arguments({ pXor->arguments()[0], pXor->arguments()[1], pOperand });

		return(pAdder);
	}
	else
	{	// standar processing of asigment, if neither of operands is xor operation
		return Qaddition::assign(operands);
	}
}

/*** Multiplication ***/

string Qmultiplication::toString(bool decomposed) const
{
	return "";
}

Qubo Qmultiplication::qubo(bool finalized) const
{
	return Qubo();
}

/*
Qexpression& Qmultiplication::execute(const Qexpression& right)
{
	qbit_def_matrix xMatrix = thisX(right);	// this * right vectors => xMatrix
	// Rotate xMatrix left-90-digrees to change the values on main diagonal
	qbit_def_matrix reversedXMatrix = xMatrix.rowwise().reverse();
	// sum values on all diagonals into 
	sumDiagonals(reversedXMatrix);
	return *this;
}

qbit_def_matrix Qmultiplication::thisX(const Qexpression& right) const
{
	qbit_def_matrix xMatrix(nobs(), right.nobs());
	int carry = 0;
	std::cout << "Matrix " << nobs() << " x " << right.nobs() << endl;
	for (Index atR = 0; atR < nobs(); atR++)
	{
		for (Index atC = 0; atC < right.nobs(); atC++)
		{
			Qop::Sp pOp = Factory<string, Qop>::Instance().create(AndQT::cMark);
			pOp->arguments({ (*this)(atR), right(atC) });
			xMatrix(atR, atC) = pOp;
		}
	}
	return xMatrix;
}

void Qmultiplication::sumDiagonals(const qbit_def_matrix& xMatrix)
{
	// resize this vector to a # of diagonals + 1 of xMatrix
	Index size = xMatrix.rows() + xMatrix.cols();
	resize(size);
	Index last = size - 1;

	// Value of 0 bit is the sum of right-most diagonal
	Index atDiagonal = xMatrix.cols() - 1;

	// add functional object to add diagonal vectors' bit symbols
	Add add;
	for (0; *add < last; add++)
	{
		Index at = *add;
		qbit_def_vector diagonal = xMatrix.diagonal(atDiagonal);
		Index nDiagElmns = diagonal.rows();

		for (Index atD = 0; atD < nDiagElmns; atD++)
		{
			if (atD == 0)
			{	// add any carry-forward operands
				(*this)(at) = add(diagonal(atD));
			}
			else
			{
				(*this)(at) = add((*this)(at), diagonal(atD));
			}
		}
		Index atNext = at + 1;
		--atDiagonal;
		_lat("sumDiagonals", (*this)(at)->toString());
	}
	Qoperand::Sp pOperand = add();
	if (pOperand != nullptr)
	{
		(*this)(last) = pOperand;	// add any carry-forward operands
		_lat("sumDiagonals", (*this)(last)->toString());
	}
	else
	{
		resize(last);
	}
}
*/