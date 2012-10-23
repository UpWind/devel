#include "instrumentview.h"

InstrumentView::InstrumentView(QWidget *parent){
    this->setParent(parent);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFrameShape(QFrame::NoFrame);

    setMouseTracking(false);
    m_handling = false;
}

void InstrumentView::mouseMoveEvent(QMouseEvent *event){
    if (m_handling)
        emit mouseMoved(event);
}

void InstrumentView::mouseDoubleClickEvent (QMouseEvent *event){
    emit mouseDoubleClicked(event);
}

void InstrumentView::mousePressEvent(QMouseEvent *event){
    m_handling= true;
    emit mousePressed(event);
}

void InstrumentView::mouseReleaseEvent (QMouseEvent *event){
    m_handling = false;
	(void)event;
}
