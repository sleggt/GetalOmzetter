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
    m_units = new QMap<QString, QString>;
//    m_units->insert("0", "nul");   nul niet hierin meenemen, speciaal geval
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
//    m_units->insert("100", "honderd");
//    m_units->insert("1000", "duizend");
//    m_units->insert("1000000", "miljoen");
//    m_units->insert("1000000000", "miljard");
//    m_units->insert("1000000000000", "triljard(?)");

    // Grote getallen volgens de "lange schaal"
    m_groteGetallen = new QMap<int, QString>;
    m_groteGetallen->insert(3, "duizend");
    m_groteGetallen->insert(6, "miljoen");
    m_groteGetallen->insert(9, "miljard");
    m_groteGetallen->insert(12, "biljoen");
    m_groteGetallen->insert(15, "biljard");
    m_groteGetallen->insert(18, "triljoen");
    m_groteGetallen->insert(21, "triljard");
    m_groteGetallen->insert(24, "quadriljoen");
    m_groteGetallen->insert(27, "quadriljard");
    m_groteGetallen->insert(30, "quintiljoen");
    m_groteGetallen->insert(33, "quintiljard");
    m_groteGetallen->insert(36, "sextiljoen");
    m_groteGetallen->insert(39, "sextiljard");
    m_groteGetallen->insert(42, "septiljoen");
    m_groteGetallen->insert(45, "septiljard");
    m_groteGetallen->insert(48, "octiljoen");
    m_groteGetallen->insert(51, "octiljard");
    m_groteGetallen->insert(54, "noniljoen");
    m_groteGetallen->insert(57, "noniljard");
    m_groteGetallen->insert(60, "deciljoen");
    m_groteGetallen->insert(63, "deciljard");
}

QString GetalOmzetter::toTextualNumber(QString space)
{
    m_space = space;

    // twee speciale gavallen
    if (m_basis == "") return "";
    if (m_basis == "0") return "nul";

    // haal nu nog voorloopnullen weg
    // TODO

    // als het direct gevonden is return dan
    m_resultaat = m_units->value(m_basis);
    if ( m_resultaat != "") return m_resultaat;

    m_werk = m_basis;
    m_resultaat = "";
    m_processed = 0;

    do {
        // bepaal start van groep van (max) 3
        int start = m_basis.length() - m_processed - 3;
        if (start < 0) start = 0;
        // bepaal lengte van groep van (max) 3
        int aantal = m_basis.length() - m_processed;
        if (aantal > 3) aantal = 3;
        QString teVerwerken = m_basis.mid(start, aantal);
        m_resultaat = drieCijfers(teVerwerken) + m_space + m_resultaat;
        m_processed += teVerwerken.length();
        QString dt = grootGetal(m_processed);
        if (dt != "" &&
            m_basis.length() > m_processed &&
            m_basis.mid(start - 3, 3) != "000" &&
            m_basis.mid(start - 3, 3) != "00" &&
            m_basis.mid(start - 3, 3) != "0") {
            m_resultaat = dt + m_space + m_resultaat;
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

    // als niet gevonden dan samenstellen
    // eers eenheden
    res = m_units->value(in.right(1));
    // eventueel "en" ertussen
    if (in.left(1) > "1" && in.left(1) <= "9") res += m_space + "en" + m_space; // geen en na nul en na 1 (tien)
    // en dan tientallen
    res += m_units->value(in.left(1)+"0");
    if (res == "00")
        return "";
    else
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
        if (in.left(1) >= "1" && in.left(1) <= "9") {
            resultaat = "honderd" + m_space + resultaat;
            if (in.left(1) >= "2" && in.left(1) <= "9") {
                resultaat = m_units->value(in.left(1))+ m_space + resultaat;
            }
        }
    }
    return resultaat;

}

QString GetalOmzetter::grootGetal(int in)
{
    // recursief samenstellen van "te grote" duizentallen
    // duizend triljard etcetera
    if (in < 3) {
        return "";
    }
    else {
        // zoek naar grootste die past
        // merk op: als we beginnen op iets wat geen veelvoud van 3 is
        // levert dit niets op. Dat is wat we willen
        int grootste;
        for  (grootste = in; grootste > 3; grootste -=3)
            if (m_groteGetallen->contains(grootste)) break;
        return grootGetal(in-grootste) + m_groteGetallen->value(grootste);
    }
}


QString zetOm(QString in)
{
    // nieuwe aanpak
    // zoek in string in map vanaf
    // als gevonden klaar
    // pak eerste cijfer naar string en vervang door 1

    //for (int i = in.length(); )

}
