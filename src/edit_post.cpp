#include "edit_post.h"
#include "ui_edit_post.h"

edit_post::edit_post(QString username, int id, QWidget *parent) :
	QWidget(parent),
	ui(new Ui::edit_post)
{
	ui->setupUi(this);
	this->setWindowState(Qt::WindowMaximized);
	this->username = username;
	this->id = id;
	db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName(addressDatabas);
	show_postt();
}

edit_post::~edit_post()
{
	delete ui;
}

void edit_post::show_postt()
{
	QString table_name = username + "_posts";

	if(!db.open()) {
		QMessageBox::warning(this, "Database Problem!", "Not Successes To Open Database. \nPage : edit_post -> Function : show_postt");
		return;
	}
	QSqlQuery q;
	QString idd = QString::number(id);
	if(!q.exec("SELECT * FROM '"+table_name +"' WHERE id = '"+idd +"'; ")) {
		QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Get Post. \nPage : edit_post -> Function : show_postt");
		db.close();
		return;
	}

	QString text;
	while(q.next())
		text = q.value(1).toString();

	q.clear();
	db.close();

	ui->pte_editpost->setPlainText(text);
}

void edit_post::on_btn_cancel_clicked()
{
	show_post * sp = new show_post(username, id);
	sp->show();
	this->close();
}

void edit_post::on_btn_save_clicked()
{
	QString table_name = username + "_posts";
	QString text = QString(ui->pte_editpost->toPlainText());

	if(!db.open()) {
		QMessageBox::warning(this, "Database Problem!", "Not Successes To Open Database. \nPage : edit_post -> Function : on_btn_save_clicked");
		return;
	}
	QSqlQuery q;
	QString idd = QString::number(id);
	if(!q.exec("UPDATE '"+table_name +"' SET txt = '"+text +"' WHERE id = '"+idd +"'; ")) {
		QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Get Post. \nPage : edit_post -> Function : on_btn_save_clicked");
		db.close();
		return;
	}

	q.clear();
	db.close();

	show_post * sp = new show_post(username, id);
	sp->show();
	this->close();
}
