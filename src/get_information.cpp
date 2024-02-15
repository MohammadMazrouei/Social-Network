#include "get_information.h"
#include "ui_get_information.h"

get_information::get_information(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::get_information)
{
	ui->setupUi(this);
	this->setWindowState(Qt::WindowMaximized);
	username = getUsername();
}

get_information::~get_information()
{
	delete ui;
}

class get_information::info {
public:
	QString username;
	QString name;
	QString lastname;
	QString birthday;
	QString email;
	QString phonenum;
	QString bio;
	int gender;
};

void get_information::on_btn_cancel_clicked()
{
	MainWindow * mw = new MainWindow();
	mw->show();
	this->close();
}

void get_information::on_btn_ok_clicked()
{
	// check if all the entries are correct!!
	{
		if(!checkName()) {
			QMessageBox::warning(this, "Name", "Name Is Wrong!!!");
			return;
		}
		if(!checkLastname()) {
			QMessageBox::warning(this, "Lastname", "Lastname Is Wrong!!!");
			return;
		}
		if(!checkEmail()) {
			QMessageBox::warning(this, "Email", "Email Is Wrong!!!");
			return;
		}
		if(!checkPhonenum()) {
			QMessageBox::warning(this, "Phone Number", "Phone Number Is Wrong!!!");
			return;
		}
		if(!checkBio()) {
			QMessageBox::warning(this, "Biography", "Biography Is Wrong!!!");
			return;
		}
		if(!checkGender()) {
			QMessageBox::warning(this, "Gender", "Gender Is Wrong!!!");
			return;
		}
		if(!checkYear()) {
			QMessageBox::warning(this, "Year of birthday", "Year of birthday Is Wrong!!!");
			return;
		}
		if(!checkMonth()) {
			QMessageBox::warning(this, "Month of birthday", "Month of birthday Is Wrong!!!");
			return;
		}
		if(!checkDay()) {
			QMessageBox::warning(this, "Day of birthday", "Day of birthday Is Wrong!!!");
			return;
		}
	}
	info inf;
	// storing information
	{
		inf.name = ui->le_name->text();
		inf.lastname = ui->le_lastname->text();
		inf.bio = ui->le_biography->text();
		inf.birthday = ui->le_year->text() + "-" + ui->le_month->text() + "-" + ui->le_day->text();
		inf.email = ui->le_email->text();
		inf.phonenum = ui->le_phonenumber->text();
		if(ui->checkBox_female->isChecked()) // female = 1, male = 0
			inf.gender = 1;
		else
			inf.gender = 0;
	}

	db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName(addressDatabas);
	if(!db.open()) {
		QMessageBox::warning(this, "Database Problem!", "Not Successes To Open Database. \nPage : get_information -> Function : on_btn_ok_clicked");
		return;
	}

	QSqlQuery q;
	QString tmp = username + "_information";

	QString genderChar = QString::number(inf.gender);

    if(!q.exec("INSERT INTO '"+tmp +"' (username, name, lastname, email, phonenum, bio, birthday, gender) VALUES ('"+username +"', '"+ inf.name+"', '"+ inf.lastname+"', '"+ inf.email+"', '"+ inf.phonenum+"', '"+ inf.bio+"', '"+ inf.birthday+"', '"+ genderChar+"');")) {
            QMessageBox::warning(this, "Query Problem!", "Not Execute Database For Insert. \nPage : get_information -> Function : on_btn_ok_clicked");
            db.close();
            return;
        }

	q.clear();
	db.close();

	recommend * re = new recommend();
	re->show();
	this->close();

}

QString get_information::getUsername()
{
	QFile curUser("curUser.txt");
	if(!curUser.open(QFile::ReadOnly | QFile::Text )) {
		QMessageBox::warning(this, "File Problem!", "Not Execute Database For Read. \nPage : get_information -> Function : getUsername");
		return "";
	}
	QTextStream in(&curUser);
	QString username = in.readLine();//.split(' ')[0];
	curUser.close();
	return username;
}

bool get_information::checkName() {
	QString name = ui->le_name->text();

	if(name.size() <= 0 || name.size() >= 20)
		return false;

	return true;
}

bool get_information::checkLastname() {
	QString lastname = ui->le_lastname->text();

	if(lastname.size() <= 0 || lastname.size() >= 20)
		return false;

	return true;
}

bool get_information::checkEmail() {
	QString email = ui->le_email->text();

	if(email.size() <= 0 || email.size() >= 40)
		return false;

	return true;
}

bool get_information::checkPhonenum() {
	QString phonenum = ui->le_phonenumber->text();

	if(!isNumber(phonenum) || phonenum.size() <= 0 || phonenum.size() >= 20)
		return false;

	return true;
}

bool get_information::checkBio() {
	QString bio = ui->le_biography->text();

	if(bio.size() < 0 || bio.size() >= 200)
		return false;

	return true;
}

bool get_information::checkYear() {
	QString year = ui->le_year->text();

	if(!isNumber(year))
		return false;

	int yearInt = year.toInt();
	if(yearInt < 1000 || yearInt > 9999)
		return false;

	return true;
}

bool get_information::checkMonth() {
	QString month = ui->le_month->text();

	if(!isNumber(month))
		return false;

	int monthInt = month.toInt();
	if(monthInt < 1 || monthInt > 12)
		return false;

	return true;
}

bool get_information::checkDay() {
	QString day = ui->le_day->text();

	if(!isNumber(day))
		return false;

	int dayInt = day.toInt();
	if(dayInt < 1 || dayInt > 31)
		return false;

	return true;
}

bool get_information::checkGender() {
	if(!(ui->checkBox_female->isChecked() || ui->checkBox_male->isChecked()))
		return false;

	return true;
}

bool get_information::isNumber(QString str) {
	for(int i = 0; i < str.size(); ++i) {
		if(str[i] < '0' || str[i] > '9')
			return false;
	}
	return true;
}

void get_information::on_checkBox_male_clicked()
{
	ui->checkBox_male->setChecked(true);
	ui->checkBox_female->setChecked(false);
}

void get_information::on_checkBox_female_clicked()
{
   ui->checkBox_female->setChecked(true);
   ui->checkBox_male->setChecked(false);
}

