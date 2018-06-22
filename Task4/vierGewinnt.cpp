#include <cmath>
#include <iostream>
#include <random>
#include "unit.h"
#include "Spielbrett.h"
#include <algorithm>

void NetzwerkMain();


const unsigned int Schwierigkeitsgrad = 6;


void outputGameField(Spielbrett gameField){
    for(int i = 0; i < 6; i++){
        for(int j = 0; j < 7; j++){
            std::cout << gameField(i, j) << " ";
        }
        std::cout << std::endl;
    }
}

template<typename T>
void outputVector(std::vector<T> vec){
    for(int i = 0; i < vec.size(); i++){
        std::cout << vec[i] << ((i < vec.size() - 1) ? ", " : "\n");
    }
}

int rows = 7;
int DISCARD_ROW = -42; //Falls eine Spalte keinen gueltigen Zug darstellt, wird ihr dieser Wert zugeordnet

// max color is the color of the player MAX, the player we want to win
double miniMax2(Spielbrett gameField, Feld maxColor, int currentColor, std::vector<int>& bestMoves, int layer, int searchDepth){
    double heuristicValue = gameField.heuristischeBewertung(maxColor);
    if(heuristicValue == -1 || heuristicValue == 1 || layer == searchDepth){
        return heuristicValue;
    }
    std::vector<double> miniMaxValues;
    for(int i = 0; i < rows; i++){
        if(gameField.addTile(i, currentColor)){
            double thisMove = miniMax2(gameField, maxColor, 1 - currentColor, bestMoves, layer + 1, searchDepth);
            miniMaxValues.push_back(thisMove);
            gameField.entferneStein(i);
        }
        else {
            miniMaxValues.push_back(DISCARD_ROW);
        }
    }
    
    double miniMaxValue = 0;
    std::vector<double> validMiniMaxValues;
    for(unsigned int i = 0; i < miniMaxValues.size(); i++){
        if(miniMaxValues[i] != DISCARD_ROW){
            validMiniMaxValues.push_back(miniMaxValues[i]); //Sammle alle minimax ergebnisse der gueltigen Zuege
        }
    }
    
    if(layer % 2 == 0){
        miniMaxValue = *std::max_element(validMiniMaxValues.begin(), validMiniMaxValues.end()); //max Wert
    } else {
        miniMaxValue = *std::min_element(validMiniMaxValues.begin(), validMiniMaxValues.end()); //min Wer
    }
    
    if(layer == 0){
        for(unsigned int i = 0; i < miniMaxValues.size(); i++){
            if(miniMaxValues[i] == miniMaxValue && !gameField.isColFull(i)){
                bestMoves.push_back(i); // Sammle alle gueltigen Spalten, die zu extremaler Bewertung fuehren
            }
        }
    }

    return miniMaxValue;
}

int errechneBestenZug(Spielbrett brett, Feld farbe) {
    std::vector<int> bestMoves;
    miniMax2(brett, farbe, brett.getColorId(farbe), bestMoves, 0, 5);
    
    //outputVector(bestMoves);
    //outputGameField(brett);
    //std::getchar();
    
    if (bestMoves.size() == 0) {
        std::cout << "ERROR: Ich habe keine Zuege mehr!" << std::endl;
    }
    

    return bestMoves[rand() % bestMoves.size()];
}


void startOfflineGame(){
    int nextMove, Gegenzug;
    Feld meineFarbe, gegnerFarbe;

    Start(Schwierigkeitsgrad);

    for(unsigned int Spiel = 1; Spiel <= AnzahlSpiele; Spiel++){
        Spielbrett brett = Spielbrett(6,7);


        if (Spiel % 2 == 1) { //Wir fangen an mit gelb
            meineFarbe = gelb;
            gegnerFarbe = rot;

            nextMove = errechneBestenZug(brett, meineFarbe);
            brett.setzeStein(nextMove, meineFarbe);
        } else { //Das Programm der Umgebung unit.o beginnt mit gelb
            meineFarbe = rot;
            gegnerFarbe = gelb;
            nextMove = -1;
        }

        Gegenzug = NaechsterZug(nextMove);

        while (Gegenzug >= 0) { //Falls Spiel vorbei, wird kleiner Null zurueckgegeben

            brett.setzeStein(Gegenzug, gegnerFarbe);
            //std::getchar();

            //nextMove = besterZug in aktuellem Spielfeld
            nextMove = errechneBestenZug(brett, meineFarbe);
            brett.setzeStein(nextMove, meineFarbe);

            Gegenzug = NaechsterZug(nextMove);

        }
    }
}

int main(){
    
    
    // Einleseschleife für Befehle. Terminiert, wenn ein gültiger Befehl gewählt wurde.
    int command;
    while (true) {
        std::cout << "\n";
        std::cout << "1 = EINZELSPIELER" << std::endl;
        std::cout << "2 = MEHRSPIELER" << std::endl;

        std::cin >> command;
        if (!std::cin) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        } else if (command >= 1 && command <= 2) {
            break;
        }
    }

    if (command == 1) { //Einzelspieler
        startOfflineGame();
    }
    
    if (command == 2) { //Mehrspieler
        NetzwerkMain();
    }
    
    
    
    
    return 0;
 
}

enum class SpielStatus {
    Verbindungsfehler,
    Niederlage,
    Unentschieden,
    Sieg,
    Running
};



