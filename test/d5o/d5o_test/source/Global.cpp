#include <Factory.h>
#include <Logger.h>

#include <Qubo.h>
#include <Qop.h>

using namespace std;
using namespace dann5;
using namespace dann5::ocean;


/**** A glonal instance of dann5 Logger ****/
auto_ptr<Logger> Logger::mpSingleton;

/**** A glonal instance of Qubo Table Factory singleton ****/
Factory<string, QuboTable> Factory<string, QuboTable>::gFactory;

/**** Equal operator Qubo Table ****/

const string EqQT::cMark = "==";
const string EqQT::cName = "equal";

/**** Factory Creator's of Equal operator Qubo Table ****/
Creator<string, QuboTable, EqQT> gEqualQTcreator_m(EqQT::cMark);
Creator<string, QuboTable, EqQT> gEqualQTcreator(EqQT::cName);


/**** Not operator Qubo Table ****/

const string NeqQT::cMark = "~";
const string NeqQT::cName = "not";

/**** Factory Creator's of Not operator Qubo Table ****/
Creator<string, QuboTable, NeqQT> gNotQTcreator_m(NeqQT::cMark);
Creator<string, QuboTable, NeqQT> gNotQTcreator(NeqQT::cName);


/**** Less-than operator Qubo Table ****/

const string LtQT::cMark = "<";
const string LtQT::cName = "lessthan";

/**** Factory Creator's of Less-than operator Qubo Table ****/
Creator<string, QuboTable, LtQT> gLessThanQTcreator_m(LtQT::cMark);
Creator<string, QuboTable, LtQT> gLessThanQTcreator(LtQT::cName);


/**** Less-equal operator Qubo Table ****/

const string LeQT::cMark = "<=";
const string LeQT::cName = "lessequal";

/**** Factory Creator's of Less-equal operator Qubo Table ****/
Creator<string, QuboTable, LeQT> gLessEqualQTcreator_m(LeQT::cMark);
Creator<string, QuboTable, LeQT> gLessEqualQTcreator(LeQT::cName);


/**** Greater-than operator Qubo Table ****/

const string GtQT::cMark = ">";
const string GtQT::cName = "greaterthan";

/**** Factory Creator's of Greater-than operator Qubo Table ****/
Creator<string, QuboTable, GtQT> gGreaterThanQTcreator_m(GtQT::cMark);
Creator<string, QuboTable, GtQT> gGreaterThanQTcreator(GtQT::cName);


/**** Greater-equal operator Qubo Table ****/

const string GeQT::cMark = ">=";
const string GeQT::cName = "greaterequal";

/**** Factory Creator's of Greater-equal operator Qubo Table ****/
Creator<string, QuboTable, GeQT> gGreaterEqualQTcreator_m(GeQT::cMark);
Creator<string, QuboTable, GeQT> gGreaterEqualQTcreator(GeQT::cName);


/**** And binary operation Qubo Table ****/

const string AndQT::cMark = "&";
const string AndQT::cName = "and";

/**** Factory Creator's of And binary operation Qubo Table ****/
Creator<string, QuboTable, AndQT> gAndQTcreator_m(AndQT::cMark);
Creator<string, QuboTable, AndQT> gAndQTcreator(AndQT::cName);


/**** Nand binary operation Qubo Table with two outputs****/

const string NandQT::cMark = "~&";
const string NandQT::cName = "nand";

/**** Factory Creator's of Nand binary operation Qubo Table ****/
Creator<string, QuboTable, NandQT> gNandQTcreator_m(NandQT::cMark);
Creator<string, QuboTable, NandQT> gNandQTcreator(NandQT::cName);


/**** Or binary operation Qubo Table ****/

const string OrQT::cMark = "|";
const string OrQT::cName = "or";

/**** Factory Creator's of Or binary operation Qubo Table ****/
Creator<string, QuboTable, OrQT> gOrQTcreator_m(OrQT::cMark);
Creator<string, QuboTable, OrQT> gOrQTcreator(OrQT::cName);


/**** Nand binary operation Qubo Table with two outputs****/

const string NorQT::cMark = "~|";
const string NorQT::cName = "nor";

/**** Factory Creator's of Nor binary operation Qubo Table ****/
Creator<string, QuboTable, NorQT> gNorQTcreator_m(NorQT::cMark);
Creator<string, QuboTable, NorQT> gNorQTcreator(NorQT::cName);


/**** Not-left-or-right binary operation Qubo Table with two outputs****/

const string NotLeftOrRightQT::cMark = "~l|r";
const string NotLeftOrRightQT::cName = "notleftorright";

/**** Factory Creator's of not-left-or-right binary operation Qubo Table ****/
Creator<string, QuboTable, NotLeftOrRightQT> gNotLeftOrRightQTcreator_m(NotLeftOrRightQT::cMark);
Creator<string, QuboTable, NotLeftOrRightQT> gNotLeftOrRightQTcreator(NotLeftOrRightQT::cName);


/**** DWave not-left-or-right binary operation Qubo Table with two outputs****/

