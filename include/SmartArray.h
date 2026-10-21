/**
 * @file SmartArray.h
 * @brief Szablon inteligentnej tablicy.
 * @author Wiktor Hankus (158159)
 */
#ifndef SMARTARRAY_H
#define SMARTARRAY_H
#include <stdexcept>
#include <iostream>

template <typename T>
class SmartArray {
private:
    T* data; //wskaznik na 1 element pamieci zaalokowanej na starcie
    unsigned int currentSize; // aktualny  liczba elementow w tablicy
    unsigned int currentCapacity; //rozmiar zarezerwowanej tablicy  - max liczba elementow bez realokacji

    /**
     * @brief Metoda pomocnicza ułatwiająca automatyczne powiększanie pojemności.
     */
    void autoReserve() {
        if (currentSize == currentCapacity) {
            reserve(currentCapacity == 0 ? 1 : currentCapacity * 2); //  pamiec na 1 element (jezeli tab pusta) || podwaja pojemnosc
        }
    }

public:


    /**
     * @brief Konstruktor domyślny. Tworzy pustą tablicę.
     */
    SmartArray() : data(nullptr), currentSize(0), currentCapacity(0) {}

    /**
     * @brief Konstruktor z prealokacją pamięci.
     * @param size Początkowa pojemność tablicy (rozmiar logiczny pozostaje 0).
     */
    SmartArray(unsigned size) : currentSize(0), currentCapacity(size) {
        data = (size > 0) ? new T[size] : nullptr; //unikniecie alokacji tablicy o zerowym rozmiarze
    }

    /**
     * @brief Destruktor. Zwalnia dynamicznie zaalokowaną pamięć.
     */
    ~SmartArray() {
        delete[] data; //niszczy cala tablice
    }

    /**
     * @brief Konstruktor kopiujący (Głęboka kopia).
     * @param other Obiekt tablicy, z którego mają zostać skopiowane dane.
     */
    SmartArray(const SmartArray& other) : currentSize(other.currentSize), currentCapacity(other.currentCapacity) {
        data = (currentCapacity > 0) ? new T[currentCapacity] : nullptr;
        for (unsigned i = 0; i < currentSize; ++i) {
            data[i] = other.data[i]; //przepisywanie wartosci do nowej tablicy
        }
    }

