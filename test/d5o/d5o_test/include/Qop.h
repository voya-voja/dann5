#pragma once

#include <pybind11/pybind11.h>

#include <utility>

#include <Qdef.h>

#include <Qubo.h>

using namespace std;

namespace dann5 {
	namespace ocean {

		// An apstraction of a Quantum operation is a Q operand
		class Qop : public Qdef
		{
		public:
			// Qop's shared pointer 
			typedef shared_ptr<Qop> Sp;

			// An Q operation has identity and should have at least one argument
			Qop(const string& id, size_t noArguments = 1);

			// Copy constructor
			Qop(const Qop&);

			// Destruct Q operation with shared pointers to its arguments
			~Qop();

			// Return true, if this Qop object is identical to the object pointed by pRight
			bool isIdentical(const Qop::Sp& pRight) { return(this == pRight.get()); }

			// The Qop's number of arguments
			size_t argsNumber() const { return mNoArguments; }

			// Return Q operation arguments
			const Qdefs& arguments() const { return mArguments; };

			// Set Q operation arguments
			// throw invalid_argument exception when # of arguments in the list is not
			// the same as value defined by argsNumber() const
			virtual void arguments(const Qdefs&);

			// add Q operation argument to the list of the arguments
			// throw invalid_argument exception preventing a new argument to be added to
			// the list once the # of arguments is equal to the # defined by
			// argsNumber() method
			virtual void add(Qdef::Sp argument);

			void labelFor(const string& id);
			string label() const { return mLabel; };

			void unlabel() { mLabel = ""; };
			bool isLabeled() const { return mLabel != ""; };

			// convert operation expration into a string
			virtual string toString(bool decomposed = false) const;

			// return Qubo presentation of this Qdef
			virtual Qubo qubo(bool finalized = true) const;

		protected:
			// Remove all the Qdefs from the list of arguments
			void releaseArguments();

			// Return the list of arguments
			Qdefs& arguments() { return mArguments; };

			void labelFor(const string& id, Index index);

		private:
			Qdefs	mArguments;
			size_t	mNoArguments;
			string	mLabel;
		};

		// A Quntum and is a binary operation
		class Qeq : public Qop
		{
		public:
			// Qeq's shared pointer 
			typedef shared_ptr<Qeq> Sp;

			// Instantiate Qeq instance with '&' identity
			Qeq() : Qop(EqQT::cMark, 2) {};

			// Copy constructor
			Qeq(const Qeq& right) : Qop(right) {};

			// Destruct Qeq with shared pointers to its Qdef
			~Qeq() {};

			// Return a Qdef's shared pointer pointing to a copy of this object 
			virtual Qdef::Sp clone() const { return Qdef::Sp(new Qeq(*this)); };

		protected:
		private:
		};

		// A Quntum and is a binary operation
		class Qneq : public Qop
		{
		public:
			// Qneq's shared pointer 
			typedef shared_ptr<Qneq> Sp;

			// Instantiate Qneq instance with '&' identity
			Qneq() : Qop(NeqQT::cMark, 2) {};

			// Copy constructor
			Qneq(const Qneq& right) : Qop(right) {};

			// Destruct Qneq with shared pointers to its Qdef
			~Qneq() {};

			// Return a Qdef's shared pointer pointing to a copy of this object 
			virtual Qdef::Sp clone() const { return Qdef::Sp(new Qneq(*this)); };

		protected:
		private:
		};

		// A Quntum and is a binary operation
		class Qlt : public Qop
		{
		public:
			// Qlt's shared pointer 
			typedef shared_ptr<Qlt> Sp;

			// Instantiate Qlt instance with '&' identity
			Qlt() : Qop(LtQT::cMark, 2) {};

			// Copy constructor
			Qlt(const Qlt& right) : Qop(right) {};

			// Destruct Qlt with shared pointers to its Qdef
			~Qlt() {};

			// Return a Qdef's shared pointer pointing to a copy of this object 
			virtual Qdef::Sp clone() const { return Qdef::Sp(new Qlt(*this)); };

		protected:
		private:
		};

		// A Quntum and is a binary operation
		class Qle : public Qop
		{
		public:
			// Qle's shared pointer 
			typedef shared_ptr<Qle> Sp;

			// Instantiate Qle instance with '&' identity
			Qle() : Qop(LeQT::cMark, 2) {};

			// Copy constructor
			Qle(const Qle& right) : Qop(right) {};

			// Destruct Qle with shared pointers to its Qdef
			~Qle() {};

