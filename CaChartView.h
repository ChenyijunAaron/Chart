#pragma once
#include<qchartview.h>
#include <QtCharts/QChartView>
#include <qchart.h>
QT_CHARTS_USE_NAMESPACE
class CaChartView : public QChartView
{
	Q_OBJECT

public:
	CaChartView(QChart *chart, QWidget *parent=0);
	~CaChartView();

protected:
//	bool viewportEvent(QEvent *event);
	//void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
//	void keyPressEvent(QKeyEvent *event);
	//void handleClickedPoint(const QPointF &point);

private:
	bool m_isTouching;
};
