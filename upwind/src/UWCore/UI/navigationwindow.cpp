#include "navigationwindow.h"
#include "ui_navigationwindow.h"
#include <QResizeEvent>

NavigationWindow::NavigationWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NavigationWindow)
{
    ui->setupUi(this);
    this->setParent(parent);
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);

    m_backButton = new QPushButton("<", this);
    m_toolboxButton = new QPushButton("Hide ToolBox", this);
    m_toolboxButton->adjustSize();

    QObject::connect(m_backButton, SIGNAL(clicked()), this, SLOT(back()));
    QObject::connect(m_toolboxButton, SIGNAL(clicked()), this, SLOT(toggleToolbox()));
}

NavigationWindow::~NavigationWindow(){
    delete ui;
}

void NavigationWindow::back(){
    emit goBack();
}

void NavigationWindow::toggleToolbox(){
    if(m_toolboxButton->text() == "Hide ToolBox")
        m_toolboxButton->setText("Show ToolBox");
    else
        m_toolboxButton->setText("Hide ToolBox");

    m_toolboxButton->adjustSize();
    m_toolboxButton->move(QPoint(geometry().bottomRight().x() - 8 - m_toolboxButton->width(), geometry().bottomRight().y() - 48));
    emit toolboxButtonClicked();
}

void NavigationWindow::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);

    m_backButton->move(QPoint(geometry().bottomLeft().x() + 8, geometry().bottomLeft().y() - 48));
    m_toolboxButton->move(QPoint(geometry().bottomRight().x() - 8 - m_toolboxButton->width(), geometry().bottomRight().y() - 48));
}