const string DwNotLeftOrRightQT::cMark = "d~l|r";
const string DwNotLeftOrRightQT::cName = "dwnotleftorright";

/**** Factory Creator's of DWave not-left-or-right binary operation Qubo Table ****/
Creator<string, QuboTable, DwNotLeftOrRightQT> gNotLeftOrRightQTcreator_m_dw(DwNotLeftOrRightQT::cMark);
Creator<string, QuboTable, DwNotLeftOrRightQT> gNotLeftOrRightQTcreator_dw(DwNotLeftOrRightQT::cName);


/**** Xor binary operation Qubo Table with two outputs****/

const string XorQT::cMark = "^";
const string XorQT::cName = "xor";

/**** Factory Creator's of Xor binary operation Qubo Table ****/
Creator<string, QuboTable, XorQT> gXorQTcreator_m(XorQT::cMark);
Creator<string, QuboTable, XorQT> gXorQTcreator(XorQT::cName);


/**** Nxor binary operation Qubo Table with two outputs****/

const string NxorQT::cMark = "~^";
const string NxorQT::cName = "nxor";

/**** Factory Creator's of Nxor binary operation Qubo Table ****/
Creator<string, QuboTable, NxorQT> gNxorQTcreator_m(NxorQT::cMark);
Creator<string, QuboTable, NxorQT> gNxorQTcreator(NxorQT::cName);


/**** Half-adder binary operation Qubo Table with two outputs****/

const string Adder05QT::cMark = "+05";
const string Adder05QT::cName = "halfadder";

/**** Factory Creator's of Half-adder binary operation Qubo Table ****/
Creator<string, QuboTable, Adder05QT> gHalfAdderQTcreator_m(Adder05QT::cMark);
Creator<string, QuboTable, Adder05QT> gHalfAdderQTcreator(Adder05QT::cName);


/**** Adder trinary operation Qubo Table with two outputs****/

const string AdderQT::cMark = "+";
const string AdderQT::cName = "adder";

/**** Factory Creator's of Adder trinary operation Qubo Table ****/
Creator<string, QuboTable, AdderQT> gAdderQTcreator_m(AdderQT::cMark);
Creator<string, QuboTable, AdderQT> gAdderQTcreator(AdderQT::cName);

/*** Operations Factory ***/

Factory<string, Qop> Factory<string, Qop>::gFactory;

Creator<string, Qop, Qeq> gEqualOperatorCreator_m(EqQT::cMark);
Creator<string, Qop, Qeq> gEqualOperatorCreator(EqQT::cName);

Creator<string, Qop, Qneq> gNotOperatorCreator_m(NeqQT::cMark);
Creator<string, Qop, Qneq> gNotOperatorCreator(NeqQT::cName);

Creator<string, Qop, Qlt> gLtOperatorCreator_m(LtQT::cMark);
Creator<string, Qop, Qlt> gLtOperatorCreator(LtQT::cName);

Creator<string, Qop, Qle> gLeOperatorCreator_m(LeQT::cMark);
Creator<string, Qop, Qle> gLeOperatorCreator(LeQT::cName);

Creator<string, Qop, Qgt> gGtOperatorCreator_m(GtQT::cMark);
Creator<string, Qop, Qgt> gGtOperatorCreator(GtQT::cName);

Creator<string, Qop, Qge> gGeOperatorCreator_m(GeQT::cMark);
Creator<string, Qop, Qge> gGeOperatorCreator(GtQT::cName);

Creator<string, Qop, Qand> gAndOpCreator_m(AndQT::cMark);
Creator<string, Qop, Qand> gAndOpCreator(AndQT::cName);

Creator<string, Qop, QnAnd> gNandOpCreator_m(NandQT::cMark);
Creator<string, Qop, QnAnd> gNandOpCreator(NandQT::cName);

Creator<string, Qop, Qor> gOrOpCreator_m(OrQT::cMark);
Creator<string, Qop, Qor> gOrOpCreator(OrQT::cName);

Creator<string, Qop, QnOr> gNorOpCreator_m(NorQT::cMark);
Creator<string, Qop, QnOr> gNorOpCreator(NorQT::cName);
/*
Creator<string, Qop, QnLeftOrRight> gNleftOrRightOpCreator("notleftorright");

Creator<string, Qop, DwNotLeftOrRightOp> gNotLeftOrRightOpCreator_dw("dwnotleftorright");
*/
Creator<string, Qop, Qxor> gXorOpCreator_m(XorQT::cMark);
Creator<string, Qop, Qxor> gXorOpCreator(XorQT::cName);
/*
Creator<string, Qop, NxorOp> gNxorOpCreator_m("~^");
Creator<string, Qop, NxorOp> gNxorOpCreator("nxor");
*/
Creator<string, Qop, Qadder05> gHalfAdderOpCreator_m(Adder05QT::cMark);
Creator<string, Qop, Qadder05> gHalfAdderOpCreator(Adder05QT::cName);

Creator<string, Qop, Qadder> gAdderOpCreator_m(AdderQT::cMark);
Creator<string, Qop, Qadder> gAdderOpCreator((string(AdderQT::cName)));