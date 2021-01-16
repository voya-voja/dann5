// pybind11 — Seamless operability between C++11 and Python
// https://pybind11.readthedocs.io/en/stable/index.html
//
#include <pybind11/pybind11.h>

#include <Qubo.h>
#include <Qdef.h>
#include <Qexpression.h>
#include <Qequation.h>
#include <Qcondition.h>
#include <Qtype.h>

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
	py::class_<EqQT, QuboTable>(m, "EqQT")
		.def(py::init<>());

	// specify C++ class->baseclass specialization
	py::class_<NotQT, QuboTable>(m, "NotQT")
		.def(py::init<>());

	// specify C++ class->baseclass specialization
	py::class_<LtQT, QuboTable>(m, "LtQT")
		.def(py::init<>());

	// specify C++ class->baseclass specialization
	py::class_<LeQT, QuboTable>(m, "LeQT")
		.def(py::init<>());

	// specify C++ class->baseclass specialization
	py::class_<GtQT, QuboTable>(m, "GtQT")
		.def(py::init<>());

	// specify C++ class->baseclass specialization
	py::class_<GeQT, QuboTable>(m, "GeQT")
		.def(py::init<>());

	// specify C++ class->baseclass specialization
	py::class_<AndQT, QuboTable>(m, "AndQT")
		.def(py::init<>());

	// specify C++ class->baseclass specialization
	py::class_<NandQT, QuboTable>(m, "NandQT")
		.def(py::init<>());

	// specify C++ class->baseclass specialization
	py::class_<OrQT, QuboTable>(m, "OrQT")
		.def(py::init<>());

	// specify C++ class->baseclass specialization
	py::class_<NorQT, QuboTable>(m, "NorQubo")
		.def(py::init<>());

	// specify C++ class->baseclass specialization
	py::class_<NotLeftOrRightQT, QuboTable>(m, "NotLeftOrRightQT")
		.def(py::init<>());

	// specify C++ class->baseclass specialization
	py::class_<DwNotLeftOrRightQT, QuboTable>(m, "DwNotLeftOrRightQT")
		.def(py::init<>());

	// specify C++ class->baseclass specialization
	py::class_<NxorQT, QuboTable>(m, "NxorQT")
		.def(py::init<>());

	// specify C++ class->baseclass specialization
	py::class_<XorQT, QuboTable>(m, "XorQT")
		.def(py::init<>());

	// specify C++ class->baseclass specialization
	m.def("Adder05QT", []() { return XorQT(); }, R"pbdoc( Same as XorQubo, i.e. it is a typedef of XorQubo.)pbdoc");

	// specify C++ class->baseclass specialization
	py::class_<AdderQT, QuboTable>(m, "AdderQT")
		.def(py::init<>());

	py::class_<QintInitializer>(m, "QintInitializer")
		.def("push", (QintInitializer& (QintInitializer::*)(const q_bit&)) &QintInitializer::operator,);

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
		R"pbdoc( Quantum definition of a definition defines Q bit definition names in a vector)pbdoc")
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
		R"pbdoc( Quantum variable has a variable definition definition, Qdef, and its value, Qint. 
			Default constructor: creates a quantum variable with default name defined and
			Qint value with 0 bits:
				v0 = Qvar()

			Overloaded constructor: instantiates a Q variable with a given definition name
			and with a given integer value:
				a = Qvar("a", 4), e.g. 4 will create 3 Qbit integer with a0 and a1 bits set to 0 
										and a2 bit set to 1. 

			Overloaded constructor: instantiates a Q variable with a given # of bits and the given
			definition name, the bits are in superposition state:
				b = Qvar(2, "b"), e.g. b0 and b1 bits are in superposition state	

			Overloaded constructor: instantiates a Q variable with a given definition Q definition,
			the bits are in superposition state:
				c_def = Qdef(3, "c"), .e.g. c definition with c0, c1 and c2 bits
				c = Qvar(c_def), e.g. defines a variable with Qint value, where c0, c1 and c2 bits
								 are in superposition state
				
			Overloaded constructor: instantiates a Q variable with a given definition Q definition and
			given Q int value:
				c_def = Qdef(3, "c"), .e.g. c definition with c0, c1 and c2 bits
				c_val = Qint(3), e.g. instantiates an quantum integer with bits 0 and 1, whcih are set to 1
				c = Qvar(c_def, c_val), e.g. defines a variable with Qint value 3, where c0 and c1 bits set to 1 
										and c2 bit set to 0

			Copy constructor: creates a quantum variable with a same name and value as right object:
				right = Qvar(...)
				...
				myVar = Qvar(right)

			Addition operator returns this Qvar object by adding right to this Q variable objects' 
			Qdef and Qint members:
				a += b

			Addition operator returns Qequation object by adding this and right Q variable objects'
			Qdef and Qint members:
				fx = a + c

			Addition operator where Q variable is left and Q equation right argument. Returns Qequation
			object by adding it self and its Qdef and Qint members to the Q equation:
				fy = v0 + fx
			
			Multiplication operator returns this Qvar object by multiplying right to this Q variable
			objects' Qdef and Qint members:
				a *= b

			Multiplication operator returns Qequation object by multiplying this and right Q variable
			objects' Qdef and Qint members:
				fm = a * c

			Multiplication operator where Q variable is left and Q equation right argument. Returns
			Qequation object by adding its Qdef and Qint members to the Q equation:
				fm2 = v0 * fm

			definition() returns a variable definition name

			value() returns a variable integer value

			toString(bool bitForamt) returns Qdef and Qint string representations of this
				variable. When bitFormat is true, the string representation are variable bit 
				symbols and values, otherwise returns sybol name and corrsponding value

			resize(int size, Qbit qBit) - resize the Qvar as a vector of Qbit definitions
				and values to a new size. If the new size is bigger, assign qBit value to
				additional Qbits

		)pbdoc")
		.def(py::init<>())
		.def(py::init<const string&, long>())
		.def(py::init<Index, const string&>())
		.def(py::init<const Qdef&>())
		.def(py::init<const Qdef&, const Qint&>())
		.def(py::init<const Qvar&>())
		.def( (~ py::self) )
		.def(py::self == py::self)
		.def(py::self != py::self)
		.def(py::self > py::self)
		.def(py::self >= py::self)
		.def(py::self <= py::self)
		.def(py::self <= py::self)
		.def(py::self &= py::self)
		.def(py::self & py::self)
		.def(py::self & Qequation())
		.def("nandMutable", &Qvar::nandMutable, "operator ~&=")
		.def("nand", static_cast<Qequation (Qvar::*)(const Qvar &) const>(&Qvar::nand), "operator ~&")
		.def("nand", static_cast<Qequation (Qvar::*)(const Qequation &) const>(&Qvar::nand), "operator ~&")
		.def(py::self |= py::self)
		.def(py::self | py::self)
		.def(py::self | Qequation())
		.def("norMutable", &Qvar::norMutable, "operator ~&=")
		.def("nor", static_cast<Qequation (Qvar::*)(const Qvar &) const>(&Qvar::nor), "operator ~&")
		.def("nor", static_cast<Qequation (Qvar::*)(const Qequation &) const>(&Qvar::nor), "operator ~&")
		.def(py::self ^= py::self)
		.def(py::self ^ py::self)
		.def(py::self ^ Qequation())
		.def(py::self += py::self)
		.def(py::self + py::self)
		.def(py::self + Qequation())
		.def(py::self - py::self)
		.def(py::self *= py::self)
		.def(py::self * py::self)
		.def(py::self * Qequation())
		.def(py::self / py::self)
		.def("definition", static_cast<const Qdef& (Qvar::*)() const>(&Qvar::definition), "return Qdef")
		.def("value", static_cast<const Qint& (Qvar::*)() const>(&Qvar::value), "return Qint")
		.def("toString", &Qvar::toString)
		.def("resize", &Qvar::resize);

	py::class_<Qstatement>(m, "Qstatement")
