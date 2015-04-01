#include "qt1px.h"
#include <QtWidgets/QApplication>
#include <QKeyEvent>

Qt1px *main_w = NULL;

class QT1pxApp : public QApplication
{
public:
	QT1pxApp(int argc, char** argv) : QApplication(argc, argv) {};
	~QT1pxApp() {};

	// global keypress hook
	bool notify(QObject* obj, QEvent *evt) {
		if (evt->type() == QEvent::KeyPress) {
			QKeyEvent *key_evt = (QKeyEvent *)evt;
			switch (key_evt->key()) {
			case Qt::Key_Escape:
				QMetaObject::invokeMethod(main_w, "close");
				break;
			}
			return true;
		}
		return QApplication::notify(obj, evt);
	};
};

int main(int argc, char *argv[])
{
	QT1pxApp a(argc, argv);
	Qt1px w;
	main_w = &w;
	w.show();
	return a.exec();
}
