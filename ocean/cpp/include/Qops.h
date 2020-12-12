#pragma once

#include <pybind11/pybind11.h>

#include <utility>
#include <Eigen/Dense>

#include <Qoperand.h>

#include <Qubo.h>

using namespace std;
using namespace Eigen;


namespace dann5 {
	namespace ocean {

		// An std::vector containing a list of shared pointers pointing to Qoperands
		typedef std::vector <Qoperand::Sp> Qoperands;

		// An apstraction of a Quantum operation 
		class Qop
		{
		public:
			// Qop's shared pointer 
			typedef shared_ptr<Qop> Sp;

			// Defaut constructor
			Qop();
			// Copy constructor
			Qop(const Qop&);
			// Destruct Q operation with shared pointers to its Qoperands
			~Qop();

			// Return true, if this Qop object is identical to the object pointed by pRight
			bool isIdentical(const Qop::Sp& pRight) { return(this == pRight.get()); }

			// The Qop's number of arguments
			virtual size_t argsNumber() const = 0;

			// Return Q operation arguments
			const Qoperands& arguments() const { return mArguments; };

			// Set Q operation arguments
			// throw invalid_argument exception when # of arguments in the list is not the
			// same as value defined by argsNumber() const
			virtual void arguments(const Qoperands&);

			// Return a shared pointer to the instance of updated operation after removing 
			// the given operand from the list of arguments. Return nullptr, if the last 
			// argument is removed
			virtual Qoperand::Sp remove(const Qoperand::Sp&);

			void labelFor(const string& id);
			string label() const { return mLabel; };

			void unlabel() { mLabel = ""; };
			bool isLabeled() const { return mLabel != ""; };

			string toString(bool decomposed, const string& opSymbol) const;

		protected:
			// Remove all the Qoperands from the list of arguments
			void releaseArguments();

			// Return the list of arguments
			Qoperands& arguments() { return mArguments; };

			void labelFor(const string& id, Index index);

		private:
			Qoperands	mArguments;
			string		mLabel;
		};

		// A Q operator is a unary Q operation and a Q operand
		class Qoperator : public Qoperand, public Qop
		{
		public:
			// QunOperation's shared pointer 
			typedef shared_ptr<Qoperator> Sp;

			// Default constructor
			Qoperator();

			// Copy constructor
			Qoperator(const Qoperator&);

			// Instantiate an unary Q operation with a given identity
			Qoperator(const string&);

			// Destruct Q operation  with a shared pointer to its Qoperand
			~Qoperator();

			// Return a string representation of unary Q operation
			virtual string toString(bool decomposed = false) const;

			// The QunOperation has one argument
			virtual size_t argsNumber() const { return(1); };

			// return Qubo presentation of this QunOperation using corresponding QuboTable 
			// transformation matrix
			virtual Qubo qubo() const;
		protected:
		private:
		};

		// A Quntum and is a binary operation
		class Qequal : public Qoperator
		{
		public:
			// Qequal's shared pointer 
			typedef shared_ptr<Qequal> Sp;

			// Instantiate Qequal instance with '&' identity
			Qequal() : Qoperator("=") {};

			// Copy constructor
			Qequal(const Qequal& right) : Qoperator(right) {};

			// Destruct Qequal with shared pointers to its Qoperand
			~Qequal() {};

			// Return a Qoperand's shared pointer pointing to a copy of this object 
			virtual Qoperand::Sp clone() const { return Qoperand::Sp(new Qequal(*this)); };

		protected:
		private:
		};

		// A Quntum and is a binary operation
		class Qnot : public Qoperator
		{
		public:
			// Qnot's shared pointer 
			typedef shared_ptr<Qnot> Sp;

			// Instantiate Qnot instance with '&' identity
			Qnot() : Qoperator("~") {};

			// Copy constructor
			Qnot(const Qnot& right) : Qoperator(right) {};

			// Destruct Qnot with shared pointers to its Qoperand
			~Qnot() {};

			// Return a Qoperand's shared pointer pointing to a copy of this object 
			virtual Qoperand::Sp clone() const { return Qoperand::Sp(new Qnot(*this)); };

		protected:
		private:
		};

		// A Quntum and is a binary operation
		class Qlt : public Qoperator
		{
		public:
			// Qlt's shared pointer 
			typedef shared_ptr<Qlt> Sp;

			// Instantiate Qlt instance with '&' identity
			Qlt() : Qoperator("<") {};

			// Copy constructor
			Qlt(const Qlt& right) : Qoperator(right) {};

			// Destruct Qlt with shared pointers to its Qoperand
			~Qlt() {};

			// Return a Qoperand's shared pointer pointing to a copy of this object 
			virtual Qoperand::Sp clone() const { return Qoperand::Sp(new Qlt(*this)); };

		protected:
		private:
		};

		// A Quntum and is a binary operation
		class Qle : public Qoperator
		{
		public:
			// Qle's shared pointer 
			typedef shared_ptr<Qle> Sp;

