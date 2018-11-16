#include "CaChart.h"
#include "InputSignalDialog.h"
#include "CalciumRec.h"
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <qmessagebox.h>
#include <qlist.h>
#include <iostream>
#include <omp.h>

CaChart::CaChart(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	//connect(this, SIGNAL(Plot_Signals(int)), this, SLOT(PlotSignal_Slots(int)));
	
	chart = new Chart();
	chartView = new CaChartView(chart);
	chartPro = new Chart();
	chartViewPro = new CaChartView(chartPro);
	ui.mainLayout->addWidget(chartView);
	ui.proLayout->addWidget(chartViewPro);

	isZoomIn = false;
	isZoomOut = false;
	zoomIntimes = 0;
	ui.actionZoomIn->setCheckable(true);
	ui.actionZoomOut->setCheckable(true);
	pZoomGroup = new QActionGroup(this);
	pZoomGroup->setExclusive(true);
	pZoomGroup->addAction(ui.actionZoomIn);
	pZoomGroup->addAction(ui.actionZoomOut);
	pZoomGroup->addAction(ui.actionMouse);
	connect(pZoomGroup, SIGNAL(triggered(QAction*)), this, SLOT(Zoom_Slots(QAction*)));
	connect(this, SIGNAL(Clear_Signals()), this, SLOT(Clear_Slots()));
	
	//ui.FilterSlider.
	//QPoint pos = chartView->mapFromGlobal(this->pos());
	//QPointF posF= chart->mapToValue(pos, origSeries.get());

	//chart->mapToValue()
	//ui.mainLayout->
}

void CaChart::on_actionInput_Signal_triggered() {
	InputSignalDialog fileDialog(this);
	if (fileDialog.exec() == QDialog::Accepted) {
		signalPath = fileDialog.GetInputSignalPath();
		tempPath = fileDialog.GetTemplatePath();

		if (signalPath.isEmpty()) {
			QMessageBox::warning(this, "warning", "Please input signal path first!");
			return;
		}
		if (chart->series().size()){
			QMessageBox::information(this, "information", "There may be data in the chart, it will be cleared.");
			emit Clear_Signals();
		}
	}
	else return;

	std::string strOrig = signalPath.toStdString();
	std::string strTemp = tempPath.toStdString();

	//--------open orignal signal file----------------//
	FILE* fp = fopen((const char*)(strOrig.c_str()), "r");
	if (!fp) {
		printf("error write signal!\n");
		return;
	}
	double times = 0, tmpmax = -10000.0, tmpmin = 10000.0, tmpmax_x, tmpmin_x;
	char line[256];
	double tmpSignal=0;
	while (!feof(fp)) {
		times++;
		if (fgets(line, 256, fp) == 0) continue;
		sscanf(line, "%lf", &tmpSignal);
		origSignal.push_back(tmpSignal);
		if (tmpSignal < tmpmin) {
			tmpmin = tmpSignal;
			tmpmin_x = times;
		}
		if (tmpSignal > tmpmax) {
			tmpmax = tmpSignal;
			tmpmax_x = times;
		}
	}
	fclose(fp);

	//--------open template signal file----------------//
	fp = fopen((const char*)(strTemp.c_str()), "r");
	if (!fp) {
		printf("error write signal!\n");
		return;
	}
	//char line[256];
	tmpSignal=0;
	while (!feof(fp)) {
		if (fgets(line, 256, fp) == 0) continue;
		sscanf(line, "%lf", &tmpSignal);
		tempSignal.push_back(tmpSignal);
	}
	fclose(fp);
	//txt 5 lie
	/*Vec2d tmpSignal;
	double time, value, tmp1, tmp2, tmp3, tmpmax = -10000.0, tmpmin = 10000.0, tmpmax_x, tmpmin_x;
	while (!feof(fp)) {
		if (fgets(line, 256, fp) == 0) continue;
		sscanf(line, "%lf %lf %lf %lf %lf ", &tmpSignal(0), &tmpSignal(1), &tmp1, &tmp2, &tmp3);
		tmpSignal(0) *= 1000;
		origSignal.push_back(tmpSignal(1));
		if (tmpSignal(1) < tmpmin) {
			tmpmin = tmpSignal(1);
			tmpmin_x = tmpSignal(0);
		}
		if (tmpSignal(1) > tmpmax) {
			tmpmax = tmpSignal(1);
			tmpmax_x = tmpSignal(0);
		}
	}*/
	posMAX_x = tmpmax_x;
	posMAX_y = tmpmax;
	posMIN_x = tmpmin_x;
	posMIN_y = tmpmin;
	
	printf("Signal loading is complete.\n");

	//--------------plot-------------//
	if (origSignal.empty()) {
		QMessageBox::warning(this, "warning", "PlotSignal is empty!");
		return;
	}
	if (origSeries.get() == Q_NULLPTR) {
		origSeries = std::shared_ptr<QLineSeries>(new QLineSeries());
		connect(origSeries.get(), SIGNAL(pressed(QPointF)), this, SLOT(seriesHandleClickedPoint(QPointF)));
	}
	origSeries->setPen(QPen(Qt::blue, 1, Qt::SolidLine));//设置曲线显示效果

	for (int i = 0; i < origSignal.size(); ++i) {
		origSeries->append(i, origSignal[i]);
	}
	chart->legend()->hide();
	chart->addSeries(origSeries.get());
	chart->createDefaultAxes();
	//axisX=new QValueAxis(chart->axisX());
	chart->update();
}
//void CaChart::on_OpenfileBtn_clicked() {
//	signalPath = QFileDialog::getOpenFileName(this, tr("Open Signal File"), "",
//		tr(" (*.txt)"));
//	if (!signalPath.isEmpty()){
//		ui.SignalPathEdit->setText(signalPath);
//	}
//}

