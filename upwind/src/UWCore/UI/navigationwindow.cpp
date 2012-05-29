#include "navigationwindow.h"
#include "ui_navigationwindow.h"

NavigationWindow::NavigationWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NavigationWindow)
{
    ui->setupUi(this);
    this->setParent(parent);
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
}

NavigationWindow::~NavigationWindow(){
    delete ui;
}

void NavigationWindow::back(){
    this->hide();
    emit goBack();
}

void NavigationWindow::toolboxButton_clicked(){
    if(toolboxButton->text() == "Hide ToolBox")
        toolboxButton->setText("Show ToolBox");
    else
        toolboxButton->setText("Hide ToolBox");

    emit toggleToolbox();
}

void NavigationWindow::addButtons(){
    QRect screenGeometry = this->geometry();
    QPoint pos(screenGeometry.bottomLeft().x() + 8, screenGeometry.bottomLeft().y() - 48);
    backButton= new QPushButton("<", this);
    backButton->move(pos);
    backButton->resize(131, 41);

    toolboxButton= new QPushButton("Hide ToolBox", this);
    toolboxButton->resize(131, 41);
    QPoint pos2(screenGeometry.bottomRight().x() - 8 - toolboxButton->width(), screenGeometry.bottomRight().y() - 48);
    toolboxButton->move(pos2);

    QObject::connect(backButton, SIGNAL(clicked()), this, SLOT(back()));
    QObject::connect(toolboxButton, SIGNAL(clicked()), this, SLOT(toolboxButton_clicked()));
}
