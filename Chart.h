#ifndef CHART_H
#define CHART_H

#include <QChart>
QT_BEGIN_NAMESPACE
class QGestureEvent;
QT_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE
class Chart : public QChart
{
	Q_OBJECT

public:
	explicit Chart(QGraphicsItem *parent = 0, Qt::WindowFlags wFlags = 0);
	~Chart();

protected:
	//bool sceneEvent(QEvent *event);
	//bool mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
	//bool gestureEvent(QGestureEvent *event);

private:
};

#endif // CHART_H