    /**
     * @brief Operator przypisania (Głęboka kopia).
     * @param other Tablica po prawej stronie znaku równości.
     * @return Referencja do zaktualizowanego obiektu (dla łańcuchowania operacji).
     */
    SmartArray& operator=(const SmartArray& other) {
        //zabezpieczenie przed samoprzypisaniem
        if (this != &other) {
            delete[] data;
            currentSize = other.currentSize;
            currentCapacity = other.currentCapacity;
            data = (currentCapacity > 0) ? new T[currentCapacity] : nullptr; //alokacja nowego bufora pamieci
            for (unsigned i = 0; i < currentSize; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    /**
     * @brief Zwraca referencję do elementu z kontrolą zakresu.
     * @param index Indeks elementu do pobrania.
     * @return Referencja do żądanego elementu.
     * @throws std::out_of_range Jeśli indeks wykracza poza rozmiar tablicy.
     */
    T& at(unsigned index) {
        if (index >= currentSize) throw std::out_of_range("Index out of range"); //indeks poza rozmiarem -> przerwanie programu
        return data[index]; //nadpisanie elementu
    }
//dwie wersje, ponieważ klasa musi umieć zachować się poprawnie w dwóch różnych kontekstach. Pierwsza wersja służy do modyfikacji elementów, a druga (const) do bezpiecznego odczytu elementów z obiektów, które zostały przekazane jako stałe.
    /**
     * @brief Stała wersja metody at() dla obiektów const.
     * @param index Indeks elementu do pobrania.
     * @return Stała referencja do elementu.
     * @throws std::out_of_range Jeśli indeks wykracza poza rozmiar tablicy.
     */
    const T& at(unsigned index) const {
        if (index >= currentSize) throw std::out_of_range("Index out of range");
        return data[index];
    }

    /**
     * @brief Operator dostępu bez kontroli zakresu (dla maksymalnej wydajności).
     * @param index Indeks elementu do pobrania.
     * @return Referencja do żądanego elementu.
     */
    T& operator[](unsigned index) {
        return data[index];
    }

    /**
     * @brief Stała wersja operatora dostępu bez kontroli zakresu.
     * @param index Indeks elementu.
     * @return Stała referencja.
     */
    const T& operator[](unsigned index) const {
        return data[index];
    }

    /**
     * @brief Sprawdza, czy tablica jest pusta.
     * @return True, jeśli nie zawiera elementów, False w przeciwnym wypadku.
     */
    bool isEmpty() const {
        return currentSize == 0;
    }

    /**
     * @brief Pobiera aktualną liczbę elementów w tablicy.
     * @return Liczba dodanych elementów (rozmiar logiczny).
     */
    unsigned size() const {
        return currentSize;
    }

    /**
     * @brief Pobiera fizyczną ilość zaalokowanego miejsca.
     * @return Maksymalna ilość elementów bez konieczności realokacji.
     */
    unsigned capacity() const {
        return currentCapacity;
    }

    /**
     * @brief Wymusza rezerwację podanej ilości pamięci.
     * @param size Nowa pojemność (capacity). Jeśli mniejsza od obecnej, nic się nie dzieje.
     */
    void reserve(unsigned size) {
        if (size > currentCapacity) {
            T* newData = new T[size]; //alokaja nowej tablicy
            for (unsigned i = 0; i < currentSize; ++i) {
                newData[i] = data[i]; //przepisanie elementow ze starej tablicy
            }
            delete[] data; //usuwanie starje tablicy
            data = newData; //przpisanie wskaznika
            currentCapacity = size; //aktualny rozmiar tablicy
        }
    }

    /**
     * @brief Zmienia logiczny rozmiar tablicy, ewentualnie dodając puste elementy.
     * @param size Nowy rozmiar tablicy.
     */
    void resize(unsigned size) {
        if (size > currentCapacity) reserve(size); //zwiekszenie pamieci jesli potrzeba
        if (size > currentSize) {
            for (unsigned i = currentSize; i < size; ++i) {
                data[i] = T(); // czyszczenie nowo dodanych miejsc w tablicy
            }
        }
        currentSize = size;
    }

    /**
     * @brief Zmienia logiczny rozmiar tablicy, wypełniając braki nowym elementem.
     * @param size Nowy rozmiar tablicy.
     * @param elem Wartość, którą zostaną uzupełnione nowe komórki.
     */
    void resize(unsigned size, const T& elem) {
        if (size > currentCapacity) reserve(size);
        if (size > currentSize) {
            for (unsigned i = currentSize; i < size; ++i) {
                data[i] = elem; //uzupelnianie nowych miejsc elementami
            }
        }
        currentSize = size;
    }

    /**
     * @brief Czyści tablicę (rozmiar = 0), ale nie zwalnia pamięci.
     */
    void clear() {
        currentSize = 0;
    }

    /**
     * @brief Wstawia nowy element w dowolne miejsce, przesuwając resztę w prawo.
     * @param index Indeks docelowy elementu.
     * @param elem Nowa wartość do dodania.
     * @throws std::out_of_range Jeśli indeks jest większy niż obecny rozmiar.
     */
    void insert(unsigned index, const T& elem) {
        if (index > currentSize) throw std::out_of_range("Index out of range");
        autoReserve(); //czy jest miejsce na nowy element

        for (unsigned i = currentSize; i > index; --i) {
            data[i] = data[i - 1];//iteracja od tylu, wstawiamy elemnt a reszte przesuwamy w prawo
        }
        data[index] = elem;
        currentSize++;
    }

    /**
     * @brief Usuwa element pod danym indeksem i scala lukę.
     * @param index Indeks elementu do usunięcia.
     * @throws std::out_of_range Jeśli podano nieprawidłowy indeks.
     */
    void erase(unsigned index) {
        if (index >= currentSize) throw std::out_of_range("Index out of range");
        for (unsigned i = index; i < currentSize - 1; ++i) {//iteracja od przodu usuwa element i nadpisujac go elementem i+1
            data[i] = data[i + 1];
        }
        currentSize--;
    }

    /**
     * @brief Usuwa partię elementów.
     * @param index Indeks początkowy.
     * @param amount Ilość elementów do usunięcia.
     * @throws std::out_of_range Jeśli zakres wykracza poza tablicę.
     */
    void erase(unsigned index, unsigned amount) {
        if (index >= currentSize || amount == 0) return;
        if (index + amount > currentSize) throw std::out_of_range("Amount out of range");

        for (unsigned i = index; i < currentSize - amount; ++i) {//usuniecie kilku elementow na raz
            data[i] = data[i + amount];
        }
        currentSize -= amount;
    }

    /**
     * @brief Najszybsza metoda dodawania elementu na sam koniec tablicy.
     * @param elem Element do dodania.
     */
    void pushBack(const T& elem) {
        autoReserve();
        data[currentSize++] = elem;//dodajemy nowy element na samym koncu
    }

    /**
     * @brief Usuwa ostatni element tablicy (o ile nie jest pusta).
     */
    void popBack() {
        if (currentSize > 0) {
            currentSize--;//zmniejszamy rozmiar - usuwamy ostatni element
        }
    }



    /**
     * @brief Konstruktor wypełniający - tworzy tablicę z N kopiami elementu.
     * @param size Liczba generowanych elementów.
     * @param elem Wartość, z której powstaną kopie.
     */
    //na potrzeby wymagan
    SmartArray(unsigned size, const T& elem) : currentSize(size), currentCapacity(size) {
        data = (size > 0) ? new T[size] : nullptr;
        for (unsigned i = 0; i < size; ++i) {
            data[i] = elem;
        }
    }

    /**
     * @brief Operator porównania. Weryfikuje strukturę i wartości element po elemencie.
     * @param array Tablica do porównania.
     * @return True, jeśli tablice są identyczne.
     */
    bool operator==(const SmartArray& array) const {
        if (currentSize != array.currentSize) return false;
        for (unsigned i = 0; i < currentSize; ++i) {
            if (data[i] != array.data[i]) return false;
        }
        return true;
    }



    /**
     * @brief Odzyskuje pamięć przez zmniejszenie capacity() do size().
     */
    void shrinkToFit() {
        if (currentCapacity > currentSize) { //tylko jezeli nadmiarowy bufor
            if (currentSize == 0) {
                delete[] data;
                data = nullptr;
                currentCapacity = 0;
            } else {
                T* newData = new T[currentSize];//nowy bufor dopasowany do logicznego rozmiaru
                for (unsigned i = 0; i < currentSize; ++i) {
                    newData[i] = data[i];
                }
                delete[] data;//pozbywamy sie starego bufora
                data = newData;
                currentCapacity = currentSize;
            }
        }
    }

    /**
     * @brief Wypycha element na sam początek tablicy (indeks 0).
     * @param elem Element do dodania.
     */
    void pushFront(const T& elem) {
        insert(0, elem); //nowy pacjent z prioytetem 1
    }

    /**
     * @brief Ściąga (usuwa) element z samego początku tablicy.
     */
    void popFront() {
        if (currentSize > 0) {
            erase(0);//pacjent zostaje wypisany
        }
    }

    /**
     * @brief Wypisywanie do strumienia (operator zaprzyjaźniony). Formatuje całą zawartość do std::cout.
     */
    friend std::ostream& operator<<(std::ostream& os, const SmartArray<T>& array) {
        os << "[";
        for (unsigned i = 0; i < array.currentSize; ++i) {
            os << array.data[i];
            if (i < array.currentSize - 1) os << ", ";
        }
        os << "]";
        return os;
    }
};

#endif