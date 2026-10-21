#include "../include/Pacjent.h"

Pacjent::Pacjent() : imie(""), nazwisko(""), pesel(""), wiek(0), czas_oczekiwania(0), priorytet(3), status(0) {}

Pacjent::Pacjent(std::string i, std::string n, std::string p, int w, int czas, int prior)
    : imie(std::move(i)), nazwisko(std::move(n)), pesel(std::move(p)),
      wiek(w), czas_oczekiwania(czas), priorytet(prior), status(0) {}

std::string Pacjent::getPesel() const { return pesel; }
std::string Pacjent::getNazwisko() const { return nazwisko; }
std::string Pacjent::getImie() const { return imie; }
int Pacjent::getWiek() const { return wiek; }
int Pacjent::getCzasOczekiwania() const { return czas_oczekiwania; }
int Pacjent::getPriorytet() const { return priorytet; }
int Pacjent::getStatus() const { return status; }

void Pacjent::setPriorytet(int nowyPriorytet) { priorytet = nowyPriorytet; }
void Pacjent::setStatus(int nowyStatus) { status = nowyStatus; }
void Pacjent::dodajCzasOczekiwania(int minuty) { czas_oczekiwania += minuty; }


std::ostream& operator<<(std::ostream& os, const Pacjent& p) {
    os << p.pesel << " " << p.nazwisko << " " << p.imie;
    return os;
}

bool Pacjent::operator==(const Pacjent& other) const {
    return pesel == other.pesel;
}

bool Pacjent::operator!=(const Pacjent& other) const {
    return !(*this == other);
}