//void CaChart::on_InputSignalBtn_clicked() {
//	if (signalPath.isEmpty()){
//		QMessageBox::warning(this, "warning", "Please input signal path first!");
//		return;
//	}
//
//	origSignal.clear();
//	std::string str = signalPath.toStdString();
//	FILE* fp = fopen((const char*)(str.c_str()), "r");
//	if (!fp) {
//		printf("error write signal!\n");
//		return;
//	}
//
//	//origSignal.clear();
//	double time, value, tmp1, tmp2, tmp3, tmpmax = -10000.0, tmpmin = 10000.0, tmpmax_x, tmpmin_x;
//	char line[256];
//	Vec2d tmpSignal;
//	while (!feof(fp)) {
//		if (fgets(line, 256, fp) == 0) continue;
//		sscanf(line, "%lf %lf %lf %lf %lf ", &tmpSignal(0), &tmpSignal(1), &tmp1, &tmp2, &tmp3);
//		tmpSignal(0) *= 1000;
//		origSignal.push_back(tmpSignal);
//		if (tmpSignal(1) < tmpmin) {
//			tmpmin = tmpSignal(1);
//			tmpmin_x = tmpSignal(0);
//		}
//		if (tmpSignal(1) > tmpmax) {
//			tmpmax = tmpSignal(1);
//			tmpmax_x = tmpSignal(0);
//		}
//	}
//	posMAX_x = tmpmax_x;
//	posMAX_y = tmpmax;
//	posMIN_x = tmpmin_x;
//	posMIN_y = tmpmin;
//	//QValueAxis *axisX = new QValueAxis;
//	//axisX->setRange(0, std::floor(origSignal[origSignal.size() - 1](0)));
//	////axisY->setRange(std::floor(tmpmin), std::ceil(tmpmax+200));
//	//QValueAxis *axisY = new QValueAxis;
//	//axisY->setRange(std::floor(tmpmin), std::ceil(tmpmax));
//	//chart->setAxisX(axisX);
//	//chart->setAxisY(axisY);
//	chart->legend()->hide();
//	//axisX->setLabelFormat("%g");
//
//	printf("Signal loading is complete.\n");
//
//	//--------------plot-------------//
//	if (origSignal.empty()) {
//		QMessageBox::warning(this, "warning", "PlotSignal is empty!");
//		return;
//	}
//	if (origSeries.get()==Q_NULLPTR){
//		origSeries = std::shared_ptr<QLineSeries>(new QLineSeries());
//		connect(origSeries.get(), SIGNAL(pressed(QPointF)), this, SLOT(seriesHandleClickedPoint(QPointF)));
//	}
//	origSeries->setPen(QPen(Qt::blue, 1, Qt::SolidLine));//设置曲线显示效果
//
//	for (int i = 0; i < origSignal.size(); ++i) {
//		origSeries->append(origSignal[i](0), origSignal[i](1));
//	}
//	//chart->setlege
//	chart->addSeries(origSeries.get());
//	//chart->setAxisY(300);
//	chart->createDefaultAxes();
//	chart->update();
//}

