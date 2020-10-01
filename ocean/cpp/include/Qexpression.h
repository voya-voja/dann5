#pragma once

#include <pybind11/pybind11.h>

#include <utility>
#include <Eigen/Dense>

#include <Qdef.h>
#include <Qops.h>

using namespace std;
using namespace Eigen;


namespace dann5 {
	namespace ocean {

		typedef Matrix<Qoperand*, Dynamic, 1> Qoperands;

		class Qexpression : public Qoperands
		{
		public:
			class Add
			{
			private:
				OperandList mOperands;
				OperandList mCarryForwards;
			public:
				Add();

				// operator function () returns result operand by Add-ing left and right operand
				Qoperand* operator () (Qoperand*, Qoperand*);
				// operator function () returns result operand by Add-ing left and carry operands
				Qoperand* operator () (Qoperand*);
				// operator function () returns result operand by Add-ing carry operands
				Qoperand* operator () ();
				bool isCarryForward() const;
			};

			Qexpression();
			Qexpression(const Qexpression&);
			Qexpression(const Qdef&);
			~Qexpression();

			Qexpression operator *(const Qdef&) const;
			Qexpression& operator *=(const Qdef&);
			Qexpression operator *(const Qexpression&) const;
			Qexpression& operator *=(const Qexpression&);

			Qexpression operator +(const Qdef&) const;
			Qexpression& operator +=(const Qdef&);
			Qexpression operator +(const Qexpression&) const;
			Qexpression& operator +=(const Qexpression&);

			friend std::ostream& operator << (std::ostream&, const Qexpression&);
		protected:
			Qexpression(Index size);
			typedef Matrix<Qoperand*, Dynamic, Dynamic> Xmatrix;

			virtual Xmatrix thisX(const Qexpression&) const;
			virtual void sumDiagonals(const Xmatrix&);

		private:
		};
	};
};