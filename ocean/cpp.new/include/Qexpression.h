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
		typedef shared_ptr<Qoperand> SpQoperand;
		typedef Matrix<SpQoperand, Dynamic, 1> Qoperands;

		class Qexpression : public Qoperands
		{
		public:
			class Add
			{
			private:
				OperandList mCarryForwards;

				Qaddition::Assignment mLastAssignment;
				bool mUseLastAssignmentCarryForward = false;

				SpQoperand construct(const SpQoperand&, const SpQoperand&);

			public:
				Add();
				~Add();

				// operator function () returns result operand by Add-ing left and right operand
				SpQoperand operator () (const SpQoperand&, const SpQoperand&);
				// operator function () returns result operand by Add-ing left and carry operands
				SpQoperand operator () (const SpQoperand&);
				// operator function () returns result operand by Add-ing carry operands
				SpQoperand operator () ();

				bool isCarryForward() const {return(!mCarryForwards.empty()); };
				bool isNeedCorretion() const { return(mLastAssignment.mpCarryForward != NULL); };
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

			string toString() const;

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