//void CaChart::on_PlotOrigSignalBtn_clicked() {
//	if (origSignal.empty()) return;
//	origSeries = std::shared_ptr<QLineSeries>(new QLineSeries());
//	origSeries->setPen(QPen(Qt::blue, 1, Qt::SolidLine));//设置曲线显示效果
//
//	for (int  i = 0; i < origSignal.size(); ++i){
//		origSeries->append(origSignal[i](0), origSignal[i](1));
//	}
//	//QChart *chart = new QChart();
//	chart->legend()->hide();
//	chart->addSeries(origSeries.get());
//	chart->createDefaultAxes();
//	//chart->setTitle("Simple line chart example");
//	//chart->axisX()->setRange(0, 5000);
//	
//	//chart->update();
//	//chart->setScale();
//	printf("2\n");
//}

void CaChart::AverageFilter(std::vector<double> &oldSignal, std::vector<double> &newSignal) {
	newSignal = oldSignal;
	double tmpAverage=0.0;
	//auto be = clock();
	omp_set_dynamic(2);
	omp_set_num_threads(4);
#pragma omp parallel for
	for (int i = 2; i < oldSignal.size()-2; i++){
		double tmpAverage = 0.0;
		for (int j = -2; j < 3; j++){
				tmpAverage += oldSignal[i + j];
		}
		newSignal[i] = tmpAverage *0.2;
		tmpAverage = 0.0;
	}
	//auto en = clock();
	//std::cout << en-be << std::endl;
}

void CaChart::on_FilterBtn_clicked() {
	//VectorVec2d filterSignal;
	AverageFilter(origSignal, filterSignal);
	for (int i = 0; i < 10; i++)
	{
		AverageFilter(filterSignal, filterSignal);
	}
	
	filterSeries = std::shared_ptr<QLineSeries>(new QLineSeries());
	filterSeries->setPen(QPen(Qt::green, 1, Qt::SolidLine));//设置曲线显示效果
	for (int i = 0; i < filterSignal.size(); ++i) {
		filterSeries->append(i, filterSignal[i]);
	}
	chart->addSeries(filterSeries.get());
	chart->update();
	printf("3\n");
}

