// debug_new.cpp
// compile by using: cl /EHsc /W4 /D_DEBUG /MDd debug_new.cpp
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

// TestConsoleApplication.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#ifndef EIGEN_NO_STATIC_ASSERT
#define EIGEN_NO_STATIC_ASSERT // turn static asserts into runtime asserts in order to check them
#endif

#include <iostream>
#include <Eigen\Dense>

#include <Qubo.h>
#include <Qtype.h>
#include <Qvar.h>
#include <Qexpression.h>
#include <Qequation.h>
#include <Factory.h>
#include <Qcondition.h>
#include <Qroutine.h>
#include <Qfunction.h>

#include <Utility.h>

using namespace Eigen;

using Eigen::Matrix;
using Eigen::MatrixXd;
using Eigen::Map;

using namespace dann5::ocean;
using namespace std;

void test1()
{
    std::cout << "Hello World!\n";

	pair <int, char> PAIR1;

	PAIR1.first = 100;
	PAIR1.second = 'G';

	cout << PAIR1.first << " ";
	cout << PAIR1.second << endl;

	Matrix<int, 2, 2> m(2, 2);
	m(0, 0) = 3;
	m(1, 0) = int(2.5);
	m(0, 1) = -1;
	m(1, 1) = m(1, 0) + m(0, 1);
	std::cout << m << std::endl << std::endl;

	typedef std::pair<string, string> lables;
	typedef std::pair<lables, double> element;
	Matrix<std::pair<lables, double>, 2, 2> notQubo;
	lables a("x", "x");
	notQubo(0, 0) = element(a, -1);
	notQubo(1, 0) = element(lables("z", "x"), 2);
	notQubo(0, 1) = element(lables("x", "z"), 0);
	notQubo(1, 1) = element(lables("z", "z"), -1);

	int i, j;
	for(i = 0; i < 2; i++)
	{
		for (j = 0; j < 2; j++)
			std::cout << "(" << notQubo(i, j).first.first << ", " << notQubo(i, j).first.second << ") = " << notQubo(i, j).second << ";\t";
		std::cout << std::endl;
	}
	std::cout << std::endl << std::endl;

	MatrixXd mZ = MatrixXd::Zero(3, 3);
	MatrixXd mC = MatrixXd::Constant(3, 3, 2.1);
	mZ = (mZ + mC);
	cout << "mZ =" << endl << mZ << endl;
	VectorXd v0(3);
	v0 << 1, 2, 3;
	cout << "mZ * v0 =" << endl << mZ * v0 << endl << endl;

	MatrixXd mO = MatrixXd::Ones(4, 4);
	mO = (mO * MatrixXd::Constant(4, 4, 2)) * 2.5;
	cout << "mO =" << endl << mO << endl;
	VectorXd v1(4);
	v1 << 1, 2, 3, 1;
	cout << "mO * v1 =" << endl << mO * v1 << endl << endl;

	MatrixXd mR = MatrixXd::Random(3, 3);
	mR = (mR + MatrixXd::Constant(3, 3, 1.2)) * 50;
	cout << "mR =" << endl << mR << endl;
	VectorXd vR(3);
	vR << 1, 2, 3;
	cout << "mR * vR =" << endl << mR * vR << endl << endl;

	MatrixXf aM(2, 2);
	std::cout << "aM is of size " << aM.rows() << "x" << aM.cols() << std::endl;
	MatrixXf bM(3, 3);
	aM = bM;
	std::cout << "aM is now of size " << aM.rows() << "x" << aM.cols() << std::endl;

	Matrix4d fixedMatrix;
	std::cout << "fixedMatrix is of size " << fixedMatrix.rows() << "x" << fixedMatrix.cols() << std::endl;

	NotQT neQ;
	std::cout << std::endl << "Not Equal" << std::endl << neQ << std::endl << std::endl;

	AndQT andQ;
	std::cout << std::endl << "And" << std::endl << andQ << std::endl << std::endl;

	NandQT nandQ;
	std::cout << std::endl << "Nand" << std::endl << nandQ << std::endl << std::endl;

	OrQT orQ;
	std::cout << std::endl << "Or" << std::endl << orQ << std::endl << std::endl;

	NorQT norQ;
	std::cout << std::endl << "Nor" << std::endl << norQ << std::endl << std::endl;

	NotLeftOrRightQT nLorRQ;
	std::cout << std::endl << "Not(left) And right" << std::endl << nLorRQ << std::endl << std::endl;

	DwNotLeftOrRightQT dWnLorRQ;
	std::cout << std::endl << "D-Wave Not(left) And right" << std::endl << dWnLorRQ << std::endl << std::endl;

	NxorQT nxorQ;
	std::cout << std::endl << "Not Xor" << std::endl << nxorQ << std::endl << std::endl;

	XorQT xorQ;
	std::cout << std::endl << "Xor" << std::endl << xorQ << std::endl << std::endl;

	Adder05QT hAdderQ;
	std::cout << std::endl << "Half Adder" << std::endl << hAdderQ << std::endl << std::endl;

	AdderQT fAdderQ;
	std::cout << std::endl << "Full Adder" << std::endl << fAdderQ << std::endl << std::endl;

	Qubo d = nxorQ.qubo();

	Qwhole v_na(Index(3)), v_nb(Index(4)), v_nr;
	v_na << 1, 0, Qbit::cSuperposition;
	v_nb << 0, 1, 1, 1;
	std::cout << std::endl << "Va: " << v_na.value() << std::endl << v_na << std::endl << "Vb: " << v_nb.value() << std::endl << v_nb << std::endl;
	v_nr = v_na & v_nb;
	std::cout << std::endl << "And: " << v_nr.value() << std::endl << v_nr << std::endl;
	v_nr = v_na | v_nb;
	std::cout << std::endl << "Or: " << v_nr.value() << std::endl << v_nr << std::endl;
	v_nr = v_na + v_nb;
	std::cout << std::endl << "Addition: " << v_nr.value() << std::endl << v_nr << std::endl;
	v_nr = v_na * v_nb;
	std::cout << std::endl << "Multiplication: " << v_nr.value() << std::endl << v_nr << std::endl;
	v_nr = ~v_nr;
	std::cout << std::endl << "Not: " << v_nr.value() << std::endl << v_nr << std::endl;

	Qand aO;
	std::cout << std::endl << typeid(aO).name() << std::endl << typeid(aO).raw_name() << std::endl;
	std::cout << std::endl << typeid(Qand).name() << std::endl << typeid(Qand).raw_name() << std::endl;

	Qdef blv_a(Index(3), "a"), blv_b(Index(3));
	Qexpression blv_r(blv_a + blv_b);
	std::cout << std::endl << "BLV R = A + B: " << std::endl << blv_r << std::endl;

	Qdef blv_z(3, "z");
	blv_r += blv_z;
	std::cout << std::endl << "BLV R += Z: " << std::endl << blv_r << std::endl;
	
	blv_r = blv_a * blv_b;
	std::cout << std::endl << "BLV R = A * B: " << std::endl << blv_r << std::endl;

	blv_r *= blv_z;
	std::cout << std::endl << "BLV R *= Z: " << std::endl << blv_r << std::endl;

	Qexpression blv_d(Qdef(3, "d"));
	blv_d *= blv_a * blv_b;
	std::cout << std::endl << "BLV D *= A * B: " << std::endl << blv_d << std::endl;
}

