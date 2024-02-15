#include "admin_control.h"
#include "ui_admin_control.h"

admin_control::admin_control(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::admin_control)
{
	ui->setupUi(this);
	this->setWindowState(Qt::WindowMaximized);
}

admin_control::~admin_control()
{
	delete ui;
}

void admin_control::on_btn_logout_clicked()
{
	MainWindow * mw = new MainWindow();
	mw->show();
	this->close();
}
