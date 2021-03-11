#pragma once

#include <pybind11/pybind11.h>

#include <utility>

#include <Qubo.h>

using namespace std;

namespace dann5 {
	namespace ocean {

		// A supertype for all definitions in dann5 Quantum library
		class Qdef
		{
		public:
			// Q definition shared pointer 
			typedef shared_ptr<Qdef> Sp;

			// Instantiate a Q definition with identity
			Qdef(const string& id) : mID(id) {};

			// Copy constructor
			Qdef(const Qdef& right) : mID(right.mID) {};

			// destruct the Q definition
			~Qdef() {};

			// Return the Q definition identity string
			// override to provide identity at the point of time
			virtual string id() const { return mID; };

			// Set a new Q definition identity
			void id(const string& id) { mID = id; };

			// Return a Qdef's shared pointer pointing to a copy of this object 
			virtual Qdef::Sp clone() const = 0;

			// return string presentation of this Qoperand
			virtual string toString(bool decomposed = false) const { return mID; };

			// return Qubo presentation of this Qoperand
			virtual Qubo qubo(bool finalized = true) const { return Qubo(); };

		protected:
		private:
			string mID;	// The identity of a Q definition
		};

		// An std::vector containing a list of shared pointers pointing to Q definitions
		typedef vector <Qdef::Sp> Qdefs;
	};
};