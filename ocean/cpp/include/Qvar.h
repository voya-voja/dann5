#pragma once

#include <pybind11/pybind11.h>

#include <Qint.h>
#include <Qdef.h>

using namespace std;
using namespace Eigen;


namespace dann5 {
	namespace ocean {
		class Qvar
		{
		public:
			Qvar(const Qvar&);
			Qvar(Index, const string&);
			Qvar(const string&, long);
			Qvar(const Qdef&);
			Qvar(const Qdef&, const Qint&);
			~Qvar();

			const Qdef& symbol() const { return mSymbol; }
			const Qint& value() const { return mValue; }

			void extendTo(Index size);

			friend std::ostream& operator << (std::ostream&, const Qvar&);
		protected:
		private:
			Qdef mSymbol;
			Qint mValue;
		};
	}
}
