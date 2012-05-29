#ifndef MAINMENU_H_
#define MAINMENU_H_


#include <QtGui>
#include "../ui_mainmenu.h"

class MainMenu : public QWidget
{
    Q_OBJECT

public:
    MainMenu(QWidget *parent = 0);
    ~MainMenu();

private:
    Ui::mainMenuDLG ui;
    QLabel *logo;

signals:
    void close();
    void showChart();
    void showSettings();

public slots:
    Ui::mainMenuDLG* getUI();
    void about();
    void exit();
    void db();
    void settings();
};

#endif /* MAINMENU_H_ */
