#include "widget.h"
#include "getalomzetter.h"
#include <QLineEdit>
#include <QLabel>
#include <QtGui>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , m_inputText(new QLineEdit("<voer getal in>"))
    , m_resultText(new QTextEdit(""))
{
    m_inputText->selectAll();

    m_inputText->setMinimumWidth(400);
    m_resultText->setMinimumWidth(400);
    m_resultText->setMinimumHeight(400);
    m_resultText->setReadOnly(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(m_inputText);
    layout->addWidget(m_resultText);
    setLayout(layout);

    connect(m_inputText, SIGNAL(textChanged(QString)), this, SLOT(bereken(QString)));
}

Widget::~Widget()
{

}

void Widget::bereken(QString tekst)
{
    m_resultText->setText(GetalOmzetter(tekst).toTextualNumber());
}
