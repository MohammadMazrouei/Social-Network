#include "recommend.h"
#include "ui_recommend.h"

recommend::recommend(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::recommend)
{
	ui->setupUi(this);
	this->setWindowState(Qt::WindowMaximized);
	db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName(addressDatabas);
	get_username();
	show_recommend();
}

recommend::~recommend()
{
	delete ui;
}

void recommend::get_username()
{
	QFile curUser("curUser.txt");
	if(!curUser.open(QFile::ReadOnly | QFile::Text )) {
			QMessageBox::warning(this, "File Problem!", "Not Open curUser.txt File. \nPage : recommend -> Function : get_username");
			return;
	}
	QTextStream out(&curUser);
	out >> username;
	curUser.close();
}

bool recommend::is_public(QString account)
{
	QString table_name = account + "_information";
	bool isPublic;
	if(!db.open()) {
		QMessageBox::warning(this, "Database Problem!", "Not Successes To Open Database. \nPage : recommend -> Function : show_recommend");
		return false;
	}
	QSqlQuery q;
	if(!q.exec("SELECT public FROM '"+table_name +"'; ")) {
		QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Get UserName. \nPage : recommend -> Function : show_recommend");
		db.close();
		return false;
	}
	while(q.next())
		isPublic = q.value(0).toBool();

	return isPublic;
}

void recommend::show_recommend()
{
	QString table_name = "information";
	QString names[7], lastnames[7];
	int i = 0;

	if(!db.open()) {
		QMessageBox::warning(this, "Database Problem!", "Not Successes To Open Database. \nPage : recommend -> Function : show_recommend");
		return;
	}
	QSqlQuery q, qu;
	if(!q.exec("SELECT username FROM '"+table_name +"' ORDER BY createAccount DESC LIMIT 7; ")) {
		QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Get UserName. \nPage : recommend -> Function : show_recommend");
		db.close();
		return;
	}

	while(q.next()) {
		usernames[i] = q.value(0).toString();
		table_name = usernames[i] + "_information";
		if(!qu.exec("SELECT * FROM '"+table_name +"'; ")) {
			QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Get Name. \nPage : recommend -> Function : show_recommend");
			db.close();
			return;
		}
		while(qu.next()) {
			names[i] = qu.value(2).toString();
			lastnames[i] = qu.value(3).toString();
		}
		i++;
		qu.clear();
	}
	q.clear();
	db.close();
	number_recommend = i;

	if(i > 1)
		ui->pte1->setPlainText("@" + usernames[1] + "\n\n" + names[1] + " " + lastnames[1]);
	if(i > 2)
		ui->pte2->setPlainText("@" + usernames[2] + "\n\n" + names[2] + " " + lastnames[2]);
	if(i > 3)
		ui->pte3->setPlainText("@" + usernames[3] + "\n\n" + names[3] + " " + lastnames[3]);
	if(i > 4)
		ui->pte4->setPlainText("@" + usernames[4] + "\n\n" + names[4] + " " + lastnames[4]);
	if(i > 5)
		ui->pte5->setPlainText("@" + usernames[5] + "\n\n" + names[5] + " " + lastnames[5]);
	if(i > 6)
		ui->pte6->setPlainText("@" + usernames[6] + "\n\n" + names[6] + " " + lastnames[6]);

}

