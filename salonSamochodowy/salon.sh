#!/bin/bash

dodaj_uzytkownika() {
    echo "Podaj ID uzytkownika:"
    read id_uzytkownika
    echo "Podaj imie i nazwisko:"
    read imie_nazwisko
    echo "$id_uzytkownika, $imie_nazwisko" >> uzytkownicy.txt
    echo "Dodano nowego uzytkownika: $imie_nazwisko"
}

dodaj_samochod() {
    echo "Podaj ID samochodu:"
    read id_samochodu
    echo "Podaj marke i model:"
    read marka_model
    echo "Podaj cene samochodu:"
    read cena_samochodu
    echo "$id_samochodu, $marka_model, $cena_samochodu" >> samochody.txt
    echo "Dodano nowy samochod: $marka_model"
}

usun_uzytkownika() {
    echo "Podaj ID uzytkownika do usuniecia:"
    read id_uzytkownika
    sed -i "/^$id_uzytkownika,/d" uzytkownicy.txt
    echo "Usunieto uzytkownika o ID: $id_uzytkownika"
}

usun_samochod() {
    echo "Podaj ID samochodu do usuniecia:"
    read id_samochodu
    sed -i "/^$id_samochodu,/d" samochody.txt
    echo "Usunieto samochod o ID: $id_samochodu"
}

sprzedaj_samochod() {
    echo "Podaj ID sprzedajacego samochodu:"
    read id_samochodu
    echo "Podaj ID kupujacego uzytkownika:"
    read id_kupujacego
    dane_kupujacego=$(grep "^$id_kupujacego," uzytkownicy.txt)
    dane_samochodu=$(grep "^$id_samochodu," samochody.txt)
    
    if [ -z "$dane_kupujacego" ]; then
        echo "Nie znaleziono uzytkownika o ID: $id_kupujacego"
    elif [ -z "$dane_samochodu" ]; then
        echo "Nie znaleziono samochodu o ID: $id_samochodu"
    else
        sed -i "/^$id_samochodu,/d" samochody.txt
        echo "$id_samochodu, $dane_samochodu, $id_kupujacego, $dane_kupujacego" >> sprzedane_samochody.txt
        echo "Sprzedano samochod o ID: $id_samochodu uzytkownikowi o ID: $id_kupujacego"
    fi
}

modyfikuj_uzytkownika() {
    echo "Podaj ID uzytkownika do modyfikacji:"
    read id_uzytkownika
    sed -i "/^$id_uzytkownika,/d" uzytkownicy.txt
    echo "Podaj nowe imie i nazwisko:"
    read imie_nazwisko
    echo "$id_uzytkownika, $imie_nazwisko" >> uzytkownicy.txt
    echo "Zaktualizowano dane uzytkownika o ID: $id_uzytkownika"
}

modyfikuj_samochod() {
    echo "Podaj ID samochodu do modyfikacji:"
    read id_samochodu
    sed -i "/^$id_samochodu,/d" samochody.txt
    echo "Podaj nowe dane samochodu (marka i model):"
    read marka_model
    echo "Podaj nowa cene samochodu: "
    read cena_samochodu
    echo "$id_samochodu, $marka_model, $cena_samochodu" >> samochody.txt
    echo "Zaktualizowano dane samochodu o ID: $id_samochodu"
}

lista_uzytkownikow() {
    echo "Wybierz atrybut sortowania (1 - ID, 2 - imie i nazwisko):"
    read wybor
    case $wybor in
        1) sort -t',' -k1,1 uzytkownicy.txt;;
        2) sort -t',' -k2,2 uzytkownicy.txt;;
        *) echo "Bledny wybor";;
    esac
}

lista_samochodow() {
    echo "Wybierz atrybut sortowania (1 - ID, 2 - marka i model, 3 - cena):"
    read wybor
    case $wybor in
        1) sort -t',' -k1,1 samochody.txt;;
        2) sort -t',' -k2,2 samochody.txt;;
        3) sort -t',' -k3,3n samochody.txt;;
        *) echo "Bledny wybor";;
    esac
}

#menu
while true; do
    echo "Menu glowne:"
    echo "1. Dodaj uzytkownika"
    echo "2. Dodaj samochod"
    echo "3. Usun uzytkownika"
    echo "4. Usun samochod"
    echo "5. Sprzedaj samochod"
    echo "6. Modyfikuj uzytkownika"
    echo "7. Modyfikuj samochod"
    echo "8. Lista uzytkownikow"
    echo "9. Lista samochodow"
    echo "10. Wyjscie"
    echo "Wybierz opcje:"
    read opcja
    case $opcja in
        1) dodaj_uzytkownika;;
        2) dodaj_samochod;;
        3) usun_uzytkownika;;
        4) usun_samochod;;
        5) sprzedaj_samochod;;
        6) modyfikuj_uzytkownika;;
        7) modyfikuj_samochod;;
        8) lista_uzytkownikow;;
        9) lista_samochodow;;
        10) exit 0;;
        *) echo "Bledna opcja, sprobuj ponownie.";;
    esac
done