			// Return a Qdef's shared pointer pointing to a copy of this object 
			virtual Qdef::Sp clone() const { return Qdef::Sp(new Qle(*this)); };

		protected:
		private:
		};

		// A Quntum and is a binary operation
		class Qgt : public Qop
		{
		public:
			// Qgt's shared pointer 
			typedef shared_ptr<Qgt> Sp;

			// Instantiate Qgt instance with '&' identity
			Qgt() : Qop(GtQT::cMark, 2) {};

			// Copy constructor
			Qgt(const Qgt& right) : Qop(right) {};

			// Destruct Qgt with shared pointers to its Qdef
			~Qgt() {};

			// Return a Qdef's shared pointer pointing to a copy of this object 
			virtual Qdef::Sp clone() const { return Qdef::Sp(new Qgt(*this)); };

		protected:
		private:
		};

		// A Quntum and is a binary operation
		class Qge : public Qop
		{
		public:
			// Qge's shared pointer 
			typedef shared_ptr<Qge> Sp;

			// Instantiate Qge instance with '&' identity
			Qge() : Qop(GeQT::cMark, 2) {};

			// Copy constructor
			Qge(const Qge& right) : Qop(right) {};

			// Destruct Qge with shared pointers to its Qdef
			~Qge() {};

			// Return a Qdef's shared pointer pointing to a copy of this object 
			virtual Qdef::Sp clone() const { return Qdef::Sp(new Qge(*this)); };

		protected:
		private:
		};

		// A Quntum and is a binary operation
		class Qand : public Qop
		{
		public:
			// Qand's shared pointer 
			typedef shared_ptr<Qand> Sp;

			// Instantiate Qand instance with '&' identity
			Qand() : Qop(AndQT::cMark, 2) {};

			// Copy constructor
			Qand(const Qand& right) : Qop(right) {};

			// Destruct Qand with shared pointers to its two Qdefs
			~Qand() {};

			// Return a Qdef's shared pointer pointing to a copy of this object 
			virtual Qdef::Sp clone() const { return Qdef::Sp(new Qand(*this)); };

		protected:
		private:
		};

		// A Quntum nAnd is a binary operation
		class QnAnd : public Qop
		{
		public:
			// QnAnd's shared pointer 
			typedef shared_ptr<QnAnd> Sp;

			// Instantiate QnAnd instance with '&' identity
			QnAnd() : Qop(NandQT::cMark, 2) {};

			// Copy constructor
			QnAnd(const QnAnd& right) : Qop(right) {};

			// Destruct QnAnd with shared pointers to its two Qdefs
			~QnAnd() {};

			// Return a Qdef's shared pointer pointing to a copy of this object 
			virtual Qdef::Sp clone() const { return Qdef::Sp(new QnAnd(*this)); };

		protected:
		private:
		};

		// A Quntum or is a binary operation
		class Qor : public Qop
		{
		public:
			// Qor's shared pointer 
			typedef shared_ptr<Qor> Sp;

			// Instantiate Qor instance with '|' identity
			Qor() : Qop(OrQT::cMark, 2) {};

			// Copy constructor
			Qor(const Qor& right) : Qop(right) {};

			// Destruct Qor with shared pointers to its two Qdefs
			~Qor() {};

			// Return a Qdef's shared pointer pointing to a copy of this object 
			virtual Qdef::Sp clone() const { return Qdef::Sp(new Qor(*this)); };

		protected:
		private:
		};

		// A Quntum nOr is a binary operation
		class QnOr : public Qop
		{
		public:
			// QnOr's shared pointer 
			typedef shared_ptr<QnOr> Sp;

			// Instantiate QnOr instance with '|' identity
			QnOr() : Qop(NorQT::cMark, 2) {};

			// Copy constructor
			QnOr(const QnOr& right) : Qop(right) {};

			// Destruct QnOr with shared pointers to its two Qdefs
			~QnOr() {};

			// Return a Qdef's shared pointer pointing to a copy of this object 
			virtual Qdef::Sp clone() const { return Qdef::Sp(new QnOr(*this)); };

		protected:
		private:
		};


		// A Q addition is a specific implementation of a Q operation
		class Qaddition : public Qop
		{
		public:
			// Qaddition's shared pointer 
			typedef shared_ptr<Qaddition> Sp;

			// A Quntum carry is an operand defined by a Q addition
			class Carry : public Qdef
			{
			public:
				const static string cSymbol;	// The carry opernd definition is #

				// Q carry's shared pointer 
				typedef shared_ptr<Carry> Sp;