void recommend::follow(int index)
{
	if(!is_public(usernames[index])) {
		QMessageBox::warning(this, "This Page Is Private!", "Your Request Send For User.");
		if(!db.open()) {
			QMessageBox::warning(this, "Database Problem!", "Not Successes To Open Database. \nPage : recommend -> Function : follow");
			return;
		}
		QSqlQuery q;
		QString table_name = usernames[index] + "_messages";
		QString message = username + " Wanted Follow You.";
		if(!q.exec("INSERT INTO '"+table_name +"'(usernameSend, txt) VALUES('"+ username +"', '"+message +"'); ")) {
			QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Insert New Following. \nPage : recommend -> Function : follow");
			db.close();
			return;
		}
		q.clear();
		db.close();
		return;
	}
	follow_users[index] = true;

	QString table_name = username + "_following";
	int number_followings = 0, number_followers = 0;
	if(!db.open()) {
		QMessageBox::warning(this, "Database Problem!", "Not Successes To Open Database. \nPage : recommend -> Function : follow");
		return;
	}
	QSqlQuery q, qu, que;
	// add new username to followings
	{
		if(!q.exec("INSERT INTO '"+table_name +"'(username) VALUES('"+ usernames[index] +"'); ")) {
			QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Insert New Following. \nPage : recommend -> Function : follow");
			db.close();
			return;
		}
		q.clear();
	}

	// increase number of followings
	{
		table_name = username + "_information";
		if(!qu.exec("SELECT followingN FROM '"+table_name +"'; ")) {
			QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Get Number Followings. \nPage : recommend -> Function : follow");
			db.close();
			return;
		}
		while(qu.next())
			number_followings = qu.value(0).toInt();

		number_followings++;

        QString number_followingsChar = QString::number(number_followings);

        if(!que.exec("UPDATE '"+table_name +"' SET followingN = '"+number_followingsChar +"'; ")) {
			QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Set Number Followings. \nPage : recommend -> Function : follow");
			db.close();
			return;
		}
		qu.clear();
		que.clear();
	}

	// add new username to followers
	{
		table_name = usernames[index] + "_followers";
		if(!q.exec("INSERT INTO '"+table_name +"'(username) VALUES('"+username +"'); ")) {
			QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Insert New Followers. \nPage : recommend -> Function : follow");
			db.close();
			return;
		}
		q.clear();
	}

	// increase number of followers
	{
		table_name = usernames[index] + "_information";
		if(!qu.exec("SELECT followerN FROM '"+table_name +"'; ")) {
			QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Get Number Followers. \nPage : recommend -> Function : follow");
			db.close();
			return;
		}
		while(qu.next())
			number_followers = qu.value(0).toInt();
		number_followers++;

        QString number_followersChar = QString::number(number_followers);

        if(!que.exec("UPDATE '"+table_name +"' SET followerN = '"+number_followersChar +"'; ")) {
			QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Set Number Followers. \nPage : recommend -> Function : follow");
			db.close();
			return;
		}
		qu.clear();
		que.clear();
	}

	db.close();
}

void recommend::unfollow(int index)
{
	follow_users[index] = false;

	QString table_name = username + "_following";
	int number_followings = 0, number_followers = 0;
	if(!db.open()) {
		QMessageBox::warning(this, "Database Problem!", "Not Successes To Open Database. \nPage : recommend -> Function : unfollow");
		return;
	}
	QSqlQuery q, qu, que;
	// delete new username to followings
		{
		if(!q.exec("DELETE FROM '"+table_name +"' WHERE username = '"+usernames[index] +"'; ")) {
			QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Delete New Following. \nPage : recommend -> Function : unfollow");
			db.close();
			return;
		}
	q.clear();
	}

	// decrease number of followings
	{
		table_name = username + "_information";
		if(!qu.exec("SELECT followingN FROM '"+table_name +"'; ")) {
			QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Get Number Followings. \nPage : recommend -> Function : unfollow");
			db.close();
			return;
		}
		while(qu.next())
			number_followings = qu.value(0).toInt();

		number_followings--;

        QString number_followingsChar = QString::number(number_followings);

        if(!que.exec("UPDATE '"+table_name +"' SET followingN = '"+number_followingsChar +"'; ")) {
			QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Set Number Followings. \nPage : recommend -> Function : follow");
			db.close();
			return;
		}
		qu.clear();
		que.clear();
	}

	// delete new username to followers
	{
		table_name = usernames[index] + "_followers";
		if(!q.exec("DELETE FROM '"+table_name +"' WHERE username = '"+username +"'; ")) {
			QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Delete Follower. \nPage : recommend -> Function : unfollow");
			db.close();
			return;
		}
		q.clear();
	}

	// decrease number of followers
	{
		table_name = usernames[index] + "_information";
		if(!qu.exec("SELECT followerN FROM '"+table_name +"'; ")) {
			QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Get Number Followers. \nPage : recommend -> Function : unfollow");
			db.close();
			return;
		}
		while(qu.next())
			number_followers = qu.value(0).toInt();
		number_followers--;

        QString number_followersChar = QString::number(number_followers);

        if(!que.exec("UPDATE '"+table_name +"' SET followerN = '"+number_followersChar +"'; ")) {
			QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Set Number Followers. \nPage : recommend -> Function : unfollow");
			db.close();
			return;
		}
		qu.clear();
		que.clear();
	}

	db.close();
}

