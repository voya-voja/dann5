#include <math.h> 
#include <iostream>

#include <Qexpression.h>
#include <Factory.h>

#include <Logger.h>

using namespace dann5::ocean;


/*** Qexpression ***/

Qexpression::Qexpression()
	: qbit_def_vector()
{
	_lct(toString());
}

Qexpression::Qexpression(const Qexpression& right)
	: qbit_def_vector(right)
{
	_lct(toString());
}

Qexpression::Qexpression(const Qdef& symbol)
	: qbit_def_vector(symbol.rows())
{
	for (Index at = 0; at < rows(); at++)
		if (at < symbol.rows())
			(*this)(at) =symbol(at);
	_lct(toString());
}

Qexpression::~Qexpression()
{
	_ldt(toString());
}

Qexpression Qexpression::operator *(const Qdef& right) const
{
	Qexpression re(right);
	return operator*(re);
}

Qexpression& Qexpression::operator *=(const Qdef& right)
{
	Qexpression re(right);
	return operator*=(re);
}

Qexpression Qexpression::operator *(const Qexpression& right) const
{
	Qexpression result(*this);
	result *= right;
	return result;
}

Qexpression& Qexpression::operator *=(const Qexpression& right)
{
	qbit_def_matrix xMatrix = thisX(right);	// this * right vectors => xMatrix
	// Rotate xMatrix left-90-digrees to change the values on main diagonal
	qbit_def_matrix reversedXMatrix = xMatrix.rowwise().reverse();
	// sum values on all diagonals into 
	sumDiagonals(reversedXMatrix);
	return *this;
}

qbit_def_matrix Qexpression::thisX(const Qexpression& right) const
{
	qbit_def_matrix xMatrix(rows(), right.rows());
	int carry = 0;
	std::cout << "Matrix " << rows() << " x " << right.rows() << endl;
	Qoperands operands;
	for (Index atR = 0; atR < rows(); atR++)
	{
		for (Index atC = 0; atC < right.rows(); atC++)
		{
			Qop::Sp pOp = Factory<string, Qop>::Instance().create("&");
			operands.push_back((*this)(atR));
			operands.push_back(right(atC));
			pOp->arguments(operands);
			xMatrix(atR, atC) = pOp;
			operands.clear();
		}
	}
	return xMatrix;
}

void Qexpression::sumDiagonals(const qbit_def_matrix& xMatrix)
{
	// resize this vector to a # of diagonals + 1 of xMatrix
	Index size = xMatrix.rows() + xMatrix.cols();
	resize(size);
	Index last = size - 1;

	// Value of 0 bit is the sum of right-most diagonal
	Index atDiagonal = xMatrix.cols() - 1;

	// add functional object to add diagonal vectors' bit symbols
	Add add;
	for ( 0; *add < last; add++)
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
	if(pOperand != nullptr)
	{
		(*this)(last) = pOperand;	// add any carry-forward operands
		_lat("sumDiagonals", (*this)(last)->toString());
	}
	else
	{
		resize(last);
	}
}


Qexpression Qexpression::operator +(const Qdef& right) const
{
	Qexpression re(right);
	return operator+(re);
}

Qexpression& Qexpression::operator +=(const Qdef& right)
{
	Qexpression re(right);
	return operator +=(re);
}

Qexpression Qexpression::operator +(const Qexpression& right) const
{
	Index tSize = rows(), rSize = right.rows();
	Index size = rSize > tSize ? rSize : tSize;
	// allocate extra bit for result for a last carry bit
	Index allocateSize = size + 1;
	if (tSize == 0 || rSize == 0)
		allocateSize = size;
	Qexpression result(size + 1);
	// add this and right vectors bit symbols
	Add add;
	// number of rows of this vector is one less then in result vector
	for (; *add < size; add++)
	{
		Index at = *add;
		if (at < tSize)
		{
			Qoperand::Sp pLeft((*this)(at)), pRight(nullptr), pResult(nullptr);
			if (at < rSize)
				pRight = right(at);
			if (pLeft == nullptr)
				pResult = add();
			else
				pResult = add(pLeft);
			if (pRight != nullptr)
				pResult = add(pResult, pRight);
			result(at) = pResult;
		}
		else
			result(at) = add(right(at));

		_lat("+", result(at)->toString());
	}
	result(*add) = add();
	while (result(*add) == nullptr)
	{
		size--;
		add--;
	}
	if (size + 1 < result.rows())
		result.resize(size + 1);
	_lat("+", result(*add)->toString());
	return result;
}

