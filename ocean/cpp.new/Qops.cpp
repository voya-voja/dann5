#include <math.h> 
#include <Qops.h>
#include <Factory.h>

#include <Qubo.h>

#include <Logger.h>

using namespace dann5::ocean;


/*** Operand ***/

Qoperand::Qoperand() 
{
	_lct(toString());
}

Qoperand::Qoperand(const Qoperand& right) 
	:mIdentity(right.mIdentity)
{
	_lct(toString());
}

Qoperand::Qoperand(const string& identity)
	:mIdentity(identity)
{
	_lct(toString());
}

Qoperand::~Qoperand() 
{
	_ldt(toString());
}

bool Qoperand::reid(const string& stale, const string& fresh)
{
	// Search for the substring in string
	size_t pos = mIdentity.find(stale);
	bool reided = pos != std::string::npos;
	if (reided)
		mIdentity.replace(pos, stale.length(), fresh);
	return reided;
}

string Qoperand::toString() const 
{
	return mIdentity;
}

bool Qoperand::isExist(const Qoperand* pRight) const
{
	string tStr(toString()), rStr(pRight->toString());
	return(tStr.find(rStr) != string::npos);
};

BQM Qoperand::bqm() const
{
	return BQM();
}

std::ostream& dann5::ocean::operator << (std::ostream& output, const Qoperand& right)
{
	output << right.toString();
	return output;
}


/*** Operation ***/

Qop::Qop()
{

}

Qop::Qop(const Qop& right)
{
	for (auto &pOp : right.mOperands)
		mOperands.push_back(pOp);
}

Qop::~Qop()
{
	for (auto &pOperand : mOperands)
		delete pOperand;
}

void Qop::releaseArguments()
{
	while (mOperands.size() > 0)
		mOperands.pop_back();
}

Qoperand* Qop::arguments(const OperandList& operands)
{
	if (operands.size() != argumentsNumber())
		throw invalid_argument("Arguments number is " + to_string(operands.size()) + " instead of " + to_string(argumentsNumber()));
	for (auto &pOp : operands)
		mOperands.push_back(pOp);
	return(instance());
}

Qoperand* Qop::remove(const Qoperand* pToRemove)
{
	Qoperand* pResult = dynamic_cast<Qoperand*>(this);
	for (auto at = mOperands.begin(); at != mOperands.end(); at++)
	{
		Qoperand* pOperand = (*at);
		if (pOperand->identity() == pToRemove->identity())
		{
			mOperands.erase(at--);
			if (mOperands.size() == 1)
			{
				pResult = mOperands[0];
				break;
			}
			else if(mOperands.size() == 0)
			{
				pResult = NULL;
				break;
			}
		}
		else
		{
			Qop* pOp = dynamic_cast<Qop*>(pOperand);
			if(pOp != NULL)
				(*at) = pOp->remove(pToRemove);
		}
	}
	return(pResult);
}


/*** UnaryOperation ***/


QunOperation::QunOperation()
	:Qoperand(), Qop()
{
	_lct(toString());
}

QunOperation::QunOperation(const QunOperation& right)
	: Qoperand(right), Qop(right)
{
	_lct(toString());
}

QunOperation::QunOperation(const string& identity)
	: Qoperand(identity), Qop()
{
	_lct(toString());
}

QunOperation::~QunOperation()
{
	_ldt(toString());
}

string QunOperation::toString() const
{
	OperandList args = arguments();
	string a0("?");
	size_t size = args.size();
	if (size > 0 && args[0] != NULL) a0 = args[0]->toString();

	string rStr = Qoperand::toString() + a0;
	return rStr;
}

bool QunOperation::reid(const string& stale, const string& fresh)
{
	bool reided = Qoperand::reid(stale, fresh);
	OperandList args = arguments();
	reided = args[0]->reid(stale, fresh);
	return reided;
}

