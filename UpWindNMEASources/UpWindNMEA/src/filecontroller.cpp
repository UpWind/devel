#include "filecontroller.h"

FileController::FileController(QObject *parent):
        QThread(parent), readFileData(0)
{
    filepathChangeble = true;
    cannotRun = false;
}

FileController::~FileController(){
    if(readFileData != 0){
        readFileData->clear();
        delete readFileData;
    }
}

int FileController::save(const QString &filepath, bool ask){
    int i = 0;
    QFile file(filepath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
             return -1;
    QTextStream out(&file);
    out<<"Simulation Type: II Date: " << getDate() << " SW: 0.1"<<"\n";

    for(i = 0; i < toBeSaved.size(); ++i){
        SavingType savet = toBeSaved.at(i);
        out << savet.getLine() << " " << savet.getTime() << "\n";
    }

    file.close();
    return 0;
}
void FileController::addToBeSaved(const QString &line, const QString &time, QString name, bool ask){
    SavingType toSave(line, time);
    if(!ask)
        toSave.setName(name);
    toBeSaved << toSave;

}
void FileController::emptyToBeSaved(){
    toBeSaved.clear();
}

QString FileController::getDate() {
    QString date;
    QString day;
    QString month;
    QDate help = QDate::currentDate();

    day = QString::number(help.day());
    month = QString::number(help.month());

    if(day.size() < 2)
        day.prepend("0");
    if(month.size() < 2)
        month.prepend("0");

    date = day;
    date.append(".");
    date.append(month);
    date.append(".");
    date.append(QString::number(help.year()));

    return date;

}

int FileController::read(const QString &filepath) {
    filepathChangeble = false;
    QList<SavingType> readData;
    int basicms = 0;
    int previous = 0;

    QFile file(filepath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        filepathChangeble = true;
        return -1;
    }

    QTextStream out(&file);
    out.readLine();
    while(!out.atEnd()){
        QString line, time;
        out >> line;
        out >> time;

        if(line.trimmed() != "" && line.trimmed().left(1) == "$"){
            SavingType sav(line, time);
            readData.append(sav);
        }
    }

    file.close();

    filepathChangeble = true;

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

void FileController::run(){
    read(filename);
}

void FileController::setFilepath(const QString &filepath){
    if(filepathChangeble == true)
        filename = filepath;
}

void FileController::setCannotRun(){
    cannotRun = true;
}

QList<QPair<QString, int> >* FileController::getFileData(const QString &filepath, bool ask){
    if(readFileData == 0){
        readFileData = new QList<QPair<QString, int> >;
    }else{
        readFileData->clear();
    }

    QFile file(filepath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        return 0;
    }
    emit giveLine("I am here");
    QTextStream out(&file);
    out.readLine();
    while(!out.atEnd()){
        QString line, time;
        out >> line;
        out >> time;

        if(line.trimmed() != "" && line.trimmed().left(1) == "$")
            readFileData->append(qMakePair(line, time.toInt()));
    }

    file.close();

    return readFileData;
}
