#pragma once

#include <pybind11/pybind11.h>

#include <utility>
#include <Eigen/Dense>

#include <Qint.h>
#include <Qdef.h>
#include <Qvar.h>
#include <Qexpression.h>
#include <Qubo.h>

using namespace std;
using namespace Eigen;


namespace dann5 {
	namespace ocean {


		typedef std::vector<Qvar>	QvarList;

		class Qequation
		{
		public:
			Qequation(const Qequation&);
			Qequation(const Qvar&, const Qexpression&);
			Qequation(const Qvar&, const Qexpression&, const QvarList&);
			~Qequation();

			Index size() const { return mResult.symbol().rows(); }

			const Qvar& result() const { return mResult; }
			const Qexpression& expression() const { return mExpression; }
			const QvarList& arguments() const { return mArguments; }

			void addArgument(const Qvar&);
			Qequation& operator<<(const Qvar&);

			BQM bqm(bool applyCondition = true) const;

			string toString() const;
			friend std::ostream& operator << (std::ostream&, const Qequation&);

		protected:
			class Corrector
			{
				typedef std::pair<string, Qbit> Correction;
				typedef std::map<string, Correction> Corrections;
			public:
				static const BQKey cSkip;
				Corrector(Qequation&);
				~Corrector();

				void init();
				BQKey correct(const BQKey&, bool) const;
			protected:
			private:
				Qequation&	mEquation;
				Corrections mCorrections;
			};
		private:
			Qvar			mResult;
			Qexpression		mExpression;
			QvarList		mArguments;
			Corrector		mCorrector;

			friend class Corrector;
		};
	};
};