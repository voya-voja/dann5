#pragma once

#include <pybind11/pybind11.h>

#include <utility>
#include <Eigen/Dense>
#include <map>

#include <Qubo.h>

using namespace std;
using namespace Eigen;

namespace dann5{
	namespace ocean {
		typedef std::pair<string, string> QuboKey;
		typedef std::map<QuboKey, double> Qubo;

		Qubo& operator+=(Qubo&, const Qubo&);
		std::ostream& operator << (std::ostream&, const Qubo&);

		class QuboTable
		{
		public:
			typedef shared_ptr<QuboTable> Sp;

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
			QuboTable& operator << (const Labels& variables);

			ElementComma operator << (const double& element);
			QuboTable& operator << (const VariableRow& row);

			// returns default qubo object based on this QuboTable rule using default argument names
			Qubo qubo() const;

			// Expects a list of argument names for 2 input and 1 output
			// returns qubo object based on this Qobo rule using provided argument list
			Qubo qubo(const Labels&) const;

			~QuboTable();

			friend std::ostream& operator << (std::ostream&, const QuboTable&); 
		protected:
			QuboTable(Size);

			virtual Labels format(const Labels&) const;
		private:

			Labels			mVariables;
			MatrixXd		mElements;
		};

		class QuboTableInOut : public QuboTable
		{
		private:
		protected:
			QuboTableInOut();

			virtual Labels format(const Labels&) const;
		public:
		};

		class QuboTable2in1out : public QuboTable
		{
		private:
		protected:
			QuboTable2in1out();

			virtual Labels format(const Labels&) const;
		public:
		};

		class QuboTable2in2out : public QuboTable
		{
		private:
		protected:
			QuboTable2in2out();

			virtual Labels format(const Labels&) const;
		public:
		};


		class EqQuboTable : public QuboTableInOut
		{
		private:
		protected:
		public:
			EqQuboTable();
		};

		class NotQuboTable : public QuboTableInOut
		{
		private:
		protected:
		public:
			NotQuboTable();
		};

		class LtQuboTable : public QuboTableInOut
		{
		private:
		protected:
		public:
			LtQuboTable();
		};

		class LeQuboTable : public QuboTableInOut
		{
		private:
		protected:
		public:
			LeQuboTable();
		};

		class GtQuboTable : public QuboTableInOut
		{
		private:
		protected:
		public:
			GtQuboTable();
		};

		class GeQuboTable : public QuboTableInOut
		{
		private:
		protected:
		public:
			GeQuboTable();
		};

		class AndQuboTable : public QuboTable2in1out
		{
		private:
		protected:
		public:
			AndQuboTable();
		};

		class NandQuboTable : public QuboTable2in2out
		{
		private:
		protected:
		public:
			NandQuboTable();
		};

		class OrQuboTable : public QuboTable2in1out
		{
		private:
		protected:
		public:
			OrQuboTable();
		};

		class NorQuboTable : public QuboTable2in2out
		{
		private:
		protected:
		public:
			NorQuboTable();
		};

		class NotLeftOrRightQuboTable : public QuboTable2in2out
		{
		private:
		protected:
		public:
			NotLeftOrRightQuboTable();
		};

		class DwNotLeftOrRightQuboTable : public QuboTable2in2out
		{
		private:
		protected:
		public:
			DwNotLeftOrRightQuboTable();
		};

		class XorQuboTable : public QuboTable2in2out
		{
		private:
		protected:
		public:
			XorQuboTable();
		};

		typedef XorQuboTable Adder05QuboTable;

		class NxorQuboTable : public QuboTable2in2out
		{
		private:
		protected:
		public:
			NxorQuboTable();
		};

		class AdderQuboTable : public QuboTable
		{
		private:
		protected:
			Labels format(const Labels&) const;
		public:
			AdderQuboTable();
		};
	};
};