			// Instantiate Qle instance with '&' identity
			Qle() : Qoperator("<=") {};

			// Copy constructor
			Qle(const Qle& right) : Qoperator(right) {};

			// Destruct Qle with shared pointers to its Qoperand
			~Qle() {};

			// Return a Qoperand's shared pointer pointing to a copy of this object 
			virtual Qoperand::Sp clone() const { return Qoperand::Sp(new Qle(*this)); };

		protected:
		private:
		};

		// A Quntum and is a binary operation
		class Qgt : public Qoperator
		{
		public:
			// Qgt's shared pointer 
			typedef shared_ptr<Qgt> Sp;

			// Instantiate Qgt instance with '&' identity
			Qgt() : Qoperator(">") {};

			// Copy constructor
			Qgt(const Qgt& right) : Qoperator(right) {};

			// Destruct Qgt with shared pointers to its Qoperand
			~Qgt() {};

			// Return a Qoperand's shared pointer pointing to a copy of this object 
			virtual Qoperand::Sp clone() const { return Qoperand::Sp(new Qgt(*this)); };

		protected:
		private:
		};

		// A Quntum and is a binary operation
		class Qge : public Qoperator
		{
		public:
			// Qge's shared pointer 
			typedef shared_ptr<Qge> Sp;

			// Instantiate Qge instance with '&' identity
			Qge() : Qoperator(">=") {};

			// Copy constructor
			Qge(const Qge& right) : Qoperator(right) {};

			// Destruct Qge with shared pointers to its Qoperand
			~Qge() {};

			// Return a Qoperand's shared pointer pointing to a copy of this object 
			virtual Qoperand::Sp clone() const { return Qoperand::Sp(new Qge(*this)); };

		protected:
		private:
		};

		// An binary Q operation is a Q operand as well
		class QbiOperation : public Qoperand, public Qop
		{
		public:
			// QbiOperation's shared pointer 
			typedef shared_ptr<QbiOperation> Sp;

			// Default constructor
			QbiOperation();

			// Copy constructor
			QbiOperation(const QbiOperation&);

			// Instantiate an binary Q operation with a given identity
			QbiOperation(const string&);

			// Destruct Q operation  with shared pointers to its two Qoperands
			~QbiOperation();

			// Return a string representation of binary Q operation
			virtual string toString(bool decomposed = false) const;

			// The QbiOperation has two argument
			virtual size_t argsNumber() const { return(2); };

			// return Qubo presentation of this QbiOperation using corresponding QuboTable 
			// transformation matrix
			virtual Qubo qubo() const;
		protected:
		private:
		};

		// A Quntum and is a binary operation
		class Qand : public QbiOperation
		{
		public:
			// Qand's shared pointer 
			typedef shared_ptr<Qand> Sp;

			// Instantiate Qand instance with '&' identity
			Qand() : QbiOperation("&") {};

			// Copy constructor
			Qand(const Qand& right) : QbiOperation(right) {};

			// Destruct Qand with shared pointers to its two Qoperands
			~Qand() {};

			// Return a Qoperand's shared pointer pointing to a copy of this object 
			virtual Qoperand::Sp clone() const { return Qoperand::Sp(new Qand(*this)); };

		protected:
		private:
		};

		// A Quntum nAnd is a binary operation
		class QnAnd : public QbiOperation
		{
		public:
			// QnAnd's shared pointer 
			typedef shared_ptr<QnAnd> Sp;

			// Instantiate QnAnd instance with '&' identity
			QnAnd() : QbiOperation("~&") {};

			// Copy constructor
			QnAnd(const QnAnd& right) : QbiOperation(right) {};

			// Destruct QnAnd with shared pointers to its two Qoperands
			~QnAnd() {};

			// Return a Qoperand's shared pointer pointing to a copy of this object 
			virtual Qoperand::Sp clone() const { return Qoperand::Sp(new QnAnd(*this)); };

		protected:
		private:
		};

		// A Quntum or is a binary operation
		class Qor : public QbiOperation
		{
		public:
			// Qor's shared pointer 
			typedef shared_ptr<Qand> Sp;

			// Instantiate Qor instance with '|' identity
			Qor() : QbiOperation("|") {};

			// Copy constructor
			Qor(const Qor& right) : QbiOperation(right) {};

			// Destruct Qor with shared pointers to its two Qoperands
			~Qor() {};

			// Return a Qoperand's shared pointer pointing to a copy of this object 
			virtual Qoperand::Sp clone() const { return Qoperand::Sp(new Qor(*this)); };

		protected:
		private:
		};

		// A Quntum nOr is a binary operation
		class QnOr : public QbiOperation
		{
		public:
			// QnOr's shared pointer 
			typedef shared_ptr<Qand> Sp;

			// Instantiate QnOr instance with '|' identity
			QnOr() : QbiOperation("~|") {};

			// Copy constructor
			QnOr(const QnOr& right) : QbiOperation(right) {};

