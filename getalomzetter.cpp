#include "getalomzetter.h"

GetalOmzetter::GetalOmzetter()
    : m_basis("")
{
    initMap();
}

GetalOmzetter::GetalOmzetter(QString getal)
    : m_basis(getal)
{
    initMap();
}

// alleen vanuit contructor aan te roepen
void GetalOmzetter::initMap()
{
    m_space = " ";

    m_units = new QMap<QString, QString>;
    m_units->insert("0", "nul");
    m_units->insert("1", "een");
    m_units->insert("2", "twee");
    m_units->insert("3", "drie");
    m_units->insert("4", "vier");
    m_units->insert("5", "vijf");
    m_units->insert("6", "zes");
    m_units->insert("7", "zeven");
    m_units->insert("8", "acht");
    m_units->insert("9", "negen");
    m_units->insert("10", "tien");
    m_units->insert("11", "elf");
    m_units->insert("12", "twaalf");
    m_units->insert("13", "dertien");
    m_units->insert("14", "veertien");
    m_units->insert("20", "twintig");
    m_units->insert("30", "dertig");
    m_units->insert("40", "veertig");
    m_units->insert("50", "vijftig");
    m_units->insert("60", "zestig");
    m_units->insert("70", "zeventig");
    m_units->insert("80", "tachtig");
    m_units->insert("90", "negentig");

    m_powers = new QMap<int, QString>;
    m_powers->insert(3, "duizend");
    m_powers->insert(6, "milioen");
    m_powers->insert(9, "miljard");
    m_powers->insert(12, "triljard(?)");
}

QString GetalOmzetter::toTextualNumber()
{
    if (m_basis == "") return "";

    // haal nu nog voorloopnullen weg
    // TODO

    // als het direct gevonden is return dan
    m_resultaat = m_units->value(m_basis);
    if ( m_resultaat != "") return m_resultaat;

    m_werk = m_basis;
    m_resultaat = "";
    m_processed = 0;


    do {
        int start = m_basis.length() - m_processed - 3;
        if (start < 0) start = 0;
        int aantal = m_basis.length() - m_processed;
        if (aantal > 3) aantal = 3;
        QString teVerwerken = m_basis.mid(start, aantal);
        m_resultaat = drieCijfers(teVerwerken) + m_space + m_resultaat;
        m_processed += teVerwerken.length();
        if (m_powers->contains(m_processed) &&
            m_basis.length() > m_processed) {
            m_resultaat = m_powers->value(m_processed) +
                          m_space + m_resultaat;
        }
    } while (m_processed < m_basis.length());
    return m_resultaat;
}

QString GetalOmzetter::tweeCijfers(QString in)
{
    // Zoek een speciale naam met 2 cijfers
    QString res = m_units->value(in);

    // return als gevonden
    if (res != "") return res;

    // als niet gevonden dan samenstellen eenheid
    res = m_units->value(in.right(1));
    // eventueel " en" ertussen
    if (in.left(1) != "1") res += m_space + "en" + m_space;
    // en dan tientallen
    res += m_units->value(in.left(1)+"0");
    return res;
}

QString GetalOmzetter::drieCijfers(QString in)
{
    if (in.length() == 1)
        return m_units->value(in);

    QString resultaat = "";

    if (in.length() > 1) {
        resultaat = tweeCijfers(in.right(2));
    }

    if (in.length() == 3) {
        resultaat = "honderd" + m_space + resultaat;
        if (in.left(1) != "1") {
            resultaat = m_units->value(in.left(1))+ m_space + resultaat;
        }
    }

    return resultaat;

}

QString zetOm(QString in)
{
    // nieuwe aanpak
    // zoek in string in map vanaf
    // als gevonden klaar
    // pak eerste cijfer naar string en vervang door 1

    //for (int i = in.length(); )

}
