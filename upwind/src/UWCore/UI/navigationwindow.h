#ifndef NAVIGATIONWINDOW_H
#define NAVIGATIONWINDOW_H

#include <QWidget>
#include <QPushButton>

namespace Ui {
class NavigationWindow;
}

class NavigationWindow : public QWidget
{
    Q_OBJECT

public:
    explicit NavigationWindow(QWidget *parent = 0);
    ~NavigationWindow();

protected:
    void resizeEvent(QResizeEvent *event);

public slots:
    void back();
    void toggleToolbox();

signals:
    void goBack();
    void toolboxButtonClicked();

private:
    QPushButton *m_backButton;
    QPushButton *m_toolboxButton;
};

#endif // NAVIGATIONWINDOW_H
