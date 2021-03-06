#pragma once

#include <pybind11/pybind11.h>

#include <utility>
#include <Eigen/Dense>

#include <Qexpression.h>
#include <Qroutine.h>

using namespace std;
using namespace Eigen;


namespace dann5 {
	namespace ocean {
		// Quantum expression of arithmetic Q operations with Q defined symbols as operands 
		class Qfunction : public Qvar
		{
		public:
			// creates Q function with a routine name 
			Qfunction(const string&);

			// creates Q function with a routine name and size of resulting variable
			Qfunction(Index size, const string&);

			// A identity Q extression initialized with a single Q defined definition
			Qfunction(const Qroutine&);

			// Copy constructor
			Qfunction(const Qfunction&);

			// Destruct the Q expression instance with containing Qdef and Q
			~Qfunction();

			const Qroutine& routine() const { return *mpRutine; };

			Qfunction& operator<<(const Qstatement& right);
			Qfunction& operator<<(const Qroutine& right);

			// Return a string representation of this Q expression
			string toString(bool decomposed = false) const;

			// Insert string representation of a Q expression into an output stream
			friend std::ostream& operator << (std::ostream&, const Qfunction&);
		protected:
			Qroutine& routine() { return *mpRutine; };

		private:
			Qroutine::Sp mpRutine;
		};

		class Qmod : public Qfunction
		{
		public:
			// dividend � divisor = quotient + remainder
			Qmod(const Qvar& dividend, const Qvar& divisor);
			Qmod(const Qvar& dividend, const Qwhole& divisor);
			Qmod(const Qdef& dividend, const Qwhole& divisor);
			Qmod(const Qdef& dividend, const Qvar& divisor);
		protected:
		private:
		};

		class Qpow : public Qfunction
		{
		public:
			// base ** power
			Qpow(const Qvar& base, int power);
			Qpow(const Qdef& base, int power);

		protected:
			void initialize(const Qvar& base, int power);

		private:
		};

		class Qgcd : public Qfunction
		{
		public:
			// dividend � divisor = quotient + remainder
			Qgcd(const Qvar& dividend, const Qvar& divisor);
			Qgcd(const Qvar& dividend, const Qdef& divisor);
			Qgcd(const Qdef& dividend, const Qvar& divisor);
			Qgcd(const Qdef& dividend, const Qdef& divisor);
		protected:
		private:
		};
	};
};