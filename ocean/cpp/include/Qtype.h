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

			// Qbit superposition and Qnni undefiined string representation
			static const string cSuporpositionSign;	// the Qnni/Qbit unknown sign is "(S)"

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
		typedef Matrix<q_bit, Dynamic, 1> q_nni;

		// Support for Qnni << operator
		typedef Eigen::CommaInitializer<q_nni> QintInitializer;

		// Quantum non-negative integer, as a vector of Qubits, implements arithmetic operations
		class Qnni : public q_nni
		{
		public:
			// the Qnni is undefined, if any of its Qbits is in superposition state
			static const long cUnknown = LONG_MIN;

			// Qbit superposition and Qnni undefiined string representation
			static const string cUnknownSign;	// the Qnni/Qbit unknown sign is "(U)"

			// instantiate a Q integer without any Q bits
			Qnni();

			// instantiate a Q integer with a given # of Qbits in superposition state
			Qnni(Index size);

			// instantiate a Q integer with a given value,
			// the # of Qbits is determined by the # of bits in a given value
			Qnni(long value);

			// copy constructor
			Qnni(const Qnni&);

			// denstruct the Q integer with its Qbits
			~Qnni();

			// return cUnknown, if any Qbit is in superposition state,
			// otherwise return corresponding long integer
			long value() const;

			// return true, if any Qbit is in superposition state
			bool isUnknown() const;

			// get Q definition's n(umber) o(f) b(its)
			Index nobs() const { return rows(); }

			Qnni& operator =(const Qnni&);

			// An insertion operator (<<) to be used in combination with list operator (,);
			// as per example:
			//		Qnni qi(Index(3)); 
			//		qi << 1, 0, 1;
			QintInitializer operator <<(const Qbit&);

			// An inversion operator returns a new Qnni with inversed values of all Qbits
			// in definite states, i.e. 0 or 1
			Qnni operator ~() const;

			// An 'and' operator returns a new Qnni with logical 'and' applied on 
			// corresponding Qbits of this and right Qnni objects. A resulting Qbit is
			// in superposition state, if any of corresponding Qbits of this or right 
			// object was in superposition state
			Qnni operator &(const Qnni&) const;

			// An 'and' operator changes this Qnni instance by applying logical 'and' on
			// corresponding Qbits of this and right Qnni object. A resulting Qbit is
			// be in superposition state, if it is or if corresponding Qbit of right object 
			// was in superposition state.
			// Return reference of this object
			Qnni& operator &=(const Qnni&);

			// An 'or' operator returns a new Qnni with logical 'or' applied on 
			// corresponding Qbits of this and right Qnni objects. A resulting Qbit is
			// in superposition state, if any of corresponding Qbits of this or right 
			// object was in superposition state
			Qnni operator |(const Qnni&) const;

			// An 'or' operator changes this Qnni instance by applying logical 'or' on
			// corresponding Qbits of this and right Qnni object. A resulting Qbit is
			// be in superposition state, if it is or if corresponding Qbit of right object 
			// was in superposition state.
			// Return reference of this object
			Qnni& operator |=(const Qnni&);

			// An 'nand' operator returns a new Qnni with logical 'nand' applied on 
			// corresponding Qbits of this and right Qnni objects. A resulting Qbit is
			// in superposition state, if any of corresponding Qbits of this or right 
			// object was in superposition state
			Qnni nand(const Qnni& right) const;

			// An 'nand' operator changes this Qnni instance by applying logical 'nand' on
			// corresponding Qbits of this and right Qnni object. A resulting Qbit is
			// be in superposition state, if it is or if corresponding Qbit of right object 
			// was in superposition state.
			// Return reference of this object
			Qnni& nandMutable(const Qnni& right);

			// An 'nor' operator returns a new Qnni with logical 'nor' applied on 
			// corresponding Qbits of this and right Qnni objects. A resulting Qbit is
			// in superposition state, if any of corresponding Qbits of this or right 
			// object was in superposition state
			Qnni nor(const Qnni& right) const;

			// An 'nor' operator changes this Qnni instance by applying logical 'nor' on
			// corresponding Qbits of this and right Qnni object. A resulting Qbit is
			// be in superposition state, if it is or if corresponding Qbit of right object 
			// was in superposition state.
			// Return reference of this object
			Qnni& norMutable(const Qnni& right);

			// An 'xor' operator returns a new Qnni with logical 'xor' applied on 
			// corresponding Qbits of this and right Qnni objects. A resulting Qbit is
			// in superposition state, if any of corresponding Qbits of this or right 
			// object was in superposition state
			Qnni operator ^(const Qnni&) const;

			// An 'xor' operator changes this Qnni instance by applying logical 'xor' on
			// corresponding Qbits of this and right Qnni object. A resulting Qbit is
			// be in superposition state, if it is or if corresponding Qbit of right object 
			// was in superposition state.
			// Return reference of this object
			Qnni& operator ^=(const Qnni&);

			// An 'addition' operator returns a new Qnni with added values of this and
			// right objects. A resulting Qbit corresponding to a Qbit of this or right
			// object that is in superposition state, and any subsequent resulting Qbit,
			// will be in superposition state.
			Qnni operator +(const Qnni&) const;

			// An 'addition' operator returns a reference to this Qnni object with added 
			// values of this and right object. A Qbit in superposition state, or 
			// corresponding to a Qbit of the right object that is in superposition state, 
			// together with any subsequent Qbit, will be in superposition state.
			Qnni& operator +=(const Qnni&);

			// An 'multiplication' operator returns a new Qnni with multiplied values of
			// this and right objects. A resulting Qbit corresponding to a Qbit of this
			// or right object that is in superposition state, and any subsequent
			// resulting Qbit, will be in superposition state.
			Qnni operator *(const Qnni&) const;

			// An 'multiplication' operator returns a reference to this Qnni object with 
			// multiplied values of this and right object. A Qbit in superposition state,
			// or corresponding to a Qbit of the right object that is in superposition 
			// state, together with any subsequent Qbit, will be in superposition state.
			Qnni& operator *=(const Qnni&);

			// Resize the Qnni as a vector of Qbits to a new size
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