#pragma once

#include <pybind11/pybind11.h>

#include <utility>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;


namespace dann5 {
	namespace ocean {
		class Qexpression;

		typedef Matrix<string, Dynamic, 1> StringVector;
		class Qdef : public StringVector
		{
		public:
			Qdef();
			Qdef(const Qdef&);
			Qdef(Index size);
			Qdef(Index size, const string& name);
			~Qdef();

			const string& name() { return mName; }
			void name(const string& name) { mName = name; }

			Qexpression operator *(const Qdef&) const;

			Qexpression operator +(const Qdef&) const;

			void extendTo(Index size);

		protected:
		private:
			const static string	cDefaultName;
			static Index		gNameIndex;

			static string AutoName();
			string	mName;
		};
	};
};