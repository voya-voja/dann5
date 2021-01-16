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


		// Quantum rutine is a Q definition of sequence of logical Q statements
		class Qrutine : public Qdef
		{
		public:
			// Qrutine's shared pointer 
			typedef shared_ptr<Qrutine> Sp;

			// creates Q rutine with a name 
			Qrutine(const string&);

			// creates Q rutine with a rutine name and size of resulting variable
			Qrutine(Index size, const string&);

			// creates a named Q rutine with a given Q statement 
			Qrutine(const string&, const Qstatement&);

			// creates a named Q rutine with a given list of Q statements
			Qrutine(const string&, const Qstatements&);

			//copy constructor
			Qrutine(const Qrutine&);

			// desruct the Q equation with its members
			~Qrutine();

			// An insertion operator (<<) to add a new statement into this Q rutine
			Qrutine& operator<<(const Qstatement& right);

			// An insertion operator (<<) to add statements from an existing
			// rutine into this one
			Qrutine& operator<<(const Qrutine& right);

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
			// this Q rutine
			string solutions() const;

			// Returns a qubo representation of this Q rutine, 
			// if not finalized, returns a full qubo definition representation
			// if finalized, returns an expression that replaces symbols with values of
			// Qbits in deterministic states for all the Q variables within Q rutine
			Qubo qubo(bool finalized = true) const;

			// Returns a string representation of this Q rutine, 
			// if not decomposed, returns an equation line per Qbit level
			// if decomposed, returns a line per Qbit operational expressions
			string toString(bool decomposed = false) const;

			// Return a shared pointer to a copy of this Q definition
			virtual Qdef::Sp clone() const { return Sp(new Qrutine(*this)); };

			// Insert string representation of a Q rutine into an output stream
			friend std::ostream& operator << (std::ostream&, const Qrutine&);

		protected:
			class Operand : public Qoperand
			{
			public:
				// Qoperand's shared pointer 
				typedef shared_ptr<Operand> Sp;

				// Instantiate a Q operand with identity
				Operand(Index level, Qrutine& pRutine);

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
				Qrutine* mpRutine;
			};

			void initialize(Index size);

			virtual Qoperand::Sp operand(Index level);

		private:
			Qstatements	mStatements;
			Samples	mSolutions;
		};
	};
};