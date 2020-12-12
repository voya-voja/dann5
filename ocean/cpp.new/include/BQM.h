#pragma once

#include <pybind11/pybind11.h>

#include <utility>
#include <Eigen/Dense>
#include <map>

using namespace std;
using namespace Eigen;

namespace dann5 {
	namespace ocean {
		typedef std::pair<string, string> BQKey;
		typedef std::map<BQKey, double> BQDictionary;

		class BQM : public BQDictionary
		{
		public:
			BQM();
			BQM(const BQM&);

			BQM& operator=(const BQM&);
			BQM operator+(const BQM&) const;
			BQM& operator+=(const BQM&);

			BQDictionary& dictionary();

			friend std::ostream& operator << (std::ostream&, const BQM&);

		protected:
		private:
		};

	};
};