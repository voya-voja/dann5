#pragma once

#include <pybind11/pybind11.h>

#include <Qtype.h>
#include <Qdef.h>

using namespace std;
using namespace Eigen;


namespace dann5 {
	namespace ocean {
		// Quantum equation is a statement with a result Q variable defined
		class Qequation;

		// Quantum condition is a conditional statement with a set of statements to be
		// executed when condition is true
		class Qcondition;

		// Quantum variable has a variable definition definition (Qdef) and its value (Qint)
		class Qvar
		{
		public:
			//default constructor
			Qvar();
			// instantiate a Q variable with a given definition and with given integer value
			Qvar(const string&, long);
			// instantiate a Q variable with a given # of bits and given definition, 
			// the bits are in superposition state
			Qvar(Index, const string&);
			// instantiate a Q variable with a given definition Q definition, 
			// the bits are in superposition state
			Qvar(const Qdef&);
			// instantiate a Q variable with a given definition Q definition and given Q int value 
			Qvar(const Qdef&, const Qint&);
			// copy constructor 
			Qvar(const Qvar&);
			// destruct the variable with its definition Q definition and Q int value
			~Qvar();

			// get variable's definition
			const Qdef& definition() const { return *mpDefinition; }

			// get variable's value
			const Qint& value() const { return mValue; }

			// A condition negation
			Qvar operator~() const;

			// Equal operator returns Q condition object by combining 
			// this and right object definition into a condition statement
			Qcondition operator==(const Qvar& right) const;

			// Not equal operator returns Q condition object by combining 
			// this and right object definition into a condition statement
			Qcondition operator!=(const Qvar& right) const;

			// Greater-then operator returns Q condition object by combining 
			// this and right object definition into a condition statement
			Qcondition operator>(const Qvar& right) const;

			// Greater-equal operator returns Q condition object by combining 
			// this and right object definition into a condition statement
			Qcondition operator>=(const Qvar& right) const;

			// Less-then operator returns Q condition object by combining 
			// this and right object definition into a condition statement
			Qcondition operator<(const Qvar& right) const;

			// Less-equal operator returns Q condition object by combining 
			// this and right object definition into a condition statement
			Qcondition operator<=(const Qvar& right) const;

			// And operator returns this Qvar object as this & right of  
			// Q variable objects' Qdef and Qint members
			Qvar& operator&=(const Qvar& right);

			// And operator returns a Qequation object with expression this & right and  
			// this and right Q variables as arguments
			Qequation operator&(const Qvar& right) const;

			// And operator where Q variable is left and Q equation right argument.
			// Returns Qequation object with expression this & right and this and right 
			// Q variables as arguments
			Qequation operator&(const Qequation& right) const;

			// Or operator returns this Qvar object as this | right of  
			// Q variable objects' Qdef and Qint members
			Qvar& operator|=(const Qvar& right);

			// Or operator returns a Qequation object with expression this | right and  
			// this and right Q variables as arguments
			Qequation operator|(const Qvar& right) const;

			// Or operator where Q variable is left and Q equation right argument.
			// Returns Qequation object with expression this | right and this and right 
			// Q variables as arguments
			Qequation operator|(const Qequation& right) const;

			// Xor operator returns this Qvar object as this ^ right of  
			// Q variable objects' Qdef and Qint members
			Qvar& operator^=(const Qvar& right);

			// Xor operator returns a Qequation object with expression this ^ right and  
			// this and right Q variables as arguments
			Qequation operator^(const Qvar& right) const;

			// Xor operator where Q variable is left and Q equation right argument.
			// Returns Qequation object with expression this ^ right and this and right 
			// Q variables as arguments
			Qequation operator^(const Qequation& right) const;

			// Nand operator returns this Qvar object as this ~& right of  
			// Q variable objects' Qdef and Qint members
			Qvar& nandMutable(const Qvar& right);

			// Nand operator returns a Qequation object with expression this ~& right and  
			// this and right Q variables as arguments
			Qequation nand(const Qvar& right) const;

			// Nand operator where Q variable is left and Q equation right argument.
			// Returns Qequation object with expression this ~& right and this and right 
			// Q variables as arguments
			Qequation nand(const Qequation& right) const;

			// Nor operator returns this Qvar object as this ~| right of  
			// Q variable objects' Qdef and Qint members
			Qvar& norMutable(const Qvar& right);

			// Nor operator returns a Qequation object with expression this ~| right and  
			// this and right Q variables as arguments
			Qequation nor(const Qvar& right) const;

			// Nor operator where Q variable is left and Q equation right argument.
			// Returns Qequation object with expression this ~| right and this and right 
			// Q variables as arguments
			Qequation nor(const Qequation& right) const;

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

			// Subtraction operator returns a Qequation instance where the result is this
			// variable and the expression is a addition of the Q euqation result and
			// right variable, e.g. for Q variabbles x, minuend, and z, subtrahend,
			// and where Q equation with result v0 is a difference:
			// v0 = x - z	=>	x = v0 + z
			Qequation operator-(const Qvar& right) const;

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

			// Division operator returns a Qequation instance where the result is this
			// variable and the expression is a multiplication of the Q euqation result
			// and right variable, e.g. for Q variabbles x, dividend, and z, divisor,
			// and where Q euation with result v0 is a quotient:
			// v0 = x / z	=>	x = v0 * z
			Qequation operator/(const Qvar& right) const;

			// Resize the Qvar as a vector of Qbit definitions and values to a new size
			// if the new size is bigger, assign qBit value to additional Qbits
			void resize(Index size, const Qbit& qBit = Qbit::cSuperposition);

			// Return Qdef and Qint string representations of this variable
			// when bitFormat is true, teh string representation are variable bit 
			// symbols and values, otherwise returns sybol name and corrsponding value
			string toString(bool bitFormat = true) const;

			// Insert string representation of a Q variable into an output stream
			friend std::ostream& operator << (std::ostream&, const Qvar&);

		protected:

			// get variable's definition
			Qdef::Sp definition() { return mpDefinition; }

			// get variable's value
			Qint& value() { return mValue; }

		private:
			Qdef::Sp mpDefinition;	// The variable definition
			Qint mValue;			// The variable value
		};

		// An std::vector containing a list of Q variables
		typedef std::vector<Qvar>	Qvars;
	}
}
