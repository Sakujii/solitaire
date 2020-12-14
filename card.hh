#ifndef CARD_HH
#define CARD_HH

#include <QLabel>
#include <QFrame>

enum CardSuit {SPADE, CLUB, DIAMOND, HEART, SUIT_COUNT};

class Card : public QFrame
{
    Q_OBJECT

public:
    Card(CardSuit suit, unsigned value, int offset, QWidget *parent = 0);

    // Kortin sisäisen tilan tarkastelemiseen ja muokkaamiseen.
    bool isOpen();
    void allowOpen();
    void denyOpen();

    // Kortin päällä olevien korttien käsittelemiseen.
    void stackCard(Card* card);
    void removeStackedCards();

    // Palauttaa kortin tiedot (arvo, maa, onko auki) merkkijonona. Tarvitaan
    // kortin raahaamisessa.
    std::string getCardData();

    // Kun korttia raahataan, raahaus-event tarvitsee kuvan, jota näytetään.
    // Sen saa haettua tällä metodilla.
    QLabel* getCurrentSideLabel();

signals:
    // Kun kortin päälle (tai sen päällä olevien korttien päällä olevien korttien
    // päälle) lisätään kortti, tarvii piirrettävän widgetin kokoa kasvattaa.
    void resized();

public slots:
    // Lisää toimenpiteitä, joita kortille voidaan suorittaa.
    void turn();
    void expand();

protected:
    // Itse toteutettu Qt:n widgetin klikkaamiseen liittyvä metodi.
    void mouseDoubleClickEvent(QMouseEvent* event);

private:
    // Apumetodeita muiden metodien käyttöön.
    void setupLayout();
    void setSuitPixmap();
    void setupBack();
    void setupFront();

    // Korttiwidgetin sisäinen tila.
    CardSuit suit_;
    unsigned value_;
    bool open_;
    bool canOpen_;
    int offset_;

    // Kuvat joita tarvitaan, kun korttia piirretään setupBack- ja setupFront-metodeissa.
    QPixmap suitPixmap_;
    QPixmap backPixmap_;

    // Osoitin kortin päällä mahdollisesti olevaan korttiin.
    Card* stackedCard_;
};

#endif // CARD_HH