void CaChart::on_FilterSlider_sliderReleased() {
	int n = ui.FilterSlider->value();
	if (n == 0 && filterSeries.get() == Q_NULLPTR) return;
	filterSignal = origSignal;
	for (int i = 1; i <= n; i++){
		AverageFilter(filterSignal, filterSignal);
	}
	//chart->removeSeries(filterSeries.get());
	if (filterSeries.get() == Q_NULLPTR) {
		filterSeries = std::shared_ptr<QLineSeries>(new QLineSeries());
		connect(filterSeries.get(), SIGNAL(clicked(QPointF)), this, SLOT(seriesHandleClickedPoint(QPointF)));
	}
	auto be = clock();
	QList<QAbstractSeries*> tmpSeries = chart->series();
	for (int i = 0; i < tmpSeries.size(); ++i) {
		if (tmpSeries.at(i) == filterSeries.get()) {
			chart->removeSeries(filterSeries.get());
			filterSeries->clear();
		}
	}
	filterSeries->setPen(QPen(Qt::green, 1, Qt::SolidLine));//设置曲线显示效果
	for (int i = 0; i < filterSignal.size(); ++i) {
		filterSeries->append(i, filterSignal[i]);
	}
	//chart->removeSeries(filterSeries);
	chart->addSeries(filterSeries.get());
	if (chart->isZoomed()){
		chart->zoomReset();
	}
	chart->createDefaultAxes();
	chart->update();
	auto en = clock();
	std::cout << en-be << std::endl;
	printf("4\n");
}

void CaChart::on_ProcessBtn_clicked() {
	//std::string str = "E:\\11.txt";
	//FILE* fp = fopen((const char*)(str.c_str()), "r");
	//if (!fp) {
	//	printf("error write signal!\n");
	//	return;
	//}

	////origSignal.clear();
	//double tmpmax = -10000.0, tmpmin = 10000.0, tmpmax_x, tmpmin_x;
	//char line[256];
	//double tmpSignal = processedSignal[0];
	//while (!feof(fp)) {
	//	if (fgets(line, 256, fp) == 0) continue;
	//	sscanf(line, "%lf %lf %lf %lf %lf ", &tmpSignal(0), &tmpSignal(1), &tmp1, &tmp2, &tmp3);
	//	//tmpSignal(0) *= 1000;
	//	processedSignal.push_back(tmpSignal);
	//	if (tmpSignal(1) < tmpmin) {
	//		tmpmin = tmpSignal(1);
	//		tmpmin_x = tmpSignal(0);
	//	}
	//	if (tmpSignal(1) > tmpmax) {
	//		tmpmax = tmpSignal(1);
	//		tmpmax_x = tmpSignal(0);
	//	}
	//}
	//if (processedSignal.empty()) {
	//	QMessageBox::warning(this, "warning", "PlotSignal is empty!");
	//	return;
	//}
	
	//std::vector<double> origSignalVector1, tmpSignalVector1;
	processedSignal.clear();
	std::shared_ptr<CalciumSignalRec> calciumSignalRec = std::shared_ptr<CalciumSignalRec>(new CalciumSignalRec());
	calciumSignalRec->SetInputs(origSignal, tempSignal);
	calciumSignalRec->Update();
	//std::vector<float>Recapp;
	calciumSignalRec->GetOutput(processedSignal);
	//processedSignal = Recapp;
	//calciumSignalRec->InputCas_Temp;
	double tmpmax = -10000.0, tmpmin = 10000.0, tmpmax_x, tmpmin_x;
	double tmpSignal = processedSignal[0];
	for (int i = 0; i < processedSignal.size(); ++i){
		if (processedSignal[i] < tmpmin) {
			tmpmin = processedSignal[i];
			tmpmin_x = i;
		}
		if (processedSignal[i] > tmpmax) {
			tmpmax = processedSignal[i];
			tmpmax_x = i;
		}

	}
	posMAXp_x = tmpmax_x;
	posMAXp_y = tmpmax;
	posMINp_x = tmpmin_x;
	posMINp_y = tmpmin;
	
	if (processedSeries.get() == Q_NULLPTR) {
		processedSeries = std::shared_ptr<QLineSeries>(new QLineSeries());
		connect(processedSeries.get(), SIGNAL(pressed(QPointF)), this, SLOT(seriesHandleClickedPoint(QPointF)));
	}
	processedSeries->setPen(QPen(Qt::blue, 1, Qt::SolidLine));//设置曲线显示效果

	for (int i = 0; i < processedSignal.size(); ++i) {
		processedSeries->append(i, processedSignal[i]);
	}
	chartPro->legend()->hide();
	//chartPro->createDefaultAxes();
	axisX = new QValueAxis();
	axisX->setRange(0, origSignal.size()-1);
	
	axisY = new QValueAxis();
	axisY->setRange(0, posMAXp_y);
	
	chartPro->addSeries(processedSeries.get());
	chartPro->setAxisX(axisX, processedSeries.get());
	chartPro->setAxisY(axisY, processedSeries.get());
	//
	//chart->addSeries(processedSeries.get());
	chartPro->update();
	
}

