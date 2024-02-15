#include "search.h"
#include "ui_search.h"

search::search(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::search)
{
	ui->setupUi(this);
	this->setWindowState(Qt::WindowMaximized);
	add_to_stack("search");
	db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName(addressDatabas);
	username_visitor = getUsername();
	suggestion();
	setData();
}

search::~search()
{
	delete ui;
}

class search::user
{
public:
	QString id;
	QString username = "";
	QString name = "";
	QString last_name = "";
	double familiarity = 0;
	QList<QString> followers;
};

bool search::is_public(QString account)
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

void search::suggestion()
{
	user friend1, me;

	if (!db.open()) {
		QMessageBox::warning(this, "Database Problem!", "Not Successes To Open Database. \nPage : show_followers -> Function : on_btn_search_clicked");
		return;
	}
	QString tableName = "information";
	QSqlQuery q;
	if(!q.exec("SELECT * FROM '"+tableName +"' WHERE username <> '"+username_visitor +"';"))
	{
		QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Search.  \nPage : show_followers -> Function : on_btn_search_clicked");
		db.close();
		return;
	}
	while(q.next()) {
		friend1.username = q.value(0).toString();
		users.push_back(friend1);
	}
	q.clear();
	int len = users.length();

	for(int i = 0; i < len; i++) {

		tableName = users[i].username + "_followers";
		if(!q.exec("SELECT username FROM '"+tableName +"';")) {
			QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Search.  \nPage : show_followers -> Function : on_btn_search_clicked");
			db.close();
			return;
		}
		while(q.next()){
			users[i].followers.push_back(q.value(0).toString());
		}
		q.clear();
	}

	tableName = username_visitor + "_followers";
	if(!q.exec("SELECT username FROM '"+tableName +"';")) {
		QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Search.  \nPage : show_followers -> Function : on_btn_search_clicked");
		db.close();
		return;
	}
	while(q.next()){
		me.followers.push_back(q.value(0).toString());
	}
	q.clear();
	db.close();

	int len2 = me.followers.length();
	double same, notsame;
	for(int i = 0; i < len; i++) {
		for(int j = 0; j < len2; j++) {
			int index = users[i].followers.indexOf(me.followers[j]);
			if(index != -1)
				same += 1;
            qDebug() << index;
		}
        notsame += len2;
        if(notsame == 0)
            users[i].familiarity = 0;
        else
            users[i].familiarity = same / notsame;
	}

    Sort();
}

