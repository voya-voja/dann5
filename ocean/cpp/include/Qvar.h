#pragma once

#include <pybind11/pybind11.h>

#include <Qtype.h>
#include <Qdef.h>

using namespace std;
using namespace Eigen;


namespace dann5 {
	namespace ocean {
		// Quantum equation is a coupling of result Q variable via Q expression with Qvar arguments
		class Qequation;

		// Quantum variable has a variable symbol definition (Qdef) and its value (Qint)
		class Qvar
		{
		public:
			//default constructor
			Qvar();
			// instantiate a Q variable with a given symbol and with given integer value
			Qvar(const string&, long);
			// instantiate a Q variable with a given # of bits and given symbol, 
			// the bits are in superposition state
			Qvar(Index, const string&);
			// instantiate a Q variable with a given symbol Q definition, 
			// the bits are in superposition state
			Qvar(const Qdef&);
			// instantiate a Q variable with a given symbol Q definition and given Q int value 
			Qvar(const Qdef&, const Qint&);
			// copy constructor 
			Qvar(const Qvar&);
			// destruct the variable with its symbol Q definition and Q int value
			~Qvar();

			// get variable's symbol
			const Qdef& symbol() const { return mSymbol; }

			// get variable's value
			const Qint& value() const { return mValue; }

			// Addition operator returns this Qvar object by adding right to this  
			// Q variable objects' Qdef and Qint members
			Qvar& operator+=(const Qvar& right);

			// Addition operator returns Qequation object by adding this and right  
			// Q variable objects' Qdef and Qint members
			Qequation operator+(const Qvar& right) const;

			// Addition operator where Q variable is left and Q equation right argument.
			// Returns Qequation object by adding its Qdef and Qint members to the
			// Q equation
			Qequation operator+(const Qequation& right) const;

			// Multiplication operator returns this Qvar object by multiplying right to  
			// this Q variable objects' Qdef and Qint members
			Qvar& operator*=(const Qvar& right);

			// Multiplication operator returns Qequation object by multiplying this and
			// right Q variable objects' Qdef and Qint members
			Qequation operator*(const Qvar& right) const;

			// Multiplication operator where Q variable is left and Q equation right
			// argument. Returns Qequation object by adding its Qdef and Qint members to the
			// Q equation
			Qequation operator*(const Qequation& right) const;


			// Resize the Qvar as a vector of Qbit definitions and values to a new size
			// if the new size is bigger, assign qBit value to additional Qbits
			void resize(Index size, const Qbit& qBit = Qbit::cSuperposition);

			// Return Qdef and Qint string representations of this variable
			string toString(bool bitFormat = true) const;

			// Insert string representation of a Q variable into an output stream
			friend std::ostream& operator << (std::ostream&, const Qvar&);

		protected:
		private:
			Qdef mSymbol;	// The variable symbol
			Qint mValue;	// The variable value
		};
	}
}
