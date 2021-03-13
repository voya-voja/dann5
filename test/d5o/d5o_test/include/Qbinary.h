#pragma once

#include <pybind11/pybind11.h>

#include <Qtype.h>
#include <Qbit.h>
#include <Qexpr.h>

#include <bitset>

#include <Eigen/Dense>

using namespace Eigen;
using namespace std;

namespace dann5 {
	namespace ocean {

		// Quantum vector of Q bits
		typedef vector<Qbit> Qbits;

		// coresponds to a bitset of the same size and unsigned long long
		typedef bitset<64> Bits;

		// Quantum non-negatve integer is a vector of Qbits
		typedef Matrix<QbitV, Dynamic, 1> QbitVector;
		typedef Matrix<QbitV, Dynamic, Dynamic> QbitMatrix;


		// Quantum binary is a Q type supporting instantiation of and operations for 
		// variables which can have one or more of Qbits. For example Qbinary with 8
		// Qbits would correspond to a Quantum byte
		class Qbinary : public Qtype
		{
		public:
			// Q definition shared pointer 
			typedef shared_ptr<Qbinary> Sp;

			// Qbinary is unknown, if any of its Qbits is in superposition state
			static const unsigned long long cUnknown = ULLONG_MAX;

			// Qbinary unknown string representation is "(U)"
			static const string cUnknownSign;

			// Initialize Q binary with a given id and byte size 0 value
			Qbinary(const string& id);

			// Initialize Q binary with a given id and unknown value of specified size
			Qbinary(size_t size, const string& id);

			// Initialize Q binary with a given id and value as a Qbit vector
			Qbinary(const string& id, const Qbits& value);

			// Initialize Q binary with a given id and a deterministic value as a bitset
			// by default it will trim leading 0, unless asis is true
			Qbinary(const string& id, const Bits& value, bool asis = false);

			// copy constructor
			Qbinary(const Qbinary& right) : Qtype(right), mValue(right.mValue) {};

			// type conversion operator to a std::vertor of Qbits
			operator Qbits() { return mValue; };
			operator const Qbits() const { return mValue; };

			// type conversion operator to a Eigen::vector of Qbits
			operator QbitVector();
			operator const QbitVector() const;

			// Return a Qdef's shared pointer pointing to a copy of this object 
			virtual Qdef::Sp clone() const { return Qdef::Sp(new Qbinary(*this)); };

			// Returns the number of Q bits that the Q binary holds
			virtual std::size_t nobs() const noexcept { return mValue.size(); };

			// Resizes the container so that it contains size elements. If size is
			// smaller than the current container size, the content is reduced to its
			// first size elements, removing those beyond (and destroying them). If size
			// is greater than the current container size, the content is expanded by 
			// inserting at the end as many elements as needed to reach the new size.
			// If val is specified, the new elements are initialized as copies of val, 
			// otherwise, they are 0 - initialized. If size is also greater than the 
			// current container capacity, an automatic reallocation of the allocated 
			// storage space takes place.
			virtual void resize(size_t size, QbitV value = 0);

			// Accesses the Q bit at position pos. The first version returns the value 
			// of the Q bit, the second version returns a reference to a Q bit object 
			// that allows modification of the value.
			QbitV operator[](size_t pos) const { return mValue[pos]; };
			Qbit& operator[](std::size_t pos) { return mValue[pos]; };

			// Checks if all, any or none of the bits are set to a value
			// by default the functions will check for supperposition state
			bool all(QbitV value = Qbit::cSuperposition) const;
			bool any(QbitV value = Qbit::cSuperposition) const;
			bool none(QbitV value = Qbit::cSuperposition) const;

			// Returns the number of bits that are set to a value
			// by default the functions will check for supperposition state
			size_t count(QbitV value = Qbit::cSuperposition) const;

			//Performs binary shift left and binary shift right. 
			// 1 - 2) Performs binary shift left with zeroes shifted in.
			Qbinary operator<<(size_t pos) const;
			// The(2) version is performs the shift to the current object.
			Qbinary& operator<<=(size_t pos);
			// 3 - 4) Performs binary shift left with zeroes shifted in.
			Qbinary operator>>(size_t pos) const;
			// The(4) version is performs the shift to the current object.
			Qbinary& operator>>=(size_t pos);
			// 5 - 6) Performs binary shift left with specified Q bit value shifted in.
			Qbinary operator<<(QbitV value) const;
			// The(6) version is performs the shift to the current object.
			Qbinary& operator<<=(QbitV value);
			// 7 - 8) Performs binary shift left with specified Q bit value shifted in.
			Qbinary operator>>(QbitV value) const;
			// The(8) version is performs the shift to the current object.
			Qbinary& operator>>=(QbitV value);

			// assignment operator changes the identity and value of this object to be
			// the same as right object
			Qbinary& operator=(const Qbinary& right);

			// change value of this object as result of and operation between this 
			// and right Q bit
			Qbinary& operator&=(const Qbinary& right);

			// change value of this object as result of or operation between this 
			// and right Q bit
			Qbinary& operator|=(const Qbinary& right);

			// change value of this object as result of xor operation between this 
			// and right Q bit
			Qbinary& operator^=(const Qbinary& right);

			// instantiate Q expression with inversion logic, e.g. for Qbinary with id 'x'
			// the expression is '~x' != 'x'
			Qexpr<Qbinary> operator~();

			// instantiate Q expression with and logic, e.g. for Qbinary ids 'x' and 'y'
			// the expression is 'x' & 'y'
			Qexpr<Qbinary> operator&(const Qbinary& right) const;

			// instantiate Q expression with or logic, e.g. for Qbinary ids 'x' and 'y'
			// the expression is 'x' | 'y'
			Qexpr<Qbinary> operator|(const Qbinary& right) const;

			// instantiate Q expression with xor logic, e.g. for Qbinary ids 'x' and 'y'
			// the expression is 'x' ^ 'y'
			Qexpr<Qbinary> operator^(const Qbinary& right) const;

			// instantiate Q expression with comparison, e.g. for Qbinary ids 'x' and 'y'
			// the expression is 'x' == 'y'
			Qexpr<Qbinary> operator==(const Qbinary& right) const;

			// instantiate Q expression with comparison, e.g. for Qbinary ids 'x' and 'y'
			// the expression is 'x' != 'y'
			Qexpr<Qbinary> operator!=(const Qbinary& right) const;

			// instantiate Q expression with and logic, e.g. for Qbinary id 'x' and [right]
			// object the expression is 'x' & [right]
			Qexpr<Qbinary> operator&(const Qexpr<Qbinary>& right) const;

			// instantiate Q expression with or logic, e.g. for Qbinary id 'x' and [right]
			// object the expression is 'x' | [right]
			Qexpr<Qbinary> operator|(const Qexpr<Qbinary>& right) const;

			// instantiate Q expression with xor logic, e.g. for Qbinary id 'x' and [right]
			// object the expression is 'x' ^ [right]
			Qexpr<Qbinary> operator^(const Qexpr<Qbinary>& right) const;

		protected:
			// return a reference to a value
			Qbits& value() { return mValue; };
			const Qbits& value() const { return mValue; };

			// set a new value
			void value(const Qbits& value) { mValue = value; };


		private:
			Qbits mValue;
		};
	};
};