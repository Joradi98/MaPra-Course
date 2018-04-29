#include "student.h"

// Eingabeoperator ">>"
std::istream& operator>> ( std::istream& s, Student& a)
{
    s >> a.Vorname >> a.Nachname >> a.MatNr >> a.Note;
    return s;
}

// Ausgabeoperator "<<"
std::ostream& operator<< ( std::ostream& s, const Student& a)
{
    // hier den Ausgabeoperator definieren
}

// Vergleichsoperator "<"
bool operator< ( const Student&, const Student&)
{
    // hier fehlt was
}

// Vergleichsoperatoren "==" bzw. "!="
bool operator== ( const Student&, const Student&)
{
    // hier fehlt was
}

bool operator!= ( const Student&, const Student&)
{
    // hier fehlt was
}


