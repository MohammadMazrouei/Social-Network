#include "show_post.h"
#include "ui_show_post.h"

show_post::show_post(QString username, int id,QWidget *parent) :
	QWidget(parent),
	ui(new Ui::show_post)
{
	ui->setupUi(this);
	this->setWindowState(Qt::WindowMaximized);
	this->username = username;
	this->id = id;
	db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName(addressDatabas);
	show_postt();
}

show_post::~show_post()
{
	delete ui;
}

void show_post::show_postt()
{
	QString table_name = username + "_posts";

	if(!db.open()) {
		QMessageBox::warning(this, "Database Problem!", "Not Successes To Open Database. \nPage : show_post -> Function : show_postt");
		return;
	}
	QSqlQuery q;
	QString idd = QString::number(id);
	if(!q.exec("SELECT txt, createDate FROM '"+table_name +"' WHERE id = '"+idd +"'; ")) {
		QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Get Post. \nPage : show_post -> Function : show_postt");
		db.close();
		return;
	}

	QString text, create_date;
	while(q.next()) {
		text = q.value(0).toString();
		create_date = q.value(1).toString();
	}

	q.clear();
	db.close();

	ui->pte_showpost->setPlainText(text);
	ui->label_date->setText(create_date);
}

void show_post::on_btn_back_clicked()
{
	personal_page * pg = new personal_page();
	pg->show();
	this->close();
}

void show_post::on_btn_edit_clicked()
{
	edit_post * ep = new edit_post(username, id);
	ep->show();
	this->close();
}

void show_post::on_btn_delete_clicked()
{
	QString table_name = username + "_posts";

	if(!db.open()) {
		QMessageBox::warning(this, "Database Problem!", "Not Successes To Open Database. \nPage : show_post -> Function : on_btn_delete_clicked");
		return;
	}
	QSqlQuery q;
	QString idd = QString::number(id);
	if(!q.exec("DELETE FROM '"+table_name +"' WHERE id = '"+idd +"'; ")) {
		QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Get Post. \nPage : show_post -> Function : on_btn_delete_clicked");
		db.close();
		return;
	}

	q.clear();
	db.close();

	personal_page * pg = new personal_page();
	pg->show();
	this->close();
}
