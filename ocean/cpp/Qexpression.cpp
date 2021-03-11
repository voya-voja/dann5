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

Qexpression::Qexpression(const Qdef& definition)
	: qbit_def_vector(definition.nobs())
{
	for (Index at = 0; at < nobs(); at++)
		if (at < definition.nobs())
			(*this)(at) =definition(at);
	_lct(toString());
}

Qexpression::~Qexpression()
{
	_ldt(toString());
}


Qexpression Qexpression::operator~() const
{
	Index size(nobs());
	Qexpression result(size);
	for (Index at = 0; at < size; at++)
	{
		Qop::Sp pOp = Factory<string, Qop>::Instance().create(NotQT::cMark);
		pOp->arguments({ (*this)(at) });
		result(at) = pOp;
	}
	return(*this);
}

Qexpression Qexpression::operation(const string& opMark, const Qexpression& right) const
{
	Index tSize = nobs(), rSize = right.nobs();
	Index size = rSize > tSize ? rSize : tSize;
	Qexpression result(size);
	for (Index at = 0; at < size; at++)
	{
		if (at < tSize)
		{
			Qoperand::Sp pLeft((*this)(at)), pRight(nullptr), pResult(nullptr);
			if (at < rSize)
				pRight = right(at);
			if (pLeft != nullptr && pRight != nullptr)
			{

				Qop::Sp pOp = Factory<string, Qop>::Instance().create(opMark);
				pOp->arguments({ pLeft, pRight });
				pResult = pOp;
			}
			else if (pLeft != nullptr)
				pResult = pLeft;
			else if (pRight != nullptr)
				pResult = pRight;
			result(at) = pResult;
		}
		else
			result(at) = right(at);
	}
	_lat(opMark, result.toString());
	return(result);
}

Qexpression Qexpression::comparisonOp(const string& opMark, const string& compMark, const Qexpression& right) const
{
	Index tSize = nobs(), rSize = right.nobs();
	Index size = rSize > tSize ? rSize : tSize;
	Qexpression result(size);
	Qexpression compare = (*this) + ~right;
	for (Index at = 0; at < size; at++)
	{
		if (at < tSize)
		{
			Qoperand::Sp pLeft((*this)(at)), pRight(nullptr), pResult(nullptr);
			if (at < rSize)
				pRight = right(at);
			if (pLeft != nullptr && pRight != nullptr)
			{

				Qop::Sp pOp = nullptr;
				if(at == 0) 
					pOp = Factory<string, Qop>::Instance().create(opMark);
				else
					pOp = Factory<string, Qop>::Instance().create(compMark);
				pOp->add(pLeft);
				pOp->add(pRight);
				if (at < size - 1)
				{
					pLeft = (*this)(at + 1);
					if (at + 1 < rSize)
						pRight = right(at + 1);
					Qop::Sp pEq = Factory<string, Qop>::Instance().create(EqQT::cMark);
					pEq->arguments({ pLeft, pRight });
					Qop::Sp pAnd = Factory<string, Qop>::Instance().create(AndQT::cMark);
					pAnd->arguments({ pEq, pOp });
					pOp = pAnd;
				}
				pResult = pOp;
			}
			else if (pLeft != nullptr)
				pResult = pLeft;
			else if (pRight != nullptr)
				pResult = pRight;
			result(at) = pResult;
		}
		else
			result(at) = right(at);
	}
	_lat(opMark, result.toString());
	return(result);
}

Qexpression Qexpression::operator ==(const Qdef& right) const
{
	Qexpression re(right);
	return operator ==(re);
}

Qexpression Qexpression::operator ==(const Qexpression& right) const
{
	Qexpression result = operation(EqQT::cMark, right);
	return result;
}

Qexpression Qexpression::operator !=(const Qdef& right) const
{
	Qexpression re(right);
	return operator !=(re);
}

Qexpression Qexpression::operator !=(const Qexpression& right) const
{
	Qexpression result = operation(NotQT::cMark, right);
	return(result);
}

Qexpression Qexpression::operator >(const Qdef& right) const
{
	Qexpression re(right);
	return operator >(re);
}

Qexpression Qexpression::operator >(const Qexpression& right) const
{
	Qexpression result = comparisonOp(GtQT::cMark, GeQT::cMark, right);
	return(result);
}

Qexpression Qexpression::operator >=(const Qdef& right) const
{
	Qexpression re(right);
	return operator >=(re);
}

Qexpression Qexpression::operator >=(const Qexpression& right) const
{
	Qexpression compare = (*this) + ~right;
	Index size = compare.nobs();
	Qexpression result(size);
	for (Index at = 0; at < size; at++)
	{
		Qop::Sp pEq = Factory<string, Qop>::Instance().create(GeQT::cMark);
//		pEq->arguments({ pLeft, pRight });
	}
	return(result);
}

Qexpression Qexpression::operator <(const Qdef& right) const
{
	Qexpression re(right);
	return operator <(re);
}

