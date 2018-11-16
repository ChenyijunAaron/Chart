#pragma once

#include <QDialog>
#include "ui_InputSignalDialog.h"

class InputSignalDialog : public QDialog
{
	Q_OBJECT

public:
	InputSignalDialog(QWidget *parent = Q_NULLPTR);
	~InputSignalDialog();
	QString GetInputSignalPath();
	QString GetTemplatePath();

private slots:
	void on_OpenfileBtn_clicked();
	void on_OpenTempBtn_clicked();
	

signals:
	void InputFinish_Signal();

private:
	Ui::InputSignalDialog ui;
	//QString signalPath;
};
