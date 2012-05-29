#include "linediagram.h"
#include "ui_widget.h"
#include "settingsmanager.h"

LineDiagram::LineDiagram():
        ui(new Ui::Widget), w(1500), h(200), next(0), takeaway(0), space(10)
{
    this->pluginName = "Wind speed changes";
    this->produceIDs = "???";
    this->parseIDs = "MWD";

    CreateGUI();
    setTimerDelay(1000);
    plugin_widget->setWindowTitle(pluginName + " (knots)");
}

LineDiagram::~LineDiagram(){
    delete ui;
    //delete allocated variables
}

void LineDiagram::CreateGUI(){
    ui->setupUi(plugin_widget);
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(clearDiagram()));

    next = w;
    lastx = 0;
    lasty = h;

    createBounds();
    ui->graphicsView->setScene(&scene);
    ui->graphicsView->scale(0.5, 0.5);
    ui->graphicsView->show();
}

void LineDiagram::parseNMEAString(const QString & text){
    if(this->parseMessageID(text) == "MWD"){
        QStringList pList = text.split(",");
        QString speed = pList.at(5);
        double speeddouble = speed.toDouble();
        if(speeddouble > 20)
            speeddouble = 20;

        int temporary = h - (speeddouble*10);
        QColor color(255,0,0);
        QPen pen(color);
        QLineF f(lastx, lasty, lastx+space, temporary);
        lines << f;
        ui->graphicsView->scene()->addLine(f, pen);
        lastx+=space;
        lasty= temporary;
    }

    drawLines();
    //If needs to work as expanding way, comment drawLines and uncomment the if-sentence below
    /*if(lastx >= ui->graphicsView->scene()->width()){
        moreBounds();
    }*/
    ui->graphicsView->scene()->update(ui->graphicsView->scene()->sceneRect());
}

void LineDiagram::updateNMEAString(){
    //Create and emit a string
}

void LineDiagram::createBounds(){
    QColor color(169,169,169);
    QPen pen(color);
    scene.setSceneRect(0,0, w, h);

    scene.addLine(0,0,0, h, pen);
    scene.addLine(0,0,w,0, pen);
    scene.addLine(0,h,w, h, pen);
    scene.addLine(w,0,w,h, pen);

    for(int i = h-50; i>0; i = i-50){
        scene.addLine(0,i,w,i, pen);
        scene.addText(QString::number(i/10))->setPos(0,h-i);
    }

    scene.addText(QString::number(h/10));
}

void LineDiagram::clearDiagram(){
    ui->graphicsView->scene()->clear();
    lastx = 0;
    lasty = h;
    next = w;
    lines.clear();
    takeaway = 0;
    createBounds();
}

void LineDiagram::moreBounds(){
    QColor color2(169,169,169);
    QPen pen2(color2);
    ui->graphicsView->scene()->setSceneRect(0,0,next+120, h);
    for(int i = h; i>-2; i = i-50){
        scene.addLine(next,i,next+120,i, pen2);
    }
    next+=120;
}

void LineDiagram::drawLines(){

    if(lastx >= ui->graphicsView->scene()->width()){
        int calc = space*takeaway;
        QColor color(255,0,0);
        QPen pen(color);
        ui->graphicsView->scene()->clear();
        next = w;
        createBounds();

        for(int i = takeaway; i<lines.size(); ++i){
            QLineF linef = lines.at(i);
            linef.setLine((linef.x1())-(calc), linef.y1(), (linef.x2())-(calc), linef.y2());
            ui->graphicsView->scene()->addLine(linef, pen);
        }
        takeaway++;
    }

}


