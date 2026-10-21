/**
 * @file Pacjent.h
 * @brief Deklaracja klasy reprezentującej pacjenta.
 * @author Wiktor Hankus (158159)
 */
#ifndef PACJENT_H
#define PACJENT_H

#include <string>
#include <iostream>

/**
 * @class Pacjent
 * @brief Klasa przechowująca dane pojedynczego pacjenta na oddziale SOR.
 */
class Pacjent {
private:
    std::string imie;
    std::string nazwisko;
    std::string pesel;
    int wiek;
    int czas_oczekiwania;
    int priorytet;
    int status;

public:
    /** * @brief Konstruktor domyślny. Tworzy pustego pacjenta.
     */
    Pacjent();

    /** * @brief Konstruktor główny parametryczny.
     * @param i Imię pacjenta
     * @param n Nazwisko pacjenta
     * @param p Numer PESEL
     * @param w Wiek w latach
     * @param czas Czas oczekiwania (domyślnie 0 przy rejestracji)
     * @param prior Priorytet (1-Czerwony, 2-Żółty, 3-Zielony)
     */
    Pacjent(std::string i, std::string n, std::string p, int w, int czas, int prior);

    /** @brief Zwraca numer PESEL pacjenta @return Numer PESEL */
    std::string getPesel() const;

    /** @brief Zwraca nazwisko pacjenta @return Nazwisko */
    std::string getNazwisko() const;

    /** @brief Zwraca imię pacjenta @return Imię */
    std::string getImie() const;

    /** @brief Zwraca wiek pacjenta @return Wiek w latach */
    int getWiek() const;

    /** @brief Zwraca czas spędzony na oddziale @return Czas oczekiwania w minutach */
    int getCzasOczekiwania() const;

    /** @brief Zwraca priorytet pilności @return Priorytet (1, 2 lub 3) */
    int getPriorytet() const;

    /** @brief Zwraca miejsce pobytu pacjenta @return Status (0-Poczekalnia, 1-Gabinet, 2-Sala) */
    int getStatus() const;

    /**
     * @brief Zmienia priorytet pacjenta
     * @param nowyPriorytet Nowa wartość priorytetu
     */
    void setPriorytet(int nowyPriorytet);

    /**
     * @brief Zmienia miejsce pobytu (status) pacjenta
     * @param nowyStatus Nowa wartość statusu
     */
    void setStatus(int nowyStatus);

    /**
     * @brief Zwiększa czas oczekiwania pacjenta
     * @param minuty Liczba minut do dodania
     */
    void dodajCzasOczekiwania(int minuty);

    /**
     * @brief Przeciążony operator strumieniowy do wypisywania danych pacjenta
     */
    friend std::ostream& operator<<(std::ostream& os, const Pacjent& p);

    /**
     * @brief Porównuje dwóch pacjentów (sprawdza unikalność po PESEL)
     * @param other Inny pacjent do porównania
     * @return True, jeśli numery PESEL są identyczne
     */
    bool operator==(const Pacjent& other) const;

    /**
     * @brief Sprawdza, czy pacjenci są różni
     * @param other Inny pacjent do porównania
     * @return True, jeśli numery PESEL się różnią
     */
    bool operator!=(const Pacjent& other) const;
};

#endif