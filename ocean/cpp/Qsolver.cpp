#include <Qsolver.h> 
#include <iostream>

using namespace dann5::ocean;

const double Qsolver::cMaxEnergy = numeric_limits<double>::max();

Qsolver::Qsolver(const Qubo& qubo, bool lowest)
	:mQubo(qubo), mLowest(lowest), mMinEnergy(cMaxEnergy)
{
}

Qsolver::~Qsolver()
{
}

Qsolver::Samples Qsolver::solution()
{
	if (mSolutions.size() == 0)
	{
		solve();
	}
	return(mSolutions);
}

void Qsolver::solve()
{
	Sample sample = createSample();
	solve(sample, sample.begin(), 0);
	solve(sample, sample.begin(), 1);
}

template<typename STR, typename NUM>
std::ostream& dann5::ocean::operator << (std::ostream& out, const map<STR, NUM>& right)
{
	for (auto element : right)
	{
		out << element.first << " = " << to_string(element.second) << "; ";
	}
	return out;
}

string Qsolver::toString(Sample sample)
{
	string sampleStr("");
	for (auto element : sample)
	{
		sampleStr += element.first + ": " + to_string(element.second) + "; ";
	}
	return sampleStr;
}

void Qsolver::solve(Sample& sample, Sample::iterator at, q_bit value)
{
	Sample::iterator next(at);
	next++;
	at->second = value;
	if (next == sample.end())
	{
		double e = energy(sample);
		if (e < mMinEnergy)
		{
			mMinEnergy = e;
			mSolutions.clear();
			mSolutionIds.clear();
		}
		if (!mLowest || e == mMinEnergy)
		{
			mSolutions.push_back(sample);
			mSolutionIds[&sample] = e;
		}
	}
	else
	{
		solve(sample, next, 0);
		solve(sample, next, 1);
	}
}

Qsolver::Sample Qsolver::createSample(q_bit init)
{
	Sample sample;
	for (auto element : mQubo)
	{
		if (sample.find(element.first.first) == sample.end())
		{
			sample[element.first.first] = init;
		}
		if (sample.find(element.first.second) == sample.end())
		{
			sample[element.first.second] = init;
		}
	}
	return sample;
}

double Qsolver::energy(Sample& sample)
{
	double energy = 0;
	for (auto element : mQubo)
	{
		energy += sample[element.first.first] * sample[element.first.second] * element.second;
	}
	return(energy);
}