QString search::getUsername() {
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

bool search::followed_username(QString username)
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

void search::add_to_stack(QString page_name)
{
	QString text_searched = QString(ui->le_search->text());
	QFile stack_pages("stack.txt");
	if(!stack_pages.open(QFile::WriteOnly | QFile::Text )) {
		QMessageBox::warning(this, "File Problem!", "Didn't Open curUser.txt File. \nPage : search -> Function : add_to_stack");
		return;
	}
	QTextStream in(&stack_pages);
	in << page_name + "," + text_searched + "\n";
	stack_pages.close();
}

void search::on_btn_back_clicked()
{
	personal_page * pg = new personal_page();
	pg->show();
	this->close();
}

void search::on_btn_search_clicked()
{
	users.clear();

	if (!db.open()) {
		QMessageBox::warning(this, "Database Problem!", "Not Successes To Open Database. \nPage : search -> Function : on_btn_search_clicked");
		return;
	}

	QString tmp = ui->le_search->text();
	tmp = "%" + tmp + "%";
	QSqlQuery q;
    if(!q.exec("SELECT username FROM information WHERE username LIKE '"+tmp +"';"))
	{
		QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Search.  \nPage : search -> Function : on_btn_search_clicked");
		db.close();
		return;
	}

	getData(q);
	setData();
	q.clear();
	db.close();
    start = 0;
}

void search::setData()
{
	ui->pte_user1->setPlainText("");
	ui->pte_user2->setPlainText("");
	ui->pte_user3->setPlainText("");
	ui->pte_user4->setPlainText("");
	ui->pte_user5->setPlainText("");
	ui->pte_user6->setPlainText("");
	ui->btn_follow1->setText("");
	ui->btn_follow2->setText("");
	ui->btn_follow3->setText("");
	ui->btn_follow4->setText("");
	ui->btn_follow5->setText("");
	ui->btn_follow6->setText("");
    ui->btn_follow1->setStyleSheet(push_button_css_follow);
    ui->btn_follow2->setStyleSheet(push_button_css_follow);
    ui->btn_follow3->setStyleSheet(push_button_css_follow);
    ui->btn_follow4->setStyleSheet(push_button_css_follow);
    ui->btn_follow5->setStyleSheet(push_button_css_follow);
    ui->btn_follow6->setStyleSheet(push_button_css_follow);
	int num, temp = start;
	QString tmp;

	if(users.size() - start <= 6)
		num = users.size() - start;
	else
		num = 6;

	switch(num){
	case 6:
		tmp = users[temp+5].username;
		ui->pte_user6->setPlainText("@" + tmp);
		if(followed_username(tmp)) {
			ui->btn_follow6->setText("UnFollow");
			ui->btn_follow6->setStyleSheet(push_button_css_unfollow);
		}
		else {
			ui->btn_follow6->setText("Follow");
			ui->btn_follow6->setStyleSheet(push_button_css_follow);
		}
	case 5:
		tmp = users[temp+4].username;
		ui->pte_user5->setPlainText("@" + tmp);
		if(followed_username(tmp)) {
			ui->btn_follow5->setText("UnFollow");
			ui->btn_follow5->setStyleSheet(push_button_css_unfollow);
		}
		else {
			ui->btn_follow5->setText("Follow");
			ui->btn_follow5->setStyleSheet(push_button_css_follow);
		}
	case 4:
		tmp = users[temp+3].username;
		ui->pte_user4->setPlainText("@" + tmp);
		if(followed_username(tmp)) {
			ui->btn_follow4->setText("UnFollow");
			ui->btn_follow4->setStyleSheet(push_button_css_unfollow);
		}
		else {
			ui->btn_follow4->setText("Follow");
			ui->btn_follow4->setStyleSheet(push_button_css_follow);
		}
	case 3:
		tmp = users[temp+2].username;
		ui->pte_user3->setPlainText("@" + tmp);
		if(followed_username(tmp)) {
			ui->btn_follow3->setText("UnFollow");
			ui->btn_follow3->setStyleSheet(push_button_css_unfollow);
		}
		else {
			ui->btn_follow3->setText("Follow");
			ui->btn_follow3->setStyleSheet(push_button_css_follow);
		}
	case 2:
		tmp = users[temp+1].username;
		ui->pte_user2->setPlainText("@" + tmp);
		if(followed_username(tmp)) {
			ui->btn_follow2->setText("UnFollow");
			ui->btn_follow2->setStyleSheet(push_button_css_unfollow);
		}
		else {
			ui->btn_follow2->setText("Follow");
			ui->btn_follow2->setStyleSheet(push_button_css_follow);
		}
	case 1:
		tmp = users[temp+0].username;
		ui->pte_user1->setPlainText("@" + tmp);
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

void search::Sort()
{
	for(int i = 0; i < users.size(); i++){
        for(int j = i; j < users.size(); j++){
            if(users[j].familiarity > users[i].familiarity) {
                QString username1 = users[j].username;
                double familiarity1 = users[j].familiarity;
                users[j].username = users[i].username;
                users[j].familiarity = users[i].familiarity;
                users[i].username = username1;
                users[j].familiarity = familiarity1;
            }
		}

	}
}

void search::getData(QSqlQuery q)
{
	while(q.next()){
		user tmp;
        tmp.username = q.value(0).toString();
        if(tmp.username == getUsername()) continue;
		users.push_back(tmp);
	}

	q.clear();
}

void search::on_btn_refresh_clicked()
{
	ui->le_search->setText("");
	users.clear();
	start = 0;
	suggestion();
	setData();
}

void search::on_btn_down_clicked()
{
	if(start < users.size() - 6){
		start++;
		setData();
	}
}

void search::on_btn_up_clicked()
{
	if(start > 0){
		start--;
		setData();
	}
}

void search::follow(int index)
{
	if(!is_public(users[index].username)) {
		QMessageBox::warning(this, "This Page Is Private!", "Your Request Send For User.");
		if(!db.open()) {
			QMessageBox::warning(this, "Database Problem!", "Not Successes To Open Database. \nPage : recommend -> Function : follow");
			return;
		}
		QSqlQuery q;
		QString table_name = users[index].username + "_messages";
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
		if(!q.exec("INSERT INTO '"+table_name +"'(username) VALUES('"+ users[index].username +"'); ")) {
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
		table_name = users[index].username + "_followers";
		if(!q.exec("INSERT INTO '"+table_name +"'(username) VALUES('"+username_visitor +"'); ")) {
			QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Insert New Followers. \nPage : recommend -> Function : follow");
			db.close();
			return;
		}
		q.clear();
	}

	// increase number of followers
	{
		table_name = users[index].username + "_information";
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

void search::unfollow(int index)
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
		if(!q.exec("DELETE FROM '"+table_name +"' WHERE username = '"+users[index].username +"'; ")) {
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
		table_name = users[index].username + "_followers";
		if(!q.exec("DELETE FROM '"+table_name +"' WHERE username = '"+username_visitor +"'; ")) {
			QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Delete Follower. \nPage : recommend -> Function : unfollow");
			db.close();
			return;
		}
		q.clear();
	}

	// decrease number of followers
	{
		table_name = users[index].username + "_information";
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

void search::on_btn_follow1_clicked()
{
	QString otherUser = ui->pte_user1->toPlainText();
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
		if(is_public(users[start].username)) {
			ui->btn_follow1->setText("UnFollow");
			ui->btn_follow1->setStyleSheet(push_button_css_unfollow);
		}
	}
}

void search::on_btn_follow2_clicked()
{
	QString otherUser = ui->pte_user2->toPlainText();
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
		if(is_public(users[start + 1].username)) {
			ui->btn_follow2->setText("UnFollow");
			ui->btn_follow2->setStyleSheet(push_button_css_unfollow);
		}
	}
}

void search::on_btn_follow3_clicked()
{
	QString otherUser = ui->pte_user3->toPlainText();
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
		if(is_public(users[start + 2].username)) {
			ui->btn_follow3->setText("UnFollow");
			ui->btn_follow3->setStyleSheet(push_button_css_unfollow);
		}
	}
}

void search::on_btn_follow4_clicked()
{
	QString otherUser = ui->pte_user4->toPlainText();
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
		if(is_public(users[start + 3].username)) {
			ui->btn_follow4->setText("UnFollow");
			ui->btn_follow4->setStyleSheet(push_button_css_unfollow);
		}
	}
}

void search::on_btn_follow5_clicked()
{
	QString otherUser = ui->pte_user5->toPlainText();
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
		if(is_public(users[start + 4].username)) {
			ui->btn_follow5->setText("UnFollow");
			ui->btn_follow5->setStyleSheet(push_button_css_unfollow);
		}
	}
}

void search::on_btn_follow6_clicked()
{
	QString otherUser = ui->pte_user6->toPlainText();
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
		if(is_public(users[start + 5].username)) {
			ui->btn_follow6->setText("UnFollow");
			ui->btn_follow6->setStyleSheet(push_button_css_unfollow);
		}
	}
}

void search::on_pte_user1_selectionChanged()
{
	QString otherUser = ui->pte_user1->toPlainText();
	if(otherUser.isEmpty()) {
		return;
	}
	otherUser = otherUser.split("@")[1];
	other_page * op = new other_page(otherUser);
	op->show();
	this->close();
}

void search::on_pte_user2_selectionChanged()
{
	QString otherUser = ui->pte_user2->toPlainText();
	if(otherUser.isEmpty()) {
		return;
	}
	otherUser = otherUser.split("@")[1];
	other_page * op = new other_page(otherUser);
	op->show();
	this->close();
}

void search::on_pte_user3_selectionChanged()
{
	QString otherUser = ui->pte_user3->toPlainText();
	if(otherUser.isEmpty()) {
		return;
	}
	otherUser = otherUser.split("@")[1];
	other_page * op = new other_page(otherUser);
	op->show();
	this->close();
}

void search::on_pte_user4_selectionChanged()
{
	QString otherUser = ui->pte_user4->toPlainText();
	if(otherUser.isEmpty()) {
		return;
	}
	otherUser = otherUser.split("@")[1];
	other_page * op = new other_page(otherUser);
	op->show();
	this->close();
}

void search::on_pte_user5_selectionChanged()
{
	QString otherUser = ui->pte_user5->toPlainText();
	if(otherUser.isEmpty()) {
		return;
	}
	otherUser = otherUser.split("@")[1];
	other_page * op = new other_page(otherUser);
	op->show();
	this->close();
}

void search::on_pte_user6_selectionChanged()
{
	QString otherUser = ui->pte_user6->toPlainText();
	if(otherUser.isEmpty()) {
		return;
	}
	otherUser = otherUser.split("@")[1];
	other_page * op = new other_page(otherUser);
	op->show();
	this->close();
}
