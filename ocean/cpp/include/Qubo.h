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
		// Qubo is a dictionary (map) of a solution graph for a binary objective
		// function where a node pair is a key (QuboKey) mapped into its bias, which
		// corespondes to Quantum energy values for that node 
		// Qubo corresponds to DWave's dimod.BinaryQuadraticModel class, represented 
		// as an upper-diagonal matrix Q, where diagonal terms are the linear
		// coefficients (key pairs with the same Quantum node name) and the nonzero
		// off-diagonal terms the quadratic coefficients(graph branches, with
		// different mode names
		typedef std::pair<string, string> QuboKey;
		typedef std::map<QuboKey, double> Qubo;

		// Addition function returns a reference of an updated left operand with added
		// elements of right Qubo object. The left Qubo object is updated/extened by
		// adding biases for elements that exist in both left and right Qubo obects.
		// The elements that exist only in riht Qubo object are concatenated to the 
		// elements in the left object
		Qubo& operator+=(Qubo& left, const Qubo& right);

		// Sends a representation of Qubo into output stream
		std::ostream& operator << (std::ostream&, const Qubo&);

		// An abstraction of predefined Qubo tables 
		class QuboTable
		{
		public:
			// a shared pointer to a specific Qubo table
			typedef shared_ptr<QuboTable> Sp;

			// Defines a Qubo table size
			struct Size {
				unsigned nRows;
				unsigned nCols;

				Size(unsigned n) : nRows(n), nCols(n) {};
//				Size(unsigned nr, unsigned nc) : nRows(nr), nCols(nc) {};
			};

			// The Qubo table column (row) labels are 1 dimensional vector
			typedef Matrix<string, Dynamic, 1> Labels;
//			typedef std::pair<string, Matrix<double, 1, Dynamic>> VariableRow;
//			QuboTable& operator << (const Labels& variables);
//			QuboTable& operator << (const VariableRow& row);


			// An insertion operator (<<) to be used in combination with list operator (,)
			// as per example, where qt is particualar implementation of QuboTable and "a"
			// and "r" represent its Qubo nodes:
			//		qt << "a", "r"; 
			typedef Eigen::CommaInitializer<
						Eigen::Matrix<std::basic_string<char, std::char_traits<char>, 
						std::allocator<char> >, -1, 1, 0, -1, 1> > VariableComma;
			VariableComma operator << (const string& variable);

			// An insertion operator (<<) to be used in combination with list operator (,)
			// as per example, where qt is particualar implementation of QuboTable with 
			// 2 nodes:
			//		qt <<  1,  -2,
			//			   0,   1;
			typedef Eigen::CommaInitializer<Eigen::Matrix<double, -1, -1, 0, -1, -1> > ElementComma;
			ElementComma operator << (const double& element);

			// returns default qubo object based on this QuboTable rule using default argument names
			Qubo qubo() const;

			// Expects a list of argument names to be provided in order specified by particual
			// implementation, which is controled by overriding format(...) method.
			// returns qubo object based on this Qobo rule using provided argument list
			Qubo qubo(const Labels&) const;

			~QuboTable();

			// Sends a representation of QuboTable into output stream
			friend std::ostream& operator << (std::ostream&, const QuboTable&);

		protected:
			// A Qubo table can be implementated by specific specialization defining
			// a generic Qubo solution graph
			QuboTable(Size);

			// A specific implementation override to map specific Qubo variables to its 
			// generic node names
			virtual Labels format(const Labels&) const;

			// creates a Qubo from thistable for given variables
			virtual Qubo createQubo(const Labels& variables) const;
		private:

			Labels			mVariables;	// Qubo table defualt node names
			MatrixXd		mElements;	// Qubo Table value matrix
		};

		// Operator specialization of QuboTable expects one input and one output variable
		class OperatorQT : public QuboTable
		{
		private:
		protected:
			OperatorQT();

			virtual Labels format(const Labels&) const;
		public:
		};


		// BInary operation specialization of QuboTable expects two input and one
		// output variable
		class BinaryOpQT : public QuboTable
		{
		private:
		protected:
			BinaryOpQT();

			virtual Labels format(const Labels&) const;
		public:
		};


		// BInary operation specialization of QuboTable with two input and two
		// output variables
		class BinaryOp2OutQT : public QuboTable
		{
		private:
		protected:
			BinaryOp2OutQT();

			virtual Labels format(const Labels&) const;
		public:
		};


		// Equal operator specialization of QuboTable defines equal mark (sign)
		// and name of the operator
		class EqQT : public OperatorQT
		{
		private:
		protected:
		public:
			static const string cMark;
			static const string cName;

			EqQT();
		};

		// Not operator specialization of QuboTable defines equal mark (sign)
		// and name of the operator
		class NotQT : public OperatorQT
		{
		private:
		protected:
		public:
			static const string cMark;
			static const string cName;

			NotQT();
		};

		// Less-than operator specialization of QuboTable defines equal mark (sign)
		// and name of the operator
		class LtQT : public OperatorQT
		{
		private:
		protected:
		public:
			static const string cMark;
			static const string cName;

			LtQT();
		};


		// Less-eqal operator specialization of QuboTable defines equal mark (sign)
		// and name of the operator
		class LeQT : public OperatorQT
		{
		private:
		protected:
		public:
			static const string cMark;
			static const string cName;

			LeQT();
		};


		// Greater-than operator specialization of QuboTable defines equal mark (sign)
		// and name of the operator
		class GtQT : public OperatorQT
		{
		private:
		protected:
		public:
			static const string cMark;
			static const string cName;

			GtQT();
		};


		// Greater-eqal operator specialization of QuboTable defines equal mark (sign)
		// and name of the operator
		class GeQT : public OperatorQT
		{
		private:
		protected:
		public:
			static const string cMark;
			static const string cName;

			GeQT();
		};


		// And binary operation specialization of QuboTable defines equal mark (sign)
		// and name of the operator
		class AndQT : public BinaryOpQT
		{
		private:
		protected:
		public:
			static const string cMark;
			static const string cName;

			AndQT();
		};


		// Nand binary operation specialization of QuboTable with two outputs defines 
		// equal mark (sign) and name of the operator
		class NandQT : public BinaryOp2OutQT
		{
		private:
		protected:
		public:
			static const string cMark;
			static const string cName;

			NandQT();
		};


		// Or binary operation specialization of QuboTable defines equal mark (sign)
		// and name of the operator
		class OrQT : public BinaryOpQT
		{
		private:
		protected:
		public:
			static const string cMark;
			static const string cName;

			OrQT();
		};


		// Nor binary operation specialization of QuboTable with two outputs defines 
		// equal mark (sign) and name of the operator
		class NorQT : public BinaryOp2OutQT
		{
		private:
		protected:
		public:
			static const string cMark;
			static const string cName;

			NorQT();
		};

		// Not-left-orright binary operation specialization of QuboTable with two 
		// outputs defines equal mark (sign) and name of the operator
		class NotLeftOrRightQT : public BinaryOp2OutQT
		{
		private:
		protected:
		public:
			static const string cMark;
			static const string cName;

			NotLeftOrRightQT();
		};

		// DWave's not-left-orright binary operation specialization of QuboTable with 
		// two outputs defines equal mark (sign) and name of the operator
		class DwNotLeftOrRightQT : public BinaryOp2OutQT
		{
		private:
		protected:
		public:
			static const string cMark;
			static const string cName;

			DwNotLeftOrRightQT();
		};


		// Xor binary operation specialization of QuboTable with two outputs defines 
		// equal mark (sign) and name of the operator
		class XorQT : public BinaryOp2OutQT
		{
		private:
		protected:
		public:
			static const string cMark;
			static const string cName;

			XorQT();
		};

		// Not-xor binary operation specialization of QuboTable with two outputs 
		// defines equal mark (sign) and name of the operator
		class NxorQT : public BinaryOp2OutQT
		{
		private:
		protected:
		public:
			static const string cMark;
			static const string cName;

			NxorQT();
		};

		// Half adder QuboTable is same sa Xor
		class  Adder05QT : public XorQT
		{
		private:
		protected:
		public:
			static const string cMark;
			static const string cName;

			Adder05QT() {};
		};

		// Adder is trinary operation specialization of QuboTable with two outputs 
		// defines equal mark (sign) and name of the operator
		class AdderQT : public QuboTable
		{
		private:
		protected:
			Labels format(const Labels&) const;
		public:
			static const string cMark;
			static const string cName;

			AdderQT();
		};
	};
};