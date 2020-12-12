#pragma once

#include <pybind11/pybind11.h>

#include <utility>
#include <Eigen/Dense>

#include <BQM.h>

using namespace std;
using namespace Eigen;


namespace dann5 {
	namespace ocean {

		class Qoperand
		{
		public:
			Qoperand();
			Qoperand(const Qoperand&);
			Qoperand(const string&);
			~Qoperand();

			string identity() const { return mIdentity; };
			void identity(const string& id) { mIdentity = id; };
			virtual bool reid(const string&, const string& fresh = "");

			bool isSame(const Qoperand* pRight) const { return(this == pRight); }
			bool isExist(const Qoperand*) const;

			virtual Qoperand* clone() const { return new Qoperand(*this); };

			virtual string toString() const;

			virtual BQM bqm() const;

			friend std::ostream& operator << (std::ostream&, const Qoperand&);
		protected:
		private:
			string mIdentity;
		};

		typedef std::vector <Qoperand*> OperandList;

		class Qop
		{
		public:
			Qop();
			Qop(const Qop&);

			~Qop();

			bool isSame(const Qop* pRight) { return(this == pRight); }

			virtual Qoperand* instance() = 0;
			virtual size_t argumentsNumber() const = 0;

			virtual Qoperand* arguments(const OperandList&);
			const OperandList& arguments() const { return mOperands; };
			virtual Qoperand* remove(const Qoperand*);

		protected:
			void releaseArguments();
			OperandList& arguments() { return mOperands; };

		private:
			OperandList mOperands;
		};

		class QunOperation : public Qoperand, public Qop
		{
		public:
			QunOperation();
			QunOperation(const QunOperation&);
			QunOperation(const string&);
			~QunOperation();

			virtual string toString() const;
			virtual bool reid(const string&, const string& fresh = "");

			virtual size_t argumentsNumber() const { return(1); };
			virtual BQM bqm() const;
		protected:
		private:
		};

		class QbiOperation : public Qoperand, public Qop
		{
		public:
			QbiOperation();
			QbiOperation(const QbiOperation&);
			QbiOperation(const string&);
			~QbiOperation();

			virtual string toString() const;
			virtual bool reid(const string&, const string& fresh = "");

			virtual size_t argumentsNumber() const { return(2); };
			virtual BQM bqm() const;
		protected:
		private:
		};

		class QtriOperation : public Qoperand, public Qop
		{
		public:
			QtriOperation();
			QtriOperation(const QtriOperation&);
			QtriOperation(const string&);
			~QtriOperation();

			virtual string toString() const;
			virtual bool reid(const string&, const string& fresh = "");

			virtual size_t argumentsNumber() const { return(3); };
			virtual BQM bqm() const;

		protected:
		private:
		};

		class Qand : public QbiOperation
		{
		public:
			Qand();
			Qand(const Qand&);
			~Qand();

			virtual Qoperand* instance() { return this; };
			virtual Qoperand* clone() const { return new Qand(*this); };

		protected:
		private:
		};

		class Qcarry;

		class Qaddition
		{
		public:
			/* 
				Assignment.first is a pointer to addition object
							.second.first is a pointer to operation's carry operand
							.second.second is NULL 
								if addition operation has not been changed during the assignment
								otherwise is a pointer to carry operand that might need to be removed from the expression
			*/
			struct Assignment
			{
				Qop* mpOp = NULL; 
				Qoperand* mpCarryForward = NULL;
				Qoperand* mpCarryToBeRemoved = NULL;
			};
			Qaddition();
			~Qaddition();

			virtual Assignment assign(const OperandList&);
			virtual void carry(Qcarry* pCarry);

			virtual Qoperand* carry();
			virtual string toString() const = 0;
		protected:

		private:
			Qcarry* mpCarry = NULL;
		};

		class Qcarry : public QunOperation
		{
		public:
			static string Symbol(const string&);

			Qcarry(Qaddition*);
			Qcarry(Qoperand*);
			Qcarry(const Qcarry&);
			~Qcarry();

			virtual Qoperand* instance() { return this; };
			virtual Qoperand* clone() const { return new Qcarry(*this); };
			virtual string toString() const;

			BQM bqm() const;

			void addition(Qaddition*);

		protected:
		private:
			const static string cSymbol;
		};

		class Qxor : public Qaddition, public QbiOperation
		{
		public:
			Qxor();
			Qxor(const Qxor&);
			~Qxor();

			virtual Assignment assign(const OperandList&);

			virtual Qoperand* instance() { return this; };
			virtual Qoperand* clone() const { return new Qxor(*this);};

			virtual string toString() const;

		protected:
		private:
		};

		typedef Qxor Qadder05;

		class Qadder : public Qaddition, public QtriOperation
		{
		public:
			Qadder();
			Qadder(const Qadder&);
			~Qadder();

			virtual Qoperand* remove(const Qoperand*);

			virtual Qoperand* instance() { return this; };
			virtual Qoperand* clone() const { return new Qadder(*this); };

			virtual string toString() const;

		protected:
		private:
		};
	};
};