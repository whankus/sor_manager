#include "../include/SorManager.h"

void SorManager::zapiszStringBinarnie(std::ofstream& file, const std::string& str) {
    size_t dlugosc = str.size();
    file.write(reinterpret_cast<const char*>(&dlugosc), sizeof(dlugosc));
    if (dlugosc > 0) file.write(str.data(), dlugosc);
}

std::string SorManager::wczytajStringBinarnie(std::ifstream& file) {
    size_t dlugosc = 0;
    file.read(reinterpret_cast<char*>(&dlugosc), sizeof(dlugosc));
    if (dlugosc == 0) return "";
    std::string str(dlugosc, '\0');
    file.read(&str[0], dlugosc);
    return str;
}

void SorManager::dodajPacjenta(const Pacjent& p) {
    bazaPacjentow.pushBack(p);
}

SmartArray<Pacjent>& SorManager::getBaza() {
    return bazaPacjentow;
}

void SorManager::zapiszDoPlikuBinarnego(const std::string& nazwaPliku) {
    std::ofstream plik(nazwaPliku, std::ios::binary | std::ios::out);
    if (!plik.is_open()) throw std::runtime_error("Blad zapisu!");

    unsigned int liczba = bazaPacjentow.size();
    plik.write(reinterpret_cast<const char*>(&liczba), sizeof(liczba));

    for (unsigned int i = 0; i < liczba; ++i) {
        const Pacjent& p = bazaPacjentow[i];
        zapiszStringBinarnie(plik, p.getPesel());
        zapiszStringBinarnie(plik, p.getNazwisko());
        zapiszStringBinarnie(plik, p.getImie());

        int wiek = p.getWiek();
        int czas = p.getCzasOczekiwania();
        int prior = p.getPriorytet();
        int stat = p.getStatus();

        plik.write(reinterpret_cast<const char*>(&wiek), sizeof(wiek));
        plik.write(reinterpret_cast<const char*>(&czas), sizeof(czas));
        plik.write(reinterpret_cast<const char*>(&prior), sizeof(prior));
        plik.write(reinterpret_cast<const char*>(&stat), sizeof(stat));
    }
    plik.close();
}

void SorManager::wczytajZPlikuBinarnego(const std::string& nazwaPliku) {
    std::ifstream plik(nazwaPliku, std::ios::binary | std::ios::in);
    if (!plik.is_open()) throw std::runtime_error("Blad odczytu!");

    bazaPacjentow.clear();
    unsigned int liczba = 0;
    plik.read(reinterpret_cast<char*>(&liczba), sizeof(liczba));

    for (unsigned int i = 0; i < liczba; ++i) {
        std::string pesel = wczytajStringBinarnie(plik);
        std::string nazwisko = wczytajStringBinarnie(plik);
        std::string imie = wczytajStringBinarnie(plik);

        int wiek = 0, czas = 0, prior = 3, stat = 0;
        plik.read(reinterpret_cast<char*>(&wiek), sizeof(wiek));
        plik.read(reinterpret_cast<char*>(&czas), sizeof(czas));
        plik.read(reinterpret_cast<char*>(&prior), sizeof(prior));
        plik.read(reinterpret_cast<char*>(&stat), sizeof(stat));

        Pacjent p(imie, nazwisko, pesel, wiek, czas, prior);
        p.setStatus(stat);
        bazaPacjentow.pushBack(p);
    }
    plik.close();
}

Pacjent* SorManager::znajdzPoPeselu(const std::string& szukanyPesel) {
    for (unsigned int i = 0; i < bazaPacjentow.size(); ++i) {
        if (bazaPacjentow[i].getPesel() == szukanyPesel) return &bazaPacjentow[i];
    }
    return nullptr;
}

SmartArray<Pacjent> SorManager::znajdzPoNazwisku(const std::string& szukaneNazwisko) {
    SmartArray<Pacjent> wyniki;
    for (unsigned int i = 0; i < bazaPacjentow.size(); ++i) {
        if (bazaPacjentow[i].getNazwisko() == szukaneNazwisko) wyniki.pushBack(bazaPacjentow[i]);
    }
    return wyniki;
}

double SorManager::obliczSredniWiek() const {
    if (bazaPacjentow.isEmpty()) return 0.0;
    double suma = 0;
    for (unsigned int i = 0; i < bazaPacjentow.size(); ++i) suma += bazaPacjentow[i].getWiek();
    return suma / bazaPacjentow.size();
}

double SorManager::sredniCzasOczekiwaniaDlaPriorytetu(int szukanyPriorytet) const {
    if (bazaPacjentow.isEmpty()) return 0.0;
    double suma = 0;
    int licznik = 0;
    for (unsigned int i = 0; i < bazaPacjentow.size(); ++i) {
        if (bazaPacjentow[i].getPriorytet() == szukanyPriorytet) {
            suma += bazaPacjentow[i].getCzasOczekiwania();
            licznik++;
        }
    }
    return licznik == 0 ? 0.0 : suma / licznik;
}

int SorManager::policzPacjentowZeStatusem(int szukanyStatus) const {
    int licznik = 0;
    for (unsigned int i = 0; i < bazaPacjentow.size(); ++i) {
        if (bazaPacjentow[i].getStatus() == szukanyStatus) licznik++;
    }
    return licznik;
}

void SorManager::symulujUplywCzasu(int minuty) {
    for (unsigned int i = 0; i < bazaPacjentow.size(); ++i) {
        if (bazaPacjentow[i].getStatus() == 0) bazaPacjentow[i].dodajCzasOczekiwania(minuty);
    }
}

double SorManager::sredniCzasDlaPriorytetuIWielku(int szukanyPriorytet, int minimalnyWiek) const {
    if (bazaPacjentow.isEmpty()) return 0.0;
    double suma = 0;
    int licznik = 0;
    for (unsigned int i = 0; i < bazaPacjentow.size(); ++i) {
        if (bazaPacjentow[i].getPriorytet() == szukanyPriorytet && bazaPacjentow[i].getWiek() > minimalnyWiek) {
            suma += bazaPacjentow[i].getCzasOczekiwania();
            licznik++;
        }
    }
    return licznik == 0 ? 0.0 : suma / licznik;
}