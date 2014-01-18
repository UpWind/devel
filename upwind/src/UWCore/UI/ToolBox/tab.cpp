#include "tab.h"


// Initializing the static  vbles
QString Tab::actual_profile = NULL;
QButtonGroup * Tab::group = new QButtonGroup();

Tab::Tab(QWidget * parent)
{
    count = 0;
    v_layout =  new QVBoxLayout(parent);
    list = QMap<QString *, QString *>();
    this->setLayout(v_layout);
}

bool Tab::add_profile(QString * profile_name, QString * image_path, QWidget * parent){

	(void)parent;

    bool add_profile = false;

    if(get_count()<MAX_PROFILE){
        //Create button & insert in the list
        QIcon *icon = new QIcon(*image_path);
        QPushButton *button = new QPushButton(*icon,*profile_name);
        button->setMinimumSize(130,50);
        button->setIconSize(QSize(45,45));
        button->setCheckable(true);

        //Add button to the group of buttons
        Tab::group->addButton(button);

        v_layout->addWidget(button);

        list.insert(profile_name, image_path);

        //Increase the count
        count++;

        add_profile = true;
    }

    return add_profile;
}

bool Tab::remove_profile_from_list(QString * profile_name){

    bool remove_profile_from_list = false;

    QMapIterator<QString *, QString *> it(list);

    //bool salir = false;
    while(it.hasNext() && *(it.next().key())!=*profile_name){
        qDebug()<<"Clave: "<<*it.key()<<" Valor: "<<*it.value();
    }

    if(*it.key()==*profile_name){
        qDebug()<<"Profile_name: "<<*profile_name<<"Key: "<<*it.key()<<" Value: "<<*it.value();
        list.remove(it.key());
        remove_profile_from_list =true;
    }

    return remove_profile_from_list;
}

bool Tab::remove_profile_from_button_group(QString * profile_name){

    bool remove_profile_from_button_group = false;

    QList<QAbstractButton *> buttons_list = group->buttons();
    QListIterator<QAbstractButton *> it(buttons_list);

    while(it.hasNext() && remove_profile_from_button_group != true){
          QAbstractButton * button = it.next();
          if(button->text() == profile_name){
              v_layout->removeWidget(button);
              group->removeButton(button);
              button->~QAbstractButton();
              remove_profile_from_button_group =true;
          }
      }

    return remove_profile_from_button_group;
}

bool Tab::remove_profile(QString * profile_name){

    bool remove_profile = false;

    // The group vble has to contain the same elements than in list vble
    remove_profile = remove_profile_from_button_group(profile_name);
    remove_profile = remove_profile_from_list(profile_name);

    count--;

    return remove_profile;
}


QString Tab::get_icon_path(QString * profile_name){

    QString  * get_icon_path = new QString();

    QMapIterator<QString *, QString *> it(list);

    while(it.hasNext() && *it.next().key()!= *profile_name){
        //true;
    }
    if(*it.key()==*profile_name)
        get_icon_path = it.value();

    return *get_icon_path;
}


int Tab::get_count(){
    return this->count;
}

QString Tab::get_actual_profile(){
    return Tab::actual_profile;
}

void Tab::set_actual_profile(QString actual_profile){
    Tab::actual_profile = actual_profile;
}

QAbstractButton * Tab::get_profile_button(QString profile_name){
    QAbstractButton * get_profile = NULL;

    QList<QAbstractButton *> buttons_list = group->buttons();
    QListIterator<QAbstractButton *> it(buttons_list);

    while(it.hasNext() && get_profile == NULL){
          QAbstractButton * button = it.next();
          if(button->text() == profile_name){
              get_profile = button;
          }
      }

    return get_profile;
}
