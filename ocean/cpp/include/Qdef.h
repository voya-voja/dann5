#pragma once

#include <pybind11/pybind11.h>

#include <utility>
#include <Eigen/Dense>

#include <Qoperand.h>

using namespace std;
using namespace Eigen;


namespace dann5 {
	namespace ocean {
		// Quantum expression of arithmetic Q operations with Q defined symbols as operands 
		class Qexpression;

		// A vector of Qoperand shared pointers representing defined Q bit symbols
		typedef Matrix<Qoperand::Sp, Dynamic, 1> qbit_def_vector;

		// Quantum definition of a symbol defines Q bit symbol names in a vector
		class Qdef : public qbit_def_vector
		{
		public:
			// Instantiate Q defined symbol with a auto-generated name, e.g. 'v1_', 'v2_', etc.,
			// without any Q bit symbol name
			Qdef();

			// Instantiate Q defined symbol with a given # of Q bit names 
			// based on an auto-generated name, e.g. 'v1_', 'v2_', etc.
			Qdef(Index size);

			// Instantiate Q defined symbol with a given # of Q bit names 
			// based on a given name, e.g. 'a', 'A', 'symbol1', etc.
			Qdef(Index size, const string& symbol);

			// copy constructor
			Qdef(const Qdef&);

			// destruct the Q defined symbol
			~Qdef();

			// Return the name of Q defined symbol
			const string& name() const { return mSymbol; }

			// Set a new name of the Q defined symbol
			void name(const string& s) { mSymbol = s; }

			// Addition operator returns this Qdef object by adding right to this  
			// Q definition object by concanating two symbol names using '+' sign
			Qdef& operator+=(const Qdef& right);

			// Addition operator returns Qexpression object with addition expressions of
			// Qbits within two Q defined symbols where this object is a left and the other
			// Qdef object is a right operand
			Qexpression operator +(const Qdef&) const;

			// Addition operator returns Qexpression object with addition expressions of
			// Qbits within this Q defined symbols and right Qexpression operand where
			// this object is a left operand
			Qexpression operator +(const Qexpression&) const;

			// Multiplication operator returns this Qdef object by adding right to this  
			// Q definition object by concanating two symbol names using '*' sign
			Qdef& operator*=(const Qdef& right);

			// Multiplication operator returns Qexpression object with multiplication
			// expressions of Qbits within two Q defined symbols where this object is 
			// a left and the other Qdef object is a right operand
			Qexpression operator *(const Qdef&) const;

			// Multiplication operator returns Qexpression object with multiplication
			// expressions of Qbits within this Q defined symbols and right Qexpression
			// operand where this object is a left operand
			Qexpression operator *(const Qexpression&) const;

			// Resize the Qdef as a vector of qbit definitions to a new size
			void resize(Index size);

		protected:
			// Concatenates right object to this object using 'concatenation sign, which
			// by default isan empty string
			virtual void concatenate(const Qdef& right, const string& sign = "");
		private:
			const static string	cDefaultName;	// default name root, e.g. 'v'
			static Index		gNameIndex;		// auto generating name index

			// Auto-generate a unique symbol name, e.g. 'v1_', 'v2_', etc.
			static string AutoName();

			// A name of teh Q defined symbol 
			string	mSymbol;
		};
	};
};