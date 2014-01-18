#ifndef TAB_H
#define TAB_H

#include <QWidget>
#include <QButtonGroup>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMap>
#include <QMapIterator>
#include <QDebug>

#define MAX_PROFILE 5

class Tab : public QWidget
{
public:
    Tab(QWidget * parent = 0);
    static QString actual_profile;
    static QButtonGroup * group;
    int count; //Number of QPushbuttons. Can't be larger than max_profile.
    QVBoxLayout * v_layout;
    QMap<QString *, QString *> list; /* <BUTTON_NAME *, IMAGE PATH *>*/

    //Functions to manage the list
    bool add_profile(QString * profile_name, QString * image_path, QWidget * parent = 0);
    bool remove_profile(QString * profile_name);
    bool remove_profile_from_list(QString * profile_name);
    bool remove_profile_from_button_group(QString * profile_name);

    //Functions to get the icon path of a profile button
    QString get_icon_path(QString * profile_name);

    int get_count();
    static QString get_actual_profile();
    static void set_actual_profile(QString actual_profile);
    static QAbstractButton * get_profile_button(QString profile_name);

public slots:
};

#endif // TAB_H
