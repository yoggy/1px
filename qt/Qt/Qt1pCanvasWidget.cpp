#include "Qt1pCanvasWidget.h"
#include <QPainter>

Qt1pCanvasWidget::Qt1pCanvasWidget(QWidget *parent)
	: QWidget(parent), frame_count_(0)
{
	setMinimumSize(200, 200);
	setSizePolicy(QSizePolicy::QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));

	QPalette pal(palette());
	pal.setColor(QPalette::Background, Qt::black);
	setAutoFillBackground(true);
	setPalette(pal);
}

Qt1pCanvasWidget::~Qt1pCanvasWidget()
{
}

void Qt1pCanvasWidget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);

	QPen pen;
	pen.setWidth(1);

	switch (frame_count_ % 4) {
	case 0:
		pen.setColor(Qt::red);
		break;
	case 1:
		pen.setColor(Qt::green);
		break;
	case 2:
		pen.setColor(Qt::blue);
		break;
	case 3:
		pen.setColor(Qt::black);
		break;
	}
	
	painter.setPen(pen);
	painter.drawPoint(QPoint(100, 100));

	frame_count_++;
}