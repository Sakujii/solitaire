#include "gamerules.hh"
#include "card.hh" 

#include <QMessageBox>
#include <QApplication>
#include <iostream>

// Vektori lopetuksen tarkastelua varten
std::map<std::string, bool> finish {{"spades",false}, {"clubs", false}, {"diamonds", false}, {"hearts", false}};

// Sääntöfunktiot saavat parametrinaan alla olevan kortin ja päälle tulevan kortin
// tiedot merkkijonomuodossa, sekä tiedon raahattavien korttien määrästä
// Funktiot palauttavat true jos raahaus on sallittu, false jos ei

// Sääntö ylärivin neljälle cardslotille
bool GameRules::sameSuitInOrder(std::string bottom, std::string top, const int count)
{
    // Tarkastetaan että slottiin laitetaan vain yksi kortti kerrallaan
    if (count > 1){
        return false;
    } else{
        std::string bottomSuit = bottom.substr(bottom.find_first_of(",")+1,1);
        std::string topSuit = top.substr(top.find_first_of(",")+1,1);
        std::string bottomValue = bottom.substr(0, bottom.find(","));
        std::string topValue = top.substr(0, top.find(","));

        // Jos 'bottomissa' on kortti, tarkistetaan onko raahattava kortti samaa maata ja arvo yhtä suurempi
        if (bottom.length() != 0){
            if (stoi(topSuit) == stoi(bottomSuit) and stoi(bottomValue) + 1 == stoi(topValue)){
                // Tarkastetaan onko peli voitettu
                checkIfCompleted(topSuit, topValue);
                return true;
            }
            else{
                return false;
            }
        }
        // Jos 'bottomissa' ei ole korttia, täytyy kortin olla arvoltaan 1
        else{
            if (stoi(topValue) == 1){
                return true;
            }
            else{
                return false;
            }
        }
        // Jos 'bottomissa' on 12 ja toppiin tulee 13,
    }
}

// Sääntö alarivin seitsemälle slotille.
bool GameRules::differentColourInOrder(std::string bottom, std::string top, const int count)
{
    if (count == 0){}
    std::string bottomSuit = bottom.substr(bottom.find_first_of(",")+1,1);
    std::string topSuit = top.substr(top.find_first_of(",")+1,1);
    std::string bottomValue = bottom.substr(0, bottom.find(","));
    std::string topValue = top.substr(0, top.find(","));

    // Jos 'bottomissa' on kortti, tarkistetaan onko raahattava kortti eri väriä ja arvo yhtä pienempi
    if (bottom.length() != 0){
        if ((stoi(bottomSuit) == SPADE or stoi(bottomSuit) ==CLUB) and
                (stoi(topSuit) == HEART or stoi(topSuit) == DIAMOND) and
                stoi(bottomValue) - 1 == stoi(topValue)){
            return true;
        }
        else if ((stoi(bottomSuit) == HEART or stoi(bottomSuit) == DIAMOND) and
                 (stoi(topSuit) == SPADE or stoi(topSuit) == CLUB) and
                 stoi(bottomValue) - 1 == stoi(topValue)){
            return true;
        }
        else{
            return false;
        }
    }
    // Jos 'bottomissa' ei ole korttia, täytyy kortin olla arvoltaan 13
    else{
        if (stoi(topValue) == 13){
            return true;
        }
        else{
            return false;
        }
    }
}

// Apufunktio tarkastaa onko peli voitettu
// Parametreina raahattavan kortin maa sekä arvo
void checkIfCompleted(std::string topSuit, std::string topValue){

    // Jos raahattava kortti on 13, muutetaan pakan lopetuksen arvoksi true
    if (stoi(topValue) == 13){
        if (stoi(topSuit) == SPADE){
            finish["spades"] = true;
        } else if (stoi(topSuit) == CLUB){
            finish["clubs"]  = true;
        } else if (stoi(topSuit) == DIAMOND){
            finish["diamonds"] = true;
        } else if (stoi(topSuit) == HEART){
            finish["hearts"] = true;
        }
        // Jos kaikki pakat ovat valmiita, lopetetaan peli
        if (finish["spades"] == true and finish["clubs"] == true and
                finish["diamonds"] == true and finish["hearts"] == true){
            // Suoritetaan messagebox
            QMessageBox msgBox;
            msgBox.setText("Jee! Voitit pelin!");
            msgBox.exec();
            // Lopetetaan graafinen ikkuna
            QApplication::quit();
        }
    }
}