Qexpression Qexpression::operator <(const Qexpression& right) const
{
	
	Qexpression result = comparisonOp(LtQT::cMark, LeQT::cMark, right);
	return(result);
}

Qexpression Qexpression::operator <=(const Qdef& right) const
{
	Qexpression re(right);
	return operator <=(re);
}

Qexpression Qexpression::operator <=(const Qexpression& right) const
{
	Qexpression result = comparisonOp(LeQT::cMark, LeQT::cMark, right);
	return(result);
}

Qexpression Qexpression::operator &(const Qdef& right) const
{
	Qexpression re(right);
	return operator&(re);
}

Qexpression& Qexpression::operator &=(const Qdef& right)
{
	Qexpression re(right);
	return operator &=(re);
}

Qexpression Qexpression::operator &(const Qexpression& right) const
{
		return operation(AndQT::cMark, right);
}

Qexpression& Qexpression::operator &=(const Qexpression& right)
{
	(*this) = (*this) & right;
	return (*this);
}

Qexpression Qexpression::operator |(const Qdef& right) const
{
	Qexpression re(right);
	return operator|(re);
}

Qexpression& Qexpression::operator |=(const Qdef& right)
{
	Qexpression re(right);
	return operator |=(re);
}

Qexpression Qexpression::operator |(const Qexpression& right) const
{
	return operation(OrQT::cMark, right);
}

Qexpression& Qexpression::operator |=(const Qexpression& right)
{
	(*this) = (*this) | right;
	return (*this);
}

Qexpression Qexpression::nand(const Qdef& right) const
{
	Qexpression re(right);
	return nand(re);
}

Qexpression& Qexpression::nandMutable(const Qdef& right)
{
	Qexpression re(right);
	return nandMutable(re);
}

Qexpression Qexpression::nand(const Qexpression& right) const
{
	return operation(NandQT::cMark, right);
}

Qexpression& Qexpression::nandMutable(const Qexpression& right)
{
	(*this) = this->nand(right);
	return (*this);
}

Qexpression Qexpression::nor(const Qdef& right) const
{
	Qexpression re(right);
	return nor(re);
}

Qexpression& Qexpression::norMutable(const Qdef& right)
{
	Qexpression re(right);
	return norMutable(re);
}

Qexpression Qexpression::nor(const Qexpression& right) const
{
	return operation(NorQT::cMark, right);
}

Qexpression& Qexpression::norMutable(const Qexpression& right)
{
	(*this) = this->nor(right);
	return (*this);
}

Qexpression Qexpression::operator ^(const Qdef& right) const
{
	Qexpression re(right);
	return operator^(re);
}

Qexpression& Qexpression::operator ^=(const Qdef& right)
{
	Qexpression re(right);
	return operator ^=(re);
}

Qexpression Qexpression::operator ^(const Qexpression& right) const
{
	return operation(XorQT::cMark, right);
}

Qexpression& Qexpression::operator ^=(const Qexpression& right)
{
	(*this) = (*this) ^ right;
	return (*this);
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
	Index tSize = nobs(), rSize = right.nobs();
	Index size = rSize > tSize ? rSize : tSize;
	// allocate extra bit for result for a last carry bit
	Index allocateSize = size + 1;
	if (tSize == 0 || rSize == 0)
		allocateSize = size;
	Qexpression result(allocateSize);
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
			{
				pRight = right(at);
			}
			if (pLeft == nullptr)
			{
				pResult = add();
			}
			else if (pRight != nullptr)
			{
				pResult = add(pLeft, pRight);
				pResult = add(pResult);
			}
			else
			{
				pResult = add(pLeft);
			}
			result(at) = pResult;
		}
		else
			result(at) = add(right(at));
	}
	result(*add) = add();
	while (result(*add) == nullptr)
	{
		size--;
		add--;
	}
	if (size + 1 < result.nobs())
		result.resize(size + 1);
	_lat("+", result.toString());
	return result;
}

Qexpression& Qexpression::operator +=(const Qexpression& right)
{
	(*this) = (*this) + right;
	return (*this);
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

void Qexpression::resize(Index size)
{
	Index oSize = nobs();
	Qexpression temp(*this);
	qbit_def_vector::resize(size, NoChange);
	for (Index at = 0; at < size; at++)
		if (at < oSize)
			(*this)(at) = temp(at);
		else
			(*this)(at) = Qoperand::Sp(new Qoperand(""));
}

string Qexpression::toString() const
{
	Index size = nobs();
	string tStr = "eXp[" + to_string(size) + "]={";
	string s = "size = " + size;
	for (Index at = 0; at < size; at++)
	{
		if ((*this)(at) == nullptr)
			tStr += "? | ";
		else
			tStr += (*this)(at)->toString() + "; ";
	}
	tStr += "}";
	return tStr;
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
	Qaddition::Sp pAddition = dynamic_pointer_cast<Qaddition>(Factory<string, Qop>::Instance().create(XorQT::cMark));

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