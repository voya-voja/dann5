#pragma once

#include <pybind11/pybind11.h>

#include <utility>
#include <Eigen/Dense>

#include <Qdef.h>

using namespace std;
using namespace Eigen;


namespace dann5 {
	namespace ocean {

		// A Quntum type is a Q definition 
		class Qtype : public Qdef
		{
		public:
			// Q type shared pointer 
			typedef shared_ptr<Qtype> Sp;

			// An instance of a Q type must have defined identity
			Qtype(const string& id) : Qdef(id) {};

			// Copy constructor
			Qtype(const Qtype& right) : Qdef(right) {};

			// destruct the Q type
			~Qtype() {};

			// Returns the number of Q bits that the Q binary holds
			virtual std::size_t nobs() const noexcept = 0;

		protected:
		private:
		};

	};
};