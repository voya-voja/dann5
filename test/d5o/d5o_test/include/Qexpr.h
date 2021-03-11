#pragma once

#include <pybind11/pybind11.h>

#include <utility>
#include <Qop.h>
#include <Factory.h>

using namespace std;

namespace dann5 {
	namespace ocean {

		// A Quantum expression
		class Qexpression
		{
		public:
			// Q expression shared pointer 
			typedef shared_ptr<Qexpression> Sp;

			// default constructor creates an empty Q expression
			Qexpression() : mpRoot(nullptr) {};

			// Initilized Q expression with its root definition 
			Qexpression(const Qdef::Sp& pRoot) : mpRoot(pRoot) {};

			// copy constructor
			Qexpression(const Qexpression& right) : mpRoot(right.mpRoot) {};

			// destructor
			~Qexpression() {};

			// returns a pointer to the Q expression root Q definition
			const Qdef::Sp& root() const { return mpRoot; };

			// set a pointer to a new root Q definition of this expression
			void root(const Qdef::Sp& pRoot) { mpRoot = pRoot; };

		protected:
		private:
			Qdef::Sp	mpRoot;
		};

		// A templated Q expression for Q types
		template <typename Q_Type>
		class Qexpr : public Qexpression
		{
		public:
			// Q definition shared pointer 
			typedef shared_ptr< Qexpr<Q_Type> > Sp;

			// default constructor creates an empty Q expression
			Qexpr() : Qexpression() {};

			// Initilized Q expression with its root definition 
			Qexpr(const Qdef::Sp& pRoot) : Qexpression(pRoot) {};

			// copy constructor
			Qexpr(const Qexpression& right) : Qexpression(right) {};

			// destructor
			~Qexpr() {};

			// Update Q expression with and logic, e.g. for an argument with id 'x' the
			// expression root points to QandOp with [previous root] & 'x'
			Qexpr<Q_Type>& operator&(const Q_Type& right) {
				Qop::Sp pOp = Factory<string, Qop>::Instance().create(AndQT::cMark);
				pOp->arguments({ root(), right.clone() });

				root(pOp);
				return(*this);
			};

			// Update Q expression with and logic, e.g. for an argument [right] the
			// expression root points to QandOp with [previous root] & [right root]
			Qexpr<Q_Type>& operator&(const Qexpr<Q_Type>& right) {
				Qop::Sp pOp = Factory<string, Qop>::Instance().create(AndQT::cMark);
				pOp->arguments({ root(), right.root() });

				root(pOp);
				return(*this);
			};

			// Update Q expression with or logic, e.g. for an argument with id 'x' the 
			// expression root points to QorOp with [previous root] | 'x'
			Qexpr<Q_Type>& operator|(const Q_Type& right) {
				Qop::Sp pOp = Factory<string, Qop>::Instance().create(OrQT::cMark);
				pOp->arguments({ root(), right.clone() });

				root(pOp);
				return(*this);
			};

			// Update Q expression with or logic, e.g. for an argument [right] the
			// expression root points to QorOp with [previous root] | [right root]
			Qexpr<Q_Type>& operator|(const Qexpr<Q_Type>& right) {
				Qop::Sp pOp = Factory<string, Qop>::Instance().create(OrQT::cMark);
				pOp->arguments({ root(), right.root() });

				root(pOp);
				return(*this);
			};

			// Update Q expression with xor logic, e.g. for an argument with id 'x' the 
			// expression root points to QxorOp with [previous root] ^ 'x'
			Qexpr<Q_Type>& operator^(const Q_Type& right) {
				Qop::Sp pOp = Factory<string, Qop>::Instance().create(XorQT::cMark);
				pOp->arguments({ root(), right.clone() });

				root(pOp);
				return(*this);
			};

			// Update Q expression with xor logic, e.g. for an argument [right] the
			// expression root points to QxorOp with [previous root] ^ [right root]
			Qexpr<Q_Type>& operator^(const Qexpr<Q_Type>& right) {
				Qop::Sp pOp = Factory<string, Qop>::Instance().create(EqQT::cMark);
				pOp->arguments({ root(), right.root() });

				root(pOp);
				return(*this);
			};

			// Update Q expression with comparison, e.g. for an argument with id 'x' the 
			// expression root points to QeqOp with [previous root] == 'x'
			Qexpr<Q_Type>& operator==(const Q_Type& right) {
				Qop::Sp pOp = Factory<string, Qop>::Instance().create(EqQT::cMark);
				pOp->arguments({ root(), right.clone() });

				root(pOp);
				return(*this);
			};

