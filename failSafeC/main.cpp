#include "qFailSafeWizard.h"

#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	qFailSafeWizard w;
	w.show();
	return a.exec();
}
