#pragma once

#include <QtWidgets/QMainWindow>
#include <QtCharts>
QT_CHARTS_USE_NAMESPACE
#include "ui_CaChart.h"
#include "basetypes.h"
#include "Chart.h"
#include "CaChartView.h"
#include <memory>
#include <string>
#include <vector>
#include <qstring.h>
class CaChart : public QMainWindow
{
	Q_OBJECT

public:
	CaChart(QWidget *parent = Q_NULLPTR);

private slots:
	//void on_InputSignalBtn_clicked();
	//void on_PlotOrigSignalBtn_clicked();
	void on_FilterBtn_clicked();
	void on_FilterSlider_sliderReleased();
	//void on_OpenfileBtn_clicked();
	void on_ProcessBtn_clicked();
	void seriesHandleClickedPoint(QPointF);
	void on_actionClear_triggered();
	void Clear_Slots();
	void on_actionInput_Signal_triggered();
	//void on_actionZoomIn_toggled(bool);
	//void on_actionZoomOut_toggled(bool);
	void Zoom_Slots(QAction *);
	void on_actionSave_ProcessedSignal_triggered();
	//void on_pZoomGroup_triggered(QAction *);
	//void PlotSignal_Slots(int);//int n=1 plot origsignal, n=2 plot filtersignal, n=3 plot processedsignal

signals:
	//void Plot_Signals(int);
	void Clear_Signals();

private:
	Ui::CaChartClass ui;

	std::vector<double> origSignal;
	std::vector<double> tempSignal;
	std::vector<double> filterSignal;
	std::vector<double> processedSignal;
	//VectorVec2d origSignal;
	//VectorVec2d filterSignal;
	//VectorVec2d processedSignal;

	QString signalPath, tempPath;
	Chart *chart, *chartPro;
	CaChartView *chartView, *chartViewPro;
	QValueAxis *axisX,*axisY;

	QActionGroup *pZoomGroup;
	std::shared_ptr< QLineSeries> origSeries;
	std::shared_ptr< QLineSeries> filterSeries;
	std::shared_ptr< QLineSeries> processedSeries;
	double posMAX_x, posMAX_y, posMIN_x, posMIN_y;//chart
	double posMAXp_x, posMAXp_y, posMINp_x, posMINp_y;//chartPro
	int zoomIntimes;
	bool isZoomIn, isZoomOut;
	bool Readtxt(std::string &);
	void AverageFilter(std::vector<double> &, std::vector<double> &);
};
