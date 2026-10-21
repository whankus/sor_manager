#include "../include/Interface.h"
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

using namespace ftxui;

Interface::Interface(SorManager& m) : manager(m) {}

void Interface::uruchom() {
    auto screen = ScreenInteractive::Fullscreen();

    int wybrana_zakladka = 0;
    std::vector<std::string> opcje_menu = {
        "1. Rejestracja nowego pacjenta",
        "2. Wyswietlanie pacjentow (Filtry)",
        "3. Wyszukiwanie i Edycja",
        "4. Wypisz pacjenta (Usun)",
        "5. Statystyki SOR",
        "6. Zapisz baze (Binarnie)",
        "7. Wczytaj baze (Binarnie)",
        "8. Symuluj uplyw czasu (+15 min)",
        "0. Wyjdz z programu"
    };
    auto menu_glowne = Menu(&opcje_menu, &wybrana_zakladka);

    std::string in_imie, in_nazwisko, in_pesel, in_wiek, in_priorytet;
    std::string komunikat_rejestracji = "";

    auto inp_imie = Input(&in_imie, "Podaj imie...");
    auto inp_nazw = Input(&in_nazwisko, "Podaj nazwisko...");
    auto inp_pesel = Input(&in_pesel, "Podaj PESEL (11 cyfr)...");
    auto inp_wiek = Input(&in_wiek, "Podaj wiek...");
    auto inp_prior = Input(&in_priorytet, "Priorytet (1-Czerwony, 2-Zolty, 3-Zielony)...");

    auto btn_dodaj = Button("DODAJ DO KOLEJKI (Wcisnij ENTER na tym przycisku)", [&] {
        if (in_imie.empty() || in_nazwisko.empty() || in_pesel.empty() || in_wiek.empty() || in_priorytet.empty()) {
            komunikat_rejestracji = "BLAD: Wypelnij wszystkie pola!"; return;
        }
        auto tylkoLitery = [](const std::string& s) { return std::all_of(s.begin(), s.end(), [](unsigned char c) { return std::isalpha(c); }); };
        if (!tylkoLitery(in_imie) || !tylkoLitery(in_nazwisko)) {
            komunikat_rejestracji = "BLAD: Imie i nazwisko tylko z liter!"; return;
        }
        auto tylkoCyfry = [](const std::string& s) { return std::all_of(s.begin(), s.end(), [](unsigned char c) { return std::isdigit(c); }); };
        if (in_pesel.length() != 11 || !tylkoCyfry(in_pesel)) {
            komunikat_rejestracji = "BLAD: PESEL musi miec 11 cyfr!"; return;
        }
        if (manager.znajdzPoPeselu(in_pesel) != nullptr) {
            komunikat_rejestracji = "BLAD: Pacjent o tym PESEL juz istnieje!"; return;
        }
        if (!tylkoCyfry(in_wiek)) { komunikat_rejestracji = "BLAD: Wiek musi byc liczba!"; return; }

        int wiek_int = 0;
        try { wiek_int = std::stoi(in_wiek); } catch (...) { komunikat_rejestracji = "BLAD: Zly wiek!"; return; }
        if (wiek_int <= 0) { komunikat_rejestracji = "BLAD: Wiek musi byc > 0!"; return; }

        if (in_priorytet != "1" && in_priorytet != "2" && in_priorytet != "3") {
            komunikat_rejestracji = "BLAD: Priorytet to 1, 2 lub 3!"; return;
        }

        int prior_int = std::stoi(in_priorytet);
        manager.dodajPacjenta(Pacjent(in_imie, in_nazwisko, in_pesel, wiek_int, 0, prior_int));
        komunikat_rejestracji = "SUKCES: Dodano pacjenta!";
        in_imie = ""; in_nazwisko = ""; in_pesel = ""; in_wiek = ""; in_priorytet = "";
    });
    auto zakladka_rejestracja = Container::Vertical({inp_imie, inp_nazw, inp_pesel, inp_wiek, inp_prior, btn_dodaj});

    int tryb_wyswietlania = 0;
    std::vector<std::string> opcje_trybow = {"1. Wszyscy (Pelne dane)", "2. Wszyscy (Skrocona lista)", "3. Konkretny indeks", "4. Zakres (od-do)"};
    auto radio_tryby = Radiobox(&opcje_trybow, &tryb_wyswietlania);

    std::string in_idx, in_od, in_do;
    auto inp_idx = Input(&in_idx, "Podaj indeks do wyswietlenia...");
    auto inp_od = Input(&in_od, "Indeks poczatkowy (OD)...");
    auto inp_do = Input(&in_do, "Indeks koncowy (DO)...");
    auto kontener_wyswietlania = Container::Vertical({radio_tryby, inp_idx, inp_od, inp_do});

    auto zakladka_lista = Renderer(kontener_wyswietlania, [&] {
        SmartArray<Pacjent>& baza = manager.getBaza();
        Elements ui_elementy;
        ui_elementy.push_back(text("WYBIERZ TRYB WYSWIETLANIA (Strzalki):") | bold);
        ui_elementy.push_back(radio_tryby->Render());
        ui_elementy.push_back(separator());

        if (tryb_wyswietlania == 2) ui_elementy.push_back(inp_idx->Render());
        if (tryb_wyswietlania == 3) { ui_elementy.push_back(inp_od->Render()); ui_elementy.push_back(inp_do->Render()); }

        ui_elementy.push_back(separator());

        if (baza.isEmpty()) {
            ui_elementy.push_back(text("Brak pacjentow na oddziale.") | color(Color::Red));
        } else {
            int start = 0, end = baza.size() - 1;
            bool ok = true;

            if (tryb_wyswietlania == 2) {
                try { start = std::stoi(in_idx); end = start; } catch(...) { ok = false; ui_elementy.push_back(text("Podaj poprawny indeks.")); }
            } else if (tryb_wyswietlania == 3) {
                try { start = std::stoi(in_od); end = std::stoi(in_do); } catch(...) { ok = false; ui_elementy.push_back(text("Podaj poprawne indeksy.")); }
            }

            if (ok) {
                for (int i = start; i <= end; ++i) {
                    if (i < 0 || i >= baza.size()) continue;
                    std::string status = (baza[i].getStatus() == 0) ? "Czeka" : (baza[i].getStatus() == 1 ? "Gabinet" : "Sala Operacyjna");
                    std::string dane;

                    if (tryb_wyswietlania == 1) {
                        dane = "[" + std::to_string(i) + "] " + baza[i].getImie() + " " + baza[i].getNazwisko() + " (" + status + ")";
                    } else { // PELNE DANE
                        dane = "[" + std::to_string(i) + "] " + baza[i].getImie() + " " + baza[i].getNazwisko() +
                               " | PESEL: " + baza[i].getPesel() + " | Wiek: " + std::to_string(baza[i].getWiek()) +
                               " | Status: " + status + " | Czas: " + std::to_string(baza[i].getCzasOczekiwania()) + " min";
                    }
                    auto kolor = (baza[i].getPriorytet() == 1) ? Color::Red : ((baza[i].getPriorytet() == 2) ? Color::Yellow : Color::Green);
                    ui_elementy.push_back(text(dane) | color(kolor));
                }
            }
        }
        return window(text("WYNIKI WYSWIETLANIA"), vbox(ui_elementy) | vscroll_indicator | frame);
    });

    int tryb_szukania = 0;
    std::vector<std::string> opcje_szukania = {"Szukaj po Indeksie", "Szukaj po Nazwisku"};
    auto radio_szukaj = Radiobox(&opcje_szukania, &tryb_szukania);

    std::string szukaj_fraza, edytuj_status, edytuj_prior;
    std::string komunikat_edycji = "";
    Pacjent* edytowany_pacjent = nullptr;

    auto inp_szukaj = Input(&szukaj_fraza, "Wpisz indeks lub nazwisko tutaj");
    auto btn_szukaj = Button("SZUKAJ", [&] {
        edytowany_pacjent = nullptr;
        SmartArray<Pacjent>& baza = manager.getBaza();

        if (tryb_szukania == 0) {
            try {
                int idx = std::stoi(szukaj_fraza);
                if (idx >= 0 && idx < baza.size()) edytowany_pacjent = &baza[idx];
            } catch (...) {}

            if (edytowany_pacjent) komunikat_edycji = "Znaleziono: " + edytowany_pacjent->getImie() + " " + edytowany_pacjent->getNazwisko();
            else komunikat_edycji = "BLAD: Nie znaleziono pacjenta pod tym indeksem.";

        } else {
            int znaleziono_ile = 0;
            for (unsigned i = 0; i < baza.size(); ++i) {
                if (baza[i].getNazwisko() == szukaj_fraza) {
                    if (znaleziono_ile == 0) edytowany_pacjent = &baza[i];
                    znaleziono_ile++;
                }
            }

            if (znaleziono_ile == 1) {
                komunikat_edycji = "Znaleziono: " + edytowany_pacjent->getImie() + " " + edytowany_pacjent->getNazwisko();
            } else if (znaleziono_ile > 1) {
                edytowany_pacjent = nullptr;
                komunikat_edycji = "UWAGA: Znaleziono " + std::to_string(znaleziono_ile) + " osob o tym nazwisku! Uzyj szukania po indeksie.";
            } else {
                komunikat_edycji = "BLAD: Nie znaleziono nikogo o takim nazwisku.";
            }
        }
    });
    auto inp_status = Input(&edytuj_status, "Nowy status (0-Czeka, 1-Gabinet, 2-Sala operacyjna)");
    auto inp_eprior = Input(&edytuj_prior, "Nowy priorytet (1-Czerwony, 2-Zolty, 3-Zielony)");
    auto btn_zapisz_edycje = Button("ZAPISZ ZMIANY", [&] {
        if (!edytowany_pacjent) { komunikat_edycji = "Najpierw wyszukaj pacjenta!"; return; }
        try {
            if (!edytuj_status.empty()) {
                int s = std::stoi(edytuj_status);
                if (s >= 0 && s <= 2) edytowany_pacjent->setStatus(s);
            }
            if (!edytuj_prior.empty()) {
                int p = std::stoi(edytuj_prior);
                if (p >= 1 && p <= 3) edytowany_pacjent->setPriorytet(p);
            }
            komunikat_edycji = "SUKCES: Zaktualizowano dane pacjenta!";
            edytuj_status = ""; edytuj_prior = "";
        } catch (...) { komunikat_edycji = "BLAD: Podano nieprawidlowe wartosci!"; }
    });
    auto zakladka_edycja = Container::Vertical({radio_szukaj, inp_szukaj, btn_szukaj, inp_status, inp_eprior, btn_zapisz_edycje});


    std::string usun_idx;
    std::string komunikat_usun = "";
    auto inp_usun = Input(&usun_idx, "Podaj indeks pacjenta do usuniecia...");
    auto btn_usun = Button("USUN PACJENTA", [&] {
        try {
            int idx = std::stoi(usun_idx);
            manager.getBaza().erase(idx);
            komunikat_usun = "SUKCES: Usunieto pacjenta z oddzialu!";
            usun_idx = "";
        } catch (...) { komunikat_usun = "BLAD: Nie ma pacjenta pod takim indeksem lub zly format!"; }
    });
    auto zakladka_usun = Container::Vertical({inp_usun, btn_usun});

    auto zakladka_statystyki = Renderer([&] {
        SmartArray<Pacjent>& baza = manager.getBaza();

        double suma_cz_all = 0; int ile_cz_all = 0;
        double suma_cz_50 = 0; int ile_cz_50 = 0;
        double suma_ziel = 0; int ile_ziel = 0;

        for(unsigned i = 0; i < baza.size(); ++i) {
            if (baza[i].getPriorytet() == 1) { suma_cz_all += baza[i].getCzasOczekiwania(); ile_cz_all++; }
            if (baza[i].getPriorytet() == 1 && baza[i].getWiek() > 50) { suma_cz_50 += baza[i].getCzasOczekiwania(); ile_cz_50++; }
            if (baza[i].getPriorytet() == 3) { suma_ziel += baza[i].getCzasOczekiwania(); ile_ziel++; }
        }

        std::string avg_cz_all = (ile_cz_all > 0) ? std::to_string(suma_cz_all / ile_cz_all) + " min" : "Brak pacjentow w tej grupie";
        std::string avg_cz_50 = (ile_cz_50 > 0) ? std::to_string(suma_cz_50 / ile_cz_50) + " min" : "Brak pacjentow w tej grupie";
        std::string avg_ziel = (ile_ziel > 0) ? std::to_string(suma_ziel / ile_ziel) + " min" : "Brak pacjentow w tej grupie";

        return window(text("Szczegolowe Statystyki Oddzialu"), vbox({
            text("Calkowita liczba pacjentow: " + std::to_string(baza.size())),
            text("Sredni wiek wszystkich pacjentow: " + std::to_string(manager.obliczSredniWiek()) + " lat"),
            text("Oczekujacy na przyjecie (Status 0): " + std::to_string(manager.policzPacjentowZeStatusem(0))),
            separator(),
            text("Sredni czas oczekiwania (Tylko Czerwoni): " + avg_cz_all) | color(Color::Red),
            text("Sredni czas oczekiwania (Czerwoni pow. 50 lat): " + avg_cz_50) | color(Color::LightCoral),
            text("Sredni czas oczekiwania (Zieloni): " + avg_ziel) | color(Color::Green)
        }));
    });

    std::string komunikat_zapis = "";
    auto btn_zapisz = Button("ZAPISZ DO PLIKU BINARNEGO", [&] { manager.zapiszDoPlikuBinarnego("baza_sor.bin"); komunikat_zapis = "SUKCES: Zapisano!"; });
    auto zakladka_zapisz = Container::Vertical({btn_zapisz});

    std::string komunikat_wczytaj = "";
    auto btn_wczytaj = Button("WCZYTAJ Z PLIKU BINARNEGO", [&] {
        try {
            manager.wczytajZPlikuBinarnego("baza_sor.bin");
            komunikat_wczytaj = "SUKCES: Wczytano!";
        } catch (const std::runtime_error&) {
            komunikat_wczytaj = "BLAD: Brak bazy do wczytania (plik nie istnieje)!";
        } catch (...) {
            komunikat_wczytaj = "BLAD: Nieznany blad odczytu!";
        }
    });
    auto zakladka_wczytaj = Container::Vertical({btn_wczytaj});

    std::string komunikat_czas = "";
    auto btn_czas = Button("DODAJ 15 MINUT DO POCZEKALNI", [&] { manager.symulujUplywCzasu(15); komunikat_czas = "SUKCES: Zaktualizowano czasy!"; });
    auto zakladka_symulacja = Container::Vertical({btn_czas});

    auto btn_wyjscie = Button("ZAKONCZ PROGRAM", [&] { screen.ExitLoopClosure()(); });
    auto zakladka_wyjscie = Container::Vertical({btn_wyjscie});
    auto kontener_glowny = Container::Tab({
        zakladka_rejestracja, zakladka_lista, zakladka_edycja, zakladka_usun,
        zakladka_statystyki, zakladka_zapisz, zakladka_wczytaj, zakladka_symulacja, zakladka_wyjscie
    }, &wybrana_zakladka);

    auto render_okna = Renderer(Container::Horizontal({menu_glowne, kontener_glowny}), [&] {
        Element aktywny_widok;
        if (wybrana_zakladka == 0) aktywny_widok = vbox({text("Uzyj TAB lub strzalek (Gora/Dol), aby przechodzic miedzy polami") | color(Color::Yellow), separator(), zakladka_rejestracja->Render(), text(komunikat_rejestracji) | color(Color::Cyan)});
        else if (wybrana_zakladka == 1) aktywny_widok = zakladka_lista->Render();
        else if (wybrana_zakladka == 2) aktywny_widok = vbox({zakladka_edycja->Render(), separator(), text(komunikat_edycji) | color(Color::Yellow)});
        else if (wybrana_zakladka == 3) aktywny_widok = vbox({zakladka_usun->Render(), separator(), text(komunikat_usun) | color(Color::Red)});
        else if (wybrana_zakladka == 4) aktywny_widok = zakladka_statystyki->Render();
        else if (wybrana_zakladka == 5) aktywny_widok = vbox({zakladka_zapisz->Render(), separator(), text(komunikat_zapis) | color(Color::Green)});
        else if (wybrana_zakladka == 6) aktywny_widok = vbox({zakladka_wczytaj->Render(), separator(), text(komunikat_wczytaj) | color(komunikat_wczytaj.find("BLAD") != std::string::npos ? Color::Red : Color::Green)});        else if (wybrana_zakladka == 7) aktywny_widok = vbox({zakladka_symulacja->Render(), separator(), text(komunikat_czas) | color(Color::Green)});
        else if (wybrana_zakladka == 8) aktywny_widok = zakladka_wyjscie->Render();

        return border(vbox({
            text(" SYSTEM ZARZADZANIA S.O.R. ") | bold | center, separator(),
            hbox({ window(text("Menu Główne"), menu_glowne->Render()) | flex, window(text("Panel Akcji"), aktywny_widok) | flex })
        }));
    });

    screen.Loop(render_okna);
}