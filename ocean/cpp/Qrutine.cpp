#include <math.h> 
#include <Qrutine.h>
#include <Utility.h>

#include <Logger.h>

using namespace dann5::ocean;

/*** QuboEquation ***/


Qrutine::Qrutine(const string& name)
	: Qdef()
{
	Qdef::name(name);
	_lct(toString());
}

Qrutine::Qrutine(Index size, const string& name)
	: Qdef()
{
	Qdef::name(name);
	initialize(size);
}


Qrutine::Qrutine(const string& name, const Qstatement& statement)
	: Qdef()
{
	Qdef::name(name);
	mStatements.push_back(statement.clone());
	Index size = statement.nobs();
	initialize(size);
}

Qrutine::Qrutine(const string& name, const Qstatements& statements)
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

Qrutine::Qrutine(const Qrutine& right)
	: Qdef(right), mStatements(right.mStatements), mSolutions(right.mSolutions)
{
	initialize(right.size());
	_lct(toString());
}

void Qrutine::initialize(Index size)
{
	qbit_def_vector::resize(size);
	for (Index at = 0; at < size; at++)
	{
		(*this)(at) = operand(at);
	}
	_lct(toString());
}

Qoperand::Sp Qrutine::operand(Index level)
{
	return Qoperand::Sp(new Operand(level, *this));
}

Qrutine::~Qrutine()
{
	_ldt(toString());
}

Qrutine& Qrutine::operator<<(const Qstatement& statement)
{
	mStatements.push_back(statement.clone());
	return(*this);
}

Qrutine& Qrutine::operator<<(const Qrutine& rutine)
{
	for (auto statement : rutine.mStatements)
	{
		mStatements.push_back(statement);
	}
	return(*this);
}

Qubo Qrutine::qubo(bool finalized) const
{
	Qubo qubo;
	for (auto statement : mStatements)
	{
		qubo += statement->qubo(finalized);
	}
	return(qubo);
}

string Qrutine::toString(bool decomposed) const
{
	string rutineStr(name() + ":\n");
	for (auto statement : (*this))
	{
		rutineStr += "\t" + statement->toString(decomposed);
	}
	return(rutineStr);
}

ostream& dann5::ocean::operator << (std::ostream& out, const Qrutine& r)
{
	out << r.toString();
	return out;
}

void Qrutine::add(Sample& sample)
{
	mSolutions.push_back(sample);
}

void Qrutine::set(Samples& samples)
{
	_lat("solutions", to_string(samples.size()) + " of samples");
	mSolutions = samples;
}

string Qrutine::solutions() const
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
				Qnni value(as_const(arg).value().nobs());
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

Qrutine::Operand::Operand(Index level, Qrutine& rutine)
	:Qoperand(rutine.name() + to_string(level)), mpRutine(&rutine), mLevel(level)
{

}

Qrutine::Operand::Operand(const Operand& right)
	:Qoperand(right), mpRutine(right.mpRutine), mLevel(right.mLevel)
{

}

Qrutine::Operand::~Operand()
{

}

string Qrutine::Operand::toString(bool decomposed) const
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

Qubo Qrutine::Operand::qubo(bool finalized) const
{
	Qubo qubo(Qoperand::qubo(finalized ));
	for (auto pStatement : mpRutine->mStatements)
	{
		qubo += pStatement->qubo(finalized, mLevel);
	}
	return qubo;
}
