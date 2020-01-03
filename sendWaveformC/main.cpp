#include "sendWaveformC.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	sendWaveformC w;
	w.show();
	return a.exec();
}
