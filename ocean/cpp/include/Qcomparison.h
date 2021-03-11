#pragma once

#include <pybind11/pybind11.h>

#include <utility>
#include <Eigen/Dense>

#include <Qvar.h>
#include <Qstatement.h>

using namespace std;
using namespace Eigen;


namespace dann5 {
	namespace ocean {


		// Quantum comparison is a Q statement wich supports definition of logical 
		// comparison Q expressions for given Q variables
		class Qcomparison : public Qstatement
		{
		public:
			// Qcomparison's shared pointer 
			typedef shared_ptr<Qcomparison> Sp;

			// Default constructor creates Q comparison witout an expression or arguments
			Qcomparison();

			// creates Q comparison with a given comparison Q expression and Q variables
			// as initial arguments
			Qcomparison(const Qexpression&, const Qvars&);

			// copy constructor
			Qcomparison(const Qcomparison&);

			// desruct the Q comparison with its members
			~Qcomparison();

			// Assignment operator returns a reference of this Q comparison with 
			// Q expression and arguments same as in right Qcomparison
			Qcomparison& operator=(const Qcomparison& right);

			// A comparison negation
			Qcomparison& operator~();

			// And operator returns a new Q comparison object with expresion this & right, 
			// and with right action concatenated to this object action
			Qcomparison operator&(const Qcomparison& right) const;

			// And operator returns a reference to this Qcomparison object with expresion
			// this & right, and right action concatenated to this object action
			Qcomparison& operator&=(const Qcomparison& right);

			// Or operator returns a reference to this Qcomparison object with expresion
			// this | right, and right action concatenated to this object action
			Qcomparison operator|(const Qcomparison& right) const;

			// Or operator returns a new Q comparison object with expresion this | right, 
			// and with right action concatenated to this object action
			Qcomparison& operator|=(const Qcomparison& right);

			// Xor operator returns a reference to this Qcomparison object with expresion
			// this ^ right, and right action concatenated to this object action
			Qcomparison operator^(const Qcomparison& right) const;

			// Xor operator returns a new Q comparison object with expresion this ^ right, 
			// and with right action concatenated to this object action
			Qcomparison& operator^=(const Qcomparison& right);

			// Or operator returns a reference to this Qcomparison object with expresion
			// this ~& right, and right action concatenated to this object action
			Qcomparison nand(const Qcomparison& right) const;

			// Or operator returns a new Q comparison object with expresion this ~& right, 
			// and with right action concatenated to this object action
			Qcomparison& nand(const Qcomparison& right);

			// Or operator returns a reference to this Qcomparison object with expresion
			// this ~| right, and right action concatenated to this object action
			Qcomparison nor(const Qcomparison& right) const;

			// Or operator returns a new Q comparison object with expresion this ~| right, 
			// and with right action concatenated to this object action
			Qcomparison& nor(const Qcomparison& right);

			// Returns a string representation of this Q comparison, 
			// if not decomposed, returns an comparison line per Qbit level
			// if decomposed, returns a line per Qbit operational expression
			virtual string toString(bool decomposed = false, Index level = Eigen::Infinity) const;

			// returns a shared_pointer on a cloned instance of this Q statement
			virtual Qstatement::Sp clone() const { return Qstatement::Sp(new Qcomparison(*this)); };

			// Insert string representation of a Q comparison into an output stream
			friend std::ostream& operator << (std::ostream&, const Qcomparison&);

		protected:
			virtual Qubo createRawQubo(bool finalized, Index level) const;

		private:
		};
	};
};