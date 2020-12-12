#pragma once

#include <pybind11/pybind11.h>

#include <utility>
#include <Eigen/Dense>
#include <map>

#include <BQM.h>

using namespace std;
using namespace Eigen;

namespace dann5{
	namespace ocean {

		class Qubo
		{
		public:

			struct Size {
				unsigned nRows;
				unsigned nCols;

				Size(unsigned n) : nRows(n), nCols(n) {};
				Size(unsigned nr, unsigned nc) : nRows(nr), nCols(nc) {};
			};

			typedef Eigen::CommaInitializer<
						Eigen::Matrix<std::basic_string<char, std::char_traits<char>, 
						std::allocator<char> >, -1, 1, 0, -1, 1> > VariableComma;
			typedef Eigen::CommaInitializer<Eigen::Matrix<double, -1, -1, 0, -1, -1> > ElementComma;

			typedef Matrix<string, Dynamic, 1> Labels;
			typedef std::pair<string, Matrix<double, 1, Dynamic>> VariableRow;

			VariableComma operator << (const string& variable);
			Qubo& operator << (const Labels& variables);

			ElementComma operator << (const double& element);
			Qubo& operator << (const VariableRow& row);

			// returns default bqm object based on this Qubo rule using default argument names
			BQM bqm() const;

			// Expects a list of argument names for 2 input and 1 output
			// returns bqm object based on this Qobo rule using provided argument list
			BQM bqm(const Labels&) const;

			~Qubo();

			friend std::ostream& operator << (std::ostream&, const Qubo&); 
		protected:
			Qubo(Size);

			virtual Labels format(const Labels&) const;
		private:

			Labels			mVariables;
			MatrixXd		mElements;
		};

		class QuboInOut : public Qubo
		{
		private:
		protected:
			QuboInOut();

			virtual Labels format(const Labels&) const;
		public:
		};

		class Qubo2in1out : public Qubo
		{
		private:
		protected:
			Qubo2in1out();

			virtual Labels format(const Labels&) const;
		public:
		};

		class Qubo2in2out : public Qubo
		{
		private:
		protected:
			Qubo2in2out();

			virtual Labels format(const Labels&) const;
		public:
		};


		class EqualQubo : public QuboInOut
		{
		private:
		protected:
		public:
			EqualQubo();
		};

		class NotQubo : public QuboInOut
		{
		private:
		protected:
		public:
			NotQubo();
		};

		class LessThanQubo : public QuboInOut
		{
		private:
		protected:
		public:
			LessThanQubo();
		};

		class LessEqualQubo : public QuboInOut
		{
		private:
		protected:
		public:
			LessEqualQubo();
		};

		class GreaterThanQubo : public QuboInOut
		{
		private:
		protected:
		public:
			GreaterThanQubo();
		};

		class GreaterEqualQubo : public QuboInOut
		{
		private:
		protected:
		public:
			GreaterEqualQubo();
		};

		class AndQubo : public Qubo2in1out
		{
		private:
		protected:
		public:
			AndQubo();
		};

		class NandQubo : public Qubo2in2out
		{
		private:
		protected:
		public:
			NandQubo();
		};

		class OrQubo : public Qubo2in1out
		{
		private:
		protected:
		public:
			OrQubo();
		};

		class NorQubo : public Qubo2in2out
		{
		private:
		protected:
		public:
			NorQubo();
		};

		class NotLeftOrRightQubo : public Qubo2in2out
		{
		private:
		protected:
		public:
			NotLeftOrRightQubo();
		};

		class DwNotLeftOrRightQubo : public Qubo2in2out
		{
		private:
		protected:
		public:
			DwNotLeftOrRightQubo();
		};

		class XorQubo : public Qubo2in2out
		{
		private:
		protected:
		public:
			XorQubo();
		};

		typedef XorQubo HalfAdderQubo;

		class NxorQubo : public Qubo2in2out
		{
		private:
		protected:
		public:
			NxorQubo();
		};

		class AdderQubo : public Qubo
		{
		private:
		protected:
			Labels format(const Labels&) const;
		public:
			AdderQubo();
		};
	};
};