void test2()
{
	QuboTable::Sp pq = Factory<string, QuboTable>::Instance().create(AndQT::cMark);
	std::cout << std::endl << "And" << std::endl << *pq << std::endl;
	std::cout << std::endl << pq->qubo() << std::endl;
	QuboTable::Labels arguments(3);
	arguments << "a0", "b0", "r0";
	std::cout << std::endl << pq->qubo(arguments) << std::endl;

	pq = Factory<string, QuboTable>::Instance().create(XorQT::cMark);
	std::cout << std::endl << "Half Adder" << std::endl << *pq << std::endl;
	std::cout << std::endl << pq->qubo() << std::endl;
	std::cout << std::endl << pq->qubo(arguments) << std::endl;

	pq = Factory<string, QuboTable>::Instance().create(AdderQT::cMark);
	std::cout << std::endl << "Full Adder" << std::endl << *pq << std::endl;
	std::cout << std::endl << pq->qubo() << std::endl;
	arguments.resize(4, NoChange);
	arguments << "a1", "b1", "x{a1+b1}", "{a1+b1+x{a1+b1}}";
	std::cout << std::endl << pq->qubo(arguments) << std::endl;

	pq = Factory<string, QuboTable>::Instance().create(XorQT::cName);
	std::cout << std::endl << "Half Adder" << std::endl << *pq << std::endl ;
	std::cout << std::endl << pq->qubo() << std::endl;
	arguments.resize(4, NoChange);
	arguments << "a1", "b1", "{a+b}1", "{c}1";
	std::cout << std::endl << pq->qubo(arguments) << std::endl << std::endl;

	Qdef sv_a(3, "a"), sv_b(3, "b");
	Qexpression ev_r(sv_a * sv_b);

	std::cout << std::endl << "R = A * B: " << std::endl << ev_r << std::endl;

	std::hash<std::string> hashT;
	std::cout << std::endl << hashT("test") << std::endl;

	Qdef a(3, "a"), b(3, "b"), c(3, "c"), d(3, "d");
	Qexpression addAbcd(a + b + c + d);
	Qvar vR("R", 6), vA(a), vB(b), vC(c), vD(d);
	Qvars abcd; abcd.push_back(vA); abcd.push_back(vB); abcd.push_back(vC); abcd.push_back(vD);
	Qequation r_addAbcd(vR, addAbcd, abcd);
	std::cout << std::endl << r_addAbcd << std::endl;
	std::cout << std::endl << r_addAbcd.qubo(false) << std::endl;
	std::cout << std::endl << r_addAbcd.qubo() << std::endl;
}