void CaChart::seriesHandleClickedPoint(QPointF po) {
	if (isZoomIn == false && isZoomOut == false) return;

	int pox = std::max(0, (int)po.x());
	int poy = std::max(0, (int)po.y());
	//chart
	QPointF poMax(posMAX_x, posMAX_y);
	QPointF poMin(posMIN_x, posMIN_y);
	QPointF poofma = chart->mapToPosition(poMax, origSeries.get());
	QPointF poofmi = chart->mapToPosition(poMin, origSeries.get());
	QPointF poof= chart->mapToPosition(po,origSeries.get());

	//charPro
	QPointF poMax_pro(posMAXp_x, posMAXp_y);
	QPointF poMin_pro(posMINp_x, posMINp_y);
	QPointF poofma_pro = chartPro->mapToPosition(poMax_pro, processedSeries.get());
	QPointF poofmi_pro = chartPro->mapToPosition(poMin_pro, processedSeries.get());
	//QPointF poof = chart->mapToPosition(po, origSeries.get());
	if (isZoomIn){
		chart->zoomIn(QRectF(poof.x()-100, poofma.y(), 200, abs(poofma.y()-poofmi.y())));
		chartPro->zoomIn(QRectF(poof.x() - 100, poofma_pro.y(), 200, abs(poofma_pro.y() - poofmi_pro.y())));
	}
	else if (isZoomOut) {
		chart->zoomReset();
		chartPro->zoomReset();
	}
	
	//chart->zoom(1.1);
	//chart->zoomIn();
	
	chart->update();
	chartPro->update();
	printf("%f,%f\n", po.x(), po.y());
}

void CaChart::on_actionClear_triggered() {
	emit Clear_Signals();
}

void CaChart::Clear_Slots() {
	origSignal.clear();
	filterSignal.clear();
	processedSignal.clear();

	if (origSeries.get() != Q_NULLPTR) {
		origSeries->clear();
		origSeries = nullptr;
	}

	if (filterSeries.get() != Q_NULLPTR) {
		filterSeries->clear();
		filterSeries = nullptr;
	}

	if (processedSeries.get() != Q_NULLPTR) {
		processedSeries->clear();
		processedSeries = nullptr;
	}
	posMAX_x = 0;
	posMAX_y = 0;
	posMIN_x = 0;
	posMIN_y = 0;
	zoomIntimes = 0;
	chart->zoomReset();
	chart->removeAllSeries();
	//chart->close();
}

void CaChart::Zoom_Slots(QAction *action) {
	if (action == ui.actionZoomIn ) {
		isZoomIn = action->isChecked();
		isZoomOut = false;
		
	}
	else if(action==ui.actionZoomOut){
		isZoomIn = false;
		isZoomOut = action->isChecked();
	}
	else
	{
		isZoomIn = false;
		isZoomOut = false;
	}
	printf("%d  %d\n", isZoomIn, isZoomOut);
}

void CaChart::on_actionSave_ProcessedSignal_triggered() {
	FILE* fp = fopen("E:\\11.txt", "w");
	if (!fp) return;
	for (size_t i = 0; i < filterSignal.size(); ++i) {
		fprintf(fp, "%lf %lf\n", i, filterSignal[i]);
	}
	fclose(fp);
}