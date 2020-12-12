#pragma once

#include <pybind11/pybind11.h>

#include <utility>
#include <Eigen/Dense>

#include <Qubo.h>

using namespace std;
using namespace Eigen;


namespace dann5 {
	namespace ocean {

		// Quantum operand as an argument of a Q operation is a component of Q expression  
		class Qoperand
		{
		public:
			// Qoperand's shared pointer 
			typedef shared_ptr<Qoperand> Sp;

			// Instantiate an empty Q operand
//			Qoperand();
			// Instantiate a Q operand with identity
			Qoperand(const string& id);

			// Copy constructor
			Qoperand(const Qoperand&);

			// destruct the operand
			~Qoperand();

			// Return the Qoperand's identity string
			string identity() const { return mIdentity; };

			// Set a new Qoperand identity
			void identity(const string& id) { mIdentity = id; };

			// Return true, if the identities are equal
			bool isSame(const Qoperand::Sp& pRight) const { return(this == pRight.get()); }

			// Return true, if the pRight operand exists as a part of this operand
			bool doesExist(const Qoperand::Sp& pRight) const;

			// Return a shared pointer to a copy of this Qoperand
			virtual Qoperand::Sp clone() const { return Sp(new Qoperand(*this)); };

			// return string presentation of this Qoperand
			virtual string toString(bool decomposed = false) const;

			// return Qubo presentation of this Qoperand
			virtual Qubo qubo() const;

			// Insert string representation of a Qoperand into an output stream
			friend std::ostream& operator << (std::ostream&, const Qoperand&);
		protected:
		private:
			string mIdentity;	// The identity of a Qoperand
		};
	};
};