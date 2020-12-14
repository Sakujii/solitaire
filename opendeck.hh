#ifndef OPENDECK_HH
#define OPENDECK_HH
#include <QFrame>

QT_BEGIN_NAMESPACE
class QDragEnterEvent;
class QDropEvent;
QT_END_NAMESPACE

#include <QStackedLayout>

#include "card.hh"
class OpenDeck : public QFrame
{
public:
    OpenDeck(QWidget* parent = 0);
public slots:
    void addCard(Card* card);
    void get_cards(std::vector<Card*> &cards);
protected:
    void mousePressEvent(QMouseEvent *event);
private:
    QStackedLayout* layout_;  // Sisältää ne kortti-widgetit, jotka avopakassa on.
};

#endif // OPENDECK_HH