//		.def(py::init<>())
//		.def(py::init<const Qstatement&>())
//		.def(py::init<Index>())
//		.def(py::init<const Qexpression&, const Qvars&>())
		.def("expression", static_cast<const Qexpression& (Qstatement::*)() const>(&Qstatement::expression), "return const Qexpression&")
		.def("expression", static_cast<Qexpression& (Qstatement::*)()>(&Qstatement::expression), "return Qexpression&")
		.def("arguments", static_cast<const Qvars& (Qstatement::*)() const>(&Qstatement::arguments), "return const Qvars&")
		.def("arguments", static_cast<Qvars& (Qstatement::*)()>(&Qstatement::arguments), "return Qvars&");


	py::class_<Qequation, Qstatement>(m, "Qequation",
		R"pbdoc( Quantum equation is a coupling of result Q variable via Q expression with Qvar arguments
			Default constructor creates Q equation witout a result Q variable so the result definition is an empty string
				fx = Qeqation()

			Overloaded constructors: create Q equation with a given Q variable as an expected result,
				r = Qvar(4, "R"), e.g. result variable with definition name "R" and 4 Q bits in superposition state
				fR = Qeauation(r), e.g. a Q eauation with a result defined and without expression and arguments 

			Overloaded constructors with 3 arguments: result Q variable, resulting Q expression and corresponding
				Q variables as arguments of the expression:
				e = Qvar("e", 8), e.g. result variable "e" is set to a value 8 (b1000)
				args = [Qvar(3, "a"), Qvar(4, "b")], e.g. there are 2 arguments of the equation
				xPrsn = args[0].definition() + args[1].definition(), e.g. the equation expresion is "a + b"
				fe = Qequation(e, xPrsn, args), e.g. the equation contains the result, its expression and 
												corresponding arguemnts
 
			Copy constructor: creates a quantum variable with a same name and value as right object:
				myEquation = Qequation(...)
				...
				aCopy = Qequation(myEquation)

			assignment(Qequation right) - an assignment operator returns a reference of this Q equation with Q result,
				expression and arguments same as in right Qequation
				a = Qvar(2, "a"), e.g. Q variable named a with 2 bits in superposition state
				b = Qvar("b", 2), e.g. Q variable named b with value of 2
				c = Qvar(3, "c"), e.g. Q variable named c with 3 bits in superposition state
				e = Qvar("e", 6), e.g. Q variable named e with value of 6
				anE = Qequation(e), e.g. a Q equation with result variable e
				anE.assignment( a * b * c ), e.g. to anE equation assign multiplication of a,b and c variable

			Addition operator returns a new Qequation object with added Q variable to this Q equation
				fx = anE + a, e.g. where existing Qequation anE and variable a, create a new Qequation fx

			Addition operator returns a new Qequation object resulting from addition of this and right Q equations
				fy = fx + anE, e.g. where fy adds results, expressions and arguments of existing equations fx and anE

			Addition operator returns a reference to this Qequation object with added Q variable
				fy += b, .e.g. where b is existing Q varaible, and fy is an existing Qequation

			Addition operator returns a reference to this Qequation object with added right Q equation
				fy += anE, .e.g. where both fy and anE are existing Qequation's

			Multiplication operator returns a new Qequation object with multiplied Q variable with this Q equation
				fx = anE * a, e.g. where existing Qequation anE and variable a, create a new Qequation fx

			Multiplication operator returns a new Qequation object resulting from multiplication of this and right Q equations
				fy = fx * anE, e.g. where fy multiplies results and expressions, 
									and adds arguments of existing equations fx and anE

			Multiplication operator returns a reference to this Qequation object with multiplied Q variable
				fy *= b, .e.g. where b is existing Q varaible, and fy is an existing Qequation

			Multiplication operator returns a reference to this Qequation object with multiplied right Q equation
				fy *= anE, .e.g. where both fy and anE are existing Qequation's

			size() - returns a number of bit level expressions in this Q equation

			result() - returns a constant reference to the result of this Q equation

			expression() - returns a constant reference to the expression of this Q equation

			arguments() - returns a constant reference to the expression arguments of this Q equation

			qubo(bool finalized) - Returns a qubo representation of this Q equation, 
				if not finalized, returns a full qubo definition representation of this Q equation
				if finalized, returns an expression that replaces symbols with values of
				Qbits in deterministic states for all the Q variables, i.e. result and expression arguments

			add(Sample& sample) Add a sample with a node list defined by qubo() of this Q equation
				A Semple is defined as a dictionary (map) of definition nodes and their values.
				The node names are defined by qubo() for each Q equation

			set(Samples& samples) - Set a sample list with a node list defined by qubo() of this Q equation
				the combination of node values should be different for each sample

			solutions() - for existing samples, returns a string representation of all solutions of this Q euation

			toString(bool decomposed) - Returns a string representation of this Q equation, 
				if not decomposed, returns an equation line per each Qbit level
				if decomposed, returns a line per Qbit operational expression

		)pbdoc")
		.def(py::init<>())
		.def(py::init<const Qequation&>())
		.def(py::init<const Qvar&>())
		.def(py::init<const Qvar&, const Qexpression&, const Qvars&>())
		.def("assign", static_cast<Qequation& (Qequation::*)(const Qequation&)>(&Qequation::operator=), "assign Qequation")
		.def("assign", static_cast<Qequation& (Qequation::*)(const Qvar&)>(&Qequation::operator=), "assign Qequation")
		.def(py::self & py::self)
		.def(py::self &= py::self)
		.def(py::self & Qvar())
		.def(py::self &= Qvar())
		.def("nand", static_cast<Qequation (Qequation::*)(const Qequation&) const>(&Qequation::nand), "operator ~&")
		.def("nandMutable", static_cast<Qequation& (Qequation::*)(const Qequation &)>(&Qequation::nandMutable), "operator ~&=")
		.def("nand", static_cast<Qequation (Qequation::*)(const Qvar &) const>(&Qequation::nand), "operator ~&")
		.def("nandMutable", static_cast<Qequation& (Qequation::*)(const Qvar &)>(&Qequation::nandMutable), "operator ~&=")
		.def(py::self | py::self)
		.def(py::self |= py::self)
		.def(py::self | Qvar())
		.def(py::self |= Qvar())
		.def("nor", static_cast<Qequation(Qequation::*)(const Qequation &) const>(&Qequation::nor), "operator ~|")
		.def("norMutable", static_cast<Qequation&  (Qequation::*)(const Qequation &)>(&Qequation::norMutable), "operator ~|=")
		.def("nor", static_cast<Qequation (Qequation::*)(const Qvar &) const>(&Qequation::nor), "operator ~|")
		.def("norMutable", static_cast<Qequation&  (Qequation::*)(const Qvar &)>(&Qequation::norMutable), "operator ~|=")
		.def(py::self ^ py::self)
		.def(py::self ^= py::self)
		.def(py::self ^ Qvar())
		.def(py::self ^= Qvar())
		.def(py::self + py::self)
		.def(py::self += py::self)
		.def(py::self + Qvar())
		.def(py::self += Qvar())
		.def(py::self * py::self)
		.def(py::self *= py::self)
		.def(py::self * Qvar())
		.def(py::self *= Qvar())
		.def("size", &Qequation::size)
		.def("solutions", &Qequation::solutions)
		.def("add", &Qequation::add)
		.def("set", &Qequation::set)
		.def("toString", &Qequation::toString)
		.def("result", &Qequation::result)
		.def("qubo", &Qequation::qubo);


		// specify C++ class->baseclass specialization
		py::class_<Qcondition, Qstatement>(m, "Qcondition")
			.def(py::init<>())
			.def(py::init<const Qcondition&>());

#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}
