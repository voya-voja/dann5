#pragma once

#include <pybind11/pybind11.h>

#include <utility>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;


namespace dann5 {
	namespace ocean {

		typedef unsigned char Qbit;
		typedef Matrix<Qbit, Dynamic, 1> qbit_int;
		typedef CommaInitializer<qbit_int> QintInitializer;

		class Qint : public qbit_int
		{
		public:
			static const Qbit cSuperposition = 255;

			Qint();
			Qint(const Qint&);
			Qint(Index size);
			Qint(long value);
			~Qint();

			Qint& operator =(const Qint&);
			long value() const;

			QintInitializer operator <<(const bool&);
			Qint operator ~() const;

			Qint operator &(const Qint&) const;
			Qint& operator &=(const Qint&);

			Qint operator |(const Qint&) const;
			Qint& operator |=(const Qint&);

			Qint operator ^(const Qint&) const;
			Qint& operator ^=(const Qint&);

			Qint operator +(const Qint&) const;
			Qint& operator +=(const Qint&);

			Qint operator *(const Qint&) const;
			Qint& operator *=(const Qint&);

			void extendTo(Index size);

		protected:
			typedef Matrix<Qbit, Dynamic, Dynamic> Xmatrix;

		private:
		};
	};
};