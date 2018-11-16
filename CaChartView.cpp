#include "CaChartView.h"
#include <qchartview.h>
CaChartView::CaChartView(QChart *chart, QWidget *parent)
	: QChartView(chart, parent)
{
	m_isTouching = false;
	setRubberBand(QChartView::RectangleRubberBand);
}

CaChartView::~CaChartView()
{
}

//bool CaChartView::viewportEvent(QEvent *event) {
//	if (event->type() == QEvent::TouchBegin) {
//		// By default touch events are converted to mouse events. So
//		// after this event we will get a mouse event also but we want
//		// to handle touch events as gestures only. So we need this safeguard
//		// to block mouse events that are actually generated from touch.
//		m_isTouching = true;
//
//		// Turn off animations when handling gestures they
//		// will only slow us down.
//		chart()->setAnimationOptions(QChart::NoAnimation);
//	}
//	return QChartView::viewportEvent(event);
//}

//void CaChartView::mousePressEvent(QMouseEvent *event)
//{
//	if (m_isTouching)
//		return;
//	QChartView::mousePressEvent(event);
//}
//
//void CaChartView::mouseMoveEvent(QMouseEvent *event)
//{
//	if (m_isTouching)
//		return;
//	QChartView::mouseMoveEvent(event);
//}

void CaChartView::mouseMoveEvent(QMouseEvent *event) 
{
	//event->localPos();
	//event->screenPos
	//printf("%f,%f\n", event->screenPos().x(), event->screenPos().y());
	QChartView::mouseReleaseEvent(event);
}

void CaChartView::mouseReleaseEvent(QMouseEvent *event)
{
	//if (m_isTouching)
	//	m_isTouching = false;

	// Because we disabled animations when touch event was detected
	// we must put them back on.
	//chart()->setAnimationOptions(QChart::SeriesAnimations);

	QChartView::mouseReleaseEvent(event);
}

//void CaChartView::keyPressEvent(QKeyEvent *event)
//{
//	switch (event->key()) {
//	case Qt::Key_Plus:
//		chart()->zoomIn();
//		break;
//	case Qt::Key_Minus:
//		chart()->zoomOut();
//		break;
//		//![1]
//	case Qt::Key_Left:
//		chart()->scroll(-10, 0);
//		break;
//	case Qt::Key_Right:
//		chart()->scroll(10, 0);
//		break;
//	case Qt::Key_Up:
//		chart()->scroll(0, 10);
//		break;
//	case Qt::Key_Down:
//		chart()->scroll(0, -10);
//		break;
//	default:
//		QGraphicsView::keyPressEvent(event);
//		break;
//	}
//}


//void CaChartView::handleClickedPoint(const QPointF &point)
//{
	//QPointF clickedPoint = point;
	//printf("%f,%f\n", clickedPoint.x(), clickedPoint.y());
	// Find the closest point from series 1
	//QPointF closest(INT_MAX, INT_MAX);
	//qreal distance(INT_MAX);
	//const auto points = m_scatter->points();
	//for (const QPointF &currentPoint : points) {
	//	qreal currentDistance = qSqrt((currentPoint.x() - clickedPoint.x())
	//		* (currentPoint.x() - clickedPoint.x())
	//		+ (currentPoint.y() - clickedPoint.y())
	//		* (currentPoint.y() - clickedPoint.y()));
	//	if (currentDistance < distance) {
	//		distance = currentDistance;
	//		closest = currentPoint;
	//	}
	//}

	//// Remove the closes point from series 1 and append it to series 2
	//m_scatter->remove(closest);
	//m_scatter2->append(closest);
//}

