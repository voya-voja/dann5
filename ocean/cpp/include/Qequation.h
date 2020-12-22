#pragma once

#include <pybind11/pybind11.h>

#include <utility>
#include <Eigen/Dense>

#include <Qtype.h>
#include <Qdef.h>
#include <Qvar.h>
#include <Qexpression.h>
#include <Qubo.h>

using namespace std;
using namespace Eigen;


namespace dann5 {
	namespace ocean {


		typedef std::vector<Qvar>	Qvars;

		// Quantum equation is a coupling of result Q variable via Q expression with
		// Qvar arguments
		class Qequation
		{
		public:
			// Default constructor creates Q equation witout a result Q variable so the
			// result symbol is an empty string
			Qequation();

			// creates Q equation with a given Q variable as an expected result
			Qequation(const Qvar&);

			// instantiate a Q equation with result Q variable, resulting Q expression
			// and corresponding Q variables as arguments of the expression
			Qequation(const Qvar&, const Qexpression&, const Qvars&);

			//copy constructor
			Qequation(const Qequation&);

			// desruct the Q equation with its members
			~Qequation();

			// Assignment operator returns a reference of this Q equation with Q result,
			// expression and arguments same as in right Qequation
			Qequation& operator=(const Qequation& right);

			// Addition operator returns a new Qequation object with added Q variable
			// to this Q equation
			Qequation operator+(const Qvar& right) const;

			// Addition operator returns a new Qequation object resulting from addition
			// of this and right Q equations
			Qequation operator+(const Qequation& right) const;

			// Addition operator returns a reference to this Qequation object with added
			// Q variable
			Qequation& operator+=(const Qvar& right);

			// Addition operator returns a reference to this Qequation object with added
			// right Q equation
			Qequation& operator+=(const Qequation& right);

			// Multiplication operator returns a new Qequation object with multiplied Q
			// variable with this Q equation
			Qequation operator*(const Qvar& right) const;

			// Multiplication operator returns a new Qequation object resulting from
			// multiplication of this and right Q equations
			Qequation operator*(const Qequation& right) const;

			// Multiplication operator returns a reference to this Qequation object with
			// multiplied Q variable
			Qequation& operator*=(const Qvar& right);

			// Multiplication operator returns a reference to this Qequation object with
			// multiplied right Q equation
			Qequation& operator*=(const Qequation& right);

			// Returns a number of bit level expressions in this Q equation
			Index size() const { return mResult.symbol().rows(); }

			// Returns a constant reference to the result of this Q equation
			const Qvar& result() const { return mResult; }

			// Returns a constant reference to the expression of this Q equation
			const Qexpression& expression() const { return mExpression; }

			// Returns a constant reference to the expression arguments of this Q equation
			const Qvars& arguments() const { return mArguments; }

			// A semple is defined as a dictionary (map) of symbol nodes and their values.
			// The node names are defined by qubo() for each Q equation
			typedef map<string, q_bit> Sample;

			// A list of samples with the same list of nodes and different combination of values
			typedef vector<Sample> Samples;

			// Add a sample with a node list defined by qubo() of this Q equation
			void add(Sample& sample);

			// Set a sample set with a node list defined by qubo() of this Q equation
			// the combination of node values should be different for each sample
			void set(Samples& samples);

			// For existing samples, returns a string representation of all solutions of 
			// this Q euation
			string solutions() const;

			// Returns a qubo representation of this Q equation, 
			// if not finalized, returns a full qubo symbol representation of this Q
			// equation
			// if finalized, returns an expression that replaces symbols with values of
			// Qbits in deterministic states for all the Q variables, i.e. result and
			// expression arguments
			Qubo qubo(bool finalized = true) const;

			// Returns a string representation of this Q equation, 
			// if not decomposed, returns an equation line per Qbit level
			// if decomposed, returns a line per Qbit operational expression
			string toString(bool decomposed = false) const;

			// Insert string representation of a Q equation into an output stream
			friend std::ostream& operator << (std::ostream&, const Qequation&);

		protected:
			class Reduct
			{
				typedef std::pair<string, Qbit> Reduction;
				typedef std::map<string, Reduction> Reductions;
			public:
				static const QuboKey cSkip;
				Reduct(Qequation&);
				~Reduct();

				void operator() ();
				QuboKey operator()(const QuboKey&, bool) const;
			protected:
			private:
				Qequation&	mEquation;
				Reductions mReductions;
			};
		private:
			Qvar			mResult;		// result variable, e.g. defined as R = 2 with bits R0 = 0, R1 = 1, ...
			Qexpression		mExpression;	// an expresion, e.g. 'a + b + c', where a, b & c are Qvar instaces
			Qvars			mArguments;		// the list of expression variables, e.g. {a, b, c} for the Q equation above
			Reduct			mReduct;		// an instace of object class that symplifies the expression of this Q equation
			bool			mNoResult;
			Samples			mSolutions;

			friend class Reduct;
		};
	};
};