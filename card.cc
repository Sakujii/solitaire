#include <QPainter>
#include <QMouseEvent>
#include <QSizePolicy>
#include "card.hh"

//unsigned const FRONT_INDEX = 1;
//unsigned const BACK_INDEX = 0;
unsigned const CARD_WIDTH = 130;
unsigned const CARD_HEIGHT = 182;
//unsigned const DEFAULT_STACK_OFFSET = 40;
QColor const CARD_FRONT_COLOR = QColor(255,255,255,255);
QSize const SUIT_PIXMAP_SIZE = QSize(25,25);

Card::Card(CardSuit suit, unsigned value, int offset, QWidget *parent) :
    QFrame(parent),
    suit_(suit),
    value_(value),
    open_(false),
    canOpen_(false),
    offset_(offset),
    backPixmap_(":/cards/card-back-130x182"),
    stackedCard_(nullptr)

{
    setBaseSize(CARD_WIDTH, CARD_HEIGHT);
    setMinimumSize(CARD_WIDTH, CARD_HEIGHT);
    setMaximumWidth(CARD_WIDTH);
    QSizePolicy policy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    setSizePolicy(policy);
    setupLayout();

}

// Funktio palauttaa tiedon siitä, onko kortti auki
bool Card::isOpen()
{
    return open_;
}

// Funktio sallii kortin aukaisun
void Card::allowOpen()
{
    canOpen_ = true;
}

// Funktio kieltää kortin aukaisun
void Card::denyOpen(){
    canOpen_ = false;
}

// Funktio pinoaa kortit
void Card::stackCard(Card *card)
{
    card->setParent(this);
    card->move(0, offset_);

    stackedCard_ = card;
    connect(stackedCard_, &Card::resized, this, &Card::expand);
    expand();

}

// Funktio poistaa pinotut kortit
void Card::removeStackedCards()
{
    stackedCard_ = nullptr;
}

// Funktio hakee tiedot kortin arvosta, maasta ja avoimuudesta
// Palauttaa tiedot string-muodossa
std::string Card::getCardData()
{
    std::string rVal;
    if(stackedCard_ != nullptr) {
        rVal += stackedCard_->getCardData() + ";";
    }
    rVal += std::to_string(value_) +
            "," +
            std::to_string(suit_) +
            "," +
            std::to_string(open_)
            ;
    return rVal;
}

// Funktio palauttaa tiedon siitä, kummin päin kortti on
QLabel* Card::getCurrentSideLabel()
{
    if (open_){
        return findChild<QLabel*>(QString ("front"));
    } else {
        return findChild<QLabel*>(QString ("back"));
    }
}

// Käännetään kortti ympäri.
void Card::turn()
{
    open_ = !open_;
    if (open_){
        findChild<QLabel*>(QString ("front"))->show();
        findChild<QLabel*>(QString ("back"))->hide();
    } else {
        findChild<QLabel*>(QString ("front"))->hide();
        findChild<QLabel*>(QString ("back"))->show();
    }
}

// Kasvatetaan widgetin piirtoalueen kokoa.
void Card::expand()
{
    setGeometry(x(), y(), width(), height() + offset_);
    emit resized();
}

// Suoritetaan, kun widgettiä kaksoisklikataan.
void Card::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(canOpen_){
        turn();
        event->accept();
    }
    else {
        event->ignore();
    }
}

void Card::setupLayout()
{
    setSuitPixmap();
    setupBack();
    setupFront();
}

// Asettaa kortin attribuuttiin oikean maakuvan, että sitä voidaan käyttää piirtämisessä.
void Card::setSuitPixmap()
{
    switch (suit_) {
    case SPADE:
        suitPixmap_ = QPixmap(":/cards/spade-300px.png").scaled(SUIT_PIXMAP_SIZE, Qt::KeepAspectRatio);
        break;
    case CLUB:
        suitPixmap_ = QPixmap(":/cards/club-300px.png").scaled(SUIT_PIXMAP_SIZE, Qt::KeepAspectRatio);
        break;
    case DIAMOND:
        suitPixmap_ = QPixmap(":/cards/diamond-300px.png").scaled(SUIT_PIXMAP_SIZE, Qt::KeepAspectRatio);
        break;
    case HEART:
        suitPixmap_ = QPixmap(":/cards/heart-300px.png").scaled(SUIT_PIXMAP_SIZE, Qt::KeepAspectRatio);
        break;
    default:
        break;
    }
}

// Piirtääkortin takapuolen widgettiin.
void Card::setupBack()
{
    // Muodostetaan uusi QLabel-olio, jonka sisällöksi laitetaan kortiolion
    // attribuuttiin talletettu QPixmap-olio ja näytetään label widgetissä.
    QLabel* backImage = new QLabel(this);
    backImage->setObjectName("back");
    backImage->setPixmap(backPixmap_);
    backImage->show();
}

// Piirtää kortin etupuolen widgettiin.
void Card::setupFront()
{
    // Muodostetaan uusi QLabel-olio.
    QLabel* frontImage = new QLabel(this);
    frontImage->setObjectName("front");

    // Muodostetaan uusi QPixmap-olio, johon aletaan piirtää kortin etupuolta.
    QPixmap frontPixmap(backPixmap_.size());
    frontPixmap.fill(Qt::transparent);
    QPainter painter (&frontPixmap);

    // Piirretään kortin pohja, eli kulmista pyöristetty suorakaide.
    QPainterPath path;
    path.addRoundedRect(QRectF(1, 1, frontPixmap.width()-1, frontPixmap.height()-2),
                        15, 15, Qt::RelativeSize);
    painter.fillPath(path, CARD_FRONT_COLOR);
    painter.drawPath(path);

    // Piirretään kortin arvo ja maan kuvio.
    painter.setPen(QPen(Qt::black));
    QFont font = painter.font();
    font.setPixelSize(suitPixmap_.height());
    painter.setFont(font);
    painter.drawPixmap(CARD_WIDTH/2 - suitPixmap_.width()/2, 10,
                       suitPixmap_, 0, 0, 0, 0);
    painter.drawPixmap(CARD_WIDTH/2 - suitPixmap_.width()/2, CARD_HEIGHT/2 - suitPixmap_.height()/2,
                       suitPixmap_, 0, 0, 0, 0);
    painter.drawText(13, 5, suitPixmap_.width() + 5,
                     suitPixmap_.height() + 5, Qt::AlignCenter, QString::number(value_));

    // Piirretään vielä alalaitaan 180-astetta käännetyt objektit
    painter.save();
    painter.translate(CARD_WIDTH/2, CARD_HEIGHT - suitPixmap_.height()/2 - 10);
    painter.rotate(180);
    painter.drawPixmap(-suitPixmap_.width()/2, -suitPixmap_.height()/2, suitPixmap_, 0, 0, 0, 0);

    painter.translate(-55 , -suitPixmap_.height() + 7);
    painter.drawText(0, 0, suitPixmap_.width() + 5,
                     suitPixmap_.height() + 5, Qt::AlignCenter, QString::number(value_));
    painter.restore();


    // Asetetaan piirretty kuva näkyviin widgetissä.
    frontImage->setPixmap(frontPixmap);
    frontImage->hide();
}
