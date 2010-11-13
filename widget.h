#ifndef WIDGET_H
#define WIDGET_H

#include <QtGui/QWidget>

class QLineEdit;
class QTextEdit;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void bereken(QString tekst);

private:
    QLineEdit *m_inputText;
    QTextEdit *m_resultText;
};

#endif // WIDGET_H
