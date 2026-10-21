/**
 * @file SorManager.h
 * @brief Deklaracja menedzera zarzadzajacego logika bazy pacjentow.
 * @author Wiktor Hankus (158159)
 */
#ifndef SORMANAGER_H
#define SORMANAGER_H

#include "SmartArray.h"
#include "Pacjent.h"
#include <fstream>
#include <stdexcept>
#include <string>

/**
 * @class SorManager
 * @brief Zarzadza obiektem inteligentnej tablicy, agregacja danych i operacjami IO.
 */
class SorManager {
private:
    SmartArray<Pacjent> bazaPacjentow;

    void zapiszStringBinarnie(std::ofstream& file, const std::string& str);
    std::string wczytajStringBinarnie(std::ifstream& file);

public:
    /** @brief Domyslny konstruktor menedzera */
    SorManager() = default;

    /**
     * @brief Dodaje nowego pacjenta na koniec bazy (tablicy)
     * @param p Obiekt pacjenta
     */
    void dodajPacjenta(const Pacjent& p);

    /**
     * @brief Daje bezposredni dostep do bazy pacjentow
     * @return Referencja do inteligentnej tablicy pacjentow
     */
    SmartArray<Pacjent>& getBaza();

    /**
     * @brief Zapisuje wszystkie obiekty pacjentow do pliku binarnego
     * @param nazwaPliku Nazwa lub sciezka pliku docelowego
     * @throws std::runtime_error Jesli plik nie moze zostac otwarty
     */
    void zapiszDoPlikuBinarnego(const std::string& nazwaPliku);

    /**
     * @brief Wczytuje obiekty pacjentow z pliku binarnego
     * @param nazwaPliku Nazwa lub sciezka pliku zrodlowego
     * @throws std::runtime_error Jesli plik nie moze zostac otwarty
     */
    void wczytajZPlikuBinarnego(const std::string& nazwaPliku);

    /**
     * @brief Wyszukuje pacjenta po jego unikalnym numerze PESEL
     * @param szukanyPesel Numer PESEL do wyszukania
     * @return Wskaznik na znalezionego pacjenta (lub nullptr, jesli nie istnieje)
     */
    Pacjent* znajdzPoPeselu(const std::string& szukanyPesel);

    /**
     * @brief Wyszukuje wszystkich pacjentow o danym nazwisku
     * @param szukaneNazwisko Nazwisko do wyszukania
     * @return Nowa tablica SmartArray zawierajaca dopasowanych pacjentow
     */
    SmartArray<Pacjent> znajdzPoNazwisku(const std::string& szukaneNazwisko);

    /**
     * @brief Funkcja agregujaca: Oblicza sredni wiek wszystkich pacjentow na oddziale
     * @return Sredni wiek w latach (lub 0.0, gdy baza jest pusta)
     */
    double obliczSredniWiek() const;

    /**
     * @brief Funkcja agregujaca z kryterium: Oblicza sredni czas oczekiwania dla danej grupy
     * @param szukanyPriorytet Priorytet, dla ktorego ma zostac obliczona srednia (1, 2 lub 3)
     * @return Sredni czas oczekiwania w minutach
     */
    double sredniCzasOczekiwaniaDlaPriorytetu(int szukanyPriorytet) const;

    /**
     * @brief Zlicza, ilu pacjentow znajduje sie w danej lokalizacji
     * @param szukanyStatus Status (miejsce pobytu) do zliczenia
     * @return Liczba dopasowanych pacjentow
     */
    int policzPacjentowZeStatusem(int szukanyStatus) const;

    /**
     * @brief Aktualizuje czas oczekiwania w systemie
     * @param minuty Liczba minut, ktora uplynela od ostatniego sprawdzenia
     */
    void symulujUplywCzasu(int minuty);

    double sredniCzasDlaPriorytetuIWielku(int szukanyPriorytet, int minimalnyWiek) const;
};

#endif