#include "qt1px.h"

#include <QCloseEvent>

Qt1px::Qt1px(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	canvas = new Qt1pCanvasWidget(this);
	ui.frameLayout->addWidget(canvas);

	timer_.start(250, this);
}

Qt1px::~Qt1px()
{

}

void Qt1px::closeEvent(QCloseEvent *e)
{
	timer_.stop();
	e->accept();
}

void Qt1px::timerEvent(QTimerEvent *e)
{
	canvas->update();
}
