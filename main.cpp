#include "CaChart.h"
#include <QtWidgets/QApplication>
QT_CHARTS_USE_NAMESPACE
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	CaChart w;
	w.show();
	return a.exec();
}
