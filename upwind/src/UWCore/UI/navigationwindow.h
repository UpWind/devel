#ifndef NAVIGATIONWINDOW_H
#define NAVIGATIONWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QPainter>
class CoastLine;

namespace Ui {
class NavigationWindow;
}

class NavigationWindow : public QWidget
{
    Q_OBJECT

public:
    explicit NavigationWindow(QWidget *parent = 0);
    ~NavigationWindow();
    void addButtons();

public slots:
    void back();
    void toolboxButton_clicked();

signals:
    void goBack();
    void toggleToolbox();

private:
    Ui::NavigationWindow *ui;
    QPushButton *backButton;
    QPushButton *toolboxButton;
    CoastLine *coastLine;
};

#endif // NAVIGATIONWINDOW_H
