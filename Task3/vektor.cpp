/***********************************************
*  Name       : vektor.cpp                     *
*  Verwendung : Vektorklasse                   *
*  Autor      :                                *
*  Datum      :                                *
***********************************************/

#include "vektor.h"
#include <iomanip>
#include <cmath>
#include <cstdlib>

// =======================
//      Konstruktoren
// =======================


// ----- Konstruktor -----

Vektor::Vektor(size_t l)
{
#ifndef NDEBUG
  if (l <= 0)
    VekFehler("Nur Vektoren mit positiver Laenge!");
#endif

  this->ReDim(l);
}


// ----- Kopierkonstruktor -----

Vektor::Vektor(const Vektor& x):Vek(x.Vek){ }


// ===========================================
//      Vektorelement schreiben und lesen
// ===========================================


// ----- Schreib- und Lesezugriff auf Vektorelemente -----

double& Vektor::operator () (size_t i) {
#ifndef NDEBUG
  if (i >= Vek.size())
    VekFehler("Ungueltiger Index!");
#endif

  return Vek[i];
}


// ----- Lesezugriff auf Elemente konstanter Vektoren -----

double Vektor::operator () (size_t i) const {
#ifndef NDEBUG
  if (i >= Vek.size())
    VekFehler("Ungueltiger Index!");
#endif

  return Vek[i];
}

// =====================
//      Zuweisungen
// =====================

// ----- Zuweisungsoperator "=" ----

Vektor& Vektor::operator = (const Vektor& x)
{
#ifndef NDEBUG
  if (Vek.size() != x.Laenge())
    VekFehler("Inkompatible Dimensionen fuer 'Vektor = Vektor'!");
#endif

  for (size_t i = 0; i < Vek.size(); i++)
    (*this) (i) = x(i);

  //oder: this->Vek=x.Vek;

  return *this;
}


// ----- Zuweisungsoperator mit Addition "+=" ----

Vektor& Vektor::operator += (const Vektor& x)
{
  // ***** Hier fehlt was *****
}


// ----- Zuweisungsoperator mit Subtraktion "-=" ----

Vektor& Vektor::operator -= (const Vektor& x)
{
  // ***** Hier fehlt was *****
}


// ----- Zuweisungsoperator mit Multiplikation "*=" ----

Vektor& Vektor::operator *= (const double c)
{
  // ***** Hier fehlt was *****
}


// ----- Zuweisungsoperator mit Division "/=" ----

Vektor& Vektor::operator /= (const double c)
{
  // ***** Hier fehlt was *****
}


// ==============================
//      Vektorlaenge aendern
// ==============================


// ----- Vektorlaenge aendern -----

Vektor& Vektor::ReDim (size_t l)
{
#ifndef NDEBUG
  if (l <= 0)
    VekFehler("Nur Vektoren mit positiver Laenge!");
#endif

  Vek.clear();
  Vek.resize(l,0);

  return *this;
}


// ======================
//      Vektornormen
// ======================


// ----- Euklidische Norm -----

double Vektor::Norm2() const
{
  // ***** Hier fehlt was *****
}


// ----- Maximum-Norm -----

double Vektor::NormMax() const
{
  // ***** Hier fehlt was *****
}


// ==================================
//      arithmetische Operatoren
// ==================================


// ----- Addition "+" -----

Vektor operator + (const Vektor& x, const Vektor& y)
{
#ifndef NDEBUG
  if (x.Laenge() != y.Laenge())
    Vektor::VekFehler("Inkompatible Dimensionen fuer 'Vektor + Vektor'!");
#endif

  Vektor z = x;
  return z += y;
}


// ----- Subtraktion "-" -----

Vektor operator - (const Vektor& x, const Vektor& y)
{
  // ***** Hier fehlt was *****
}


// ----- Vorzeichen wechseln "-" -----

Vektor operator - (const Vektor& x)
{
  // ***** Hier fehlt was *****
}


// ----- Skalarprodukt -----

double dot (const Vektor& x, const Vektor& y)
{
  // ***** Hier fehlt was *****
}


// ----- Multiplikation Skalar*Vektor "*" -----

Vektor operator * (const double c, const Vektor& x)
{
  // ***** Hier fehlt was *****
}


// ----- Multiplikation Vektor*Skalar "*" -----

Vektor operator * (const Vektor& x, const double c)
{
  // ***** Hier fehlt was *****
}


// ----- Division Vektor/Skalar "/" -----

Vektor operator / (const Vektor& x, const double c)
{
  // ***** Hier fehlt was *****
}


// ==============================
//      Vergleichsoperatoren
// ==============================


// ----- Test auf Gleichheit "==" -----

bool operator == (const Vektor& x, const Vektor& y)
{
  if (x.Laenge() !=y.Laenge()) return false;

  for (size_t i = 0, xsize = x.Laenge(); i < xsize; i++)
    if (x(i)!=y(i)) return false;

  return true;
}


// ----- Test auf Ungleichheit "!=" -----

bool operator != (const Vektor& x, const Vektor& y)
{
  // ***** Hier fehlt was *****
}


// ==========================
//      Ein- und Ausgabe
// ==========================


// ----- Ausgabe "<<" -----

std::ostream& operator << (std::ostream& s, const Vektor& x)
{
  s << std::setiosflags(std::ios::right);
  s << "# Laenge: " << x.Laenge();
  for (size_t i = 0, xsize = x.Laenge(); i < xsize; i++)
    s << "\n" << x(i);

  return s << std::endl;
}


// ----- Eingabe ">>" -----

std::istream& operator >> (std::istream& s, Vektor& x)
{
  for (size_t i = 0, xsize = x.Laenge(); i < xsize; i++) {
	s >> x(i);
  }
  return s;
}


// ==========================
//      Fehlerbehandlung
// ==========================


// ----- Ausgabe der Fehlermeldung "str" -----

void Vektor::VekFehler (const std::string& str)
{
  std::cerr << "\nVektorfehler: " << str << '\n' << std::endl;
  exit(1);
}
