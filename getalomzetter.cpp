#include "getalomzetter.h"
#include <QRegExp>

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

    int startRest = 0;
    QString resultaat("");

    // zoek start van getal
    int startGetal = m_basis.indexOf(QRegExp("[0-9]"), 0);

    while (startGetal != -1) {
        // neem alles over tot begin van getal
        resultaat += m_basis.mid(startRest, startGetal - startRest);

        // zoek eerste niet-cijfer
        if ((startRest = m_basis.indexOf(QRegExp("[^0-9]"), startGetal)) == -1)
            startRest = m_basis.length();

        resultaat += zetGetalOm(m_basis.mid(startGetal, startRest - startGetal));
        startGetal = m_basis.indexOf(QRegExp("[0-9]"), startRest);
    }

    // neem de rest over
    return resultaat + m_basis.mid(startRest);
}

// Deze method gaat ervan uit dat de hele string uit cijfers bestaat
QString GetalOmzetter::zetGetalOm(QString in)
{
    // verwijder voolooopnullen
    in.replace(QRegExp("^0+"), "");

    // als dan niets overblijft bestond getal uit alleen nullen
    if (in == "") return "nul";

    // als het direct gevonden is return dan
    QString resultaat = m_units->value(in);
    if ( resultaat != "") return resultaat;

    resultaat = "";
    m_processed = 0;
    bool spaceMandatory = false;
    QString space;

    // als we tussen 1100 een 9999 zitten maken we er honderdtallen van
    if (in.length() == 4 && in.mid(1,1) != "0") {
        resultaat += tweeCijfers(in.left(2)) + "honderd ";
        resultaat += tweeCijfers(in.right(2));
    } else {
        do {
            // bepaal start van groep van (max) 3
            int start = in.length() - m_processed - 3;
            if (start < 0) start = 0;
            // bepaal lengte van groep van (max) 3
            int aantal = in.length() - m_processed;
            if (aantal > 3) aantal = 3;
            QString teVerwerken = in.mid(start, aantal);
            space = spaceMandatory || teVerwerken.toInt() > 100 ? " " : "";
            resultaat = drieCijfers(teVerwerken) + space + resultaat;
            m_processed += teVerwerken.length();
            QString dt = grootGetal(m_processed);
            if (dt != "" &&
                in.length() > m_processed &&        // als er nog iets voor komt
                in.mid(start - 3, 3) != "000" &&    // en dat is
                in.mid(start - 3, 3) != "00" &&
                in.mid(start - 3, 3) != "0") {
                resultaat = dt + " " + resultaat;
                if (m_processed > 3) spaceMandatory = true;
            }
        } while (m_processed < in.length());
    }
    return resultaat;
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
    // eventueel "en" ertussen (zonder spaties)
    // behalve na nul en na 1 (vijftien etcetera)
    if (in.left(1) > "1") {
        if (in.right(1) == "2" || in.right(1) == "3") {
            res += QString(QChar(0x00EB)) + "n";  //  "ën"
        } else {
            res += "en";
        }
    }
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
            if (resultaat != "") resultaat = " " + resultaat;
            resultaat = "honderd" + resultaat;
            if (in.left(1) >= "2" && in.left(1) <= "9") {
                // honderdtal ervoor zonder spatie
                resultaat = m_units->value(in.left(1)) + resultaat;
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

