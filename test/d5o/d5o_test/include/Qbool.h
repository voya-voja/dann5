#pragma once

#include <pybind11/pybind11.h>

#include <Qbit.h>
#include <Qexpr.h>

namespace dann5 {
	namespace ocean {

		class Qbool : public Qtype
		{
		public:
			// Q definition shared pointer 
			typedef shared_ptr<Qbool> Sp;

			// Qbool default superposition value, 
			// though any value except 0 and 1 will be considered superposition
			static const QbitV cSuperposition = 'S'; // value 83
			static const QbitV cTrue	= 'T'; // value 84
			static const QbitV cFalse	= 'F'; // value 71

			// Initialize Q bool with a given id and supperposition value
			Qbool(const string& id) : Qtype(id), mValue(cSuperposition) {};

			// Initialize Q bool with a given id and value. If value is different from 'T' or 'F', 
			// Qbool into superposition state, i.e. value 'S'
			Qbool(const string& id, QbitV value) : Qtype(id), mValue(value)
			{
				if (value != cTrue && value != cFalse) mValue = cSuperposition;
			};

			// copy constructor
			Qbool(const Qbool& right) : Qtype(right), mValue(right.mValue) {};

			// type conversion operator
			operator QbitV() { return mValue; };
			operator const QbitV() const { return mValue; };

			// Return a Qdef's shared pointer pointing to a copy of this object 
			virtual Qdef::Sp clone() const { return Qdef::Sp(new Qbool(*this)); };

			// Returns the number of Q bits that the Q binary holds
			virtual std::size_t nobs() const noexcept { return 1; };

			// assignment operator changes the identity and value of this object to be
			// the same as right object
			Qbool& operator=(const Qbool& right);

			// change value of this object as result of and operation between this 
			// and right Q bool
			Qbool& operator&=(const Qbool& right);

			// change value of this object as result of or operation between this 
			// and right Q bool
			Qbool& operator|=(const Qbool& right);

			// change value of this object as result of xor operation between this 
			// and right Q bool
			Qbool& operator^=(const Qbool& right);

			// instantiate Q expression with inversion logic, e.g. for Qbool with id 'x'
			// the expression is '~x' != 'x'
			Qexpr<Qbool> operator~();

			// instantiate Q expression with and logic, e.g. for Qbool ids 'x' and 'y'
			// the expression is 'x' & 'y'
			Qexpr<Qbool> operator&(const Qbool& right) const;

			// instantiate Q expression with or logic, e.g. for Qbool ids 'x' and 'y'
			// the expression is 'x' | 'y'
			Qexpr<Qbool> operator|(const Qbool& right) const;

			// instantiate Q expression with xor logic, e.g. for Qbool ids 'x' and 'y'
			// the expression is 'x' ^ 'y'
			Qexpr<Qbool> operator^(const Qbool& right) const;

			// instantiate Q expression with comparison, e.g. for Qbool ids 'x' and 'y'
			// the expression is 'x' == 'y'
			Qexpr<Qbool> operator==(const Qbool& right) const;

			// instantiate Q expression with comparison, e.g. for Qbool ids 'x' and 'y'
			// the expression is 'x' != 'y'
			Qexpr<Qbool> operator!=(const Qbool& right) const;

			// instantiate Q expression with and logic, e.g. for Qbool id 'x' and [right]
			// object the expression is 'x' & [right]
			Qexpr<Qbool> operator&(const Qexpr<Qbool>& right) const;

			// instantiate Q expression with or logic, e.g. for Qbool id 'x' and [right]
			// object the expression is 'x' | [right]
			Qexpr<Qbool> operator|(const Qexpr<Qbool>& right) const;

			// instantiate Q expression with xor logic, e.g. for Qbool id 'x' and [right]
			// object the expression is 'x' ^ [right]
			Qexpr<Qbool> operator^(const Qexpr<Qbool>& right) const;
		protected:
		private:
			QbitV mValue;
		};
	};
};