void test3()
{
	Qvar x(3, "x"), y(3, "y"), z(3, "z"), w(3, "w"), vQ("Q", 6);
	Qequation eQ(vQ);
	eQ = x + y + z + w;
	cout << endl << eQ << endl;
	cout << endl << eQ.toString(true) << endl;
	cout << endl << eQ.qubo() << endl;

	Qsolver solver(eQ.qubo());
	Qsolver::Samples samples = solver.solution();
	eQ.set(samples);
	cout << endl << eQ.solutions() << endl;

	Qvar a(2, "a"), b(2, "b"), c(2, "c"), vR("R", 6);
	Qequation eR(vR);
	eR = a * b *c;
	cout << endl << eR.qubo(false) << endl;
	cout << endl << eR.toString(true) << endl;
	cout << endl << eR.toString() << endl;
	cout << endl << eR.qubo(true) << endl;

	Qroutine::Sample sample;
	sample["# | R1 | "] = 0;
	sample["# | R2 | "] = 0;
	sample["# | R3 | "] = 0;
	sample["# | a0 & b1 ^ a1 & b0 | "] = 0;
	sample["# | a1 & b1 ^ # | a0 & b1 ^ a1 & b0 || "] = 0;
	sample["# | a1 & b1 ^ # | a0 & b1 ^ a1 & b0 || &c0"] = 0;
	sample["# | a1 & b1 ^ # | a0 & b1 ^ a1 & b0 || &c1"] = 0;
	sample["a0"] = 0;
	sample["a0 & b0"] = 0;
	sample["a0 & b0 & c1"] = 0;
	sample["a0 & b1"] = 0;
	sample["a0 & b1 ^ a1 & b0"] = 1;
	sample["a0 & b1 ^ a1 & b0 & c0"] = 1;
	sample["a0 & b1 ^ a1 & b0 & c1"] = 0;
	sample["a1"] = 1;
	sample["a1 & b0"] = 1;
	sample["a1 & b1"] = 1;
	sample["a1 & b1 ^ # | a0 & b1 ^ a1 & b0 | "] = 1;
	sample["a1 & b1 ^ # | a0 & b1 ^ a1 & b0 | &c0"] = 1;
	sample["a1 & b1 ^ # | a0 & b1 ^ a1 & b0 | &c1"] = 0;
	sample["b0"] = 1;
	sample["b1"] = 1;
	sample["c0"] = 1;
	sample["c1"] = 0;
	Qroutine aRutine("test", eR);
	aRutine.add(sample);
	cout << endl << aRutine.solutions() << endl;
}

// (5.1) p = s^2 + 2t^2	when p == 3(mod 8) s==t==1(mod 2) gcd(s,t) = 1
void test4()
{
	// constants
	Qvar _7("7", 7), _8("8", 8);
	// valiables
	Qvar prime(6, "p"), p8(3, "p8"), p7m(3, "p7m");
	// equations
	Qequation p8E1(p8), p8E2(p8), p8E(p8), p7mE1(p7m), p7mE2(p7m);
	p7mE1 = prime & _7;
	p8E1 = p7m;
	p8E2 = prime;
	p7mE2 = p8 * _8;

	Qcondition p7m_lt8(p7m < _8), p7m_ge8(p7m >= _8);
	p7m_lt8 << p8E;
	p7m_ge8 << p7mE2;

	Qroutine p_ge8R("prime >= 8");
	p_ge8R << p7mE1 << p7m_lt8 << p7m_ge8;

	Qcondition p_lt8(prime < _8), p_ge8(p7m >= _8);
	p_lt8 << p8E2;
	p_ge8 << p_ge8R;

	Qfunction p_mod_8("prime % 8");
	p_mod_8 << p_lt8 << p_ge8;

	Qroutine program("Find prime numbers");
	// constants
	Qvar _2("2", 2), _3("3", 3);
	// variables
	Qvar s(3, "s"), t(3,"t");
	Qequation pE(prime);
	Qcondition p8_eq3(_3 == p_mod_8);
	pE = s * s + _2 * t * t;
	p8_eq3 << pE;
	program << p8_eq3; // << s==t==1(mod 2) << gcd(s,t) = 1
}

