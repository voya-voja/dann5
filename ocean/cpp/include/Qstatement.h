#pragma once

#include <pybind11/pybind11.h>

#include <utility>
#include <Eigen/Dense>

#include <Qvar.h>
#include <Qexpression.h>
#include <Qubo.h>

using namespace std;
using namespace Eigen;


namespace dann5 {
	namespace ocean {

		// Quantum equation is a coupling of result Q variable via Q expression with
		// Qvar arguments
		class Qstatement
		{
		public:
			// Qoperand's shared pointer 
			typedef shared_ptr<Qstatement> Sp;

			// Default constructor creates Q equation witout a result Q variable so the
			// result definition is an empty string
			Qstatement();

			// creates Q equation with a given Q variable as an expected result
			Qstatement(Index size);

			// instantiate a Q equation with result Q variable, resulting Q expression
			// and corresponding Q variables as arguments of the expression
			Qstatement(const Qexpression&, const Qvars&);

			//copy constructor
			Qstatement(const Qstatement&);

			// desruct the Q equation with its members
			~Qstatement();

			// Returns a number of bit level expressions in this Q equation
			Index nobs() const { return mExpression.rows(); }

			// Returns a constant reference to the expression of this Q equation
			const Qexpression& expression() const { return mExpression; }

			// Returns a constant reference to the expression of this Q equation
			Qexpression& expression() { return mExpression; }

			// Returns a constant reference to the expression arguments of this Q equation
			const Qvars& arguments() const { return mArguments; }

			// Returns a constant reference to the expression arguments of this Q equation
			Qvars& arguments() { return mArguments; }

			// Returns a qubo representation of this Q equation, 
			// if not finalized, returns a full qubo definition representation of this Q
			// equation
			// if finalized, returns an expression that replaces symbols with values of
			// Qbits in deterministic states for all the Q variables, i.e. result and
			// expression arguments
			virtual Qubo qubo(bool finalized = true, Index level = Eigen::Infinity) const = 0;

			// Returns a string representation of this Q equation, 
			// if not decomposed, returns an equation line per Qbit level
			// if decomposed, returns a line per Qbit operational expression
			virtual string toString(bool decomposed = false, Index level = Eigen::Infinity) const = 0;

			// returns a shared_pointer on a cloned instance of this Q statement
			virtual Qstatement::Sp clone() const = 0;

		protected:

		private:
			Qexpression		mExpression;	// an expresion, e.g. 'a + b + c', where a, b & c are Qvar instaces
			Qvars			mArguments;		// the list of expression variables, e.g. {a, b, c} for the Q equation above
		};

		// An std::vector containing a list of Q statements
		typedef vector<Qstatement::Sp> Qstatements;
	};
};