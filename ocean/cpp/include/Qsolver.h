#pragma once

#include <pybind11/pybind11.h>

#include <utility>

#include <Qtype.h>
#include <Qubo.h>

using namespace std;


namespace dann5 {
	namespace ocean {
		class Qsolver {
		public:
			// A semple is defined as a dictionary (map) of definition nodes and their values.
			// The node names are defined by qubo() for each Q equation
			typedef map<string, q_bit> Sample;

			// A list of samples with the same list of nodes and different combination of values
			typedef vector<Sample> Samples;

			static const double cMaxEnergy;

			Qsolver(const Qubo& qubo, bool lowest = true);
			~Qsolver();

			Samples solution();

			size_t nOfNodes() { return createSample().size(); };
		protected:
			void solve();

			void solve(Sample& sample, Sample::iterator at, q_bit value);

			Sample createSample(q_bit init = 0);

			double energy(Sample& sample);

			string toString(Sample sample);

		private:
			Qubo mQubo;
			bool mLowest;
			double mMinEnergy;
			Samples mSolutions;
			map<Sample*, double> mSolutionIds;
		};
		template<typename STR, typename NUM>
		std::ostream& operator << (std::ostream& out, const map<STR, NUM>& right);
	};
};
