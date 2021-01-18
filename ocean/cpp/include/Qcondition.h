#pragma once

#include <pybind11/pybind11.h>

#include <utility>
#include <Eigen/Dense>

#include <Qvar.h>
#include <Qstatement.h>
#include <Qroutine.h>

using namespace std;
using namespace Eigen;


namespace dann5 {
	namespace ocean {


		// Quantum equation is a coupling of result Q variable via Q expression with
		// Qvar arguments
		class Qcondition : public Qstatement
		{
		public:
			// Default constructor creates Q equation witout a result Q variable so the
			// result definition is an empty string
			Qcondition();

			// creates Q equation with a given Q variable as an expected result
			Qcondition(const Qroutine&);

			// instantiate a Q equation with result Q variable, resulting Q expression
			// and corresponding Q variables as arguments of the expression
			Qcondition(const Qroutine&, const Qexpression&, const Qvars&);

			//copy constructor
			Qcondition(const Qcondition&);

			// desruct the Q equation with its members
			~Qcondition();

			// Assignment operator returns a reference of this Q equation with Q result,
			// expression and arguments same as in right Qcondition
			Qcondition& operator=(const Qcondition& right);

			// An insertion operator (<<) to add a new statement into this Q contition actins
			Qcondition& operator<<(const Qstatement& right);

			// An insertion operator (<<) to add statements from an existing
			// routine into this Q condition actions
			Qcondition& operator<<(const Qroutine& right);

			// A condition negation
			Qcondition& operator~();

			// And operator returns a new Q condition object with expresion this & right, 
			// and with right action concatenated to this object action
			Qcondition operator&(const Qcondition& right) const;

			// And operator returns a reference to this Qcondition object with expresion
			// this & right, and right action concatenated to this object action
			Qcondition& operator&=(const Qcondition& right);

			// Or operator returns a reference to this Qcondition object with expresion
			// this | right, and right action concatenated to this object action
			Qcondition operator|(const Qcondition& right) const;

			// Or operator returns a new Q condition object with expresion this | right, 
			// and with right action concatenated to this object action
			Qcondition& operator|=(const Qcondition& right);

			// Xor operator returns a reference to this Qcondition object with expresion
			// this ^ right, and right action concatenated to this object action
			Qcondition operator^(const Qcondition& right) const;

			// Xor operator returns a new Q condition object with expresion this ^ right, 
			// and with right action concatenated to this object action
			Qcondition& operator^=(const Qcondition& right);

			// Or operator returns a reference to this Qcondition object with expresion
			// this ~& right, and right action concatenated to this object action
			Qcondition nand(const Qcondition& right) const;

			// Or operator returns a new Q condition object with expresion this ~& right, 
			// and with right action concatenated to this object action
			Qcondition& nand(const Qcondition& right);

			// Or operator returns a reference to this Qcondition object with expresion
			// this ~| right, and right action concatenated to this object action
			Qcondition nor(const Qcondition& right) const;

			// Or operator returns a new Q condition object with expresion this ~| right, 
			// and with right action concatenated to this object action
			Qcondition& nor(const Qcondition& right);

			// Returns a qubo representation of this Q equation, 
			// if not finalized, returns a full qubo definition representation of this Q
			// equation
			// if finalized, returns an expression that replaces symbols with values of
			// Qbits in deterministic states for all the Q variables, i.e. result and
			// expression arguments
			Qubo qubo(bool finalized = true, Index level = Eigen::Infinity) const;

			// Returns a string representation of this Q equation, 
			// if not decomposed, returns an equation line per Qbit level
			// if decomposed, returns a line per Qbit operational expression
			virtual string toString(bool decomposed = false, Index level = Eigen::Infinity) const;

			// returns a shared_pointer on a cloned instance of this Q statement
			virtual Qstatement::Sp clone() const { return Qstatement::Sp(new Qcondition(*this)); };

			// Insert string representation of a Q equation into an output stream
			friend std::ostream& operator << (std::ostream&, const Qcondition&);

		protected:

		private:
			Qroutine			mAction;		// a routine to be activated when condition is true
		};
	};
};