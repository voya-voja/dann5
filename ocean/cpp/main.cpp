// pybind11 — Seamless operability between C++11 and Python
// https://pybind11.readthedocs.io/en/stable/index.html
//
#include <pybind11/pybind11.h>

#include <Qubo.h>
#include <Qdef.h>
#include <Qexpression.h>
#include <Qequation.h>
#include <Qubo.h>

#include <pybind11/stl.h>
#include <pybind11/operators.h>
//#include <pybind11/eigen.h>
//#include <pybind11/embed.h>

using namespace dann5::ocean;

#define VERSION_INFO "0.0.1"

namespace py = pybind11;

PYBIND11_MODULE(d5o, m) {
    m.doc() = R"pbdoc(
        d5o plugin
        -----------------------

        .. currentmodule:: d5o

        .. autosummary::
           :toctree: _generate

			Qubo
			QuboTable
			Qint
			Qdef
			Qexpression
			Qvar
			Qequation
    )pbdoc";

	// without instantiating py::class<QuboTable>
	py::class_<Qubo>(m, "Qubo", R"pbdoc( a Qubo model dictionary)pbdoc");

	py::class_<QuboTable>(m, "QuboTable", R"pbdoc( a Qubo abstraction)pbdoc")
//		.def(py::init<>())
//		.def("qubo", &QuboTable::qubo)
	.def("qubo", (Qubo(QuboTable::*)() const) &QuboTable::qubo, "Default Qubo qubo")
	.def("qubo", (Qubo(QuboTable::*)(const QuboTable::Labels&) const) &QuboTable::qubo, "Qubo qubo with the input list of arguments");

	// specify C++ class->baseclass specialization
	py::class_<EqQuboTable, QuboTable>(m, "EqQuboTable")
		.def(py::init<>());

	// specify C++ class->baseclass specialization
	py::class_<NotQuboTable, QuboTable>(m, "NotQuboTable")
		.def(py::init<>());

	// specify C++ class->baseclass specialization
	py::class_<LtQuboTable, QuboTable>(m, "LtQuboTable")
		.def(py::init<>());

	// specify C++ class->baseclass specialization
	py::class_<LeQuboTable, QuboTable>(m, "LeQuboTable")
		.def(py::init<>());

	// specify C++ class->baseclass specialization
	py::class_<GtQuboTable, QuboTable>(m, "GtQuboTable")
		.def(py::init<>());

	// specify C++ class->baseclass specialization
	py::class_<GeQuboTable, QuboTable>(m, "GeQuboTable")
		.def(py::init<>());

	// specify C++ class->baseclass specialization
	py::class_<AndQuboTable, QuboTable>(m, "AndQuboTable")
		.def(py::init<>());

	// specify C++ class->baseclass specialization
	py::class_<NandQuboTable, QuboTable>(m, "NandQuboTable")
		.def(py::init<>());

	// specify C++ class->baseclass specialization
	py::class_<OrQuboTable, QuboTable>(m, "OrQuboTable")
		.def(py::init<>());

	// specify C++ class->baseclass specialization
	py::class_<NorQuboTable, QuboTable>(m, "NorQubo")
		.def(py::init<>());

	// specify C++ class->baseclass specialization
	py::class_<NotLeftOrRightQuboTable, QuboTable>(m, "NotLeftOrRightQuboTable")
		.def(py::init<>());

	// specify C++ class->baseclass specialization
	py::class_<DwNotLeftOrRightQuboTable, QuboTable>(m, "DwNotLeftOrRightQuboTable")
		.def(py::init<>());

	// specify C++ class->baseclass specialization
	py::class_<NxorQuboTable, QuboTable>(m, "NxorQuboTable")
		.def(py::init<>());

	// specify C++ class->baseclass specialization
	py::class_<XorQuboTable, QuboTable>(m, "XorQuboTable")
		.def(py::init<>());

	// specify C++ class->baseclass specialization
	m.def("Adder05QuboTable", []() { return XorQuboTable(); }, R"pbdoc( Same as XorQubo, i.e. it is a typedef of XorQubo.)pbdoc");

	// specify C++ class->baseclass specialization
	py::class_<AdderQuboTable, QuboTable>(m, "AdderQuboTable")
		.def(py::init<>());

	py::class_<QintInitializer>(m, "QintInitializer")
		.def("push", (QintInitializer& (QintInitializer::*)(const Qbit&)) &QintInitializer::operator,);

	py::class_<Qint>(m, "Qint", 
		R"pbdoc( Quantum bit is in superposition state for any value except 0 and 1)pbdoc")
		.def(py::init<>())
		.def(py::init<const Qint&>())
		.def(py::init<Index>())
		.def(py::init<long>())
		.def("assign", &Qint::operator=)
		.def("push", &Qint::operator<<)
		.def(-py::self)
		.def(py::self & py::self)
		.def(py::self &= py::self)
		.def(py::self | py::self)
		.def(py::self |= py::self)
		.def(py::self ^ py::self)
		.def(py::self ^= py::self)
		.def(py::self + py::self)
		.def(py::self += py::self)
		.def(py::self * py::self)
		.def(py::self *= py::self)
		.def("value", &Qint::value)
		.def("resize", &Qint::resize);

	py::class_<Qdef>(m, "Qdef", 
		R"pbdoc( Quantum definition of a symbol defines Q bit symbol names in a vector)pbdoc")
		.def(py::init<>())
		.def(py::init<const Qdef&>())
		.def(py::init<Index>())
		.def(py::init<Index, const std::string&>())
		.def(py::self + py::self)
		.def(py::self * py::self)
		.def("resize", &Qdef::resize);

	py::class_<Qexpression>(m, "Qexpression", 
		R"pbdoc( Quantum expression of arithmetic Q operations with Q defined symbols as operands)pbdoc")
		.def(py::init<>())
		.def(py::init<const Qexpression&>())
		.def(py::init<const Qdef&>())
		.def(py::self + py::self)
		.def(py::self += py::self)
		.def(py::self + Qdef())
		.def(py::self += Qdef())
		.def(py::self * py::self)
		.def(py::self *= py::self)
		.def(py::self * Qdef())
		.def(py::self *= Qdef());

	py::class_<Qvar>(m, "Qvar",
		R"pbdoc( Quantum variable has a variable symbol definition, Qdef, and its value, Qint. )pbdoc")
		.def(py::init<>(),
			R"pbdoc( default constructor: Quantum variable has a variable symbol definition, Qdef, and its value, Qint. )pbdoc")
		.def(py::init<const string&, long>(),
			R"pbdoc( Quantum variable has a variable symbol definition, Qdef, and its value, Qint. )pbdoc")
		.def(py::init<Index, const string&>(),
			R"pbdoc( instantiate a Q variable with a given # of bits and given symbol, the bits are in superposition state )pbdoc")
		.def(py::init<const Qdef&>(),
			R"pbdoc( instantiate a Q variable with a given symbol Q definition, the bits are in superposition state )pbdoc")
		.def(py::init<const Qdef&, const Qint&>(),
			R"pbdoc( instantiate a Q variable with a given symbol Q definition and given Q int value )pbdoc")
		.def(py::init<const Qvar&>(),
			R"pbdoc( copy constructor )pbdoc")
		.def(py::self += py::self,
			R"pbdoc( Addition operator returns this Qvar object by adding right to this Q variable objects' Qdef and Qint members )pbdoc")
		.def(py::self + py::self,
			R"pbdoc( Addition operator returns Qequation object by adding this and right Q variable objects' Qdef and Qint members )pbdoc")
		.def(py::self + Qequation(),
			R"pbdoc( Addition operator where Q variable is left and Q equation right argument. Returns Qequation object by adding its Qdef and Qint members to the Q equation )pbdoc")
		.def(py::self *= py::self,
			R"pbdoc( Multiplication operator returns this Qvar object by multiplying right to this Q variable objects' Qdef and Qint members)pbdoc")
		.def(py::self * py::self,
			R"pbdoc( Multiplication operator returns Qequation object by multiplying this and right Q variable objects' Qdef and Qint members)pbdoc")
		.def(py::self * Qequation(),
			R"pbdoc( Multiplication operator where Q variable is left and Q equation right argument. Returns Qequation object by adding its Qdef and Qint members to the Q equation)pbdoc")
		.def("symbol", &Qvar::symbol,
			R"pbdoc( get variable's symbol )pbdoc")
		.def("value", &Qvar::value,
			R"pbdoc( get variable's value )pbdoc")
		.def("toString", &Qvar::toString,
			R"pbdoc( Return Qdef and Qint string representations of this variable)pbdoc")
		.def("resize", &Qvar::resize,
			R"pbdoc( Resize the Qvar as a vector of Qbit definitions and values to a new size if the new size is bigger, assign qBit value to additional Qbits)pbdoc");

	py::class_<Qequation>(m, "Qequation",
		R"pbdoc( Quantum equation is a coupling of result Q variable via Q expression with Qvar arguments)pbdoc")
		.def(py::init<>())
		.def(py::init<const Qequation&>())
		.def(py::init<const Qvar&>())
		.def(py::init<const Qvar&, const Qexpression&, const Qvars&>())
		.def("assign", &Qequation::operator=)
		.def(py::self + py::self)
		.def(py::self += py::self)
		.def(py::self + Qvar())
		.def(py::self += Qvar())
		.def(py::self * py::self)
		.def(py::self *= py::self)
		.def(py::self * Qvar())
		.def(py::self *= Qvar())
		.def("size", &Qequation::size)
		.def("setSamples", &Qequation::setSamples)
		.def("set", &Qequation::set)
		.def("toString", &Qequation::toString)
		.def("result", &Qequation::result)
		.def("expression", &Qequation::expression)
		.def("arguments", &Qequation::arguments)
		.def("qubo", &Qequation::qubo,
			R"pbdoc( Returns a qubo representation of this Q equation, 
			 if not finalized, returns a full qubo symbol representation of this Q equation
			 if finalized, returns an expression that replaces symbols with values of
			 Qbits in deterministic states for all the Q variables, i.e. result and expression arguments)pbdoc");

#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}
