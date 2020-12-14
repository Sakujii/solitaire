#include <QFrame>
#include <QHBoxLayout>

#include "mainwindow.hh"
#include "cardslot.hh"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    setupLayout();
    connect(deck_, &Deck::cardPicked, pickedCards_, &OpenDeck::addCard);
}

MainWindow::~MainWindow()
{
}

// Funktio asettaa pelin alkutilaansa
void MainWindow::setupLayout()
{
    // Pääikkunan isoimpana rakenteena on frame, joka sisältää...
    QFrame* frame = new QFrame;
    QVBoxLayout* frameLayout = new QVBoxLayout(frame);

    // ... alarivin ja ylärivin.
    QHBoxLayout* bottomRowLayout = new QHBoxLayout();
    QHBoxLayout* topRowLayout = new QHBoxLayout();
    frameLayout->addLayout(topRowLayout);
    frameLayout->addLayout(bottomRowLayout);

    // Luodaan pakkaoliot.
    pickedCards_ = new OpenDeck(this);
    deck_ = new Deck(pickedCards_, this);

    // Lisätään yläriville suljettu ja avoin pakka sekä neljä tyhjää slotia...
    topRowLayout->addWidget(deck_);
    topRowLayout->addWidget(pickedCards_);
    for (int i = 0; i < 4; ++i){
        CardSlot* slot;
        slot = new CardSlot(&GameRules::sameSuitInOrder, 0, this);
        topRowLayout->addWidget(slot);
    }
    // ... ja alariville seitsemän cardslottia.
    for (int i = 0; i < 7; ++i){
        CardSlot* slot;
        slot = new CardSlot(&GameRules::differentColourInOrder, 35, this);
        bottomRowLayout->addWidget(slot);

        // Lisätään vasemmalta oikealle ensimmäiseen slotiin 1 kortti,
        // seuraavaan 2 korttia ja niin edelleen
        for (int j=0; j < i+1; ++j){
            Card* card = deck_->pickCard();
            slot->addCard(card);
            // Avataan päällimmäiset kortit
            if (j == i){
                card->turn();
            }
        }
    }
    setCentralWidget(frame);
}
