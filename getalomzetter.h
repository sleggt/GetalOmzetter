#ifndef GETALOMZETTER_H
#define GETALOMZETTER_H

#include <QString>
#include <QMap>

class GetalOmzetter : public QString
{
public:
    GetalOmzetter();
    GetalOmzetter(QString getal);

    QString toTextualNumber();

private:
    void initMap();
    QMap<QString, QString> *m_units;
    QMap<int, QString> *m_powers;

    QString m_basis;
    QString m_werk;
    QString m_resultaat;
    QString m_space;

    int m_processed;

    QString tweeCijfers(QString in);
    QString drieCijfers(QString in);
};

#endif // GETALOMZETTER_H
