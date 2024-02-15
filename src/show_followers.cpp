#include "show_followers.h"
#include "ui_show_followers.h"

show_followers::show_followers(QString username, QWidget *parent) :
	QWidget(parent),
	ui(new Ui::show_followers)
{
	ui->setupUi(this);
	this->setWindowState(Qt::WindowMaximized);
	db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName(addressDatabas);
	username_visitor = getUsername();
	username_owner = username;
	readData(username);
	setData();
	add_to_satck("show_followers");
}

show_followers::~show_followers()
{
	delete ui;
}

bool show_followers::is_public(QString account)
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

void show_followers::add_to_satck(QString page_name)
{
	QFile stack_pages("stack.txt");
	if(!stack_pages.open(QFile::ReadWrite | QFile::Text )) {
		QMessageBox::warning(this, "File Problem!", "Didn't Open curUser.txt File. \nPage : show_followers -> Function : add_to_stack");
		return;
	}
	QTextStream in(&stack_pages);
	while(!in.atEnd())
		in.readLine();
	in << page_name + "," + username_owner + "\n";
	stack_pages.close();
}

QString show_followers::getUsername() {
	QFile curUser("curUser.txt");
	if(!curUser.open(QFile::ReadOnly | QFile::Text )) {
		QMessageBox::warning(this, "File Problem!", "Not Open curUser.txt File. \nPage : show_followers -> Function : getUsername");
		return "";
	}
	QTextStream in(&curUser);
	QString username = in.readLine();
	curUser.close();
	return username;
}

bool show_followers::followed_username(QString username)
{
	if (!db.open()) {
		QMessageBox::warning(this, "Database Problem!", "Not Successes To Open Database. \nPage : show_followings -> Function : readData");
		return false;
	}

	QString tmp = username_visitor + "_following";
	QString exist = "";
	QSqlQuery q;
	if(!q.exec("SELECT username FROM '"+tmp +"' WHERE username = '"+username +"';")) {
		QMessageBox::warning(this, "Query Problem!", "Not Execute Database For Read. \nPage : show_followings -> Function : readData");
		db.close();
		return false;
	}
	while(q.next())
		exist = q.value(0).toString();
	q.clear();
	db.close();

	if(exist == "")
		return false;
	return true;
}

