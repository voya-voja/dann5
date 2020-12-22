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

		class OperatorQT : public QuboTable
		{
		private:
		protected:
			OperatorQT();

			virtual Labels format(const Labels&) const;
		public:
		};

		class BinaryOpQT : public QuboTable
		{
		private:
		protected:
			BinaryOpQT();

			virtual Labels format(const Labels&) const;
		public:
		};

		class BinaryOp2OutQT : public QuboTable
		{
		private:
		protected:
			BinaryOp2OutQT();

			virtual Labels format(const Labels&) const;
		public:
		};


		class EqQT : public OperatorQT
		{
		private:
		protected:
		public:
			EqQT();
		};

		class NotQT : public OperatorQT
		{
		private:
		protected:
		public:
			NotQT();
		};

		class LtQT : public OperatorQT
		{
		private:
		protected:
		public:
			LtQT();
		};

		class LeQT : public OperatorQT
		{
		private:
		protected:
		public:
			LeQT();
		};

		class GtQT : public OperatorQT
		{
		private:
		protected:
		public:
			GtQT();
		};

		class GeQT : public OperatorQT
		{
		private:
		protected:
		public:
			GeQT();
		};

		class AndQT : public BinaryOpQT
		{
		private:
		protected:
		public:
			AndQT();
		};

		class NandQT : public BinaryOp2OutQT
		{
		private:
		protected:
		public:
			NandQT();
		};

		class OrQT : public BinaryOpQT
		{
		private:
		protected:
		public:
			OrQT();
		};

		class NorQT : public BinaryOp2OutQT
		{
		private:
		protected:
		public:
			NorQT();
		};

		class NotLeftOrRightQT : public BinaryOp2OutQT
		{
		private:
		protected:
		public:
			NotLeftOrRightQT();
		};

		class DwNotLeftOrRightQT : public BinaryOp2OutQT
		{
		private:
		protected:
		public:
			DwNotLeftOrRightQT();
		};

		class XorQuboTable : public BinaryOp2OutQT
		{
		private:
		protected:
		public:
			XorQuboTable();
		};

		typedef XorQuboTable Adder05QuboTable;

		class NxorQuboTable : public BinaryOp2OutQT
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