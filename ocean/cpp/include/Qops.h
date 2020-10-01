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

			virtual Qoperand* clone() const { return new Qoperand(*this); };

			virtual bool rename(const string&, const string&);

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

			virtual Qoperand* instance() = 0;
			virtual size_t argumentsNumber() const = 0;
			virtual Qoperand* arguments(const OperandList&);

			const OperandList& arguments() const { return mOperands; };
		protected:
		private:
			OperandList mOperands;
		};

		class QbiOperation : public Qoperand, public Qop
		{
		public:
			QbiOperation();
			QbiOperation(const QbiOperation&);
			QbiOperation(const string&);
			~QbiOperation();

			virtual string toString() const;
			virtual bool rename(const string&, const string&);

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
			virtual bool rename(const string&, const string&);

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

		class Qaddition
		{
		public:
			static string CarrySymbol(const string&);

			Qaddition();
			~Qaddition();

			virtual Qoperand* carry() const;
			virtual string toString() const = 0;
		protected:
		private:
		};

		class Qxor : public Qaddition, public QbiOperation
		{
		public:
			Qxor();
			Qxor(const Qxor&);
			~Qxor();

			virtual Qoperand* instance() { return this; };
			virtual Qoperand* clone() const;

			virtual Qoperand* arguments(const OperandList&);
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

			virtual Qoperand* instance() { return this; };
			virtual Qoperand* clone() const { return new Qadder(*this); };
			virtual string toString() const;

		protected:
		private:
		};
	};
};