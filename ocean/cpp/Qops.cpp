#include <math.h> 
#include <Qops.h>
#include <Factory.h>

#include <Qubo.h>

#include <Logger.h>

using namespace dann5::ocean;


/*** Operation ***/

Qop::Qop()
{

}

Qop::Qop(const Qop& right)
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
	if (operands.size() != argsNumber())
		throw invalid_argument("Arguments number is " + to_string(operands.size())
										+ " instead of " + to_string(argsNumber()));
	for (auto &pOp : operands)
		mArguments.push_back(pOp);
}

Qoperand::Sp Qop::remove(const Qoperand::Sp& pToRemove)
{
	Qoperand::Sp pResult = Qoperand::Sp(dynamic_cast<Qoperand*>(this));
	for (auto at = mArguments.begin(); at != mArguments.end(); at++)
	{
		Qoperand::Sp pOperand = (*at);
		if (pOperand->identity() == pToRemove->identity())
		{
			mArguments.erase(at--);
			if(mArguments.size() == 0)
			{
				pResult = nullptr;
				break;
			}
		}
		else
		{
			Qop::Sp pOp = dynamic_pointer_cast<Qop>(pOperand);
			if(pOp != nullptr)
				(*at) = pOp->remove(pToRemove);
		}
	}
	return(pResult);
}

void Qop::labelFor(const string& id)
{
	return labelFor(id, -1);
}

void Qop::labelFor(const string& id, Index index)
{
	if(index >= 0)
		mLabel = dynamic_cast<Qoperand*>(this)->identity() + "|" + id + to_string(index) + "|";
	size_t size = mArguments.size();
	for (size_t at = 0; at < size; at++)
	{
		Qoperand::Sp pOperand = mArguments[at];
		Qop::Sp pOp = dynamic_pointer_cast<Qop>(pOperand);
		if (pOp != nullptr) pOp->labelFor(id + to_string(at), index + 1);
	}
}


string Qop::toString(bool decomposed, const string& opSymbol) const
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
			rStr += " " + opSymbol + " ";
	}
	if (decomposed)
		rStr += rest;
	return rStr;
}

/*** Operator - Unary Operation ***/


Qoperator::Qoperator()
	:Qoperand(), Qop()
{
	_lct(toString());
}

Qoperator::Qoperator(const Qoperator& right)
	: Qoperand(right), Qop(right)
{
	_lct(toString());
}

Qoperator::Qoperator(const string& identity)
	: Qoperand(identity), Qop()
{
	_lct(toString());
}

Qoperator::~Qoperator()
{
	_ldt(toString());
}

string Qoperator::toString(bool decomposed) const
{
	return Qop::toString(decomposed, identity());
}

Qubo Qoperator::qubo() const
{
	// get sub-qubo from argument operand
	Qubo aQubo = arguments()[0]->qubo();

	// create QuboTable rule object for this operand
	QuboTable::Sp pQubo = Factory<string, QuboTable>::Instance().create(identity());

	// use names of argument and this operands as unique string describing arguments
	QuboTable::Labels qubo_arguments(2);
	string aName = arguments()[0]->toString();
	string tName = toString();
	qubo_arguments << aName, tName;

	Qubo qubo = pQubo->qubo(qubo_arguments);
	qubo += aQubo;

	return(qubo);
}


/*** BinaryOperation ***/

QbiOperation::QbiOperation() 
	:Qoperand(), Qop()
{
	_lct(toString());
}

QbiOperation::QbiOperation(const QbiOperation& right) 
	:Qoperand(right), Qop(right)
{
	_lct(toString());
}

QbiOperation::QbiOperation(const string& identity)
	: Qoperand(identity), Qop()
{
	_lct(toString());
}

QbiOperation::~QbiOperation()
{
	_ldt(toString());
}

string QbiOperation::toString(bool decomposed) const
{
	return Qop::toString(decomposed, identity());
}

Qubo QbiOperation::qubo() const
{
	// get sub-qubo's from left and right operand
	Qubo lBqm = arguments()[0]->qubo();
	Qubo rBqm = arguments()[1]->qubo();

	// create QuboTable rule object for this operand
	QuboTable::Sp pQubo = Factory<string, QuboTable>::Instance().create(identity());

	// use names of left, right and this operands as unique string describing arguments
	QuboTable::Labels qubo_arguments(3);
	string lName = arguments()[0]->toString();
	string rName = arguments()[1]->toString();
	string tName = toString();
	qubo_arguments << lName, rName, tName;

	Qubo qubo = pQubo->qubo(qubo_arguments);
	qubo += lBqm;
	qubo += rBqm;

	return(qubo);
}

/*** TrinaryOperation ***/

QtriOperation::QtriOperation()
	:Qoperand(), Qop()
{
	_lct(toString());
}

QtriOperation::QtriOperation(const QtriOperation& right)
	: Qoperand(right), Qop(right)
{
	_lct(toString());
}

QtriOperation::QtriOperation(const string& identity)
	: Qoperand(identity), Qop()
{
	_lct(toString());
}

QtriOperation::~QtriOperation()
{
	_ldt(toString());
}