BQM QunOperation::bqm() const
{
	// get sub-bqm from argument operand
	BQM aBqm = arguments()[0]->bqm();

	// create Qubo rule object for this operand
	Qubo* pQubo = Factory<string, Qubo>::Instance().create(identity());

	// use names of argument and this operands as unique string describing arguments
	Qubo::Labels bqm_arguments(2);
	string aName = arguments()[0]->toString();
	string tName = toString();
	bqm_arguments << aName, tName;

	BQM bqm = pQubo->bqm(bqm_arguments);
	bqm += aBqm;

	return(bqm);
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

string QbiOperation::toString() const
{
	OperandList args = arguments();
	string a0("?"), a1("?");
	size_t size = args.size();
	if (size > 0 && args[0] != NULL) a0 = args[0]->toString();
	if (size > 1 && args[1] != NULL) a1 = args[1]->toString();

	string rStr = a0 + " " + Qoperand::toString() + " " + a1;
	return rStr;
}

bool QbiOperation::reid(const string& stale, const string& fresh)
{
	bool reided = Qoperand::reid(stale, fresh);
	OperandList args = arguments();
	reided |= args[0]->reid(stale, fresh);
	reided |= args[1]->reid(stale, fresh);
	return reided;
}

BQM QbiOperation::bqm() const
{
	// get sub-bqm's from left and right operand
	BQM lBqm = arguments()[0]->bqm();
	BQM rBqm = arguments()[1]->bqm();

	// create Qubo rule object for this operand
	Qubo* pQubo = Factory<string, Qubo>::Instance().create(identity());

	// use names of left, right and this operands as unique string describing arguments
	Qubo::Labels bqm_arguments(3);
	string lName = arguments()[0]->toString();
	string rName = arguments()[1]->toString();
	string tName = toString();
	bqm_arguments << lName, rName, tName;

	BQM bqm = pQubo->bqm(bqm_arguments);
	bqm += lBqm;
	bqm += rBqm;

	return(bqm);
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

string QtriOperation::toString() const
{
	OperandList args = arguments();
	string a0("?"), a1("?"), a2("?");
	size_t size = args.size();
	if (size > 0 && args[0] != NULL) a0 = args[0]->toString();
	if (size > 1 && args[1] != NULL) a1 = args[1]->toString();
	if (size > 2 && args[2] != NULL) a2 = args[2]->toString();

	string op = Qoperand::toString();
	string rStr = a0 + " " + op + " " + a1 + " " + op + " " + a2;
	return rStr;
}

bool QtriOperation::reid(const string& stale, const string& fresh)
{
	bool reided = Qoperand::reid(stale, fresh);
	OperandList args = arguments();
	reided |= args[0]->reid(stale, fresh);
	reided |= args[1]->reid(stale, fresh);
	reided |= args[2]->reid(stale, fresh);
	return reided;
}

BQM QtriOperation::bqm() const
{
	// create Qubo rule object for this operand
	Qubo* pQubo = Factory<string, Qubo>::Instance().create(identity());

	// use names of left, right and this operands as unique string describing arguments
	string opName1 = arguments()[0]->toString();
	string opName2 = arguments()[1]->toString();
	string opName3 = arguments()[2]->toString();
	string tName = toString();
	Qubo::Labels bqm_arguments(4);
	bqm_arguments << opName1, opName2, opName3, tName;

	BQM bqm = pQubo->bqm(bqm_arguments);
	OperandList operands = arguments();
	for (auto &pOperand : operands)
		bqm += pOperand->bqm();

	return(bqm);
}


/*** AndOperation ***/

Qand::Qand()
	:QbiOperation("&")
{}

Qand::Qand(const Qand& right)
	: QbiOperation(right)
{}

Qand::~Qand()
{}


/*** Addition ***/

Qaddition::Qaddition()
{}

Qaddition::~Qaddition()
{}

Qaddition::Assignment Qaddition::assign(const OperandList& operands)
{
	Qaddition::Assignment assignment;
	assignment.mpOp = dynamic_cast<Qop*>(this);
	assignment.mpOp->arguments(operands);
	assignment.mpCarryForward = carry();
	assignment.mpCarryToBeRemoved = NULL;
	return assignment;
}

Qoperand* Qaddition::carry()
{
	if (mpCarry == NULL)
		mpCarry = new Qcarry(this);
	return(mpCarry);
}

void Qaddition::carry(Qcarry* pCarry)
{
	mpCarry = pCarry;
	if(pCarry != NULL)
		mpCarry->addition(this);
};


/*** Carry Operation ***/

const string Qcarry::cSymbol = "#";

string Qcarry::Symbol(const string& operand)
{
	return(cSymbol + "{" + operand + "}");
}

Qcarry::Qcarry(Qoperand* pOperand)
	: QunOperation(cSymbol)
{
	arguments().push_back(pOperand);
}

Qcarry::Qcarry(Qaddition* pAddition)
	: QunOperation(cSymbol)
{
	arguments().push_back(dynamic_cast<Qoperand*>(pAddition));
}

Qcarry::Qcarry(const Qcarry& right)
	: QunOperation(right)
{}

Qcarry::~Qcarry()
{}

string Qcarry::toString() const
{
	OperandList args = arguments();
	return Symbol(args[0]->toString());
}

BQM Qcarry::bqm() const
{
	return(Qoperand::bqm());
}

void Qcarry::addition(Qaddition* pAddition)
{
	OperandList& args = arguments();
	if(args.empty())
		args.push_back(dynamic_cast<Qoperand*>(pAddition));
	else
		args[0] = (dynamic_cast<Qoperand*>(pAddition));
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

Qaddition::Assignment Qxor::assign(const OperandList& operands)
{
	Qaddition::Assignment assignment;
	if (operands[0]->identity() == "^" || operands[1]->identity() == "^")
	{
		Qxor* pXor = NULL;
		Qoperand* pOperand = NULL;
		try
		{
			pXor = dynamic_cast<Qxor*>(operands[0]);
			pOperand = operands[1];
		}
		catch(...)
		{
			pOperand = operands[0];
			pXor = dynamic_cast<Qxor*>(operands[1]);
		}

		Qadder* pAdder = new Qadder;
		pAdder->carry(dynamic_cast<Qcarry*>(pXor->carry()));
		pXor->carry(NULL);
		OperandList adder_args;
		adder_args.push_back(pXor->arguments()[0]);
		adder_args.push_back(pXor->arguments()[1]);
		adder_args.push_back(pOperand);

		pAdder->arguments(adder_args);
		assignment.mpOp = pAdder;
		assignment.mpCarryForward = pAdder->carry();
		assignment.mpCarryToBeRemoved = pXor->carry();
		delete this;
	}
	else
	{
		assignment = Qaddition::assign(operands);
	}
	return assignment;
}

string Qxor::toString() const
{
	return QbiOperation::toString();
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

Qoperand* Qadder::remove(const Qoperand* pToRemove)
{
	Qoperand* pResult = Qop::remove(pToRemove);
	if (pResult == this && arguments().size() == 2)
	{
		Qxor* pXor = new Qxor;
		pXor->carry(dynamic_cast<Qcarry*>(carry()));
		carry(NULL);
		OperandList args = arguments();
		pXor->arguments(args);
		releaseArguments();
		pResult = pXor;
		delete this;
	}
	return pResult;
}

string Qadder::toString() const
{
	return QtriOperation::toString();
}

/*** Operations Factory ***/

Factory<string, Qop> Factory<string, Qop>::gFactory;
/*
Creator<string, Operation, EqualOperator> gEqualOperatorCreator_s("=");
Creator<string, Operation, EqualOperator> gEqualOperatorCreator("equal");

Creator<string, Operation, NotOperator> gNotOperatorCreator_s("~");
Creator<string, Operation, NotOperator> gNotOperatorCreator("not");

Creator<string, Operation, LessThanOperator> gLessThanOperatorCreator_s("<");
Creator<string, Operation, LessThanOperator> gLessThanOperatorCreator("lessthan");

Creator<string, Operation, LessEqualOperator> gLessEqualOperatorCreator_s("<=");
Creator<string, Operation, LessEqualOperator> gLessEqualOperatorCreator("lessequal");

Creator<string, Operation, GreaterThanOperator> gGreaterThanOperatorCreator_s(">");
Creator<string, Operation, GreaterThanOperator> gGreaterThanOperatorCreator("greaterthan");

Creator<string, Operation, GreaterEqualOperator> gGreaterEqualOperatorCreator_s(">=");
Creator<string, Operation, GreaterEqualOperator> gGreaterEqualOperatorCreator("greaterequal");
*/
Creator<string, Qop, Qand> gAndOperationCreator_s("&");
Creator<string, Qop, Qand> gAndOperationCreator("and");
/*
Creator<string, Operation, NandOperation> gNandOperationCreator_s("~&");
Creator<string, Operation, NandOperation> gNandOperationCreator("nand");

Creator<string, Operation, OrOperation> gOrOperationCreator_s("|");
Creator<string, Operation, OrOperation> gOrOperationCreator("or");

Creator<string, Operation, NorOperation> gNorOperationCreator_s("~|");
Creator<string, Operation, NorOperation> gNorOperationCreator("nor");

Creator<string, Operation, NotLeftOrRightOperation> gNotLeftOrRightOperationCreator("notleftorright");

Creator<string, Operation, DwNotLeftOrRightOperation> gNotLeftOrRightOperationCreator_dw("dwnotleftorright");
*/
Creator<string, Qop, Qxor> gXorOperationCreator_s("^");
Creator<string, Qop, Qxor> gXorOperationCreator("xor");

Creator<string, Qop, Qadder05> gHalfAdderOperationCreator_s("#");
Creator<string, Qop, Qadder05> gHalfAdderOperationCreator("halfadder");
/*
Creator<string, Operation, NxorOperation> gNxorOperationCreator_s("~^");
Creator<string, Operation, NxorOperation> gNxorOperationCreator("nxor");
*/
Creator<string, Qop, Qadder> gAdderOperationCreator_s("+");
Creator<string, Qop, Qadder> gAdderOperationCreator("adder");