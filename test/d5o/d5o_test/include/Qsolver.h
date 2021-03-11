#pragma once

#include <pybind11/pybind11.h>

#include <utility>

#include <Qbit.h>
#include <Qubo.h>

using namespace std;


namespace dann5 {
	namespace ocean {
		class Qsolver {
			struct RawElement
			{
				Qnode*	pNode = nullptr;
				vector<double>	nodesEnergy;
				QbitV	value = 0;
				double	valuesXenergySum = cMaxEnergy;
			};

			typedef RawElement RawSample[];
		public:
			// A semple is defined as a dictionary (map) of definition nodes and their values.
			// The node names are defined by qubo() for each Q equation
			typedef map<string, QbitV> Sample;

			// A list of samples with the same list of nodes and different combination of values
			typedef vector<Sample> Samples;

			static const double cMaxEnergy;

			Qsolver(const Qubo& qubo, bool lowest = true);
			~Qsolver();

			Samples solution();
		protected:
			void solve(Qnodes&);
			void solve(Sample& sample, Sample::iterator at, QbitV value);

			void solveRaw(Qnodes&);
			void solve(RawElement* pSample, size_t at, size_t last, QbitV value);

		private:
			inline Sample createSample(Qnodes&, QbitV init = 0);
			inline double energyOf(Sample& sample);

			inline RawElement* createRawSample(Qnodes&, QbitV init = 0);
			inline double energyOf(RawElement* pSample, size_t at);

			Qubo		mQubo;
			Qanalyzer	mAnalyzer;
			bool		mLowest;
			double		mMinEnergy;
			Samples		mSolutions;

			long long	mCombinations = 0;
		};
	};
};
