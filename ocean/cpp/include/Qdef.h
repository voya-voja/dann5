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

		// Quantum definition of a definition defines Q bit definition names in a vector
		class Qdef : public qbit_def_vector
		{
		public:
			// Qdef's shared pointer 
			typedef shared_ptr<Qdef> Sp;

			// Instantiate Q defined definition with a auto-generated name, e.g. 'v1_', 'v2_', etc.,
			// without any Q bit definition name
			Qdef();

			// Instantiate Q defined definition with a given # of Q bit names 
			// based on an auto-generated name, e.g. 'v1_', 'v2_', etc.
			Qdef(Index size);

			// Instantiate Q defined definition with a given # of Q bit names 
			// based on a given name, e.g. 'a', 'A', 'symbol1', etc.
			Qdef(Index size, const string& definition);

			// copy constructor
			Qdef(const Qdef&);

			// destruct the Q defined definition
			~Qdef();

			// Return the name of Q defined definition
			const string& name() const { return mDefinition; }

			// Set a new name of the Q defined definition
			void name(const string& s) { mDefinition = s; }

			// get Q definition's n(umber) o(f) b(its)
			Index nobs() const { return rows(); }

			// Resize the Qdef as a vector of qbit definitions to a new size
			void resize(Index size);

			// Return a shared pointer to a copy of this Q definition
			virtual Qdef::Sp clone() const { return Sp(new Qdef(*this)); };

			// A condition negation
			Qdef& operator~();

			// And operator returns this Qdef object with concatenated Q definitions
			// by concanating two definition names on a same bit level with '&' sign in 
			// between, e.g. when this is 'a' and right is 'b' 2 bits Q definitions:
			//    a &= b; will result in Q defintion a becomes 'a&b' with 'a0&b0' 0
			// bit definition, and 'a1&b1' 1 bit definition
			Qdef& operator&=(const Qdef& right);

			// And operator returns Qexpression object with and expressions of bit level
			// symbols within two Q definitions where this object is a left and the other
			// Qdef object is a right operand, e.g. for a and b 2 bit Q defintitions:
			// exp = a & b; will result in an 2 bit Q expression vector where a0 & b0 is 
			// on 0 bit level and a1 & b1 on 1 bit level
			Qexpression operator &(const Qdef&) const;

			// And operator returns Qexpression object with and expressions of bit level
			// symbols within this Q definition and right Qexpression operand where
			// this object is a left operand, e.g. for c 2 bit Q defintitions and exp Q
			// expresion from above:
			// exp1 = c & exp; will result in an 2 bit Q expression vector where  
			// c0 & a0 & b0 is on 0 bit level and c1 & a1 & b1 on 1 bit level
			Qexpression operator &(const Qexpression&) const;

			// Or operator returns this Qdef object with concatenated Q definitions
			// by concanating two definition names on a same bit level with '|' sign in  
			// between, e.g. when this is 'a' and right is 'b' 2 bits Q definitions:
			//    a |= b; will result in Q defintion a becomes 'a|b' with 'a0|b0' 0
			// bit definition, and 'a1|b1' 1 bit definition
			Qdef& operator|=(const Qdef& right);

			// Or operator returns Qexpression object with and expressions of bit level
			// symbols within two Q definitions where this object is a left and the other
			// Qdef object is a right operand, e.g. for x and y 2 bit Q defintitions:
			// e = x | y; will result in an 2 bit Q expression vector where x0 | y0 is 
			// on 0 bit level and x1 | y1 on 1 bit level
			Qexpression operator |(const Qdef&) const;

			// Or operator returns Qexpression object with and expressions of bit level
			// symbols within this Q definition and right Qexpression operand where
			// this object is a left operand, e.g. for c 2 bit Q defintitions and exp Q
			// expresion from above:
			// e1 = c | exp; will result in an 2 bit Q expression vector where  
			// c0 | a0 & b0 is on 0 bit level and c1 | a1 & b1 on 1 bit level
			Qexpression operator |(const Qexpression&) const;

			// Nand operator returns this Qdef object with concatenated Q definitions
			// by concanating two definition names on a same bit level with '|' sign in  
			// between, e.g. when this is 'a' and right is 'b' 2 bits Q definitions:
			//    a.nand(b); will result in Q defintion a becomes 'a~&b' with 'a0~&b0' 0
			// bit definition, and 'a1~&b1' 1 bit definition
			Qdef& nandMutable(const Qdef& right);

			// Nand operator returns Qexpression object with and expressions of bit level
			// symbols within two Q definitions where this object is a left and the other
			// Qdef object is a right operand, e.g. for x and y 2 bit Q defintitions:
			// e = x.nand(y); will result in an 2 bit Q expression vector where x0 ~& y0 is 
			// on 0 bit level and x1 ~& y1 on 1 bit level
			Qexpression nand(const Qdef&) const;

			// Nand operator returns Qexpression object with and expressions of bit level
			// symbols within this Q definition and right Qexpression operand where
			// this object is a left operand, e.g. for c 2 bit Q defintitions and exp Q
			// expresion from above:
			// e1 = c.nand(exp); will result in an 2 bit Q expression vector where  
			// c0 ~& a0 & b0 is on 0 bit level and c1 ~& a1 & b1 on 1 bit level
			Qexpression nand(const Qexpression&) const;

			// Nor operator returns this Qdef object with concatenated Q definitions
			// by concanating two definition names on a same bit level with '|' sign in  
			// between, e.g. when this is 'a' and right is 'b' 2 bits Q definitions:
			//    a.nor(b); will result in Q defintion a becomes 'a~|b' with 'a0~|b0' 0
			// bit definition, and 'a1~|b1' 1 bit definition
			Qdef& norMutable(const Qdef& right);

			// Nor operator returns Qexpression object with and expressions of bit level
			// symbols within two Q definitions where this object is a left and the other
			// Qdef object is a right operand, e.g. for x and y 2 bit Q defintitions:
			// e = x.nor(y); will result in an 2 bit Q expression vector where x0 ~| y0 is 
			// on 0 bit level and x1 ~| y1 on 1 bit level
			Qexpression nor(const Qdef&) const;

			// Nor operator returns Qexpression object with and expressions of bit level
			// symbols within this Q definition and right Qexpression operand where
			// this object is a left operand, e.g. for c 2 bit Q defintitions and exp Q
			// expresion from above:
			// e1 = c.nor(exp); will result in an 2 bit Q expression vector where  
			// c0 ~| a0 & b0 is on 0 bit level and c1 ~| a1 & b1 on 1 bit level
			Qexpression nor(const Qexpression&) const;


			// Or operator returns this Qdef object with concatenated Q definitions
			// by concanating two definition names on a same bit level with '|' sign in  
			// between, e.g. when this is 'a' and right is 'b' 2 bits Q definitions:
			//    a |= b; will result in Q defintion a becomes 'a|b' with 'a0|b0' 0
			// bit definition, and 'a1|b1' 1 bit definition
			Qdef& operator^=(const Qdef& right);

			// Or operator returns Qexpression object with and expressions of bit level
			// symbols within two Q definitions where this object is a left and the other
			// Qdef object is a right operand, e.g. for x and y 2 bit Q defintitions:
			// e = x | y; will result in an 2 bit Q expression vector where x0 | y0 is 
			// on 0 bit level and x1 | y1 on 1 bit level
			Qexpression operator ^(const Qdef&) const;

			// Or operator returns Qexpression object with and expressions of bit level
			// symbols within this Q definition and right Qexpression operand where
			// this object is a left operand, e.g. for c 2 bit Q defintitions and exp Q
			// expresion from above:
			// e1 = c | exp; will result in an 2 bit Q expression vector where  
			// c0 | a0 & b0 is on 0 bit level and c1 | a1 & b1 on 1 bit level
			Qexpression operator ^(const Qexpression&) const;

			// Addition operator returns this Qdef object with concatenated Q definitions
			// by concanating two definition names on a same bit level with '+' sign in between
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
			// Q definition object by concanating two definition names using '*' sign
			Qdef& operator*=(const Qdef& right);

			// Multiplication operator returns Qexpression object with multiplication
			// expressions of Qbits within two Q defined symbols where this object is 
			// a left and the other Qdef object is a right operand
			Qexpression operator *(const Qdef&) const;

			// Multiplication operator returns Qexpression object with multiplication
			// expressions of Qbits within this Q defined symbols and right Qexpression
			// operand where this object is a left operand
			Qexpression operator *(const Qexpression&) const;

		protected:
			// Concatenates right object to this object using 'concatenation sign, which
			// by default is an empty string
			virtual void concatenate(const Qdef& right, const string& sign = "");

			void initialize(Index size);

			virtual Qoperand::Sp operand(Index level);

		private:
			const static string	cDefaultName;	// default name root, e.g. 'v'
			static Index		gNameIndex;		// auto generating name index

			// Auto-generate a unique definition name, e.g. 'v1_', 'v2_', etc.
			static string AutoName();

			// A name of teh Q defined definition 
			string	mDefinition;
		};
	};
};