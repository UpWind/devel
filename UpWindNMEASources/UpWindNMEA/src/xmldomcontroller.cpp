#include "xmldomcontroller.h"

XMLDomController::XMLDomController(QObject *parent):
        FileController(parent)
{
}

int XMLDomController::save(const QString &filepath, bool ask){
    int i = 0;
    QString s;
    SavingType sav("", "");

    QDomDocument doc("ListML");
    QDomElement root = doc.createElement("Simulation");

    if(toBeSaved.size() > 0) {
        sav = toBeSaved.at(0);
        s = sav.getLine().left(3);
        s = s.right(2);
        root.setAttribute("type", s);
    }else
        root.setAttribute("type", "II");

    root.setAttribute("date", getDate());
    root.setAttribute("swversion", "0.1");

   doc.appendChild(root);

   for(i = 0; i < toBeSaved.size(); ++i){
       SavingType savet = toBeSaved.at(i);
       QDomElement cn = doc.createElement("entry");

       QString temp = savet.getLine().left(6);
       temp = temp.right(3);

       cn.setAttribute("id",temp);
       cn.setAttribute("value",savet.getLine());
       cn.setAttribute("time",savet.getTime());

       root.appendChild(cn);

   }

   QFile file(filepath);
   if(!file.open( QIODevice::WriteOnly | QIODevice::Text))
           return -1;

   QTextStream ts(&file);
   ts << doc.toString();

   file.close();

    return 0;
}

int XMLDomController::read(const QString &filepath){
    filepathChangeble = false;
    QList<SavingType> readData;
    int basicms = 0;
    int previous = 0;
    QDomDocument doc("ListML");
    QFile::QFile file(filepath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        filepathChangeble = true;
        return -1;
    }
    if(!doc.setContent(&file)){
        filepathChangeble = true;
        file.close();
        return -1;
    }
    file.close();
    filepathChangeble = true;

    QDomElement root = doc.documentElement();
    if(root.tagName() != "Simulation")
        return -1;

    QDomNode n = root.firstChild();
    while(!n.isNull()){
        QDomElement e = n.toElement();
        if(!e.isNull()){
            if(e.tagName() == "entry"){
                QString line = e.attribute("value", "");
                QString time = e.attribute("time", "");
                SavingType sav(line, time);
                readData.append(sav);
            }
        }

        n = n.nextSibling();
    }

    if(readData.size() == 0)
        return -1;

    SavingType basic = readData.at(0);
    previous = basic.getTime().toInt();

    cannotRun = false;
    int i;
    for(i=0; i<readData.size(); ++i) {

        if(cannotRun == true){
            break;
        }

        SavingType send = readData.at(i);
        int ms = send.getTime().toInt();
        QString line = send.getLine();

        basicms = ms - previous;
        previous = ms;

        msleep(basicms);
        emit giveLine(line);

    }

    readData.clear();
    cannotRun = false;

    return 0;
}


void XMLDomController::run(){
    read(filename);
}

QList<QPair<QString, int> >* XMLDomController::getFileData(const QString &filepath, bool ask){
    if(readFileData == 0){
        readFileData = new QList<QPair<QString, int> >;
    }else{
        readFileData->clear();
    }

    QDomDocument doc("ListML");
    QFile::QFile file(filepath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        return 0;
    }

    if(!doc.setContent(&file)){
        file.close();
        return 0;
    }

    file.close();

    QDomElement root = doc.documentElement();
    if(root.tagName() != "Simulation")
        return 0;

    QDomNode n = root.firstChild();
    while(!n.isNull()){
        QDomElement e = n.toElement();
        if(!e.isNull()){
            if(e.tagName() == "entry"){
                QString line = e.attribute("value", "");
                QString time = e.attribute("time", "");
                readFileData->append(qMakePair(line, time.toInt()));
            }
         }

         n = n.nextSibling();
     }

    return readFileData;
}

