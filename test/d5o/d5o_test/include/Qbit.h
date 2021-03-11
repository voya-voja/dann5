#pragma once

#include <pybind11/pybind11.h>

#include <Qtype.h>
#include <Qexpr.h>

#include <utility>

using namespace std;

namespace dann5 {
	namespace ocean {

		// Quantum bit value is in S-uperposition state for any value except 0 and 1 
		typedef unsigned char QbitV;

		// Quantum bit is a Q type supporting instantiation of and operations for 
		// variables which can have values 0, 1 and 'S'-uperposition, value 83
		class Qbit : public Qtype
		{
		public:
			// Q definition shared pointer 
			typedef shared_ptr<Qbit> Sp;

			// Qbit default superposition value, 
			// though any value except 0 and 1 will be considered superposition
			static const QbitV cSuperposition = 'S'; // value 83

			// Default constructor initializes Q bit with an empty id and supperposition
			// value
			Qbit() : Qtype(""), mValue(cSuperposition) {};

			// Initialize Q bit with a given id and supperposition value
			Qbit(const string& id) : Qtype(id), mValue(cSuperposition) {};

			// Initialize Q bit with a given id and value. If value is different  from 0 or 1, 
			// Qbit into superposition state
			Qbit(const string& id, QbitV value) : Qtype(id), mValue(value)
			{
				if (value > 1) mValue = cSuperposition;
			};

			// copy constructor
			Qbit(const Qbit& right) : Qtype(right), mValue(right.mValue) {};

			// type conversion operator
			operator QbitV() { return mValue; };
			operator const QbitV() const { return mValue; };

			// set the new value of this Q bit
			void value(QbitV v) { mValue = v; };

			// Return a Qdef's shared pointer pointing to a copy of this object 
			virtual Qdef::Sp clone() const { return Qdef::Sp(new Qbit(*this)); };

			// Returns the number of Q bits that the Q binary holds
			virtual std::size_t nobs() const noexcept { return 1; };

			// assignment operator changes the identity and value of this object to be
			// the same as right object
			Qbit& operator=(const Qbit& right);

			// change value of this object as result of and operation between this 
			// and right Q bit
			Qbit& operator&=(const Qbit& right);

			// change value of this object as result of or operation between this 
			// and right Q bit
			Qbit& operator|=(const Qbit& right);

			// change value of this object as result of xor operation between this 
			// and right Q bit
			Qbit& operator^=(const Qbit& right);

			// instantiate Q expression with inversion logic, e.g. for Qbit with id 'x'
			// the expression is '~x' != 'x'
			Qexpr<Qbit> operator~();

			// instantiate Q expression with and logic, e.g. for Qbit ids 'x' and 'y'
			// the expression is 'x' & 'y'
			Qexpr<Qbit> operator&(const Qbit& right) const;

			// instantiate Q expression with or logic, e.g. for Qbit ids 'x' and 'y'
			// the expression is 'x' | 'y'
			Qexpr<Qbit> operator|(const Qbit& right) const;

			// instantiate Q expression with xor logic, e.g. for Qbit ids 'x' and 'y'
			// the expression is 'x' ^ 'y'
			Qexpr<Qbit> operator^(const Qbit& right) const;

			// instantiate Q expression with comparison, e.g. for Qbit ids 'x' and 'y'
			// the expression is 'x' == 'y'
			Qexpr<Qbit> operator==(const Qbit& right) const;

			// instantiate Q expression with comparison, e.g. for Qbit ids 'x' and 'y'
			// the expression is 'x' != 'y'
			Qexpr<Qbit> operator!=(const Qbit& right) const;

			// instantiate Q expression with comparison, e.g. for Qbit ids 'x' and 'y'
			// the expression is 'x' > 'y'
			Qexpr<Qbit> operator>(const Qbit& right) const;

			// instantiate Q expression with comparison, e.g. for Qbit ids 'x' and 'y'
			// the expression is 'x' >= 'y'
			Qexpr<Qbit> operator>=(const Qbit& right) const;

			// instantiate Q expression with comparison, e.g. for Qbit ids 'x' and 'y'
			// the expression is 'x' < 'y'
			Qexpr<Qbit> operator<(const Qbit& right) const;

			// instantiate Q expression with comparison, e.g. for Qbit ids 'x' and 'y'
			// the expression is 'x' <= 'y'
			Qexpr<Qbit> operator<=(const Qbit& right) const;

			// instantiate Q expression with and logic, e.g. for Qbit id 'x' and [right]
			// object the expression is 'x' & [right]
			Qexpr<Qbit> operator&(const Qexpr<Qbit>& right) const;

			// instantiate Q expression with or logic, e.g. for Qbit id 'x' and [right]
			// object the expression is 'x' | [right]
			Qexpr<Qbit> operator|(const Qexpr<Qbit>& right) const;

			// instantiate Q expression with xor logic, e.g. for Qbit id 'x' and [right]
			// object the expression is 'x' ^ [right]
			Qexpr<Qbit> operator^(const Qexpr<Qbit>& right) const;
/*
			// instantiate Q expression with comparison, e.g. for Qbit id 'x' and [right]
			// object the expression is 'x' == [right]
			Qexpr<Qbit> operator==(const Qexpr<Qbit>& right) const;

			// instantiate Q expression with comparison, e.g. for Qbit id 'x' and [right]
			// object the expression is 'x' != [right]
			Qexpr<Qbit> operator!=(const Qexpr<Qbit>& right) const;

			// instantiate Q expression with comparison, e.g. for Qbit id 'x' and [right]
			// object the expression is 'x' > [right]
			Qexpr<Qbit> operator>(const Qexpr<Qbit>& right) const;

			// instantiate Q expression with comparison, e.g. for Qbit id 'x' and [right]
			// object the expression is 'x' >= [right]
			Qexpr<Qbit> operator>=(const Qexpr<Qbit>& right) const;

			// instantiate Q expression with comparison, e.g. for Qbit id 'x' and [right]
			// object the expression is 'x' < [right]
			Qexpr<Qbit> operator<(const Qexpr<Qbit>& right) const;

			// instantiate Q expression with comparison, e.g. for Qbit id 'x' and [right]
			// object the expression is 'x' <= [right]
			Qexpr<Qbit> operator<=(const Qexpr<Qbit>& right) const;
*/
		protected:
		private:
			QbitV mValue;
		};
	};
};