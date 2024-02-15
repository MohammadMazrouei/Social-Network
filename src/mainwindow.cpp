#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	this->setWindowState(Qt::WindowMaximized);
	db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName(addressDatabas);
	if(!db.open()) {
		QMessageBox::warning(this, "Database Problem!", "Not Successes To Open Database. \nPage : mainwindow -> Function : MainWindow");
		return;
	}
	QSqlQuery q;
    if(!q.exec("CREATE TABLE IF NOT EXISTS information(username VARCHAR(20), password VARCHAR(20), createAccount DATE, public BIT DEFAULT 1); ")) {
		QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Create Table Information. \nPage : mainwindow -> Function : MainWindow");
		db.close();
		return;
	}
	q.clear();
	db.close();
}

MainWindow::~MainWindow()
{
	delete ui;
}

bool MainWindow::check_username() {
	QString username = ui->le_username->text();
	if(username.size() <= 0 || username.size() > 20 || username == "information" || username == username_admin)
		return false;

	return true;
}

bool MainWindow::check_password() {
	QString password = ui->le_password->text();
	if(password.size() <= 0 || password.size() > 20)
		return false;

	return true;
}

void MainWindow::on_btn_login_clicked()
{
	QString username = ui->le_username->text();
	QString password = ui->le_password->text();
	QString pass = "";

	// check admin
	if(username == username_admin && password == password_admin) {
		admin_control * ac = new admin_control();
		ac->show();
		this->close();
		return;
	}
	// check information
	{
		if(!check_username()) {
			QMessageBox::warning(this, "Incorrect Name!", "Format Of Username Is Incorrect.");
			return;
		}
		if(!check_password()) {
			QMessageBox::warning(this, "Incorrect Pass!", "Format Of Password Is Incorrect.");
			return;
		}
	}

	if (!db.open()) {
		QMessageBox::warning(this, "Not Open Database!", "Not Successes To Open Database. \nPage : mainwindow -> Function : on_btn_login_clicked");
		return;
	}



	QSqlQuery q;
	if(!q.exec("SELECT * FROM information WHERE username = '"+username +"' ;")) {
		QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Login Search. \nPage : mainwindow -> Function : on_btn_login_clicked");
		db.close();
		return;
	}

	while(q.next())
		pass = q.value(1).toString();
	q.clear();
	db.close();

	if (pass == "")
		QMessageBox::warning(this, "Didn't Find Username!", "Username Don't Exist.");
	else if (pass != password)
		QMessageBox::warning(this, "Password!", "Password Is Incorrect.");
	else {
		QFile curUser("curUser.txt");
		if(!curUser.open(QFile::WriteOnly | QFile::Text )) {
			QMessageBox::warning(this, "File Problem!", "Didn't Open curUser.txt File. \nPage : mainwindow -> Function : on_btn_login_clicked");
			return;
		}
		QTextStream in(&curUser);
		in << username;
		curUser.close();

		personal_page * pg = new personal_page();
		pg->show();
		this->close();
	}

}

void MainWindow::on_btn_signup_clicked()
{
	// check information
	{
		if(!check_username()) {
			QMessageBox::warning(this, "Incorrect Name!", "Format Of Username Is Incorrect.");
			return;
		}
		if(!check_password()) {
			QMessageBox::warning(this, "Incorrect Pass!", "Format Of Password Is Incorrect.");
			return;
		}
	}

	if (!db.open()) {
		QMessageBox::warning(this, "Database Problem!", "Not Successes To Open Database. \nPage : mainwindow -> Function : on_btn_signup_clicked");
		return;
	}

	QString username = ui->le_username->text();
	QString password = ui->le_password->text();
	QString create_account = QDateTime::currentDateTime().toString("yyyy-MM-dd-HH-mm-ss");
	QString name = "";

	QSqlQuery q;
	if(!q.exec("SELECT * FROM information WHERE username = '"+username +"' ;")) {
		QMessageBox::warning(this, "Query Problem!", "Not Execute Database For Signup Search. \nPage : mainwindow -> Function : on_btn_signup_clicked");
		db.close();
		return;
	}

	while(q.next())
		name = q.value(0).toString();
	q.clear();

	if(name != "") {
		QMessageBox::warning(this, "Name Exists!", "This Username Is Already Taken.");
		db.close();
		return;
	}


    if(!q.exec("INSERT INTO information VALUES('"+username +"', '"+password +"', '"+create_account +"', 1); ")) {
		QMessageBox::warning(this, "Query Problem!", "Information Don't Add To Database. \nPage : mainwindow -> Function : on_btn_signup_clicked");
		db.close();
		return;
	}

	QFile curUser("curUser.txt");
	if(!curUser.open(QFile::WriteOnly | QFile::Text )) {
			QMessageBox::warning(this, "File Problem!", "Not Open curUser.txt File. \nPage : mainwindow -> Function : on_btn_sigtup_clicked");
			db.close();
			return;
	}
	QTextStream in(&curUser);
	in << username;
	curUser.close();
	QString tmp;
	// create table for new user
	{
		tmp = username + "_information";
		if(!q.exec("CREATE TABLE IF NOT EXISTS '"+tmp +"'"
				   "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
				   "username VARCHAR(20), "
				   "name VARCHAR(20), "
				   "lastname VARCHAR(20), "
				   "email VARCHAR(40), "
				   "phonenum VARCHAR(20), "
				   "bio VARCHAR(200), "
				   "birthday DATE, "
				   "gender INT,"
                   "followerN INT DEFAULT 0,"
				   "followingN INT DEFAULT 0,"
				   "public BIT DEFAULT 1); "))
		{
			QMessageBox::warning(this, "Database Problem!", "Didn't Create Table For New User");
			db.close();
			return;
		}
	}
	// create table for new user following
	{
		tmp = username + "_following";
		if(!q.exec("CREATE TABLE IF NOT EXISTS '"+tmp +"'"
				   "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
				   "username VARCHAR(20)); ")) {
			QMessageBox::warning(this, "Database Problem!", "Didn't Create Table For following of New User");
			db.close();
			return;
		}
	}
	// create table for new user followers
	{
		tmp = username + "_followers";
		if(!q.exec("CREATE TABLE IF NOT EXISTS '"+tmp +"'"
				   "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
				   "username VARCHAR(20)); ")) {
			QMessageBox::warning(this, "Database Problem!", "Didn't Create Table For followers of New User");
			db.close();
			return;
		}
	}
	// create table for new user posts
	{
		tmp = username + "_posts";
		if(!q.exec("CREATE TABLE IF NOT EXISTS '"+tmp +"'"
				   "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
                   "txt VARCHAR(128), "
				   "likeN INTEGER DEFAULT 0, "
				   "likeUsers TEXT,"
				   "createDate DATE); ")) {
			QMessageBox::warning(this, "Database Problem!", "Didn't Create Table For posts of New User!!");
			db.close();
			return;
		}
	}
	// create table for new user messages
	{
		tmp = username + "_messages";
		if(!q.exec("CREATE TABLE IF NOT EXISTS '"+tmp +"'"
				   "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
				   "txt VARCHAR(128), "
				   "usernameSend VARCHAR(20), "
				   "createDate DATE); ")) {
			QMessageBox::warning(this, "Database Problem!", "Didn't Create Table For messages of New User!!");
			db.close();
			return;
		}
	}
	q.clear();
	db.close();

	get_information * gi = new get_information();
	gi->show();
	this->close();
}