// (5.2) p = s^2 + 4t^2	when p == 5(mod 8) s==t==1(mod 2) gcd(s,t) = 1
void test5()
{
	Qroutine program("Find prime numbers with Qmod, Qpow and Qgcd functions");
	// constants
	Qvar _1("1",1), _2("2", 2), _4("4", 4), _5("5", 5), _8("8", 8);
	// variables
	Qvar prime(6, "p"), s(3, "s"), t(3, "t");
	Qequation pE(prime), gcdE(_1);
	Qcondition p8_eq5(Qmod(prime, _8) == _5), s2_eq1(Qmod(s, _2) == _1), t2_eq1(Qmod(t, _2) == _1);
	pE = Qpow(s, 2) + _4 * Qpow(t, 2);
	p8_eq5 << pE;
	s2_eq1 << p8_eq5;
	t2_eq1 << s2_eq1;
	gcdE = Qgcd(s, t);
	program << t2_eq1 << gcdE;
}

// (5.3) p = s^2 + 4st + 2t^2	when p == 7(mod 8) s==t==1(mod 2) gcd(s,t) = 1
void test6()
{
	Qroutine program("Find prime numbers with Qmod, Qpow and Qgcd functions");
	// constants
	Qvar _1("1", 1), _2("2", 2), _4("4", 4), _7("7", 7), _8("8", 8);
	// variables
	Qvar prime(6, "p"), s(3, "s"), t(3, "t");
	Qequation pE(prime), gcdE(_1);
	Qcondition p8_eq5(Qmod(prime, _8) == _7), s2_eq1(Qmod(s, _2) == _1), t2_eq1(Qmod(t, _2) == _1);
	pE = Qpow(s, 2) + _4 * s * t + _2 * Qpow(t, 2);
	p8_eq5 << pE;
	s2_eq1 << p8_eq5;
	t2_eq1 << s2_eq1;
	gcdE = Qgcd(s, t);
	program << t2_eq1 << gcdE;
}


void test5short()
{
	Qroutine program("Find prime numbers with Qmod, Qpow and Qgcd functions");
	// constants
	Qvar _4("4", 4), _5("5", 5), _8("8", 8);
	// variables
	Qvar p(3, "p"), s(2, "s"), t(2, "t");
	Qequation eq(p);
	Qcondition cond(Qmod(p, _8) == _5);
	eq = Qpow(s, 2) + _4 * Qpow(t, 2);
	cond << eq;
	program << cond;
}

void test7()
{
	Qdef a(Index(3), "a"), b(Index(3));
	Qexpression r(a.nand(b));
	std::cout << std::endl << "BLV R = A ~& B: " << std::endl << r << std::endl;
	Qexpression e(a.nor(b));
	std::cout << std::endl << "BLV R = A ~| B: " << std::endl << e << std::endl;
}

