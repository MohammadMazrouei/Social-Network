#include "add_post.h"
#include "ui_add_post.h"

add_post::add_post(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::add_post)
{
	ui->setupUi(this);
	this->setWindowState(Qt::WindowMaximized);
	db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName(addressDatabas);
	get_username();
}

add_post::~add_post()
{
	delete ui;
}

bool add_post::check_post()
{
	QString text = QString(ui->pte_newpost->toPlainText());
	if (text.length() > 128 || text.length() == 0)
		return false;

	return true;
}

void add_post::get_username()
{
	QFile curUser("curUser.txt");
	if(!curUser.open(QFile::ReadOnly | QFile::Text )) {
			QMessageBox::warning(this, "File Problem!", "Not Open curUser.txt File. \nPage : add_post -> Function : get_username");
			return;
	}
	QTextStream out(&curUser);
	out >> username;
	curUser.close();
}

void add_post::on_btn_ok_clicked()
{
	if (!check_post()) {
		QMessageBox::warning(this, "Long Post!", "Your Post Must Less Than 128 Characters And Not Empty.");
		return;
	}

	QString text = QString(ui->pte_newpost->toPlainText());
	QString create_date = QDateTime::currentDateTime().toString("yyyy-MM-dd-HH-mm-ss");
	QString table_name = username + "_posts";

	if(!db.open()) {
		QMessageBox::warning(this, "Database Problem!", "Not Successes To Open Database. \nPage : add_post -> Function : on_btn_ok_clicked");
		return;
	}
	QSqlQuery q;
	if(!q.exec("INSERT INTO '"+table_name +"'(txt, createDate) VALUES('"+text +"', '"+create_date +"'); ")) {
		QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Insert Information. \nPage : add_post -> Function : on_btn_ok_clicked");
		db.close();
		return;
	}
	q.clear();
	db.close();

	personal_page * pg = new personal_page();
	pg->show();
	this->close();
}

void add_post::on_btn_cancel_clicked()
{
	personal_page * pg = new personal_page();
	pg->show();
	this->close();
}

void add_post::on_btn_clear_clicked()
{
	ui->pte_newpost->clear();
}
