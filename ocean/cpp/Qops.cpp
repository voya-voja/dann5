#include <math.h> 
#include <Qops.h>
#include <Factory.h>

#include <Qubo.h>

#include <Logger.h>

using namespace dann5::ocean;


/*** Operation ***/

Qop::Qop(const string& id, size_t noArgs)
	:Qoperand(id), mNoArguments(noArgs)
{
}

Qop::Qop(const Qop& right)
	:Qoperand(right), mNoArguments(right.mNoArguments)
{
	for (auto &pOp : right.mArguments)
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

void Qop::arguments(const Qoperands& operands)
{
	if (operands.size() != mNoArguments)
		throw invalid_argument("Arguments number is " + to_string(operands.size())
										+ " instead of " + to_string(argsNumber()));
	for (auto &pOp : operands)
		mArguments.push_back(pOp);
}

void Qop::labelFor(const string& id)
{
	return labelFor(id, -1);
}

void Qop::labelFor(const string& id, Index index)
{
	if(index >= 0)
		mLabel = identity() + "|" + id + to_string(index) + "|";
	size_t size = mArguments.size();
	for (size_t at = 0; at < size; at++)
	{
		Qop::Sp pOp = dynamic_pointer_cast<Qop>(mArguments[at]);
		if (pOp != nullptr) 
			pOp->labelFor(id + to_string(at), index + 1);
	}
}

string Qop::toString(bool decomposed) const
{
	Qoperands args = arguments();
	size_t size = args.size();
	string rStr(""), rest("");
	if (decomposed && isLabeled())
		rStr += "\n\t" + label() + " = ";
	for(size_t atArg = 0; atArg < size; atArg++)
	{
		Qoperand::Sp arg = args[atArg];
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
		if(atArg != size - 1)
			rStr += " " + identity() + " ";
	}
	if (decomposed)
		rStr += rest;
	return rStr;
}

// return Qubo presentation of this Qoperand
Qubo Qop::qubo(bool finalized) const
{
	Qoperands args = arguments();
	size_t size = args.size();
	Qubo argsQubo;
	QuboTable::Labels qubo_arguments(size + 1);
	for (size_t atArg = 0; atArg < size; atArg++)
	{
		// add sub-qubo from argument operand
		argsQubo += args[atArg]->qubo(finalized);

		// use names of argument and this operands as unique string describing arguments
		string aName = arguments()[atArg]->toString();
		qubo_arguments[atArg] = aName;

	}
	string tName = toString();
	qubo_arguments[size] = tName;
	// create QuboTable rule object for this operand
	QuboTable::Sp pQubo = Factory<string, QuboTable>::Instance().create(identity());
	Qubo qubo = pQubo->qubo(qubo_arguments);
	qubo += argsQubo;
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

Qaddition::Sp Qaddition::assign(const Qoperands& operands)
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
	if(pCarry != nullptr)
		mpCarry->addition(this);
};


/*** Carry Operation ***/

const string Qaddition::Carry::cSymbol = "#";

string Qaddition::Carry::Symbol(const string& operand)
{
	return(cSymbol + "|" + operand + "|");
}

Qaddition::Carry::Carry(Qaddition* pAddition)
	: Qoperand(cSymbol), mpAddition(pAddition)
{
}

Qaddition::Carry::Carry(const Carry& right)
	: Qoperand(right)
{}

Qaddition::Carry::~Carry()
{}

string Qaddition::Carry::toString(bool decomposed) const
{
	if (decomposed)
	{
		if(mpAddition->isLabeled())
			return Symbol(mpAddition->label());
	}
	return Symbol(mpAddition->toString(decomposed));
}

void Qaddition::Carry::addition(Qaddition* pAddition)
{
	mpAddition = pAddition;
}

/*** Xor Operation ***/


Qaddition::Sp Qxor::assign(const Qoperands& operands)
{
	if (operands[0]->identity() == "^" || operands[1]->identity() == "^")
	{
		// identify xor operand
		Qxor::Sp pXor = nullptr;
		Qoperand::Sp pOperand = nullptr;
		try
		{
			pXor = dynamic_pointer_cast<Qxor>(operands[0]);
			pOperand = operands[1];
		}
		catch(...)
		{
			pOperand = operands[0];
			pXor = dynamic_pointer_cast<Qxor>(operands[1]);
		}

		// replace xor binary-operation with adder trinary-operation
		Qadder::Sp pAdder = dynamic_pointer_cast<Qadder>(Factory<string, Qop>::Instance().create("+"));
		pAdder->carry(pXor->carry());
		pXor->carry(nullptr);

		// push adder's tri arguments
		Qoperands adder_args;
		adder_args.push_back(pXor->arguments()[0]);
		adder_args.push_back(pXor->arguments()[1]);
		adder_args.push_back(pOperand);
		pAdder->arguments(adder_args);

		return(pAdder);
	}
	else
	{	// standar processing of asigment, if neither of operands is xor operation
		return Qaddition::assign(operands);
	}
}