Qexpression& Qexpression::operator +=(const Qexpression& right)
{
	(*this) = (*this) + right;
	return (*this);
}

string Qexpression::toString() const
{
	Index size = rows();
	string tStr = "eXp[" + to_string(size) + "]={";
	string s = "size = " + size;
	for (Index at = 0; at < size; at++)
	{
		if ((*this)(at) == nullptr) 
			tStr += "? | ";
		else
			tStr += (*this)(at)->toString() + " | ";
	}
	tStr += "}";
	return tStr;
}

void Qexpression::resize(Index size)
{
	Index oSize = rows();
	Qexpression temp(*this);
	qbit_def_vector::resize(size, NoChange);
	for (Index at = 0; at < size; at++)
		if (at < oSize)
			(*this)(at) = temp(at);
		else
			(*this)(at) = Qoperand::Sp(new Qoperand(""));
}

std::ostream& dann5::ocean::operator << (std::ostream& out, const Qexpression& right)
{
	out << right.toString();
	return out;
}


Qexpression::Add::Add()
	:mAtBit(0)
{
	_lc;
}

Qexpression::Add::Add(Index startBitIndex)
	:mAtBit(startBitIndex)
{
	_lc;
}

Qexpression::Add::~Add()
{
	_ld;
}

Qoperand::Sp Qexpression::Add::operator () (const Qoperand::Sp& pLeft, const Qoperand::Sp& pRight)
{
	// Create new addition operation as xor instance
	Qaddition::Sp pAddition = dynamic_pointer_cast<Qaddition>(Factory<string, Qop>::Instance().create("^"));

	// assign 2 operands to addition operation
	Qoperands operands;
	operands.push_back(pLeft);
	operands.push_back(pRight);
	Qaddition::Sp pAdder = pAddition->assign(operands);

	// change addition operation, if assignment upgraded xor (binary) to adder (trinary) operation
	if (pAdder != nullptr) pAddition = pAdder;

	// push carry operand into FIFO to be processes while adding of next 2 bits
	CarryItem carryItem(mAtBit + 1, pAddition->carry());
	mCarryFIFO.push_back(carryItem);

	return pAddition;
}

Qoperand::Sp Qexpression::Add::operator () (const Qoperand::Sp& pLeft)
{
	Qoperand::Sp pOperand = pLeft;	// return operand argument, if carry FIFO is empty

	// while there are carry bits in FIFO to be added at current bit position
	while (!mCarryFIFO.empty() && mCarryFIFO.begin()->first <= mAtBit)
	{
		// pop the first carry operand from  the FIFO
		Qoperand::Sp pCarry = mCarryFIFO.begin()->second;
		mCarryFIFO.erase(mCarryFIFO.begin());
		
		if (!pOperand->doesExist(pCarry))
		{
			// add operand and carry bits
			pOperand = (*this)(pOperand, pCarry);
		}
	}
	return(pOperand);
}

Qoperand::Sp Qexpression::Add::operator () ()
{
	// return null if there are no carry operands in the FIFO
	Qoperand::Sp pOperand = nullptr;

	if(!mCarryFIFO.empty())
	{
		// pop the first carry operand from the FIFO
		pOperand = mCarryFIFO.begin()->second;
		mCarryFIFO.erase(mCarryFIFO.begin());

		// add the first carry operand to other carry operands in FIFO at this bit position
		pOperand = (*this)(pOperand);
	}
	return(pOperand);
}