#include "semanticObject.h"

SemanticObject::SemanticObject() : type(""), identifier(""), shape(""), size(""),
                                   size2(""), calltype("") { }

SemanticObject::SemanticObject(SemanticObject const &s) :
        type(s.type), identifier(s.identifier),
        shape(s.shape), size(s.size), size2(s.size2),
        calltype(s.calltype) { }

SemanticObject::SemanticObject(string id, string shp, string siz, string siz2,
                               string typ) : type(typ), identifier(id), shape(shp),
                                             size(siz), size2(siz2), calltype("-") { }

SemanticObject::SemanticObject(string id, string shp, string siz, string siz2,
                               string typ, string call) :
        type(typ), identifier(id), shape(shp), size(siz),
        size2(siz2), calltype(call) { }

void SemanticObject::print() {
    cout << "| " << setw(11) << "Identifier  | " << setw(8) << "  Type    | " << setw(8) << "  Shape  | " << setw(5) <<
    " Rows | " << setw(5) << " Cols |" << endl;
    cout << "------------------------------------------------------" << endl;
    cout << "| " << setw(11) << getIdentifier() << " | " << setw(9) << getType()
    << " | " << setw(8) << getShape() << " | " << setw(5) << getSize() << " | " << setw(5) << getSize2() << " | " <<
    endl;
    cout << "------------------------------------------------------" << endl;
}

SemanticObject &SemanticObject::operator=(SemanticObject const &s) {
    type = s.type;
    identifier = s.identifier;
    shape = s.shape;
    size = s.size;
    size2 = s.size2;
    calltype = s.calltype;
    return *this;
}

