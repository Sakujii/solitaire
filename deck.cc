#include "deck.hh"

#include <QDrag>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QPainter>
#include <QLabel>
#include <QMessageBox>
#include <QApplication>
#include <random>
#include <ctime>

// Muuttujat slotin koolle
unsigned const SLOT_WIDTH = 135;
unsigned const SLOT_HEIGHT = 187;
// Laskuri pelin häviämistä varten
int counter = 0;

Deck::Deck(OpenDeck* openDeck, QWidget *parent) : QFrame(parent), open_deck_(openDeck)
{
    setMinimumSize(SLOT_WIDTH, SLOT_HEIGHT);
    setMaximumSize(SLOT_WIDTH, SLOT_HEIGHT);
    setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);
    for (int suit = SPADE; suit != SUIT_COUNT; ++suit){

        for (unsigned value = 1; value <= 13; ++value){
            auto card = new Card(static_cast<CardSuit>(suit), value, 35, this);
            int xOffset = (this->width() - card->width()) / 2;
            int yOffset = (this->height() - card->height()) / 2;
            card->move(xOffset, yOffset);
            cards_.push_back(card);
        }
    }
    shuffle();
}

// Funktio sekoittaa korttipakan
void Deck::shuffle(){
    std::shuffle(cards_.begin(), cards_.end(), std::default_random_engine(time(0)));
}

// Funktio 'nostaa' pinosta kortin ja palauttaa pointterin
Card* Deck::pickCard()
{
    if (cards_.empty()){
        return nullptr;
    } else {
        Card* card = cards_.back();
        cards_.pop_back();
        return card;
    }
}

// Suoritetaan, kun pakkaa klikataan.
void Deck::mousePressEvent(QMouseEvent *event)
{
    // Ohjelma ei saa kaatua tyhjän pakan klikkaukseen.
    if (cards_.empty()){
        counter += 1;

        // Jos pakka klikataan kolme kertaa läpi, peli on hävitty
        if (counter == 5){
            // Suoritetaan messagebox viestillä "Hävisit pelin"
            QMessageBox msgBox;
            msgBox.setText("Hävisit pelin.");
            msgBox.exec();
            // Lopetetaan graafinen ikkuna
            QApplication::quit();
        }
        open_deck_->get_cards(cards_);
        for (Card* card: cards_){

            // Käännetään kortit, asetetaan vanhempi, piirretään näytölle
            card->turn();
            card->setParent(this);
            card->show();
        }
        return;
    }

    // Otetaan osoittimen päähän pakan päällimmäinen kortti ja poistetaan se pakasta.
    Card* card = cards_.back();
    cards_.pop_back();

    // Ohjelma ei saa kaatua, jos korttia ei olekaan.
    if (!card){
        return;
    }

    // Ilmoitetaan, että pakasta otettiin kortti.
    emit cardPicked(card);

    // Hyväksytään klikkaus, koska se käsitellään aina tässä metodissa.
    event->accept();
}
