#pragma once

#include <pybind11/pybind11.h>

#include <utility>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;


namespace dann5 {
	namespace ocean {

		class Qtype
		{
		public:
		protected:
		private:
		};

		// Quantum bit is in superposition state for any value except 0 and 1 
		typedef unsigned char q_bit;

		class Qbit : public Qtype
		{
		public:
			// Qbit default superposition value, 
			// though any value except 0 and 1 will be considered superposition
			static const q_bit cSuperposition = 255;

			// Qbit superposition and Qwhole undefiined string representation
			static const string cSuporpositionSign;	// the Qwhole/Qbit unknown sign is "(S)"

			// default constructor puts Qbit into supperposition state
			Qbit() : mValue(cSuperposition) {};

			// copy constructor
			Qbit(const Qbit& right) : mValue(right.mValue) {};

			// Initialized Qbit object with a value 0 or 1, for any other value puts 
			// Qbit into superposition state
			Qbit(q_bit value) : mValue(value)
			{
				if (value > 1) mValue = cSuperposition;
			};

			operator q_bit() { return mValue; };
			operator const q_bit() const { return mValue; };

			Qbit& operator=(const Qbit right) 
			{ 
				mValue = right.mValue;
				return(*this);
			};
			Qbit& operator&=(const Qbit right)
			{
				if (mValue == cSuperposition || mValue == cSuperposition)
					mValue = cSuperposition;
				else
					mValue &= right.mValue;
				return(*this);
			};
			Qbit& operator|=(const Qbit right)
			{
				if (mValue == cSuperposition || mValue == cSuperposition)
					mValue = cSuperposition;
				else
					mValue |= right.mValue;
				return(*this);
			};
			Qbit& operator^=(const Qbit right)
			{
				if (mValue == cSuperposition || mValue == cSuperposition)
					mValue = cSuperposition;
				else
					mValue ^= right.mValue;
				return(*this);
			};

		protected:
		private:
			q_bit mValue;
		};


		class Qbool : public Qtype
		{
		public:
			// default constructor puts Qbool into supperposition state
			Qbool() : mValue(Qbit::cSuperposition) {};

			// copy constructor
			Qbool(const Qbool& right) : mValue(right.mValue) {};

			// Initialized Qbool object with a value 0 or 1, for any other value puts 
			// Qbool into superposition state
			Qbool(bool value) : mValue(value) {};

			operator bool() { return mValue; };
			operator const bool() const { return mValue; };

			Qbool& operator=(const Qbool right)
			{
				if (mValue == Qbit::cSuperposition || mValue == Qbit::cSuperposition)
					mValue = Qbit::cSuperposition;
				else
					mValue = right.mValue;
				return(*this);
			};
			Qbool& operator&=(const Qbool right)
			{
				if (mValue == Qbit::cSuperposition || mValue == Qbit::cSuperposition)
					mValue = Qbit::cSuperposition;
				else
					mValue &= right.mValue;
				return(*this);
			};
			Qbool& operator|=(const Qbool right)
			{
				if (mValue == Qbit::cSuperposition || mValue == Qbit::cSuperposition)
					mValue = Qbit::cSuperposition;
				else
					mValue |= right.mValue;
				return(*this);
			};
			Qbool& operator^=(const Qbool right)
			{
				if (mValue == Qbit::cSuperposition || mValue == Qbit::cSuperposition)
					mValue = Qbit::cSuperposition;
				else
					mValue ^= right.mValue;
				return(*this);
			};

		protected:
		private:
			q_bit mValue;
		};

		// Quantum non-negatve integer is a vector of Qbits
		typedef Matrix<q_bit, Dynamic, 1> q_binary;

		// Support for Qwhole << operator
		typedef Eigen::CommaInitializer<q_binary> QintInitializer;

		// Quantum non-negative integer, as a vector of Qubits, implements arithmetic operations
		class Qwhole : public q_binary
		{
		public:
			// the Qwhole is undefined, if any of its Qbits is in superposition state
			static const long cUnknown = LONG_MIN;

			// Qbit superposition and Qwhole undefiined string representation
			static const string cUnknownSign;	// the Qwhole/Qbit unknown sign is "(U)"

			// instantiate a Q integer without any Q bits
			Qwhole();

			// instantiate a Q integer with a given # of Qbits in superposition state
			Qwhole(Index size);

			// instantiate a Q integer with a given value,
			// the # of Qbits is determined by the # of bits in a given value
			Qwhole(long value);

			// copy constructor
			Qwhole(const Qwhole&);

			// denstruct the Q integer with its Qbits
			~Qwhole();

			// return cUnknown, if any Qbit is in superposition state,
			// otherwise return corresponding long integer
			long value() const;

			// return true, if any Qbit is in superposition state
			bool isUnknown() const;

			// get Q definition's n(umber) o(f) b(its)
			Index nobs() const { return rows(); }

			Qwhole& operator =(const Qwhole&);

			// An insertion operator (<<) to be used in combination with list operator (,);
			// as per example:
			//		Qwhole qi(Index(3)); 
			//		qi << 1, 0, 1;
			QintInitializer operator <<(const Qbit&);

