#pragma once


#include <pybind11/pybind11.h>

#include <Qbinary.h>
#include <Qexpr.h>

#include <bitset>

using namespace std;

namespace dann5 {
	namespace ocean {

		// Quantum whole is a Q type, subtype of Qbinary supporting instantiation of 
		// and operations for positive whole number variables. As Qbianry, Q whole
		// can be unknown when at least one of its Qbits is in supoerposition.
		class Qwhole : public Qbinary
		{
		public:
			// Q definition shared pointer 
			typedef shared_ptr<Qwhole> Sp;

			// Qwhole is unknown, if any of its Qbits is in superposition state
			static const unsigned long long cUnknown = ULLONG_MAX;

			// Qwhole unknown string representation is "(U)"
			static const string cUnknownSign;

			// Initialize Q whole with a given id and byte size 0 value
			Qwhole(const string& id) : Qbinary(id) {};

			// Initialize Q whole with a given id and unknown value of specified size
			Qwhole(size_t size, const string& id) : Qbinary(size, id) {};

			// Initialize Q whole with a given id and value as a Qbit vector
			Qwhole(const string& id, const Qbits& value) : Qbinary(id, value) {};

			// Initialize Q whole with a given id and a deterministic value as a bitset
			// by default it will trim leading 0, unless asis is true
			Qwhole(const string& id, const Bits& value, bool asis = false) : Qbinary(id, value, asis) {};

			// copy constructor
			Qwhole(const Qwhole& right) : Qbinary(right) {};

			// type conversion operator
			operator unsigned long long();
			operator const unsigned long long() const;

			// Return a Qdef's shared pointer pointing to a copy of this object 
			virtual Qdef::Sp clone() const { return Qdef::Sp(new Qwhole(*this)); };

			// assignment operator changes the identity and value of this object to be
			// the same as right object
			Qwhole& operator=(const Qwhole& right);

			// change value of this object as result of and operation between this 
			// and right Q bit
			Qwhole& operator+=(const Qwhole& right);

			// change value of this object as result of or operation between this 
			// and right Q bit
			Qwhole& operator*=(const Qwhole& right);

			// change value of this object as result of xor operation between this 
			// and right Q bit
			Qwhole& operator-=(const Qwhole& right);

			// change value of this object as result of xor operation between this 
			// and right Q bit
			Qwhole& operator/=(const Qwhole& right);

			// instantiate Q expression with and logic, e.g. for Qwhole ids 'x' and 'y'
			// the expression is 'x' + 'y'
			Qexpr<Qwhole> operator+(const Qwhole& right) const;

			// instantiate Q expression with or logic, e.g. for Qwhole ids 'x' and 'y'
			// the expression is 'x' * 'y'
			Qexpr<Qwhole> operator*(const Qwhole& right) const;

			// instantiate Q expression with comparison, e.g. for Qwhole ids 'x' and 'y'
			// the expression is 'x' == 'y'
			Qexpr<Qwhole> operator==(const Qwhole& right) const;

			// instantiate Q expression with comparison, e.g. for Qwhole ids 'x' and 'y'
			// the expression is 'x' != 'y'
			Qexpr<Qwhole> operator!=(const Qwhole& right) const;

			// instantiate Q expression with comparison, e.g. for Qwhole ids 'x' and 'y'
			// the expression is 'x' > 'y'
			Qexpr<Qwhole> operator>(const Qwhole& right) const;

			// instantiate Q expression with comparison, e.g. for Qwhole ids 'x' and 'y'
			// the expression is 'x' >= 'y'
			Qexpr<Qwhole> operator>=(const Qwhole& right) const;

			// instantiate Q expression with comparison, e.g. for Qwhole ids 'x' and 'y'
			// the expression is 'x' < 'y'
			Qexpr<Qwhole> operator<(const Qwhole& right) const;

			// instantiate Q expression with comparison, e.g. for Qwhole ids 'x' and 'y'
			// the expression is 'x' <= 'y'
			Qexpr<Qwhole> operator<=(const Qwhole& right) const;

			// instantiate Q expression with and logic, e.g. for Qwhole ids 'x' and 'y'
			// the expression is 'x' + 'y'
			Qexpr<Qwhole> operator+(const Qexpr<Qwhole>& right) const;

			// instantiate Q expression with or logic, e.g. for Qwhole ids 'x' and 'y'
			// the expression is 'x' * 'y'
			Qexpr<Qwhole> operator*(const Qexpr<Qwhole>& right) const;

		protected:
		private:
		};
	};
};