void testCondition()
{
	Qvar a(3, "a"), b(3, "b"), A("A", 4), _2("2_", 2);
	Qequation eA(A);
	eA = a + b;
	cout << endl << eA.toString() << endl;
	cout << endl << eA.toString(true) << endl;
	cout << endl << eA.qubo(false) << endl;
	Qubo Q = eA.qubo();
	cout << endl << Q << endl;
	Qanalyzer analyzer(Q);
	size_t nNodes = analyzer.nodesNo();
	cout << endl << "Nodes #: " << to_string(nNodes) << ", Combination #: " << to_string(long(pow(2, nNodes))) << endl;
	const clock_t begin_time = clock();
	Qsolver solver(Q);
	Qsolver::Samples samples = solver.solution();
	cout << endl << "Solving time: " << to_string(float(clock() - begin_time) / CLOCKS_PER_SEC) << "s";
	eA.set(samples);
	cout << endl << eA.solutions() << endl;

	Qcondition aGT5(a >= b);
	aGT5 << eA;
	cout << endl << aGT5.toString() << endl;
	cout << endl << aGT5.qubo(false) << endl;
	Qubo Qc = aGT5.qubo();
	cout << endl << Qc << endl;
	Qanalyzer analyzerC(Qc);
	size_t nNodesC = analyzerC.nodesNo();
	cout << endl << "Nodes #: " << to_string(nNodesC) << ", Combination #: " << to_string(long(pow(2, nNodesC))) << endl;
	const clock_t begin_timeC = clock();
	Qsolver solverC(Qc);
	Qsolver::Samples samplesC = solverC.solution();
	cout << endl << "Solving time: " << to_string(float(clock() - begin_timeC) / CLOCKS_PER_SEC) << "s";
	eA.set(samplesC);
	cout << endl << eA.solutions() << endl;
}

void testComparison()
{
	Qvar a(3, "a"), b(3, "b"), _2("2_", 2);
	Qcomparison aGT2(a >= b);
	cout << endl << aGT2.toString() << endl;
	cout << endl << aGT2.qubo(false) << endl;
	Qubo Qc = aGT2.qubo();
	cout << endl << Qc << endl;
	Qanalyzer analyzerC(Qc);
	size_t nNodesC = analyzerC.nodesNo();
	cout << endl << "Nodes #: " << to_string(nNodesC) << ", Combination #: " << to_string(long(pow(2, nNodesC))) << endl;
	const clock_t begin_timeC = clock();
	Qsolver solverC(Qc);
	Qsolver::Samples samplesC = solverC.solution();
	cout << endl << "Solving time: " << to_string(float(clock() - begin_timeC) / CLOCKS_PER_SEC) << "s";
	aGT2.set(samplesC);
	cout << endl << aGT2.solutions() << endl;
}

void testQpow()
{
	Qvar x(2, "x"), y("y", 4), y1("y", 4);
	Qequation ey(y);
	ey = Qpow(x, 2);
	cout << endl << ey.toString(true) << endl;
	cout << endl << ey.qubo(false) << endl;
	cout << endl << ey.qubo(true) << endl;

	Qequation et(y1);
	et = x * x;
	cout << endl << et.toString(true) << endl;
	cout << endl << et.qubo(false) << endl;
	cout << endl << et.qubo(true) << endl;
}

void testSubAndDiv()
{
	Qvar a(3, "a"), b(3, "b"), c(2, "c"), d(2, "d"), R("r", 15);
	Qequation eS(R - a);
//	eS = a - b;
	cout << endl << eS.toString() << endl;
	Qequation eD(d / b);
//	eD = a - b;
	cout << endl << eD.toString() << endl;
}

void testAddition()
{
	Qvar a(3, "a"), b(3, "b"), c(2, "c"), d(2, "d"), A("A", 15);
	Qequation eA(A);
	eA = a + b + c + d;
	cout << endl << eA.toString() << endl;
	cout << endl << eA.toString(true) << endl;
	cout << endl << eA.qubo(false) << endl;
	cout << endl << eA.qubo() << endl;
}

void testMultiplication()
{
	Qvar a(3, "a"), b(2, "b"), c(2, "c"), d(2, "d"), X("X", 6);
	Qequation eX(X);
	eX = a * b * c;
	cout << endl << eX.toString() << endl;
	cout << endl << eX.toString(true) << endl;
	cout << endl << eX.qubo(false) << endl;
	cout << endl << eX.qubo() << endl;
}

void testComplexEq()
{
	Qvar k(3, "k"), l(2, "l"), m(2, "m"), _2("2", 2), Z("Z", 13);
	Qequation eZ(Z), e2xK(Qvar(6, "2xK")), eLxM(Qvar(3, "LxM")), eZ2(Qvar("Z2", 11));

	e2xK = _2 * k;
	cout << endl << e2xK.toString() << endl;
	cout << endl << e2xK.toString(true) << endl;
	eLxM = l * m;
	cout << endl << eLxM.toString() << endl;
	cout << endl << eLxM.toString(true) << endl;
	eZ2 = e2xK + eLxM;
	cout << endl << eZ2.toString() << endl;
	cout << endl << eZ2.toString(true) << endl;

 	eZ = _2 * k + l * m;
	cout << endl << eZ.toString() << endl;
	cout << endl << eZ.toString(true) << endl;

	cout << endl << eZ.qubo(false) << endl;
	Qubo Q = eZ.qubo();
	cout << endl << Q << endl;
	Qanalyzer analyzer(Q);
	size_t nNodes = analyzer.nodesNo();
	cout << endl << "Nodes #: " << to_string(nNodes) << ", Combination #: " << to_string(long long(pow(2, nNodes))) << endl;
	Qsolver solver(Q);
	Qsolver::Samples samples = solver.solution();
	eZ.set(samples);
	cout << endl << eZ.solutions() << endl;
}