			// An inversion operator returns a new Qwhole with inversed values of all Qbits
			// in definite states, i.e. 0 or 1
			Qwhole operator ~() const;

			// An 'and' operator returns a new Qwhole with logical 'and' applied on 
			// corresponding Qbits of this and right Qwhole objects. A resulting Qbit is
			// in superposition state, if any of corresponding Qbits of this or right 
			// object was in superposition state
			Qwhole operator &(const Qwhole&) const;

			// An 'and' operator changes this Qwhole instance by applying logical 'and' on
			// corresponding Qbits of this and right Qwhole object. A resulting Qbit is
			// be in superposition state, if it is or if corresponding Qbit of right object 
			// was in superposition state.
			// Return reference of this object
			Qwhole& operator &=(const Qwhole&);

			// An 'or' operator returns a new Qwhole with logical 'or' applied on 
			// corresponding Qbits of this and right Qwhole objects. A resulting Qbit is
			// in superposition state, if any of corresponding Qbits of this or right 
			// object was in superposition state
			Qwhole operator |(const Qwhole&) const;

			// An 'or' operator changes this Qwhole instance by applying logical 'or' on
			// corresponding Qbits of this and right Qwhole object. A resulting Qbit is
			// be in superposition state, if it is or if corresponding Qbit of right object 
			// was in superposition state.
			// Return reference of this object
			Qwhole& operator |=(const Qwhole&);

			// An 'nand' operator returns a new Qwhole with logical 'nand' applied on 
			// corresponding Qbits of this and right Qwhole objects. A resulting Qbit is
			// in superposition state, if any of corresponding Qbits of this or right 
			// object was in superposition state
			Qwhole nand(const Qwhole& right) const;

			// An 'nand' operator changes this Qwhole instance by applying logical 'nand' on
			// corresponding Qbits of this and right Qwhole object. A resulting Qbit is
			// be in superposition state, if it is or if corresponding Qbit of right object 
			// was in superposition state.
			// Return reference of this object
			Qwhole& nandMutable(const Qwhole& right);

			// An 'nor' operator returns a new Qwhole with logical 'nor' applied on 
			// corresponding Qbits of this and right Qwhole objects. A resulting Qbit is
			// in superposition state, if any of corresponding Qbits of this or right 
			// object was in superposition state
			Qwhole nor(const Qwhole& right) const;

			// An 'nor' operator changes this Qwhole instance by applying logical 'nor' on
			// corresponding Qbits of this and right Qwhole object. A resulting Qbit is
			// be in superposition state, if it is or if corresponding Qbit of right object 
			// was in superposition state.
			// Return reference of this object
			Qwhole& norMutable(const Qwhole& right);

			// An 'xor' operator returns a new Qwhole with logical 'xor' applied on 
			// corresponding Qbits of this and right Qwhole objects. A resulting Qbit is
			// in superposition state, if any of corresponding Qbits of this or right 
			// object was in superposition state
			Qwhole operator ^(const Qwhole&) const;

			// An 'xor' operator changes this Qwhole instance by applying logical 'xor' on
			// corresponding Qbits of this and right Qwhole object. A resulting Qbit is
			// be in superposition state, if it is or if corresponding Qbit of right object 
			// was in superposition state.
			// Return reference of this object
			Qwhole& operator ^=(const Qwhole&);

			// An 'addition' operator returns a new Qwhole with added values of this and
			// right objects. A resulting Qbit corresponding to a Qbit of this or right
			// object that is in superposition state, and any subsequent resulting Qbit,
			// will be in superposition state.
			Qwhole operator +(const Qwhole&) const;

			// An 'addition' operator returns a reference to this Qwhole object with added 
			// values of this and right object. A Qbit in superposition state, or 
			// corresponding to a Qbit of the right object that is in superposition state, 
			// together with any subsequent Qbit, will be in superposition state.
			Qwhole& operator +=(const Qwhole&);

			// An 'multiplication' operator returns a new Qwhole with multiplied values of
			// this and right objects. A resulting Qbit corresponding to a Qbit of this
			// or right object that is in superposition state, and any subsequent
			// resulting Qbit, will be in superposition state.
			Qwhole operator *(const Qwhole&) const;

			// An 'multiplication' operator returns a reference to this Qwhole object with 
			// multiplied values of this and right object. A Qbit in superposition state,
			// or corresponding to a Qbit of the right object that is in superposition 
			// state, together with any subsequent Qbit, will be in superposition state.
			Qwhole& operator *=(const Qwhole&);

			// Resize the Qwhole as a vector of Qbits to a new size
			// if the new size is bigger, assign qBit value to additional Qbits
			void resize(Index size, const Qbit& qBit = Qbit::cSuperposition);

			// If undefined, return "(S)", otherwise convert value to string
			string toString() const;

		protected:
			// Used by
			typedef Matrix<q_bit, Dynamic, Dynamic> QbitMatrix;

		private:
		};
	};
};