void show_followers::follow(int index)
{
	if(!is_public(followers[index].username)) {
		QMessageBox::warning(this, "This Page Is Private!", "Your Request Send For User.");
		if(!db.open()) {
			QMessageBox::warning(this, "Database Problem!", "Not Successes To Open Database. \nPage : recommend -> Function : follow");
			return;
		}
		QSqlQuery q;
		QString table_name = followers[index].username + "_messages";
		QString message = username_visitor + " Wanted Follow You.";
		if(!q.exec("INSERT INTO '"+table_name +"'(usernameSend, txt) VALUES('"+ username_visitor +"', '"+message +"'); ")) {
			QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Insert New Following. \nPage : recommend -> Function : follow");
			db.close();
			return;
		}
		q.clear();
		db.close();
		return;
	}

	QString table_name = username_visitor + "_following";
	int number_followings = 0, number_followers = 0;
	if(!db.open()) {
		QMessageBox::warning(this, "Database Problem!", "Not Successes To Open Database. \nPage : recommend -> Function : follow");
		return;
	}
	QSqlQuery q, qu, que;
	// add new username to followings
	{
		if(!q.exec("INSERT INTO '"+table_name +"'(username) VALUES('"+ followers[index].username +"'); ")) {
			QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Insert New Following. \nPage : recommend -> Function : follow");
			db.close();
			return;
		}
		q.clear();
	}

	// increase number of followings
	{
		table_name = username_visitor + "_information";
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
		table_name = followers[index].username + "_followers";
		if(!q.exec("INSERT INTO '"+table_name +"'(username) VALUES('"+username_visitor +"'); ")) {
			QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Insert New Followers. \nPage : recommend -> Function : follow");
			db.close();
			return;
		}
		q.clear();
	}

	// increase number of followers
	{
		table_name = followers[index].username + "_information";
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

void show_followers::unfollow(int index)
{
	QString table_name = username_visitor + "_following";
	int number_followings = 0, number_followers = 0;
	if(!db.open()) {
		QMessageBox::warning(this, "Database Problem!", "Not Successes To Open Database. \nPage : recommend -> Function : unfollow");
		return;
	}
	QSqlQuery q, qu, que;
	// delete new username to followings
		{
		if(!q.exec("DELETE FROM '"+table_name +"' WHERE username = '"+followers[index].username +"'; ")) {
			QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Delete New Following. \nPage : recommend -> Function : unfollow");
			db.close();
			return;
		}
	q.clear();
	}

	// decrease number of followings
	{
		table_name = username_visitor + "_information";
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
		table_name = followers[index].username + "_followers";
		if(!q.exec("DELETE FROM '"+table_name +"' WHERE username = '"+username_visitor +"'; ")) {
			QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Delete Follower. \nPage : recommend -> Function : unfollow");
			db.close();
			return;
		}
		q.clear();
	}

	// decrease number of followers
	{
		table_name = followers[index].username + "_information";
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

void show_followers::setData()
{
	ui->pte_follower1->setPlainText("");
	ui->pte_follower2->setPlainText("");
	ui->pte_follower3->setPlainText("");
	ui->pte_follower4->setPlainText("");
	ui->pte_follower5->setPlainText("");
	ui->pte_follower6->setPlainText("");
	ui->pte_follower7->setPlainText("");
	ui->pte_follower8->setPlainText("");
	ui->pte_follower9->setPlainText("");
	ui->pte_follower10->setPlainText("");
	ui->btn_follow1->setText("");
	ui->btn_follow2->setText("");
	ui->btn_follow3->setText("");
	ui->btn_follow4->setText("");
	ui->btn_follow5->setText("");
	ui->btn_follow6->setText("");
	ui->btn_follow7->setText("");
	ui->btn_follow8->setText("");
	ui->btn_follow9->setText("");
	ui->btn_follow10->setText("");

	int num, temp = start;
	QString tmp;

	if(followers.size() - start <= 10)
		num = followers.size() - start;
	else
		num = 10;

	switch(num){
	case 10:
		tmp = followers[temp+9].username;
		ui->pte_follower10->setPlainText("@" + tmp);
		if(followed_username(tmp)) {
			ui->btn_follow10->setText("UnFollow");
			ui->btn_follow10->setStyleSheet(push_button_css_unfollow);
		}
		else {
			ui->btn_follow10->setText("Follow");
			ui->btn_follow10->setStyleSheet(push_button_css_follow);
		}
	case 9:
		tmp = followers[temp+8].username;
		ui->pte_follower9->setPlainText("@" + tmp);
		if(followed_username(tmp)) {
			ui->btn_follow9->setText("UnFollow");
			ui->btn_follow9->setStyleSheet(push_button_css_unfollow);
		}
		else {
			ui->btn_follow9->setText("Follow");
			ui->btn_follow9->setStyleSheet(push_button_css_follow);
		}
	case 8:
		tmp = followers[temp+7].username;
		ui->pte_follower8->setPlainText("@" + tmp);
		if(followed_username(tmp)) {
			ui->btn_follow8->setText("UnFollow");
			ui->btn_follow8->setStyleSheet(push_button_css_unfollow);
		}
		else {
			ui->btn_follow8->setText("Follow");
			ui->btn_follow8->setStyleSheet(push_button_css_follow);
		}
	case 7:
		tmp = followers[temp+6].username;
		ui->pte_follower7->setPlainText("@" + tmp);
		if(followed_username(tmp)) {
			ui->btn_follow7->setText("UnFollow");
			ui->btn_follow7->setStyleSheet(push_button_css_unfollow);
		}
		else {
			ui->btn_follow7->setText("Follow");
			ui->btn_follow7->setStyleSheet(push_button_css_follow);
		}
	case 6:
		tmp = followers[temp+5].username;
		ui->pte_follower6->setPlainText("@" + tmp);
		if(followed_username(tmp)) {
			ui->btn_follow6->setText("UnFollow");
			ui->btn_follow6->setStyleSheet(push_button_css_unfollow);
		}
		else {
			ui->btn_follow6->setText("Follow");
			ui->btn_follow6->setStyleSheet(push_button_css_follow);
		}
	case 5:
		tmp = followers[temp+4].username;
		ui->pte_follower5->setPlainText("@" + tmp);
		if(followed_username(tmp)) {
			ui->btn_follow5->setText("UnFollow");
			ui->btn_follow5->setStyleSheet(push_button_css_unfollow);
		}
		else {
			ui->btn_follow5->setText("Follow");
			ui->btn_follow5->setStyleSheet(push_button_css_follow);
		}
	case 4:
		tmp = followers[temp+3].username;
		ui->pte_follower4->setPlainText("@" + tmp);
		if(followed_username(tmp)) {
			ui->btn_follow4->setText("UnFollow");
			ui->btn_follow4->setStyleSheet(push_button_css_unfollow);
		}
		else {
			ui->btn_follow4->setText("Follow");
			ui->btn_follow4->setStyleSheet(push_button_css_follow);
		}
	case 3:
		tmp = followers[temp+2].username;
		ui->pte_follower3->setPlainText("@" + tmp);
		if(followed_username(tmp)) {
			ui->btn_follow3->setText("UnFollow");
			ui->btn_follow3->setStyleSheet(push_button_css_unfollow);
		}
		else {
			ui->btn_follow3->setText("Follow");
			ui->btn_follow3->setStyleSheet(push_button_css_follow);
		}
	case 2:
		tmp = followers[temp+1].username;
		ui->pte_follower2->setPlainText("@" + tmp);
		if(followed_username(tmp)) {
			ui->btn_follow2->setText("UnFollow");
			ui->btn_follow2->setStyleSheet(push_button_css_unfollow);
		}
		else {
			ui->btn_follow2->setText("Follow");
			ui->btn_follow1->setStyleSheet(push_button_css_follow);
		}
	case 1:
		tmp = followers[temp+0].username;
		ui->pte_follower1->setPlainText("@" + tmp);
		if(followed_username(tmp)) {
			ui->btn_follow1->setText("UnFollow");
			ui->btn_follow1->setStyleSheet(push_button_css_unfollow);
		}
		else {
			ui->btn_follow1->setText("Follow");
			ui->btn_follow1->setStyleSheet(push_button_css_follow);
		}
	}
}

void show_followers::getData(QSqlQuery q)
{
	while(q.next()){
		follower tmp;
		tmp.id = q.value(0).toString();
		tmp.username = q.value(1).toString();
		followers.push_back(tmp);
	}
	q.clear();
}

void show_followers::on_btn_back_clicked()
{
	QFile stack_pages("stack.txt");
	if(!stack_pages.open(QFile::ReadWrite | QFile::Text )) {
		QMessageBox::warning(this, "File Problem!", "Didn't Open curUser.txt File. \nPage : other_page -> Function : on_btn_back_clicked");
		return;
	}

	QQueue<QString> pages;
	QTextStream in(&stack_pages);
	while(!in.atEnd())
		pages.enqueue(in.readLine());
	stack_pages.resize(0);
	while(pages.size() > 2)
		  in << pages.dequeue() << "\n";
	stack_pages.close();

	if(pages.size() < 2) {
		personal_page * pg = new personal_page();
		pg->show();
		this->close();
		return;
	}
	QString pervious_page = pages.dequeue();
	if(pervious_page.split(",")[0] == "followings_posts") {
		Followings_posts * fp = new Followings_posts();
		fp->show();
		this->close();
	}
	if(pervious_page.split(",")[0] == "explorer") {
		explorer * ex = new explorer();
		ex->show();
		this->close();
	}
	if(pervious_page.split(",")[0] == "search") {
		search * se = new search();
		se->show();
		this->close();
	}
	if(pervious_page.split(",")[0] == "other_page") {
		other_page * op = new other_page(pervious_page.split(",")[1]);
		op->show();
		this->close();
	}
	if(pervious_page.split(",")[0] == "show_followings") {
		show_followings * sf = new show_followings(pervious_page.split(",")[1]);
		sf->show();
		this->close();
	}
	if(pervious_page.split(",")[0] == "show_followers") {
		show_followers * sf = new show_followers(pervious_page.split(",")[1]);
		sf->show();
		this->close();
	}
}

void show_followers::readData(QString username)
{
	followers.clear();

	if (!db.open()) {
		QMessageBox::warning(this, "Database Problem!", "Not Successes To Open Database. \nPage : show_followers -> Function : readData");
		return;
	}

	QString tmp = username + "_followers";
	QSqlQuery q;
	if(!q.exec("SELECT * FROM '"+tmp +"';")) {
		QMessageBox::warning(this, "Query Problem!", "Not Execute Database For Read. \nPage : show_followers -> Function : readData");
		db.close();
		return;
	}

	getData(q);

	q.clear();
	db.close();
}

void show_followers::on_btn_search_clicked()
{
	followers.clear();

	if (!db.open()) {
		QMessageBox::warning(this, "Database Problem!", "Not Successes To Open Database. \nPage : show_followers -> Function : on_btn_search_clicked");
		return;
	}
	QString tmp = ui->le_search->text();
	tmp = "%" + tmp + "%";
	QString tableName = username_owner + "_followers";
	QSqlQuery q;
	if(!q.exec("SELECT * FROM '"+tableName +"' WHERE username LIKE '"+tmp +"';"))
	{
		QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Search.  \nPage : show_followers -> Function : on_btn_search_clicked");
		db.close();
		return;
	}

	getData(q);
	setData();
	q.clear();
	db.close();
}

void show_followers::on_btn_down_clicked()
{
	if(start < followers.size() - 10){
		start++;
		setData();
	}
}

void show_followers::on_btn_up_clicked()
{
	if(start > 0){
		start--;
		setData();
	}
}

void show_followers::on_btn_refresh_clicked()
{
	ui->le_search->setText("");
	followers.clear();
	start = 0;
	readData(username_owner);
	setData();
}

void show_followers::on_btn_follow1_clicked()
{
	QString otherUser = ui->pte_follower1->toPlainText();
	if(otherUser.isEmpty()) {
		return;
	}

	if(ui->btn_follow1->text() == "UnFollow") {
		unfollow(start);
		ui->btn_follow1->setText("Follow");
		ui->btn_follow1->setStyleSheet(push_button_css_follow);
	}
	else {
		follow(start);
		if(is_public(followers[start].username)) {
			ui->btn_follow1->setText("UnFollow");
			ui->btn_follow1->setStyleSheet(push_button_css_unfollow);
		}
	}
}

void show_followers::on_btn_follow2_clicked()
{
	QString otherUser = ui->pte_follower2->toPlainText();
	if(otherUser.isEmpty()) {
		return;
	}

	if(ui->btn_follow2->text() == "UnFollow") {
		unfollow(start + 1);
		ui->btn_follow2->setText("Follow");
		ui->btn_follow2->setStyleSheet(push_button_css_follow);
	}
	else {
		follow(start + 1);
		if(is_public(followers[start + 1].username)) {
			ui->btn_follow2->setText("UnFollow");
			ui->btn_follow2->setStyleSheet(push_button_css_unfollow);
		}
	}
}

void show_followers::on_btn_follow3_clicked()
{
	QString otherUser = ui->pte_follower3->toPlainText();
	if(otherUser.isEmpty()) {
		return;
	}

	if(ui->btn_follow3->text() == "UnFollow") {
		unfollow(start + 2);
		ui->btn_follow3->setText("Follow");
		ui->btn_follow3->setStyleSheet(push_button_css_follow);
	}
	else {
		follow(start + 2);
		if(is_public(followers[start + 2].username)) {
			ui->btn_follow3->setText("UnFollow");
			ui->btn_follow3->setStyleSheet(push_button_css_unfollow);
		}
	}
}

void show_followers::on_btn_follow4_clicked()
{
	QString otherUser = ui->pte_follower4->toPlainText();
	if(otherUser.isEmpty()) {
		return;
	}

	if(ui->btn_follow4->text() == "UnFollow") {
		unfollow(start + 3);
		ui->btn_follow4->setText("Follow");
		ui->btn_follow4->setStyleSheet(push_button_css_follow);
	}
	else {
		follow(start + 3);
		if(is_public(followers[start + 3].username)) {
			ui->btn_follow4->setText("UnFollow");
			ui->btn_follow4->setStyleSheet(push_button_css_unfollow);
		}
	}

}

void show_followers::on_btn_follow5_clicked()
{
	QString otherUser = ui->pte_follower5->toPlainText();
	if(otherUser.isEmpty()) {
		return;
	}

	if(ui->btn_follow5->text() == "UnFollow") {
		unfollow(start + 4);
		ui->btn_follow5->setText("Follow");
		ui->btn_follow5->setStyleSheet(push_button_css_follow);
	}
	else {
		follow(start + 4);
		if(is_public(followers[start + 4].username)) {
			ui->btn_follow5->setText("UnFollow");
			ui->btn_follow5->setStyleSheet(push_button_css_unfollow);
		}
	}
}

void show_followers::on_btn_follow6_clicked()
{
	QString otherUser = ui->pte_follower6->toPlainText();
	if(otherUser.isEmpty()) {
		return;
	}

	if(ui->btn_follow6->text() == "UnFollow") {
		unfollow(start + 5);
		ui->btn_follow6->setText("Follow");
		ui->btn_follow6->setStyleSheet(push_button_css_follow);
	}
	else {
		follow(start + 5);
		if(is_public(followers[start + 5].username)) {
			ui->btn_follow6->setText("UnFollow");
			ui->btn_follow6->setStyleSheet(push_button_css_unfollow);
		}
	}
}

void show_followers::on_btn_follow7_clicked()
{
	QString otherUser = ui->pte_follower7->toPlainText();
	if(otherUser.isEmpty()) {
		return;
	}

	if(ui->btn_follow7->text() == "UnFollow") {
		unfollow(start + 6);
		ui->btn_follow7->setText("Follow");
		ui->btn_follow7->setStyleSheet(push_button_css_follow);
	}
	else {
		follow(start + 6);
		if(is_public(followers[start + 6].username)) {
			ui->btn_follow7->setText("UnFollow");
			ui->btn_follow7->setStyleSheet(push_button_css_unfollow);
		}
	}
}

void show_followers::on_btn_follow8_clicked()
{
	QString otherUser = ui->pte_follower8->toPlainText();
	if(otherUser.isEmpty()) {
		return;
	}

	if(ui->btn_follow8->text() == "UnFollow") {
		unfollow(start + 7);
		ui->btn_follow8->setText("Follow");
		ui->btn_follow8->setStyleSheet(push_button_css_follow);
	}
	else {
		follow(start + 7);
		if(is_public(followers[start + 7].username)) {
			ui->btn_follow8->setText("UnFollow");
			ui->btn_follow8->setStyleSheet(push_button_css_unfollow);
		}
	}
}

void show_followers::on_btn_follow9_clicked()
{
	QString otherUser = ui->pte_follower9->toPlainText();
	if(otherUser.isEmpty()) {
		return;
	}

	if(ui->btn_follow9->text() == "UnFollow") {
		unfollow(start + 8);
		ui->btn_follow9->setText("Follow");
		ui->btn_follow9->setStyleSheet(push_button_css_follow);
	}
	else {
		follow(start + 8);
		if(is_public(followers[start + 8].username)) {
			ui->btn_follow9->setText("UnFollow");
			ui->btn_follow9->setStyleSheet(push_button_css_unfollow);
		}
	}
}

void show_followers::on_btn_follow10_clicked()
{
	QString otherUser = ui->pte_follower10->toPlainText();
	if(otherUser.isEmpty()) {
		return;
	}

	if(ui->btn_follow10->text() == "UnFollow") {
		unfollow(start + 9);
		ui->btn_follow10->setText("Follow");
		ui->btn_follow10->setStyleSheet(push_button_css_follow);
	}
	else {
		follow(start + 9);
		if(is_public(followers[start + 9].username)) {
			ui->btn_follow10->setText("UnFollow");
			ui->btn_follow10->setStyleSheet(push_button_css_unfollow);
		}
	}
}

void show_followers::on_pte_follower1_selectionChanged()
{
	QString otherUser = ui->pte_follower1->toPlainText();
	if(otherUser.isEmpty()) {
		return;
	}
	otherUser = otherUser.split("@")[1];
	other_page * op = new other_page(otherUser);
	op->show();
	this->close();
}

void show_followers::on_pte_follower2_selectionChanged()
{
	QString otherUser = ui->pte_follower2->toPlainText();
	if(otherUser.isEmpty()) {
		return;
	}
	otherUser = otherUser.split("@")[1];
	other_page * op = new other_page(otherUser);
	op->show();
	this->close();
}

void show_followers::on_pte_follower3_selectionChanged()
{
	QString otherUser = ui->pte_follower3->toPlainText();
	if(otherUser.isEmpty()) {
		return;
	}
	otherUser = otherUser.split("@")[1];
	other_page * op = new other_page(otherUser);
	op->show();
	this->close();
}

void show_followers::on_pte_follower4_selectionChanged()
{
	QString otherUser = ui->pte_follower4->toPlainText();
	if(otherUser.isEmpty()) {
		return;
	}
	otherUser = otherUser.split("@")[1];
	other_page * op = new other_page(otherUser);
	op->show();
	this->close();
}

void show_followers::on_pte_follower5_selectionChanged()
{
	QString otherUser = ui->pte_follower5->toPlainText();
	if(otherUser.isEmpty()) {
		return;
	}
	otherUser = otherUser.split("@")[1];
	other_page * op = new other_page(otherUser);
	op->show();
	this->close();
}

void show_followers::on_pte_follower6_selectionChanged()
{
	QString otherUser = ui->pte_follower6->toPlainText();
	if(otherUser.isEmpty()) {
		return;
	}
	otherUser = otherUser.split("@")[1];
	other_page * op = new other_page(otherUser);
	op->show();
	this->close();
}

void show_followers::on_pte_follower7_selectionChanged()
{
	QString otherUser = ui->pte_follower7->toPlainText();
	if(otherUser.isEmpty()) {
		return;
	}
	otherUser = otherUser.split("@")[1];
	other_page * op = new other_page(otherUser);
	op->show();
	this->close();
}

void show_followers::on_pte_follower8_selectionChanged()
{
	QString otherUser = ui->pte_follower8->toPlainText();
	if(otherUser.isEmpty()) {
		return;
	}
	otherUser = otherUser.split("@")[1];
	other_page * op = new other_page(otherUser);
	op->show();
	this->close();
}

void show_followers::on_pte_follower9_selectionChanged()
{
	QString otherUser = ui->pte_follower9->toPlainText();
	if(otherUser.isEmpty()) {
		return;
	}
	otherUser = otherUser.split("@")[1];
	other_page * op = new other_page(otherUser);
	op->show();
	this->close();
}

void show_followers::on_pte_follower10_selectionChanged()
{
	QString otherUser = ui->pte_follower10->toPlainText();
	if(otherUser.isEmpty()) {
		return;
	}
	otherUser = otherUser.split("@")[1];
	other_page * op = new other_page(otherUser);
	op->show();
	this->close();
}