void testSolver()
{
	Qvar _2("2_",2), a(2, "a"), b(2, "b"), c(2, "c"), A("A", 11);
	Qequation eA(A);
	eA = _2 * a + b * c;
	Qubo Q = eA.qubo();
	cout << endl << Q << endl;
	Qanalyzer analyzer(Q);
	size_t nNodes = analyzer.nodesNo();
	cout << endl << "Nodes #: " << to_string(nNodes) << ", Combination #: " << to_string(long(pow(2, nNodes))) << endl;
	const clock_t begin_time = clock();
	Qsolver solver(Q);
	Qsolver::Samples samples = solver.solution();
	cout << endl << "Solving time: " << to_string(float(clock() - begin_time) / CLOCKS_PER_SEC) << "s";
	eA.set(samples);
	cout << endl << eA.solutions() << endl;
}

void testAddEqs()
{
	Qvar a(2, "a"), b(2, "b"), c(2, "c"), d(3, "d"), e(3, "e"), f(3, "f"), A("A", 13);
	Qequation eA(A), e1(Qvar(2, "e1")), e2(Qvar(3, "e2"));

	e1 = a + b + c;
	cout << endl << e1.toString() << endl;
	cout << endl << e1.toString(true) << endl;
	e2 = d + e;
	cout << endl << e2.toString() << endl;
	cout << endl << e2.toString(true) << endl;
	eA = e1 + e2; 
	cout << endl << eA.toString() << endl;
	cout << endl << eA.toString(true) << endl;
}

void testLargeQubo()
{
	const clock_t begin_time = clock();
	//	Qvar factor1(8, "factor1"), factor2(8, "factor2"), factor3(8, "factor3"), product("product", 8258175);
	Qvar factor1(5, "a"), factor2(5, "b"), factor3(4 , "c");
	Qvar product("P", 13020);
	cout << to_string(as_const(product).value().value()) << endl;
	cout << endl << "To initialize variables: " << to_string(float(clock() - begin_time) / CLOCKS_PER_SEC) << "s" << endl;

	const clock_t begin_time1 = clock();
	Qequation eP(product);
	cout << to_string(eP.result().value().value()) << endl;
	cout << endl << "To initialize equation: " << to_string(float(clock() - begin_time1) / CLOCKS_PER_SEC) << "s" << endl;

	const clock_t begin_time2 = clock();
	eP = factor1 * factor2 * factor3;
	cout << to_string(eP.result().value().value()) << endl;
	cout << endl << "To multiply variables and assign equation: " << to_string(float(clock() - begin_time2) / CLOCKS_PER_SEC) << "s" << endl;

	const clock_t begin_time3 = clock();
	cout << eP.toString();
	cout << endl << "To convert to string: " << to_string(float(clock() - begin_time3) / CLOCKS_PER_SEC) << "s" << endl;

	const clock_t begin_time4 = clock();
	Qubo Q = eP.qubo();
	cout << endl << "To create qubo: " << to_string(float(clock() - begin_time4) / CLOCKS_PER_SEC) << "s" << endl;

	size_t nNo = Qanalyzer(Q).nodesNo();
	cout << endl << "number of nodes: " << to_string(nNo) << " has " << (pow(2, nNo - 1) / 2 + 2 * nNo) << " possible solutions" << endl;
}

int main()
{
	// NOTE(mbozzi): ownership of the newly-allocated facet is assumed by the library
	// Typically, the following idiom is used:
	std::cout.imbue(std::locale(std::cout.getloc(), new dann5::group_thousands));

	const clock_t begin_time = clock();
//	testComparison();
//	testCondition();
//	testLargeQubo();
	testSolver();
//	testComplexEq();
//	testAddEqs();
//	testSubAndDiv();
//	test5short();
//	test3();
	cout << endl << "Running time: " << to_string(float(clock() - begin_time) / CLOCKS_PER_SEC) << "s";

	_CrtDumpMemoryLeaks();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