void recommend::on_btn_cancel_clicked()
{
	MainWindow * mw = new MainWindow();
	mw->show();
	this->close();
}

void recommend::on_btn_ok_clicked()
{
	personal_page * pg = new personal_page();
	pg->show();
	this->close();
}

void recommend::on_btn_follow1_clicked()
{
	if(number_recommend > 1) {
		if(follow_users[1] == false) {
			follow(1);
			if(is_public(usernames[1])) {
				ui->btn_follow1->setText("UnFollow");
				ui->btn_follow1->setStyleSheet(push_button_css_unfollow);
			}
		}
		else {
			unfollow(1);
			ui->btn_follow1->setText("Follow");
			ui->btn_follow1->setStyleSheet(push_button_css_follow);
		}
	}
}

void recommend::on_btn_follow2_clicked()
{
	if(number_recommend > 2) {
		if(follow_users[2] == false) {
			follow(2);
			if(is_public(usernames[2])) {
				ui->btn_follow2->setText("UnFollow");
				ui->btn_follow2->setStyleSheet(push_button_css_unfollow);
			}
		}
		else {
			unfollow(2);
			ui->btn_follow2->setText("Follow");
			ui->btn_follow2->setStyleSheet(push_button_css_follow);
		}
	}
}

void recommend::on_btn_follow3_clicked()
{
	if(number_recommend > 3) {
		if(follow_users[3] == false) {
			follow(3);
			if(is_public(usernames[3])) {
				ui->btn_follow3->setText("UnFollow");
				ui->btn_follow3->setStyleSheet(push_button_css_unfollow);
			}
		}
		else {
			unfollow(3);
			ui->btn_follow3->setText("Follow");
			ui->btn_follow3->setStyleSheet(push_button_css_follow);
		}
	}
}

void recommend::on_btn_follow4_clicked()
{
	if(number_recommend > 4) {
		if(follow_users[4] == false) {
			follow(4);
			if(is_public(usernames[4])) {
				ui->btn_follow4->setText("UnFollow");
				ui->btn_follow4->setStyleSheet(push_button_css_unfollow);
			}
		}
		else {
			unfollow(4);
			ui->btn_follow4->setText("Follow");
			ui->btn_follow4->setStyleSheet(push_button_css_follow);
		}
	}
}

void recommend::on_btn_follow5_clicked()
{
	if(number_recommend > 5) {
		if(follow_users[5] == false) {
			follow(5);
			if(is_public(usernames[5])) {
				ui->btn_follow5->setText("UnFollow");
				ui->btn_follow5->setStyleSheet(push_button_css_unfollow);
			}
		}
		else {
			unfollow(5);
			ui->btn_follow5->setText("Follow");
			ui->btn_follow5->setStyleSheet(push_button_css_follow);
		}
	}
}

void recommend::on_btn_follow6_clicked()
{
	if(number_recommend > 6) {
		if(follow_users[6] == false) {
			follow(6);
			if(is_public(usernames[6])) {
				ui->btn_follow6->setText("UnFollow");
				ui->btn_follow6->setStyleSheet(push_button_css_unfollow);
			}
		}
		else {
			unfollow(6);
			ui->btn_follow6->setText("Follow");
			ui->btn_follow6->setStyleSheet(push_button_css_follow);
		}
	}
}
