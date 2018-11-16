#include "InputSignalDialog.h"
#include <qstring.h>
#include <qfiledialog.h>
#include <qmessagebox.h>
InputSignalDialog::InputSignalDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	this->setFixedSize(431, 122);
	//ui.OKButton->setEnabled(false);
	connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

InputSignalDialog::~InputSignalDialog()
{
}

void InputSignalDialog::on_OpenfileBtn_clicked() {
	QString signalPath = QFileDialog::getOpenFileName(this, tr("Open Signal File"), "",
		tr(" (*.txt)"));
	if (!signalPath.isEmpty()) {
		ui.SignalPathEdit->setText(signalPath);
	}
}

void InputSignalDialog::on_OpenTempBtn_clicked() {
	QString TemplatePath = QFileDialog::getOpenFileName(this, tr("Open Signal File"), "",
		tr(" (*.txt)"));
	if (!TemplatePath.isEmpty()) {
		ui.TempPathEdit->setText(TemplatePath);
	}
}

QString InputSignalDialog::GetInputSignalPath() {
	return ui.SignalPathEdit->text();
}

QString InputSignalDialog::GetTemplatePath() {
	return ui.TempPathEdit->text();
}
