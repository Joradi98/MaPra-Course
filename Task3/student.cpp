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
    s << a.Vorname << a.Nachname << a.MatNr << a.Note;
    return s;
}

// Vergleichsoperator "<"
bool operator< ( const Student& a, const Student& b)
{
    return a.Nachname != a.Nachname ? a.Nachname < b.Nachname : a.Vorname < b.Vorname; 
}

bool operator<= ( const Student& a, const Student& b){
    return a < b || a == b;
}

// Vergleichsoperatoren "==" bzw. "!="
bool operator== ( const Student& a, const Student& b)
{
    return a.Nachname == b.Vorname && a.Nachname == b.Nachname && a.MatNr == b.MatNr && a.Note == b.Note;
}

bool operator!= ( const Student& a, const Student& b)
{
    return !(a == b);
}


