#pragma once

#include <pybind11/pybind11.h>

#include <utility>
#include <Eigen/Dense>

#include <Qvar.h>
#include <Qcomparison.h>
#include <Qroutine.h>

using namespace std;
using namespace Eigen;


namespace dann5 {
	namespace ocean {


		// Quantum condition is a coupling of result Q variable via Q expression with
		// Qvar arguments
		class Qcondition : public Qcomparison
		{
		public:
			// Qcondition's shared pointer 
			typedef shared_ptr<Qcondition> Sp;

			// Default constructor creates Q condition witout an Q comparison expression
			// or arguments
			Qcondition();

			// creates Q condition with a given Q expression and variable as an initial 
			// arguments
			Qcondition(const Qexpression&, const Qvars&);

			// instantiate a Q condition with a Q condition expression, corresponding 
			// Q variables as arguments of the expression and conditional Q code rutine
			Qcondition(const Qexpression&, const Qvars&, const Qroutine&);

			// instantiate a Q condition with an initialzed Q comparison object
			Qcondition(const Qcomparison&);

			//copy constructor
			Qcondition(const Qcondition&);

			// desruct the Q condition with its members
			~Qcondition();

			// Assignment operator returns a reference of this Q condition with Q result,
			// expression and arguments same as in right Qcondition
			Qcondition& operator=(const Qcondition& right);

			// An insertion operator (<<) to add a new statement into this Q contition actins
			Qroutine& operator<<(const Qstatement& right);

			// An insertion operator (<<) to add statements from an existing
			// routine into this Q condition actions
			Qroutine& operator<<(const Qroutine& right);

			// Returns a string representation of this Q condition, 
			// if not decomposed, returns an condition line per Qbit level
			// if decomposed, returns a line per Qbit operational expression
			virtual string toString(bool decomposed = false, Index level = Eigen::Infinity) const;

			// returns a shared_pointer on a cloned instance of this Q statement
			virtual Qstatement::Sp clone() const { return Qstatement::Sp(new Qcondition(*this)); };

			// Insert string representation of a Q condition into an output stream
			friend std::ostream& operator << (std::ostream&, const Qcondition&);

			// Add a sample with a node list defined by qubo() of this Q equation
			virtual void add(Qsolver::Sample& sample);

			// Set a sample set with a node list defined by qubo() of this Q equation
			// the combination of node values should be different for each sample
			virtual void set(Qsolver::Samples& samples);

			// For existing samples, returns a string representation of all solutions of 
			// this Q routine
			virtual string solutions() const;

		protected:
			virtual Qubo initQubo(bool finalized) const { return mAction.qubo(finalized); };

		private:
			Qroutine			mAction;		// a routine to be activated when condition is true
		};
	};
};