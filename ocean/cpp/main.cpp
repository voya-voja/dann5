// pybind11 — Seamless operability between C++11 and Python
// https://pybind11.readthedocs.io/en/stable/index.html
//
#include <pybind11/pybind11.h>

#include <Qubo.h>
#include <Qdef.h>
#include <Qexpression.h>
#include <Qequation.h>
#include <BQM.h>

#include <pybind11/stl.h>
#include <pybind11/operators.h>

using namespace dann5::ocean;

#define VERSION_INFO "0.0.1"

int add(int i, int j) {
    return i + j;
}

namespace py = pybind11;

PYBIND11_MODULE(d5o, m) {
    m.doc() = R"pbdoc(
        Pybind11 example plugin
        -----------------------

        .. currentmodule:: dann5_ocean

        .. autosummary::
           :toctree: _generate

           add
           subtract
    )pbdoc";

    m.def("add", &add, R"pbdoc(
        Add two numbers

        Some other explanation about the add function.
    )pbdoc");

    m.def("subtract", [](int i, int j) { return i - j; }, R"pbdoc(
        Subtract two numbers

        Some other explanation about the subtract function.
    )pbdoc");

	// without instantiating py::class<Qubo>
	py::class_<BQDictionary>(m, "BQDictionary");

	py::class_<BQM, BQDictionary>(m, "BQM")
		.def(py::init<>())
		.def(py::init <const BQM&>())
		.def("assign", &BQM::operator=)
		.def(py::self + py::self)
		.def(py::self += py::self)
		.def("dictionary", &BQM::dictionary);

	py::class_<Qubo>(m, "Qubo")
//		.def(py::init<>())
//		.def("bqm", &Qubo::bqm)
	.def("bqm", (BQM(Qubo::*)() const) &Qubo::bqm, "Default Qubo bqm")
	.def("bqm", (BQM(Qubo::*)(const Qubo::Labels&) const) &Qubo::bqm, "Qubo bqm with the input list of arguments");

	// specify C++ class->baseclass specialization
	py::class_<EqualQubo, Qubo>(m, "EqualQubo")
		.def(py::init<>());

	// specify C++ class->baseclass specialization
	py::class_<NotQubo, Qubo>(m, "NotQubo")
		.def(py::init<>());

	// specify C++ class->baseclass specialization
	py::class_<LessThanQubo, Qubo>(m, "LessThanQubo")
		.def(py::init<>());

	// specify C++ class->baseclass specialization
	py::class_<LessEqualQubo, Qubo>(m, "LessEqualQubo")
		.def(py::init<>());

	// specify C++ class->baseclass specialization
	py::class_<GreaterThanQubo, Qubo>(m, "GreaterThanQubo")
		.def(py::init<>());

	// specify C++ class->baseclass specialization
	py::class_<GreaterEqualQubo, Qubo>(m, "GreaterEqualQubo")
		.def(py::init<>());

	// specify C++ class->baseclass specialization
	py::class_<AndQubo, Qubo>(m, "AndQubo")
		.def(py::init<>());

	// specify C++ class->baseclass specialization
	py::class_<NandQubo, Qubo>(m, "NandQubo")
		.def(py::init<>());

	// specify C++ class->baseclass specialization
	py::class_<OrQubo, Qubo>(m, "OrQubo")
		.def(py::init<>());

	// specify C++ class->baseclass specialization
	py::class_<NorQubo, Qubo>(m, "NorQubo")
		.def(py::init<>());

	// specify C++ class->baseclass specialization
	py::class_<NotLeftOrRightQubo, Qubo>(m, "NotLeftOrRightQubo")
		.def(py::init<>());

	// specify C++ class->baseclass specialization
	py::class_<DwNotLeftOrRightQubo, Qubo>(m, "DwNotLeftOrRightQubo")
		.def(py::init<>());

	// specify C++ class->baseclass specialization
	py::class_<NxorQubo, Qubo>(m, "NxorQubo")
		.def(py::init<>());

	// specify C++ class->baseclass specialization
	py::class_<XorQubo, Qubo>(m, "XorQubo")
		.def(py::init<>());

	// specify C++ class->baseclass specialization
	m.def("HalfAdderQubo", []() { return XorQubo(); }, R"pbdoc( Same as XorQubo, i.e. it is a typedef of XorQubo.)pbdoc");

	// specify C++ class->baseclass specialization
	py::class_<AdderQubo, Qubo>(m, "AdderQubo")
		.def(py::init<>());

	py::class_<QintInitializer>(m, "QintInitializer")
		.def("push", (QintInitializer& (QintInitializer::*)(const Qbit&)) &QintInitializer::operator,);

	py::class_<Qint>(m, "Qint")
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
		.def("extendTo", &Qint::extendTo);

	py::class_<Qdef>(m, "Qdef")
		.def(py::init<>())
		.def(py::init<const Qdef&>())
		.def(py::init<Index>())
		.def(py::init<Index, const std::string&>())
		.def(py::self + py::self)
		.def(py::self * py::self)
		.def("extendTo", &Qdef::extendTo);

	py::class_<Qexpression>(m, "Qexpression")
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

	py::class_<Qvar>(m, "Qvar")
		.def(py::init<const Qvar&>())
		.def(py::init<Index, const string&>())
		.def(py::init<const string&, long>())
		.def(py::init<const Qdef&>())
		.def(py::init<const Qdef&, const Qint&>())
		.def("symbol", &Qvar::symbol)
		.def("value", &Qvar::value)
		.def("extendTo", &Qvar::extendTo);

	py::class_<Qequation>(m, "Qequation")
		.def(py::init<const Qequation&>())
		.def(py::init<const Qvar&, const Qexpression&>())
		.def(py::init<const Qvar&, const Qexpression&, const QvarList&>())
		.def("size", &Qequation::size)
		.def("result", &Qequation::result)
		.def("expression", &Qequation::expression)
		.def("arguments", &Qequation::arguments)
		.def("addArgument", &Qequation::addArgument)
		.def("push", &Qequation::operator<<)
		.def("bqm", &Qequation::bqm);

#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}
