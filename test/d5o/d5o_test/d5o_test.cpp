// d5o_test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include <Qbit.h>
#include <Qbool.h>
#include <Qbinary.h>
#include <Qwhole.h>
#include <Qexpr.h>

using namespace dann5::ocean;

void testTypes()
{
    Qbit a0("0a", 1), a1("1a"), a2("2a", 5);
    Qexpr<Qbit> bitExpr = a0 & a1 > a2;

    Qbool b0("0b", Qbool::cTrue), b1("1b", 'F'), b2("2b", 33);
    Qexpr<Qbool> boolExpr = b0 != b1 & b2 == b0 | b1;

    Bits bits5("101");
    Qbinary c0(3, "0c"), c1("1c", 33), c2("2c", bits5), c3("3c", 0x7);
    Qexpr<Qbinary> binExpr = c0 & c1 ^ c2 | c3;

    Qwhole w0(4, "0w"), w1("1w", 33), w2("2w", 0b01011), w3("3c", 0x7);
    Qexpr<Qbinary> wholeExpr = w0 * w1 + w2 * w3;
}

int main()
{
    std::cout << "Hello World!\n";

    testTypes();
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
