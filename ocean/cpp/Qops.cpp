#include <math.h> 
#include <Qops.h>
#include <Factory.h>

#include <Qubo.h>

using namespace dann5::ocean;


/*** Operand ***/

Qoperand::Qoperand() {}

Qoperand::Qoperand(const Qoperand& right) 
	:mIdentity(right.mIdentity)
{}

Qoperand::Qoperand(const string& identity)
	:mIdentity(identity)
{

}

bool Qoperand::rename(const string& stale, const string& fresh)
{
	bool reid = mIdentity == stale;
	if (reid)
		mIdentity = fresh;
	return reid;
}

string Qoperand::toString() const 
{
	return mIdentity;
}

BQM Qoperand::bqm() const
{
	return BQM();
}

Qoperand::~Qoperand() {}


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

Qoperand* Qop::arguments(const OperandList& operands)
{
	if (operands.size() != argumentsNumber())
		throw invalid_argument("Arguments number is " + to_string(operands.size()) + " instead of " + to_string(argumentsNumber()));
	for (auto &pOp : operands)
		mOperands.push_back(pOp);
	return(instance());
}

/*** BinaryOperation ***/


QbiOperation::QbiOperation() 
	:Qoperand(), Qop()
{}

QbiOperation::QbiOperation(const QbiOperation& right) 
	:Qoperand(right), Qop(right)
{}

QbiOperation::QbiOperation(const string& identity)
	: Qoperand(identity), Qop()
{
}

QbiOperation::~QbiOperation()
{}

string QbiOperation::toString() const
{
	OperandList args = arguments();
	string rStr = args[0]->toString() + " " +
		Qoperand::toString() + " " + args[1]->toString();
	return rStr;
}

bool QbiOperation::rename(const string& stale, const string& fresh)
{
	bool reided = Qoperand::rename(stale, fresh);
	OperandList args = arguments();
	reided |= args[0]->rename(stale, fresh);
	reided |= args[1]->rename(stale, fresh);
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
{}

QtriOperation::QtriOperation(const QtriOperation& right)
	: Qoperand(right), Qop(right)
{}

QtriOperation::QtriOperation(const string& identity)
	: Qoperand(identity), Qop()
{
}

QtriOperation::~QtriOperation()
{
}

string QtriOperation::toString() const
{
	OperandList args = arguments();
	string rStr = args[0]->toString() + " " +
		Qoperand::toString() + " " + args[1]->toString() + " " +
		Qoperand::toString() + " " + args[2]->toString();
	return rStr;
}

bool QtriOperation::rename(const string& stale, const string& fresh)
{
	bool reided = Qoperand::rename(stale, fresh);
	OperandList args = arguments();
	reided |= args[0]->rename(stale, fresh);
	reided |= args[1]->rename(stale, fresh);
	reided |= args[2]->rename(stale, fresh);
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

string Qaddition::CarrySymbol(const string& arg)
{
	return("x{" + arg + "}");
}

Qoperand* Qaddition::carry() const
{
	string identifier = Qaddition::CarrySymbol(toString());
	return(new Qoperand(identifier));
}
/*** XorOperation ***/

Qxor::Qxor()
	: QbiOperation("^")
{}

Qxor::Qxor(const Qxor& right)
	: QbiOperation(right)
{}


Qxor::~Qxor()
{}

Qoperand* Qxor::clone() const
{
	return new Qxor(*this);
}

Qoperand* Qxor::arguments(const OperandList& operands)
{
	if (operands[0]->identity() == "^" || operands[1]->identity() == "^")
	{
		Qoperand* pXorArg = operands[0];
		Qoperand* pOperand = operands[1];
		if (operands[1]->identity() == "^")
		{
			pOperand = operands[0];
			Qoperand* pXorArg = operands[1];
		}

		Qadder* pAdder = new Qadder;
		OperandList operands_adder;
		operands_adder.push_back(dynamic_cast<Qop*>(pXorArg)->arguments()[0]);
		operands_adder.push_back(dynamic_cast<Qop*>(pXorArg)->arguments()[1]);
		operands_adder.push_back(pOperand);

		pAdder->arguments(operands_adder);
		return pAdder;
	}
	Qop::arguments(operands);
	return this;
}

string Qxor::toString() const
{
	return QbiOperation::toString();
}


/*** AdderOperation ***/

Qadder::Qadder()
	: QtriOperation("+")
{}

Qadder::Qadder(const Qadder& right)
	: QtriOperation(right)
{}

Qadder::~Qadder()
{}


string Qadder::toString() const
{
	return QtriOperation::toString();
}

/*** OperandFactory ***/

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