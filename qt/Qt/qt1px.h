#ifndef QT1PX_H
#define QT1PX_H

#include <QtWidgets/QMainWindow>
#include <QBasicTimer>

#include "ui_qt1px.h"
#include "Qt1pCanvasWidget.h"

class Qt1px : public QMainWindow
{
	Q_OBJECT

public:
	Qt1px(QWidget *parent = 0);
	~Qt1px();

private:
	Ui::Qt1pxClass ui;

	Qt1pCanvasWidget *canvas;
	QBasicTimer timer_;

protected:
	void closeEvent(QCloseEvent *e);
	void timerEvent(QTimerEvent *e);

protected slots:
	void onButtonQuit();
};

#endif // QT1PX_H
