#include "change_profile.h"
#include "ui_change_profile.h"

change_profile::change_profile(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::change_profile)
{
	ui->setupUi(this);
	this->setWindowState(Qt::WindowMaximized);
    username = getUsername();
    setData();
}

change_profile::~change_profile()
{
	delete ui;
}

class change_profile::info {
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

void change_profile::on_btn_cancel_clicked()
{
	setting * se = new setting();
	se->show();
	this->close();
}

void change_profile::on_btn_ok_clicked()
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
    bool usernameChanged = false;
	// storing information
	{
        inf.username = ui->le_username->text();
        if(username != inf.username) usernameChanged = true;
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
		QMessageBox::warning(this, "Database Problem!", "Not Successes To Open Database. \nPage : change_profile -> Function : on_btn_ok_clicked");
		return;
	}

	QSqlQuery q;
	QString genderChar = QString::number(inf.gender);

	QString tmp = username + "_information";
    if(!q.exec("UPDATE '"+tmp +"' SET username = '"+inf.username +"', name = '"+inf.name +"', lastname = '"+inf.lastname +"', email = '"+inf.email +"', phonenum = '"+inf.phonenum +"', bio = '"+inf.bio +"', birthday = '"+inf.birthday +"', gender = '"+genderChar+"'; ")) {
		QMessageBox::warning(this, "Query Problem!", "Not Execute Database For Update. \nPage : change_profile -> function : on_btn_ok_clicked");
		db.close();
		return;
	}
	q.clear();

    if(usernameChanged) {
        QFile curUser("curUser.txt");
        if(!curUser.open(QFile::WriteOnly | QFile::Text )) {
            QMessageBox::warning(this, "File Problem!", "Not Execute Database For Write. \nPage : change_profile -> Function : on_btn_ok_clicked");
            db.close();
            return;
        }
        curUser.close();

        // Updating information table
        if(!q.exec("UPDATE information SET username = '"+inf.username +"' WHERE username = '"+username +"'; ")) {
            QMessageBox::warning(this, "Query Problem!", "Not Execute Database For Update information table. \nPage : change_profile -> Function : on_btn_ok_clicked");
            db.close();
            return;
        }

        // Change name of tables
        {
            // information table
            QString preTableName = username + "_information";
            QString NewTableName = inf.username + "_information";
            if(!q.exec("ALTER TABLE '"+preTableName +"' RENAME TO '"+NewTableName +"'; ")) {
                QMessageBox::warning(this, "Query Problem!", "Not Execute Database For Update user information table name. \nPage : change_profile -> Function : on_btn_ok_clicked");
                db.close();
                return;
            }

            // followers table
            preTableName = username + "_followers";
            NewTableName = inf.username + "_followers";
            if(!q.exec("ALTER TABLE '"+preTableName +"' RENAME TO '"+NewTableName +"'; ")) {
                QMessageBox::warning(this, "Query Problem!", "Not Execute Database For Update user followers table name. \nPage : change_profile -> Function : on_btn_ok_clicked");
                db.close();
                return;
            }

            // following table
            preTableName = username + "_following";
            NewTableName = inf.username + "_following";
            if(!q.exec("ALTER TABLE '"+preTableName +"' RENAME TO '"+NewTableName +"'; ")) {
                QMessageBox::warning(this, "Query Problem!", "Not Execute Database For Update user following table name. \nPage : change_profile -> Function : on_btn_ok_clicked");
                db.close();
                return;
            }

            // posts table
            preTableName = username + "_posts";
            NewTableName = inf.username + "_posts";
            if(!q.exec("ALTER TABLE '"+preTableName +"' RENAME TO '"+NewTableName +"'; ")) {
                QMessageBox::warning(this, "Query Problem!", "Not Execute Database For Update user posts table name. \nPage : change_profile -> Function : on_btn_ok_clicked");
                db.close();
                return;
            }

            // Messages table
            preTableName = username + "_messages";
            NewTableName = inf.username + "_messages";
            if(!q.exec("ALTER TABLE '"+preTableName +"' RENAME TO '"+NewTableName +"'; ")) {
                QMessageBox::warning(this, "Query Problem!", "Not Execute Database For Update user messages table name. \nPage : change_profile -> Function : on_btn_ok_clicked");
                db.close();
                return;
            }
        }

        MainWindow * mw = new MainWindow;
        mw->show();
        this->close();

        db.close();
        return;
    }
    db.close();

