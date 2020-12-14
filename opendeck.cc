#include "opendeck.hh"

#include <QDrag>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QPainter>
#include <QLabel>

unsigned const SLOT_WIDTH = 135;
unsigned const SLOT_HEIGHT = 187;

OpenDeck::OpenDeck(QWidget *parent):
    QFrame(parent),
    layout_(new QStackedLayout(this))
{
    setMinimumSize(SLOT_WIDTH, SLOT_HEIGHT);
    setMaximumSize(SLOT_WIDTH, SLOT_HEIGHT);
    setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);
    setLayout(layout_);
}

// Funktio lisää kortin opendeckiin
void OpenDeck::addCard(Card *card)
{
    layout()->addWidget(card);
    layout_->setCurrentWidget(card);
    card->turn();
}

// Funktio kerää kortit opendeckistä
// Parametrinä suljetun pakan korttivektori,
// johon opendeckin kortit siirretään
void OpenDeck::get_cards(std::vector<Card *> &cards)
{
    while (layout_->count() != 0){
        // Otetaan päällimmäinen kortti talteen...
        Card* top_card;
        top_card = static_cast<Card*> (layout_->currentWidget());
        // lisätään kortti cards-vectoriin...
        cards.push_back(top_card);
        // poistetaan kortti open deckistä
        layout_->removeWidget(top_card);
    }
}

// Suoritetaan, kun avoimen pakan päällimmäinen kortti raahataan jonnekin.
void OpenDeck::mousePressEvent(QMouseEvent *event)
{
    // 1) Metodin alkuosa suoritetaan, kun avopakkaa klikataan.

    // Otetaan pakan sisältämä kortti osoittimen päähän.
    Card *card = static_cast<Card*>(layout_->currentWidget());

    // Tyhjästä pakasta ei voi raahata, eli lopetetaan.
    if (card == nullptr){
        return;
    }

    // Otetaan raahattavan kortin kuva valmiiksi muuttujaan pixmap.
    QPixmap pixmap = *card->getCurrentSideLabel()->pixmap();

    // Luodaan mimeData-olio, jonka avulla raahattavan kortin tiedot (=merkkijono)
    // välitetään sinne, minne kortti raahataan.
    QMimeData *mimeData = new QMimeData;
    mimeData->setText(QString::fromStdString(card->getCardData()));

    // Luodaan drag-olio, jonka avulla raahaaminen tapahtuu. Drag-oliolle annetaan
    // raahauksen aikana näytettävä kuva, sekä tiedot raahattavasta oliosta.
    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(pixmap);
    drag->setHotSpot(event->pos() - card->pos());  // Se kohta raahattavasta kuvasta, jonka pitää osua kohteeseen.

    // Asetetaan harmautettu korttikuva väliaikaiseksi korttikuvaksi raahaamisen ajaksi.
    QPixmap tempPixmap = pixmap;
    QPainter painter;
    painter.begin(&tempPixmap);
    painter.fillRect(pixmap.rect(), QColor(127, 127, 127, 127));
    painter.end();
    card->getCurrentSideLabel()->setPixmap(tempPixmap);

    // 2) Aloitetaan raahaus ja tarkastellaa onnistuiko.

    if (drag->exec( Qt::MoveAction) == Qt::MoveAction) {
        // Tämä suoritetaan, jos raahaus onnistui.
        card->getCurrentSideLabel()->setPixmap(pixmap);
        layout_->removeWidget(layout_->currentWidget());
        layout_->setCurrentIndex(layout_->count()-1);
        card->setAttribute(Qt::WA_DeleteOnClose);
        card->close();

    } else {
        // Tämä suoritetaan, jos raahaus epäonnistui.
        card->show();
        card->getCurrentSideLabel()->setPixmap(pixmap);
    }
}