string QtriOperation::toString(bool decomposed) const
{
	return Qop::toString(decomposed, identity());
}

Qubo QtriOperation::qubo() const
{
	// create QuboTable rule object for this operand
	QuboTable::Sp pQubo = Factory<string, QuboTable>::Instance().create(identity());

	// use names of left, right and this operands as unique string describing arguments
	string opName1 = arguments()[0]->toString();
	string opName2 = arguments()[1]->toString();
	string opName3 = arguments()[2]->toString();
	string tName = toString();
	QuboTable::Labels qubo_arguments(4);
	qubo_arguments << opName1, opName2, opName3, tName;

	Qubo qubo = pQubo->qubo(qubo_arguments);
	Qoperands operands = arguments();
	for (auto &pOperand : operands)
		qubo += pOperand->qubo();

	return(qubo);
}

/*** Addition ***/

Qaddition::Qaddition()
{
	mpCarry = Carry::Sp(new Carry(this));
}

Qaddition::~Qaddition()
{}

Qaddition::Sp Qaddition::assign(const Qoperands& operands)
{
	Qop* pAddition = dynamic_cast<Qop*>(this);
	pAddition->arguments(operands);
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
		Qop* arg = dynamic_cast<Qop*>(mpAddition);
		if(arg->isLabeled())
			return Symbol(arg->label());
	}
	Qoperand* arg = dynamic_cast<Qoperand*>(mpAddition);
	return Symbol(arg->toString(decomposed));
}

void Qaddition::Carry::addition(Qaddition* pAddition)
{
	mpAddition = pAddition;
}

/*** Xor Operation ***/

Qxor::Qxor()
	: QbiOperation("^")
{}

Qxor::Qxor(const Qxor& right)
	: QbiOperation(right)
{}

Qxor::~Qxor()
{}

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


/*** Adder Operation ***/

Qadder::Qadder()
	: QtriOperation("+")
{}

Qadder::Qadder(const Qadder& right)
	: QtriOperation(right)
{}

Qadder::~Qadder()
{}

Qoperand::Sp Qadder::remove(const Qoperand::Sp& pToRemove)
{
	Qoperand::Sp pResult = Qop::remove(pToRemove);
	if (pResult.get() == this && arguments().size() == 2)
	{
		Qxor::Sp pXor = dynamic_pointer_cast<Qxor>(Factory<string, Qop>::Instance().create("^"));;
		pXor->carry(dynamic_pointer_cast<Qaddition::Carry>(carry()));
		carry(nullptr);
		Qoperands args = arguments();
		pXor->arguments(args);
		releaseArguments();
		pResult = Qoperand::Sp(pXor);
	}
	return pResult;
}

/*** Operations Factory ***/

Factory<string, Qop> Factory<string, Qop>::gFactory;

Creator<string, Qop, Qequal> gEqualOperatorCreator_s("=");
Creator<string, Qop, Qequal> gEqualOperatorCreator("equal");

Creator<string, Qop, Qnot> gNotOperatorCreator_s("~");
Creator<string, Qop, Qnot> gNotOperatorCreator("not");

Creator<string, Qop, Qlt> gLtOperatorCreator_s("<");
Creator<string, Qop, Qlt> gLtOperatorCreator("lessthan");

Creator<string, Qop, Qle> gLeOperatorCreator_s("<=");
Creator<string, Qop, Qle> gLeOperatorCreator("lessequal");

Creator<string, Qop, Qgt> gGtOperatorCreator_s(">");
Creator<string, Qop, Qgt> gGtOperatorCreator("greaterthan");

Creator<string, Qop, Qge> gGeOperatorCreator_s(">=");
Creator<string, Qop, Qge> gGeOperatorCreator("greaterequal");

Creator<string, Qop, Qand> gAndOpCreator_s("&");
Creator<string, Qop, Qand> gAndOpCreator("and");

Creator<string, Qop, QnAnd> gNandOpCreator_s("~&");
Creator<string, Qop, QnAnd> gNandOpCreator("nand");

Creator<string, Qop, Qor> gOrOpCreator_s("|");
Creator<string, Qop, Qor> gOrOpCreator("or");

Creator<string, Qop, QnOr> gNorOpCreator_s("~|");
Creator<string, Qop, QnOr> gNorOpCreator("nor");
/*
Creator<string, Qop, QnLeftOrRight> gNleftOrRightOpCreator("notleftorright");

Creator<string, Qop, DwNotLeftOrRightOp> gNotLeftOrRightOpCreator_dw("dwnotleftorright");
*/
Creator<string, Qop, Qxor> gXorOpCreator_s("^");
Creator<string, Qop, Qxor> gXorOpCreator("xor");

Creator<string, Qop, Qadder05> gHalfAdderOpCreator_s("+05");
Creator<string, Qop, Qadder05> gHalfAdderOpCreator("halfadder");
/*
Creator<string, Qop, NxorOp> gNxorOpCreator_s("~^");
Creator<string, Qop, NxorOp> gNxorOpCreator("nxor");
*/
Creator<string, Qop, Qadder> gAdderOpCreator_s("+");
Creator<string, Qop, Qadder> gAdderOpCreator("adder");