// Spielt ein einzelnes Netzwerkspiel gegen einen anderen Computer im Netzwerk.
// Sollte das Spiel beendet sein oder ein Netzwerkfehler auftreten, muss diese Methode
// das zugehoerige Element der Enumeration SpielStatus zurueckgeben.
SpielStatus Netzwerkspiel( Feld color) {
    Feld gegenfarbe  = color == rot ? gelb : rot;

    Spielbrett brett = Spielbrett(6,7);   
    std::cout << "Du spielst mit " << brett.getColorId(color) << " (gelb = 0, rot = 1)" << std::endl;
 
    int nextMove;
        
    
    if (color == gelb) { //Wir fangen an mit gelb
        std::getchar();
        nextMove = errechneBestenZug(brett, color); 
        brett.setzeStein(nextMove, color);
        SendeZug(nextMove);
    }
    
    int Gegenzug = EmpfangeZug();
    
    while (Gegenzug >= 0 && Gegenzug <= 6) { //Falls Spiel vorbei, wird kleiner Null zurueckgegeben
        
        brett.setzeStein(Gegenzug, gegenfarbe);

        if (brett.spielIstBeendet()) {
            SendeZug(SPIELENDE);
            Gegenzug = SPIELENDE; // Derjenige, der den letzten Stein legt, weiss nun auch bescheid, dass das Spiel vorbei ist am Ende der Schleife
            break; 
        } else {
            std::getchar();
            nextMove = errechneBestenZug(brett, color); 
            brett.setzeStein(nextMove, color);
            SendeZug(nextMove);
        }

        Gegenzug = EmpfangeZug();
    }
    
    if (Gegenzug == SPIELENDE) {
        
        if (!brett.spielIstBeendet()) { //Das Spiel wurde wahrscheinlich vom anderen Spieler abgebrochen
            std::cout << "Das Spiel wurde von deinem Mitspieler abgebrochen" << std::endl;
            return SpielStatus::Verbindungsfehler;
        }
        
        if ( brett.hatGewonnen(color) ) {
            return SpielStatus::Sieg;
        } else {
            return SpielStatus::Niederlage;
        }
    } 
    
    return SpielStatus::Verbindungsfehler;
}

void NetzwerkMain() {
    int command;

    // Einleseschleife für Befehle. Terminiert, wenn ein gültiger Befehl gewählt wurde.
    while (true) {
        std::cout << "\n";
        std::cout << "1 = VERBINDE  mit einem anderen Spieler\n";
        std::cout << "2 = WARTE     auf einen anderen Spieler" << std::endl;

        std::cin >> command;
        if (!std::cin) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        } else if (command >= 1 && command <= 2) {
            break;
        }
    }

    if (command == 0) {
        return;
    }

    Feld MeineFarbe=  gelb,
         GegnerFarbe= rot;

    if (command == 1) {
        std::string ip;
        unsigned short port = 0;

        std::cout << "Bitte geben Sie die IP ein, mit der Sie sich verbinden wollen. \n";
        std::cout << "Format: xxx.xxx.xxx.xxx" << std::endl;
        std::cin >> ip;
        std::cout << "Bitte geben Sie den Port ein, mit dem Sie sich verbinden wollen. \n";
        std::cin >> port;

        if (!Verbinde(ip.c_str(), port)) {
            std::cout << "Verbindung fehlgeschlagen." << std::endl;
            return;
        }

        std::cout << "Verbindung hergestellt." << std::endl;

        // Ich verbinde mich -> Meine Farbe ist rot
        std::swap(MeineFarbe, GegnerFarbe);
    } else {
        unsigned short port = 0;

        std::cout << "Bitte geben Sie den Port ein, mit dem sich Ihr Mitspieler verbinden soll. \n";
        std::cin >> port;
        std::cout << "Warte auf Gegner..." << std::endl;

        if (!WarteAufVerbindung(port)) {
            std::cout << "Kein Gegner gefunden.";
            return;
        }
        std::cout << "Verbindung hergestellt." << std::endl;

        // Ich warte auf Spieler -> meine Farbe ist gelb
    }

    bool nochEinSpiel = true;
    while (nochEinSpiel) {
        auto ergebnis = Netzwerkspiel( MeineFarbe );

        if (ergebnis == SpielStatus::Verbindungsfehler) {
            nochEinSpiel = false;
            std::cout << "Verbindungsfehler!" << std::endl;
        } else {
            switch(ergebnis) {
                case SpielStatus::Niederlage:
                    std::cout << "Sie haben verloren!" << std::endl;
                    break;
                case SpielStatus::Unentschieden:
                    std::cout << "Es gab ein Unentschieden!" << std::endl;
                    break;
                case SpielStatus::Sieg:
                    std::cout << "Sie haben gewonnen!" << std::endl;
                    break;
                default:
                    std::cout << "Verbindungsfehler!" << std::endl;
                    SchliesseVerbindung();
                    return;
            }

            std::cout << "Nochmal spielen? [y/n]" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
            char antwort;
            std::cin >> antwort;
            nochEinSpiel = ! (!std::cin || (antwort != 'Y' && antwort != 'y') );

            if (nochEinSpiel) {
                std::swap( MeineFarbe, GegnerFarbe );
                NaechstesNetzwerkspiel();
            }
        }
    }

    SchliesseVerbindung();
}
