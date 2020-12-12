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
		// A matrix of Qoperand shared pointers
		typedef Matrix<Qoperand::Sp, Dynamic, Dynamic> qbit_def_matrix;

		// Quantum expression of arithmetic Q operations with Q defined symbols as operands 
		class Qexpression : public qbit_def_vector
		{
		public:
			// An emty Q extression
			Qexpression();

			// A identity Q extression initialized with a single Q defined symbol
			Qexpression(const Qdef&);

			// Copy constructor
			Qexpression(const Qexpression&);

			// Destruct the Q expression instance with containing Qdef and Q
			~Qexpression();

			// Addition operator returns a new Qexpression object by adding Qbits within
			// this Q expression and right Q defined symbols operand where this object
			// is a left operand
			Qexpression operator +(const Qdef&) const;

			// Addition operator updates this Qexpression object by adding Qbits within
			// this and right Q defined symbols operand where this object is a left operand
			Qexpression& operator +=(const Qdef&);

			// Addition operator returns a new Qexpression object with adding Qbits within
			// this and right Q expression where this object is a left operand
			Qexpression operator +(const Qexpression&) const;

			// Addition operator updates this Qexpression object by adding Qbits within
			// this and right Q expression where this object is a left operand
			Qexpression& operator +=(const Qexpression&);

			// Multiplication operator returns a new Qexpression object by multiplying
			// Qbits within this Q expression and right Q defined symbols operand where
			// this object is a left operand
			Qexpression operator *(const Qdef&) const;

			// Multiplication operator updates this Qexpression object by multiplying
			// Qbits within this and right Q defined symbols operand where this object is
			// a left operand
			Qexpression& operator *=(const Qdef&);

			// Multiplication operator updates this Qexpression object by multiplying
			// Qbits within this and right Q expression where this object is a left operand
			Qexpression operator *(const Qexpression&) const;

			// Multiplication operator updates this Qexpression object by multiplying
			// Qbits within this and right Q expression where this object is a left operand
			Qexpression& operator *=(const Qexpression&);

			// Resize the Qexpression as a vector of qbit definitions to a new size
			void resize(Index size);

			// Return a string representation of this Q expression
			string toString() const;

			// Insert string representation of a Q expression into an output stream
			friend std::ostream& operator << (std::ostream&, const Qexpression&);
		protected:
			// Return Q bit symbol matrix that is result of matrix multiplication of two
			// Q bit vectors, this and right Q expression objects
			virtual qbit_def_matrix thisX(const Qexpression& right) const;

			// Updates this Q expression by summarizing diagonals of given Q bit symbol
			// matrix. The size of resulting Q expression vector is
			// #_of_rows + #_of_cols - 1
			virtual void sumDiagonals(const qbit_def_matrix&);

		private:
			// An add functional object augmenting Qexprfession addition operators 
			class Add
			{
			private:
				// A carry item is a pair of Q expresion bit index and Q additon carry operand
				typedef pair<Index, Qaddition::Carry::Sp> CarryItem;

				// A list of Q additon carry items
				typedef vector<CarryItem>	CarryList;

			public:
				// Default constructor, starts processing from bit 0
				Add();

				// Initiate an add object starting processing from a given Q expression bit index
				Add(Index startBitIndex);

				// Destruct the add functional object
				~Add();

				// Set a current Q expression bit index
				void operator=(Index at) { mAtBit = at; };

				// Return the current Q expression bit index
				Index operator*() { return(mAtBit); };

				// Prefix and postfix increment operators moving the current Q expression
				// bit index
				Index& operator++() { return(++mAtBit); };
				Index operator++(int) { return(++mAtBit); };

				// Prefix and postfix decrement operators moving the current Q expression
				// bit index
				Index& operator--() { return(--mAtBit); };
				Index operator--(int) { return(--mAtBit); };

				// Set Q expression bit index to 0
				void resetAtBit() { mAtBit = 0; };

				// operator function () returns result operand by Add-ing left and right operand
				Qoperand::Sp operator () (const Qoperand::Sp&, const Qoperand::Sp&);

				// operator function () returns result operand by Add-ing left and carry operands
				Qoperand::Sp operator () (const Qoperand::Sp&);

				// operator function () returns result operand by Add-ing carry operands
				Qoperand::Sp operator () ();

			private:
				CarryList mCarryFIFO;	// Contains unprocessed Q addition carry items
				Index mAtBit;			// An index of current Q expresion bit being processed
			};
		};
	};
};