#include <math.h> 
#include <Qroutine.h>
#include <Utility.h>

#include <Logger.h>

using namespace dann5::ocean;

/*** QuboEquation ***/


Qroutine::Qroutine(const string& name)
	: Qdef()
{
	Qdef::name(name);
	_lct(toString());
}

Qroutine::Qroutine(Index size, const string& name)
	: Qdef()
{
	Qdef::name(name);
	initialize(size);
}


Qroutine::Qroutine(const string& name, const Qstatement& statement)
	: Qdef()
{
	Qdef::name(name);
	mStatements.push_back(statement.clone());
	Index size = statement.nobs();
	initialize(size);
}

Qroutine::Qroutine(const string& name, const Qstatements& statements)
	: Qdef(), mStatements(statements)
{
	Qdef::name(name);
	Index size = 1;
	for (auto statement : mStatements)
	{
		Index sSize = statement->nobs();
		if (size < sSize)
			size = sSize;
	}
	initialize(size);
}

Qroutine::Qroutine(const Qroutine& right)
	: Qdef(right), mStatements(right.mStatements), mSolutions(right.mSolutions)
{
	initialize(right.size());
	_lct(toString());
}

void Qroutine::initialize(Index size)
{
	qbit_def_vector::resize(size);
	for (Index at = 0; at < size; at++)
	{
		(*this)(at) = operand(at);
	}
	_lct(toString());
}

Qoperand::Sp Qroutine::operand(Index level)
{
	return Qoperand::Sp(new Operand(level, *this));
}

Qroutine::~Qroutine()
{
	_ldt(toString());
}

Qroutine& Qroutine::operator<<(const Qstatement& statement)
{
	mStatements.push_back(statement.clone());
	return(*this);
}

Qroutine& Qroutine::operator<<(const Qroutine& routine)
{
	for (auto statement : routine.mStatements)
	{
		mStatements.push_back(statement);
	}
	return(*this);
}

Qubo Qroutine::qubo(bool finalized) const
{
	Qubo qubo;
	for (auto statement : mStatements)
	{
		qubo += statement->qubo(finalized);
	}
	return(qubo);
}

string Qroutine::toString(bool decomposed) const
{
	string routineStr(name() + ":\n");
	for (auto pStatement : mStatements)
	{
		routineStr += "\t" + pStatement->toString(decomposed);
	}
	return(routineStr);
}

ostream& dann5::ocean::operator << (std::ostream& out, const Qroutine& r)
{
	out << r.toString();
	return out;
}

void Qroutine::add(Sample& sample)
{
	mSolutions.push_back(sample);
}

void Qroutine::set(Samples& samples)
{
	_lat("solutions", to_string(samples.size()) + " of samples");
	mSolutions = samples;
}

string Qroutine::solutions() const
{
	string values("");
	for (auto sample : mSolutions)
	{
		for (auto statement : mStatements)
		{
			const Qvars& args = statement->arguments();
			for (auto arg : args)
			{
				Qdef definition = as_const(arg).definition();
				Qwhole value(as_const(arg).value().nobs());
				int atBit = 0;
				for (auto bitSymbol : definition)
					value[atBit++] = sample[bitSymbol->identity()];
				values += definition.name() + " = " + value.toString() + "; ";
			}
			values += "\n";
		}
	}
	_lat("solutions", values);
	return(values);
}

Qroutine::Operand::Operand(Index level, Qroutine& routine)
	:Qoperand(routine.name() + to_string(level)), mpRutine(&routine), mLevel(level)
{

}

Qroutine::Operand::Operand(const Operand& right)
	:Qoperand(right), mpRutine(right.mpRutine), mLevel(right.mLevel)
{

}

Qroutine::Operand::~Operand()
{

}

string Qroutine::Operand::toString(bool decomposed) const
{
	string rOpStr(identity());
	if (decomposed)
	{
		rOpStr += "\t";
		for (auto pStatement : mpRutine->mStatements)
		{
			rOpStr += pStatement->toString(decomposed, mLevel);
		}
	}
	return rOpStr;
}

Qubo Qroutine::Operand::qubo(bool finalized) const
{
	Qubo qubo(Qoperand::qubo(finalized ));
	for (auto pStatement : mpRutine->mStatements)
	{
		qubo += pStatement->qubo(finalized, mLevel);
	}
	return qubo;
}
