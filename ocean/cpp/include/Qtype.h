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

			// Qbit superposition and Qint undefiined string representation
			static const string cSuporpositionSign;	// the Qint/Qbit unknown sign is "(S)"

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

		// Quantum integer is a vector of Qbits
		typedef Matrix<q_bit, Dynamic, 1> q_int;

		// Support for Qint << operator
		typedef Eigen::CommaInitializer<q_int> QintInitializer;

		// Quantum integer, as a vector of Qubits, implements arithmetic operations
		class Qint : public q_int
		{
		public:
			// the Qint is undefined, if any of its Qbits is in superposition state
			static const long cUnknown = LONG_MIN;

			// Qbit superposition and Qint undefiined string representation
			static const string cUnknownSign;	// the Qint/Qbit unknown sign is "(U)"

			// instantiate a Q integer without any Q bits
			Qint();

			// instantiate a Q integer with a given # of Qbits in superposition state
			Qint(Index size);

			// instantiate a Q integer with a given value,
			// the # of Qbits is determined by the # of bits in a given value
			Qint(long value);

			// copy constructor
			Qint(const Qint&);

			// denstruct the Q integer with its Qbits
			~Qint();

			Qint& operator =(const Qint&);

			// return cUnknown, if any Qbit is in superposition state,
			// otherwise return corresponding long integer
			long value() const;

			// return true, if any Qbit is in superposition state
			bool isUnknown() const;

			// An insertion operator (<<) to be used in combination with list operator (,);
			// as per example:
			//		Qint qi(Index(3)); 
			//		qi << 1, 0, 1;
			QintInitializer operator <<(const Qbit&);

			// An inversion operator returns a new Qint with inversed values of all Qbits
			// in definite states, i.e. 0 or 1
			Qint operator ~() const;

			// An 'and' operator returns a new Qint with logical 'and' applied on 
			// corresponding Qbits of this and right Qint objects. A resulting Qbit is
			// in superposition state, if any of corresponding Qbits of this or right 
			// object was in superposition state
			Qint operator &(const Qint&) const;

			// An 'and' operator changes this Qint instance by applying logical 'and' on
			// corresponding Qbits of this and right Qint object. A resulting Qbit is
			// be in superposition state, if it is or if corresponding Qbit of right object 
			// was in superposition state.
			// Return reference of this object
			Qint& operator &=(const Qint&);

			// An 'or' operator returns a new Qint with logical 'or' applied on 
			// corresponding Qbits of this and right Qint objects. A resulting Qbit is
			// in superposition state, if any of corresponding Qbits of this or right 
			// object was in superposition state
			Qint operator |(const Qint&) const;

			// An 'or' operator changes this Qint instance by applying logical 'or' on
			// corresponding Qbits of this and right Qint object. A resulting Qbit is
			// be in superposition state, if it is or if corresponding Qbit of right object 
			// was in superposition state.
			// Return reference of this object
			Qint& operator |=(const Qint&);

			// An 'xor' operator returns a new Qint with logical 'xor' applied on 
			// corresponding Qbits of this and right Qint objects. A resulting Qbit is
			// in superposition state, if any of corresponding Qbits of this or right 
			// object was in superposition state
			Qint operator ^(const Qint&) const;

			// An 'xor' operator changes this Qint instance by applying logical 'xor' on
			// corresponding Qbits of this and right Qint object. A resulting Qbit is
			// be in superposition state, if it is or if corresponding Qbit of right object 
			// was in superposition state.
			// Return reference of this object
			Qint& operator ^=(const Qint&);

			// An 'addition' operator returns a new Qint with added values of this and
			// right objects. A resulting Qbit corresponding to a Qbit of this or right
			// object that is in superposition state, and any subsequent resulting Qbit,
			// will be in superposition state.
			Qint operator +(const Qint&) const;

			// An 'addition' operator returns a reference to this Qint object with added 
			// values of this and right object. A Qbit in superposition state, or 
			// corresponding to a Qbit of the right object that is in superposition state, 
			// together with any subsequent Qbit, will be in superposition state.
			Qint& operator +=(const Qint&);

			// An 'multiplication' operator returns a new Qint with multiplied values of
			// this and right objects. A resulting Qbit corresponding to a Qbit of this
			// or right object that is in superposition state, and any subsequent
			// resulting Qbit, will be in superposition state.
			Qint operator *(const Qint&) const;

			// An 'multiplication' operator returns a reference to this Qint object with 
			// multiplied values of this and right object. A Qbit in superposition state,
			// or corresponding to a Qbit of the right object that is in superposition 
			// state, together with any subsequent Qbit, will be in superposition state.
			Qint& operator *=(const Qint&);

			// Resize the Qint as a vector of Qbits to a new size
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