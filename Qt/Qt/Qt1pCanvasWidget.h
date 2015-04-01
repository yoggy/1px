#pragma once
#include <QWidget>

class Qt1pCanvasWidget : public QWidget
{
	Q_OBJECT

public:
	Qt1pCanvasWidget(QWidget *parent = 0);
	virtual ~Qt1pCanvasWidget();

protected:
	long frame_count_;

protected:
	void paintEvent(QPaintEvent *event);
};