				// Return converted given string to Q carry definition, e.g. 'a0 ^ b0' is 
				// converted into '#{a0 ^ b0}'
				static string Symbol(const string&);

				// Initiate Carry instance with a pointer to a parent Q addition
				Carry(Qaddition*);

				// Copy constructor
				Carry(const Carry&);

				// Destruct a Q carry operand
				~Carry();

				// Return a Qdef's shared pointer pointing to a copy of this object 
				virtual Qdef::Sp clone() const { return Qdef::Sp(new Carry(*this)); };

				// Return a string representation of Q carry operand
				virtual string toString(bool decomposed = false) const;

				// Set a pointer to a new parent Q addition
				void addition(Qaddition*);

			protected:
			private:
				Qaddition* mpAddition;	// A parent Q addition
			};

			// An Q addition has identity and should have at least two argument
			Qaddition(const string& id, size_t noArguments = 2);

			// Copy constructor
			Qaddition(const Qaddition& right);

			// Destruct the Q addition instance with a shared pointer to its carry operand
			~Qaddition();

			// Assignes the arguments to this additon and returns nullptr
			// Override if assignment of operands should triger creation of different
			// Q addition object and return a shared pointer to a newlly created Q addition
			// object
			virtual Qaddition::Sp assign(const Qdefs&);

			// Assign a share point to a new Q carry operand
			virtual void carry(const Carry::Sp& pCarry);

			// Return a share pointer to Q carry operand of this Qaddition
			virtual Carry::Sp carry();
		protected:

		private:
			Carry::Sp mpCarry = nullptr;
		};

		// A Quntum xOr is a binary Q addition operation
		class Qxor : public Qaddition
		{
		public:
			// Qxor's shared pointer 
			typedef shared_ptr<Qxor> Sp;

			// Default constructor
			Qxor() : Qaddition(XorQT::cMark) {};

			// Copy constructor
			Qxor(const Qxor& right) : Qaddition(right) {};

			// Destruct the Q xor instance with a shared pointer to its carry operand
			// and two arguments
			~Qxor() {};

			// Return a shared pointer to this object if non of the operands is QxOr object,
			// otherwise return a shared pointer to a new Q adder object inheriting two
			// two arguments of Qxor operand and the other operand, resulting in conversion
			// of two xOr binary operations (this and one of the operands) into trinary
			// Qadder operation, e.g. (x ^ y) ^ z) or x ^ (y ^ z) is converted into a
			// single adder operation, x + y + z
			virtual Qaddition::Sp assign(const Qdefs&);

			// Return a Qdef's shared pointer pointing to a copy of this object 
			virtual Qdef::Sp clone() const { return Qdef::Sp(new Qxor(*this)); };
		protected:
		private:
		};

		// A Quntum xOr is also Q half adder
		typedef Qxor Qadder05;

		class Qadder : public Qaddition
		{
		public:
			// QtriOperation's shared pointer 
			typedef shared_ptr<Qadder> Sp;

			// Default constructor
			Qadder() : Qaddition(AdderQT::cMark, 3) {};

			// Copy constructor
			Qadder(const Qadder& right) : Qaddition(right) {};

			// Destruct the Q adder instance with a shared pointer to its carry operand
			// and two arguments
			~Qadder() {};

			// Return a Qdef's shared pointer pointing to a copy of this object 
			virtual Qdef::Sp clone() const { return Qdef::Sp(new Qadder(*this)); };

		protected:
		private:
		};

		// A Q multiplication is a specific implementation of a Q operation
		class Qmultiplication : public Qop
		{
		public:
			// Qaddition's shared pointer 
			typedef shared_ptr<Qmultiplication> Sp;

			static const string cMark;
			static const string cName;

			// An Q addition has identity and should have at least two argument
			Qmultiplication() : Qop(cMark, 2) {};

			// Copy constructor
			Qmultiplication(const Qaddition& right) : Qop(right) {};

			// Destruct the Q addition instance with a shared pointer to its carry operand
			~Qmultiplication() {};

			// Return a Qdef's shared pointer pointing to a copy of this object 
			virtual Qdef::Sp clone() const { return Qdef::Sp(new Qmultiplication(*this)); };

			// convert multiplication expration into a string
			virtual string toString(bool decomposed = false) const;

			// return Qubo presentation of this Qdef
			virtual Qubo qubo(bool finalized = true) const;

		protected:
/*			virtual Qexpression& execute(const Qexpression& right);
			virtual qbit_def_matrix X(const Qexpression& right) const;
			virtual void sumDiagonals(const qbit_def_matrix& xMatrix)
*/
		private:
		};
	};
};