    setting * st = new setting();
	st->show();
	this->close();
}
bool change_profile::checkName() {
	QString name = ui->le_name->text();

	if(name.size() <= 0 || name.size() >= 20)
		return false;

	return true;
}

bool change_profile::checkLastname() {
	QString lastname = ui->le_lastname->text();

	if(lastname.size() <= 0 || lastname.size() >= 20)
		return false;

	return true;
}

bool change_profile::checkEmail() {
	QString email = ui->le_email->text();

	if(email.size() <= 0 || email.size() >= 40)
		return false;

	return true;
}

bool change_profile::checkPhonenum() {
	QString phonenum = ui->le_phonenumber->text();

	if(!isNumber(phonenum) || phonenum.size() <= 0 || phonenum.size() >= 20)
		return false;

	return true;
}

bool change_profile::checkBio() {
	QString bio = ui->le_biography->text();

	if(bio.size() < 0 || bio.size() >= 200)
		return false;

	return true;
}

bool change_profile::checkYear() {
	QString year = ui->le_year->text();

	if(!isNumber(year))
		return false;

	int yearInt = year.toInt();
	if(yearInt < 1000 || yearInt > 9999)
		return false;

	return true;
}

bool change_profile::checkMonth() {
	QString month = ui->le_month->text();

	if(!isNumber(month))
		return false;

	int monthInt = month.toInt();
	if(monthInt < 1 || monthInt > 12)
		return false;

	return true;
}

bool change_profile::checkDay() {
	QString day = ui->le_day->text();

	if(!isNumber(day))
		return false;

	int dayInt = day.toInt();
	if(dayInt < 1 || dayInt > 31)
		return false;

	return true;
}

bool change_profile::checkGender() {
	if(!(ui->checkBox_female->isChecked() || ui->checkBox_male->isChecked()))
		return false;

	return true;
}

bool change_profile::isNumber(QString str) {
	for(int i = 0; i < str.size(); ++i) {
		if(str[i] < '0' || str[i] > '9')
			return false;
	}
	return true;
}

void change_profile::on_checkBox_male_clicked()
{
	ui->checkBox_male->setChecked(true);
	ui->checkBox_female->setChecked(false);
}

void change_profile::on_checkBox_female_clicked()
{
	ui->checkBox_female->setChecked(true);
	ui->checkBox_male->setChecked(false);
}

QString change_profile::getUsername()
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

void change_profile::setData() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(addressDatabas);
    if(!db.open()) {
        QMessageBox::warning(this, "Database Problem!", "Not Successes To Open Database. \nPage : change_profile -> Function : setData");
        return;
    }
    QString tmp = username + "_information";
    QSqlQuery q;
    if(!q.exec("SELECT * FROM '"+tmp +"' ;")) {
        QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Login Search. \nPage : change_profile -> Function : setData");
        db.close();
        return;
    }

    info inf;
    while(q.next()) {
        inf.username = username;
        inf.name = q.value(2).toString();
        inf.lastname = q.value(3).toString();
        inf.email = q.value(4).toString();
        inf.phonenum = q.value(5).toString();
        inf.bio = q.value(6).toString();
        inf.birthday = q.value(7).toString();
        inf.gender = q.value(8).toInt();
    }

    ui->le_username->setText(username);
    ui->le_name->setText(inf.name);
    ui->le_lastname->setText(inf.lastname);
    ui->le_biography->setText(inf.bio);
    ui->le_email->setText(inf.email);
    ui->le_phonenumber->setText(inf.phonenum);
    ui->le_day->setText(inf.birthday.split('-')[2]);
    ui->le_month->setText(inf.birthday.split('-')[1]);
    ui->le_year->setText(inf.birthday.split('-')[0]);
    if(inf.gender == 1)
        ui->checkBox_female->setChecked(true); // female = 1, male = 0;
    else
        ui->checkBox_male->setChecked(true);

}
