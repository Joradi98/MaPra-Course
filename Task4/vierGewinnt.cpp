#include <cmath>
#include <iostream>
#include <random>
#include "unit.h"
#include "Spielbrett.h"
#include <algorithm>

void NetzwerkMain();


const unsigned int Schwierigkeitsgrad = 5;


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
int DISCARD_ROW = -42;
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
    for(int i = 0; i < miniMaxValues.size(); i++){
        if(miniMaxValues[i] != DISCARD_ROW){
            validMiniMaxValues.push_back(miniMaxValues[i]);
        }
    }
    if(layer % 2 == 0){
        miniMaxValue = *std::max_element(validMiniMaxValues.begin(), validMiniMaxValues.end());
    } else {
        miniMaxValue = *std::min_element(validMiniMaxValues.begin(), validMiniMaxValues.end());
    }
    if(layer == 0){
        for(int i = 0; i < miniMaxValues.size(); i++){
            if(miniMaxValues[i] == miniMaxValue && !gameField.isColFull(i)){
                bestMoves.push_back(i);
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
    /*
    bool networkGame = true;
    if(networkGame){
        NetzwerkMain();
    }
    else {
        startOfflineGame();
    }
    return 0;
    */
    Spielbrett brett = Spielbrett(5, 5);
    return 0;
}

enum class SpielStatus {
    Verbindungsfehler,
    Niederlage,
    Unentschieden,
    Sieg,
    Running
};

void doNetworkGameMove(Spielbrett board, int myColor){
    int col = errechneBestenZug(board, board.getColor(myColor));
    board.addTile(col, myColor);
    outputGameField(board);
    SendeZug(col);
}

void doReceivedNetworkGameMove(Spielbrett board, int col, int myColor){
    board.addTile(col, 1 - myColor);
}

SpielStatus getSpecialGameState(Spielbrett board, int myColor){
    int heuristicValue = board.heuristischeBewertung(board.getColor(myColor));
    if(heuristicValue == 1){
        return SpielStatus::Sieg;
    }
    else if(heuristicValue == -1){
        return SpielStatus::Niederlage;
    }
    //else if(board.isFull()){
    //    return SpielStatus::Unentschieden;
    //}
    else {
        return SpielStatus::Running;
    }
}

// Spielt ein einzelnes Netzwerkspiel gegen einen anderen Computer im Netzwerk.
// Sollte das Spiel beendet sein oder ein Netzwerkfehler auftreten, muss diese Methode
// das zugehoerige Element der Enumeration SpielStatus zurueckgeben.
SpielStatus Netzwerkspiel( Feld color) {
    Spielbrett board = Spielbrett(6, 7);
    int myColor = board.getColorId(color);
    if(myColor == 0){
        doNetworkGameMove(board, myColor);
    }
    while(true){
        outputGameField(board); std::getchar();
        /*int receivedData = EmpfangeZug();
        if(receivedData == SPIELENDE){
            return getSpecialGameState(board, myColor);
        }
        else if(receivedData == VERBINDUNGSFEHLER){
            return SpielStatus::Verbindungsfehler;
        }
        else if(receivedData >= 0 && receivedData < rows){
            doReceivedNetworkGameMove(board, receivedData, myColor);
        }*/
        doReceivedNetworkGameMove(board, EmpfangeZug(), myColor);
        outputGameField(board); std::getchar();
/*
        SpielStatus state = getSpecialGameState(board, myColor);
        if(state != SpielStatus::Running){
            return state;
        }*/
        doNetworkGameMove(board, myColor);

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
