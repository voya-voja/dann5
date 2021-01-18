#pragma once

#include <pybind11/pybind11.h>

#include <utility>
#include <Eigen/Dense>

#include <Qvar.h>
#include <Qstatement.h>

using namespace std;
using namespace Eigen;


namespace dann5 {
	namespace ocean {


		// Quantum routine is a Q definition of sequence of logical Q statements
		class Qroutine : public Qdef
		{
		public:
			// Qroutine's shared pointer 
			typedef shared_ptr<Qroutine> Sp;

			// creates Q routine with a name 
			Qroutine(const string&);

			// creates Q routine with a routine name and size of resulting variable
			Qroutine(Index size, const string&);

			// creates a named Q routine with a given Q statement 
			Qroutine(const string&, const Qstatement&);

			// creates a named Q routine with a given list of Q statements
			Qroutine(const string&, const Qstatements&);

			//copy constructor
			Qroutine(const Qroutine&);

			// desruct the Q equation with its members
			~Qroutine();

			// Returns a qubo representation of this Q routine, 
			// if not finalized, returns a full qubo definition representation
			// if finalized, returns an expression that replaces symbols with values of
			// Qbits in deterministic states for all the Q variables within Q routine
			Qubo qubo(bool finalized = true) const;

			// Returns a string representation of this Q routine, 
			// if not decomposed, returns an equation line per Qbit level
			// if decomposed, returns a line per Qbit operational expressions
			string toString(bool decomposed = false) const;

			// Return a shared pointer to a copy of this Q definition
			virtual Qdef::Sp clone() const { return Sp(new Qroutine(*this)); };

			// An insertion operator (<<) to add a new statement into this Q routine
			Qroutine& operator<<(const Qstatement& right);

			// An insertion operator (<<) to add statements from an existing
			// routine into this one
			Qroutine& operator<<(const Qroutine& right);

			// A semple is defined as a dictionary (map) of definition nodes and their values.
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
			// this Q routine
			string solutions() const;

			// Insert string representation of a Q routine into an output stream
			friend std::ostream& operator << (std::ostream&, const Qroutine&);

		protected:
			class Operand : public Qoperand
			{
			public:
				// Qoperand's shared pointer 
				typedef shared_ptr<Operand> Sp;

				// Instantiate a Q operand with identity
				Operand(Index level, Qroutine& pRutine);

				// Copy constructor
				Operand(const Operand&);

				// destruct the operand
				~Operand();

				// Return a shared pointer to a copy of this Qoperand
				virtual Qoperand::Sp clone() const { return Sp(new Operand(*this)); };

				// return string presentation of this Qoperand
				virtual string toString(bool decomposed = false) const;

				// return Qubo presentation of this Qoperand
				virtual Qubo qubo(bool finalized = true) const;

			protected:
			private:
				Index mLevel;
				Qroutine* mpRutine;
			};

			void initialize(Index size);

			virtual Qoperand::Sp operand(Index level);

		private:
			Qstatements	mStatements;
			Samples	mSolutions;
		};
	};
};