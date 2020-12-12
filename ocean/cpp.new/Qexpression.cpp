#include <math.h> 
#include <iostream>

#include <Qexpression.h>
#include <Factory.h>

#include <Logger.h>

using namespace dann5::ocean;


/*** Qexpression ***/

Qexpression::Qexpression()
	: Qoperands()
{
	_lct(toString());
}

Qexpression::Qexpression(const Qexpression& right)
	: Qoperands(right)
{
	_lct(toString());
}

Qexpression::Qexpression(Index size)
	:Qoperands(size)
{
/*	for (Index at = 0; at < rows(); at++)
		(*this)(at) = NULL;
*/	_lct(toString());
}

Qexpression::Qexpression(const Qdef& operands)
	: Qoperands(operands.rows())
{
	for (Index at = 0; at < operands.rows(); at++)
		(*this)(at).reset(new Qoperand(operands(at)));
	_lct(toString());
}

Qexpression::~Qexpression()
{
	_ldt(toString());
	for (auto &pOp : *this)
	{
/*		try
		{
			if(pOp != NULL)
				delete pOp;
		}
		catch (...) {}
*/
	}
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
	Xmatrix xMatrix = thisX(right);	// this * right vectors => xMatrix
	// Rotate xMatrix left-90-digrees to change the values on main diagonal
	Xmatrix reversedXMatrix = xMatrix.rowwise().reverse();
	// sum values on all diagonals into 
	sumDiagonals(reversedXMatrix);
	return *this;
}

Qexpression::Xmatrix Qexpression::thisX(const Qexpression& right) const
{
	Xmatrix xMatrix(rows(), right.rows());
	int carry = 0;
	std::cout << "Matrix " << rows() << " x " << right.rows() << endl;
	OperandList operands;
	for (Index atR = 0; atR < rows(); atR++)
	{
		for (Index atC = 0; atC < right.rows(); atC++)
		{
			Qop* pOp = Factory<string, Qop>::Instance().create("&");
			operands.push_back((*this)(atR).get());
			operands.push_back(right(atC).get());
			xMatrix(atR, atC) = pOp->arguments(operands);
			std::cout << *xMatrix(atR, atC) << " ";
			operands.clear();
		}
		std::cout << endl;
	}
	return xMatrix;
}

void Qexpression::sumDiagonals(const Qexpression::Xmatrix& xMatrix)
{
	// resize this vector to a # of diagonals + 1 of xMatrix
	Index size = xMatrix.rows() + xMatrix.cols();
	resize(size, NoChange);
	Index last = size - 1;

	// Value of 0 bit is the sum of right-most diagonal
	Index atDiagonal = xMatrix.cols() - 1;

	// add functional object to add diagonal vectors' bit symbols
	Add add;
	for (Index at = 0; at < last; at++)
	{
		bool qbitInitialized = add.isCarryForward();
		(*this)(at) = add();	// add any carry-forward operands

		Qoperands diagonal = xMatrix.diagonal(atDiagonal);
		Index nDiagElmns = diagonal.rows();
		for (Index atD = 0; atD < nDiagElmns; atD++)
		{
			if (!qbitInitialized)
			{
				(*this)(at) = diagonal(atD);
				qbitInitialized = true;
			}
			else
			{
				(*this)(at) = add((*this)(at), diagonal(atD));
			}
		}
		Index atNext = at + 1;
		--atDiagonal;
		cout << *(*this)(at) << endl;
	}
	(*this)(last) = add();	// add any carry-forward operands
	cout << *(*this)(last) << endl;
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
	Qexpression result(size + 1);
	// add this and right vectors bit symbols
	Add add;
	// number of rows of this vector is one less then in result vector
	for (Index at = 0; at < size; at++)
	{
		if (at < tSize)
		{
			SpQoperand pResult = add((*this)(at));
			if (at < rSize)
				pResult = add(pResult, right(at));
			result(at) = pResult;
		}
		else
			result(at) = add(right(at));

		cout << *result(at) << endl; // TBR
	}
	result(size) = add();
	cout << *result(size) << endl;
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
		if ((*this)(at).get() == 0) 
			tStr += "? ";
		else
			tStr += (*this)(at)->toString() + " ";
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
{
	_lc;
}

Qexpression::Add::~Add()
{
	_ld;
}

SpQoperand Qexpression::Add::construct(const SpQoperand& pLeft, const SpQoperand& pRight)
{
	OperandList operands;
	operands.push_back(pLeft.get());
	operands.push_back(pRight.get());
	Qxor* pAdd = dynamic_cast<Qxor*>(Factory<string, Qop>::Instance().create("^"));

	mLastAssignment = pAdd->assign(operands);
	mUseLastAssignmentCarryForward = true;
	return(SpQoperand(dynamic_cast<Qoperand*>(mLastAssignment.mpOp)));
}

SpQoperand Qexpression::Add::operator () (const SpQoperand& pLeft, const SpQoperand& pRight)
{
	SpQoperand pOperand = construct(pLeft, pRight);
	mCarryForwards.push_back(mLastAssignment.mpCarryForward);
	mUseLastAssignmentCarryForward = false;

	return(pOperand);
}

SpQoperand Qexpression::Add::operator () (const SpQoperand& pLeft)
{
	SpQoperand pOperand(&*pLeft);
	while (!mCarryForwards.empty())
	{
		// pop first element from  the carry-forward queue
		SpQoperand pCarry(*mCarryForwards.begin());
		if (!pLeft->isExist(&*pCarry))
		{
			pOperand = construct(pOperand, pCarry);
		}
		mCarryForwards.erase(mCarryForwards.begin());
	}
	return(pOperand);
}

SpQoperand Qexpression::Add::operator () ()
{
	if (mCarryForwards.empty() && !mUseLastAssignmentCarryForward)
		return SpQoperand(new Qoperand);

	SpQoperand pOperand;

	if(!mCarryForwards.empty())
	{
		// pop first element from the carry-forward queue
		pOperand.reset(*mCarryForwards.begin());
		mCarryForwards.erase(mCarryForwards.begin());
		pOperand = (*this)(pOperand);
	}
	if (mUseLastAssignmentCarryForward)
	{
		pOperand = (*this)(SpQoperand(mLastAssignment.mpCarryForward));
	}
	return(pOperand);
}