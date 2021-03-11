#include <Qsolver.h> 
#include <iostream>

using namespace dann5::ocean;

const double Qsolver::cMaxEnergy = numeric_limits<double>::max();

Qsolver::Qsolver(const Qubo& qubo, bool lowest)
	:mQubo(qubo), mAnalyzer(mQubo), mLowest(lowest), mMinEnergy(cMaxEnergy)
{
}

Qsolver::~Qsolver()
{
}

Qsolver::Samples Qsolver::solution()
{
	if (mSolutions.size() == 0)
	{
		Qnodes& nodes = mAnalyzer.nodes();
/*		if(mAnalyzer.nodesNo() < 16)
			solve(nodes);
		else
*/			solveRaw(nodes);
	}
	return(mSolutions);
}

void Qsolver::solve(Qnodes& nodes)
{
	Sample sample = createSample(nodes);
	solve(sample, sample.begin(), 0);
	solve(sample, sample.begin(), 1);
}

void Qsolver::solve(Sample& sample, Sample::iterator at, QbitV value)
{
	mCombinations++;
	Sample::iterator next(at);
	next++;
	at->second = value;
	if (next == sample.end())
	{
		double e = energyOf(sample);
		if (e < mMinEnergy)
		{
			mMinEnergy = e;
			mSolutions.clear();
		}
		if (!mLowest || e == mMinEnergy)
		{
			mSolutions.push_back(sample);
		}
	}
	else
	{
		solve(sample, next, 0);
		solve(sample, next, 1);
	}
}

inline Qsolver::Sample Qsolver::createSample(Qnodes& nodes, QbitV init)
{
	Sample sample;
	for (auto element : nodes)
	{
		sample[element.first] = init;
	}
	return sample;
}

inline double Qsolver::energyOf(Sample& sample)
{
	double energy = 0;
	for (auto element : mQubo)
	{
		energy += sample[element.first.first] * sample[element.first.second] * element.second;
	}
	return(energy);
}

void  Qsolver::solveRaw(Qnodes& nodes)
{
	RawElement* rawSample = createRawSample(nodes);
	size_t last = nodes.size() - 1;
	solve(rawSample, 0, last, 0);
	solve(rawSample, 0, last, 1);
	delete [] rawSample;
}

void Qsolver::solve(RawElement* rawSample, size_t at, size_t last, QbitV value)
{
	mCombinations++;
	rawSample[at].value = value;
	rawSample[at].valuesXenergySum = energyOf(rawSample, at);
	if (at == last)
	{
		if (rawSample[at].valuesXenergySum < mMinEnergy)
		{
			mMinEnergy = rawSample[at].valuesXenergySum;
			mSolutions.clear();
		}
		if (!mLowest || rawSample[at].valuesXenergySum == mMinEnergy)
		{
			Sample sample;
			for (size_t index = 0; index <= last; index++)
				sample[rawSample[index].pNode->first] = rawSample[index].value;
			mSolutions.push_back(sample);
		}
	}
	else
	{
		solve(rawSample, at + 1, last, 0);
		solve(rawSample, at + 1, last, 1);
	}
}

inline Qsolver::RawElement* Qsolver::createRawSample(Qnodes& nodes, QbitV init)
{
	RawElement* sample = new RawElement[nodes.size()];
	size_t nodesNo = nodes.size();
	Qubo::iterator quboEnd = mQubo.end();
	for (size_t at = 0; at < nodesNo; at++)
	{
		sample[at].pNode = &nodes[at];
		sample[at].value = init;
		for (size_t prev = 0; prev <= at; prev++)
		{
			Qkey key(sample[prev].pNode->first, sample[at].pNode->first);
			Qubo::iterator pElement = mQubo.find(key);
			if (pElement != quboEnd)
			{
				sample[at].nodesEnergy.push_back(pElement->second);
			}
			else
			{
				key = Qkey(sample[at].pNode->first, sample[prev].pNode->first);
				pElement = mQubo.find(key);
				if (pElement != quboEnd)
				{
					sample[at].nodesEnergy.push_back(pElement->second);
				}
				else
				{
					sample[at].nodesEnergy.push_back(0);
				}
			}
		}
	}
	return sample;
}

inline double Qsolver::energyOf(RawElement* sample, size_t at)
{
	double energy = 0;
	if (at > 0)	// 
		energy = sample[at - 1].valuesXenergySum;

	for (size_t prev = 0; prev <= at; prev++)
	{
		energy += sample[prev].value * sample[at].value * sample[at].nodesEnergy[prev];
/*		Qkey key(pSample[prev].pNode->first, pSample[at].pNode->first);
		Qubo::iterator pElement = mQubo.find(key);
		if (pElement != quboEnd)
		{
			energy += pSample[prev].value * pSample[at].value * pElement->second;
		}
		else
		{
			key = Qkey(pSample[at].pNode->first, pSample[prev].pNode->first);
			pElement = mQubo.find(key);
			if (pElement != quboEnd)
			{
				energy += pSample[at].value * pSample[prev].value * pElement->second;
			}
		}*/
	}
	return(energy);
}