			// Destruct QnOr with shared pointers to its two Qoperands
			~QnOr() {};

			// Return a Qoperand's shared pointer pointing to a copy of this object 
			virtual Qoperand::Sp clone() const { return Qoperand::Sp(new QnOr(*this)); };

		protected:
		private:
		};


		// An abstraction od Q addition
		class Qaddition
		{
		public:
			// Qaddition's shared pointer 
			typedef shared_ptr<Qaddition> Sp;


			// A Quntum carry is an operand
			class Carry : public Qoperand
			{
			public:
				const static string cSymbol;	// The carry opernd symbol is #

				// Q carry's shared pointer 
				typedef shared_ptr<Carry> Sp;

				// Return converted given string to Q carry symbol, e.g. 'a0 ^ b0' is 
				// converted into '#{a0 ^ b0}'
				static string Symbol(const string&);

				// Initiate Carry instance with a pointer to a parent Q addition
				Carry(Qaddition*);

				// Copy constructor
				Carry(const Carry&);

				// Destruct a Q carry operand
				~Carry();

				// Return a Qoperand's shared pointer pointing to a copy of this object 
				virtual Qoperand::Sp clone() const { return Qoperand::Sp(new Carry(*this)); };

				// Return a string representation of Q carry operand
				virtual string toString(bool decomposed = false) const;

				// Set a pointer to a new parent Q addition
				void addition(Qaddition*);

			protected:
			private:
				Qaddition* mpAddition;	// A parent Q addition
			};

			// Deafult constructor
			Qaddition();

			// Destruct the Q addition instance with a shared pointer to its carry operand
			~Qaddition();

			// Return a shared pointer to Q addition object appropriate for assigned 
			// operands list
			virtual Qaddition::Sp assign(const Qoperands&);

			// Assign a share point to a new Q carry operand
			virtual void carry(const Carry::Sp& pCarry);

			// Return a share pointer to Q carry operand of this Qaddition
			virtual Carry::Sp carry();

			// Override to return string representation of this Qaddition
//			virtual string toString() const = 0;
		protected:

		private:
			Carry::Sp mpCarry = nullptr;
		};

		// A Quntum xOr is a binary Q addition operation
		class Qxor : public Qaddition, public QbiOperation
		{
		public:
			// Qxor's shared pointer 
			typedef shared_ptr<Qxor> Sp;
			
			// Default constructor
			Qxor();

			// Copy constructor
			Qxor(const Qxor&);

			// Destruct the Q xor instance with a shared pointer to its carry operand
			// and two arguments
			~Qxor();

			// Return a shared pointer to this object if one of the operands is QxOr
			// object, otherwise return a shared pointer to a new Q adder object ineriting
			// two arguments of Qxor operand and other operand, resulting in conversion of
			// two xor binary operations, e.g. ^(^(a0, b0), c0) to one adder trinary operation,
			// e.g. +(a0, b0, c0)
			virtual Qaddition::Sp assign(const Qoperands&);

			// Return a Qoperand's shared pointer pointing to a copy of this object 
			virtual Qoperand::Sp clone() const { return Qoperand::Sp(new Qxor(*this));};
		protected:
		private:
		};

		// A Quntum xOr is also Q half adder
		typedef Qxor Qadder05;


		// An trinary Q operation is a Q operand as well
		class QtriOperation : public Qoperand, public Qop
		{
		public:
			// QtriOperation's shared pointer 
			typedef shared_ptr<QtriOperation> Sp;

			// Default constructor
			QtriOperation();

			// Copy constructor
			QtriOperation(const QtriOperation&);

			// Instantiate an trinary Q operation with a given identity
			QtriOperation(const string&);

			// Destruct Q operation with shared pointers to its three Qoperands
			~QtriOperation();

			// Return a string representation of trinary Q operation
			virtual string toString(bool decomposed = false) const;

			// The QtriOperation has three argument
			virtual size_t argsNumber() const { return(3); };

			// return Qubo presentation of this QtriOperation using corresponding QuboTable 
			// transformation matrix
			virtual Qubo qubo() const;

		protected:
		private:
		};

		class Qadder : public Qaddition, public QtriOperation
		{
		public:
			// QtriOperation's shared pointer 
			typedef shared_ptr<Qadder> Sp;

			// Default constructor
			Qadder();

			// Copy constructor
			Qadder(const Qadder&);

			// Destruct the Q adder instance with a shared pointer to its carry operand
			// and two arguments
			~Qadder();

			// Return a shared pointer to this Qadder, if shared pointer is not pointing 
			// to any of adders three arguments, otherwise return a shared pointer to a 
			// new QxOr operation with two remaining arguments
			virtual Qoperand::Sp remove(const Qoperand::Sp& pToRemove);

			// Return a Qoperand's shared pointer pointing to a copy of this object 
			virtual Qoperand::Sp clone() const { return Qoperand::Sp(new Qadder(*this)); };

		protected:
		private:
		};
	};
};