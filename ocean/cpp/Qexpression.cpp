#include <math.h> 
#include <iostream>

#include <Qexpression.h>
#include <Factory.h>


using namespace dann5::ocean;


/*** Qexpression ***/

Qexpression::Qexpression()
	: Qoperands()
{
}

Qexpression::Qexpression(const Qexpression& right)
	: Qoperands(right)
{
}

Qexpression::Qexpression(Index size)
	:Qoperands(size)
{
}

Qexpression::Qexpression(const Qdef& operands)
	: Qoperands(operands.rows())
{
	for (Index at = 0; at < operands.rows(); at++)
		(*this)(at) = new Qoperand(operands(at));
}

Qexpression::~Qexpression()
{
/*	for (auto &pOp : *this)
		delete pOp;
		*/
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
			operands.push_back((*this)(atR));
			operands.push_back(right(atC));
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
			result(at) = add((*this)(at));
			if (at < rSize)
				result(at) = add(result(at), right(at));
		}
		else
			result(at) = add(right(at));
		cout << *result(at) << endl;
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


Qexpression::Add::Add()
{  }


Qoperand* Qexpression::Add::operator () (Qoperand* left, Qoperand* right)
{
	mOperands.push_back(left);
	mOperands.push_back(right);
	Qop* pAdd = Factory<string, Qop>::Instance().create("^");

	Qoperand* pResultAdd = pAdd->arguments(mOperands);
	Qoperand* pCarry = dynamic_cast<Qaddition*>(pResultAdd)->carry();
	mCarryForwards.push_back(pCarry);

	mOperands.clear();

	return(pResultAdd);
}

Qoperand* Qexpression::Add::operator () (Qoperand* pLeft)
{
	Qoperand* pResultAdd = pLeft;
	while (!mCarryForwards.empty())
	{
		mOperands.push_back(pResultAdd);

		// pop first element from  the carry-forward queue
		mOperands.push_back(*mCarryForwards.begin());
		mCarryForwards.erase(mCarryForwards.begin());

		Qop* pAdd = Factory<string, Qop>::Instance().create("^");

		pResultAdd = pAdd->arguments(mOperands);
		mOperands.clear();
	}

	return(pResultAdd);
}

Qoperand* Qexpression::Add::operator () ()
{
	if (mCarryForwards.empty()) return new Qoperand;

	// pop first element from the carry-forward queue
	Qoperand* pResultAdd = *mCarryForwards.begin();
	mCarryForwards.erase(mCarryForwards.begin());

	while (!mCarryForwards.empty())
	{
		mOperands.push_back(pResultAdd);

		// pop first element from  the carry-forward queue
		mOperands.push_back(*mCarryForwards.begin());
		mCarryForwards.erase(mCarryForwards.begin());

		Qop* pAdd = Factory<string, Qop>::Instance().create("^");

		pResultAdd = pAdd->arguments(mOperands);
		mOperands.clear();
	}

	return(pResultAdd);
}

bool Qexpression::Add::isCarryForward() const
{ 
	return !mCarryForwards.empty();
}


std::ostream& dann5::ocean::operator << (std::ostream& output, const Qexpression& right)
{
	for (Index at = 0; at < right.rows(); at++)
		output << *right(at) << endl;
	return output;
}