			// Update Q expression with comparison, e.g. for an argument [right] the
			// expression root points to QxorOp with [previous root] == [right root]
			Qexpr<Q_Type>& operator==(const Qexpr<Q_Type>& right) {
				Qop::Sp pOp = Factory<string, Qop>::Instance().create(EqQT::cMark);
				pOp->arguments({ root(), right.root() });

				root(pOp);
				return(*this);
			};

			// Update Q expression with comparison, e.g. for an argument with id 'x' the 
			// expression root points to QneqOp with [previous root] != 'x'
			Qexpr<Q_Type>& operator!=(const Q_Type& right) {
				Qop::Sp pOp = Factory<string, Qop>::Instance().create(NeqQT::cMark);
				pOp->arguments({ root(), right.clone() });

				root(pOp);
				return(*this);
			};

			// Update Q expression with comparison, e.g. for an argument [right] the
			// expression root points to QxorOp with [previous root] != [right root]
			Qexpr<Q_Type>& operator!=(const Qexpr<Q_Type>& right) {
				Qop::Sp pOp = Factory<string, Qop>::Instance().create(NeqQT::cMark);
				pOp->arguments({ root(), right.root() });

				root(pOp);
				return(*this);
			};

			// Update Q expression with comparison, e.g. for an argument with id 'x' the 
			// expression root points to QgtOp with [previous root] > 'x'
			Qexpr<Q_Type>& operator>(const Q_Type& right) {
				Qop::Sp pOp = Factory<string, Qop>::Instance().create(GtQT::cMark);
				pOp->arguments({ root(), right.clone() });

				root(pOp);
				return(*this);
			};

			// Update Q expression with comparison, e.g. for an argument [right] the
			// expression root points to QxorOp with [previous root] > [right root]
			Qexpr<Q_Type>& operator>(const Qexpr<Q_Type>& right) {
				Qop::Sp pOp = Factory<string, Qop>::Instance().create(GtQT::cMark);
				pOp->arguments({ root(), right.root() });

				root(pOp);
				return(*this);
			};

			// Update Q expression with comparison, e.g. for an argument with id 'x' the 
			// expression root points to QgeOp with [previous root] >= 'x'
			Qexpr<Q_Type>& operator>=(const Q_Type& right) {
				Qop::Sp pOp = Factory<string, Qop>::Instance().create(GeQT::cMark);
				pOp->arguments({ root(), right.clone() });

				root(pOp);
				return(*this);
			};

			// Update Q expression with comparison, e.g. for an argument [right] the
			// expression root points to QxorOp with [previous root] >= [right root]
			Qexpr<Q_Type>& operator>=(const Qexpr<Q_Type>& right) {
				Qop::Sp pOp = Factory<string, Qop>::Instance().create(GeQT::cMark);
				pOp->arguments({ root(), right.root() });

				root(pOp);
				return(*this);
			};

			// Update Q expression with comparison, e.g. for an argument with id 'x' the 
			// expression root points to QltOp with [previous root] < 'x'
			Qexpr<Q_Type>& operator<(const Q_Type& right) {
				Qop::Sp pOp = Factory<string, Qop>::Instance().create(LtQT::cMark);
				pOp->arguments({ root(), right.clone() });

				root(pOp);
				return(*this);
			};

			// Update Q expression with comparison, e.g. for an argument [right] the
			// expression root points to QxorOp with [previous root] < [right root]
			Qexpr<Q_Type>& operator<(const Qexpr<Q_Type>& right) {
				Qop::Sp pOp = Factory<string, Qop>::Instance().create(LtQT::cMark);
				pOp->arguments({ root(), right.root() });

				root(pOp);
				return(*this);
			};

			// Update Q expression with comparison, e.g. for an argument with id 'x' the 
			// expression root points to QleOp with [previous root] <= 'x'
			Qexpr<Q_Type>& operator<=(const Q_Type& right) {
				Qop::Sp pOp = Factory<string, Qop>::Instance().create(LeQT::cMark);
				pOp->arguments({ root(), right.clone() });

				root(pOp);
				return(*this);
			};

			// Update Q expression with comparison, e.g. for an argument [right] the
			// expression root points to QxorOp with [previous root] <= [right root]
			Qexpr<Q_Type>& operator<=(const Qexpr<Q_Type>& right) {
				Qop::Sp pOp = Factory<string, Qop>::Instance().create(LeQT::cMark);
				pOp->arguments({ root(), right.root() });

				root(pOp);
				return(*this);
			};
		};
	};
};