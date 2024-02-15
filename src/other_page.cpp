#include "other_page.h"
#include "ui_other_page.h"

other_page::other_page(QString username, QWidget *parent) :
	QWidget(parent),
	ui(new Ui::other_page)
{
	ui->setupUi(this);
	this->setWindowState(Qt::WindowMaximized);
	db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName(addressDatabas);
	username_owner = username;
	get_username();
	show_posts();
	show_number_followings_followers();
	show_information();
	set_followed();
	ui->label_userposts->setText(username_owner + " Posts");
	add_to_stack("other_page");
}

other_page::~other_page()
{
	delete ui;
}

class other_page::post {
public:
	int id;
	QString text;
	int number_like;
	QString users_like;
	QList<QString> list_users_like;
	QString create_date;
};

bool other_page::is_public(QString account)
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

void other_page::add_to_stack(QString page_name)
{
	QFile stack_pages("stack.txt");
	if(!stack_pages.open(QFile::ReadWrite | QFile::Text )) {
		QMessageBox::warning(this, "File Problem!", "Didn't Open curUser.txt File. \nPage : other_page -> Function : add_to_stack");
		return;
	}

	QTextStream in(&stack_pages);
	while(!in.atEnd())
		in.readLine();
	in << page_name + "," + username_owner + "\n";
	stack_pages.close();
}

void other_page::follow()
{
	if(!is_public(username_owner)) {
		QMessageBox::warning(this, "This Page Is Private!", "Your Request Send For User.");
		if(!db.open()) {
			QMessageBox::warning(this, "Database Problem!", "Not Successes To Open Database. \nPage : recommend -> Function : follow");
			return;
		}
		QSqlQuery q;
		QString table_name = username_owner + "_messages";
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

	followed = true;

	QString table_name = username_visitor + "_following";
	int number_followings = 0, number_followers = 0;
	if(!db.open()) {
		QMessageBox::warning(this, "Database Problem!", "Not Successes To Open Database. \nPage : recommend -> Function : follow");
		return;
	}
	QSqlQuery q, qu, que;
	// add new username to followings
	{
		if(!q.exec("INSERT INTO '"+table_name +"'(username) VALUES('"+username_owner +"'); ")) {
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
		table_name = username_owner + "_followers";
		if(!q.exec("INSERT INTO '"+table_name +"'(username) VALUES('"+username_visitor +"'); ")) {
			QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Insert New Followers. \nPage : recommend -> Function : follow");
			db.close();
			return;
		}
		q.clear();
	}

	// increase number of followers
	{
		table_name = username_owner + "_information";
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

void other_page::unfollow()
{
	followed = false;

	QString table_name = username_visitor + "_following";
	int number_followings = 0, number_followers = 0;
	if(!db.open()) {
		QMessageBox::warning(this, "Database Problem!", "Not Successes To Open Database. \nPage : recommend -> Function : unfollow");
		return;
	}
	QSqlQuery q, qu, que;
	// delete new username to followings
		{
		if(!q.exec("DELETE FROM '"+table_name +"' WHERE username = '"+username_owner +"'; ")) {
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
		table_name = username_owner+ "_followers";
		if(!q.exec("DELETE FROM '"+table_name +"' WHERE username = '"+username_visitor +"'; ")) {
			QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Delete Follower. \nPage : recommend -> Function : unfollow");
			db.close();
			return;
		}
		q.clear();
	}

	// decrease number of followers
	{
		table_name = username_owner + "_information";
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

void other_page::set_followed()
{
	QString table_name = username_visitor + "_following";
	if(!db.open()) {
		QMessageBox::warning(this, "Database Problem!", "Not Successes To Open Database. \nPage : other_page -> Function : set_followed");
		return;
	}
	QSqlQuery q;
	if(!q.exec("SELECT * FROM '"+table_name +"' WHERE username = '"+username_owner +"'; ")) {
		QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Update. \nPage : other_page -> Function : set_followed");
		db.close();
		return;
	}
	while(q.next())
		followed = true;
	q.clear();
	db.close();

	if(followed) {
		ui->btn_follow->setText("UnFollow");
		ui->btn_follow->setStyleSheet(push_button_css_unfollow);
	}
	else {
		ui->btn_follow->setText("Follow");
		ui->btn_follow->setStyleSheet(push_button_css_follow);
	}
}

void other_page::get_username()
{
	QFile curUser("curUser.txt");
	if(!curUser.open(QFile::ReadOnly | QFile::Text )) {
			QMessageBox::warning(this, "File Problem!", "Not Open curUser.txt File. \nPage : personal_page -> Function : get_username");
			return;
	}
	QTextStream out(&curUser);
	out >> username_visitor;
	curUser.close();
}

bool other_page::check_like(int index)
{
	int exist = posts[index].list_users_like.count(username_visitor);
	if(exist == 0) {
		liked[index] = false;
		return false;
	}
	liked[index] = true;
	return true;
}

void other_page::list_users_liked()
{
	int len = posts.length();
	QString text;
	for(int i = 0; i < len; i++) {
		text = posts[i].users_like;
		int len2 = text.split(",").length();
		for (int j = 0; j < len2; j++)
			posts[i].list_users_like.push_back(text.split(",")[j]);
	}
}

void other_page::like(int index)
{
	liked[first_post_index+index] = true;

	posts[first_post_index+index].list_users_like.push_back(username_visitor);
	QString users;
	if(posts[first_post_index+index].users_like == "")
		users = username_visitor;
	else
		users = posts[first_post_index+index].users_like + "," + username_visitor;

	QString table_name = username_owner + "_posts";
	QString id = QString::number(posts[first_post_index+index].id);
	QString number_like = QString::number(++(posts[first_post_index+index].number_like));

	if(!db.open()) {
		QMessageBox::warning(this, "Database Problem!", "Not Successes To Open Database. \nPage : other_page -> Function : on_btn_like1_clicked");
		return;
	}
	QSqlQuery q;
	if(!q.exec("UPDATE '"+table_name +"' SET likeUsers = '"+users +"' WHERE id = '"+id +"'; ")) {
		QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Update. \nPage : other_page -> Function : on_btn_like1_cliked");
		db.close();
		return;
	}
	q.clear();
	if(!q.exec("UPDATE '"+table_name +"' SET likeN = '"+number_like +"' WHERE id = '"+id +"'; ")) {
		QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Update. \nPage : other_page -> Function : on_btn_like1_cliked");
		db.close();
		return;
	}

	q.clear();
	db.close();
}

void other_page::unlike(int index)
{
	liked[first_post_index+index] = false;

	int ind = posts[first_post_index+index].list_users_like.indexOf(username_visitor);
	posts[first_post_index+index].list_users_like.removeAt(ind);

	QString users = "";
	int len2 = posts[first_post_index+index].list_users_like.length();
	for (int i = 0; i < len2; i++) {
		if (i == len2-1)
			users = users + posts[first_post_index+index].list_users_like[i];
		else
			users = users + posts[first_post_index+index].list_users_like[i] + ",";
	}


	QString table_name = username_owner + "_posts";
	QString id = QString::number(posts[first_post_index+index].id);
	QString number_like = QString::number(--(posts[first_post_index+index].number_like));

	if(!db.open()) {
		QMessageBox::warning(this, "Database Problem!", "Not Successes To Open Database. \nPage : other_page -> Function : on_btn_like1_clicked");
		return;
	}
	QSqlQuery q;
	if(!q.exec("UPDATE '"+table_name +"' SET likeUsers = '"+users +"' WHERE id = '"+id +"'; ")) {
		QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Update. \nPage : other_page -> Function : on_btn_like1_cliked");
		db.close();
		return;
	}
	q.clear();
	if(!q.exec("UPDATE '"+table_name +"' SET likeN = '"+number_like +"' WHERE id = '"+id +"'; ")) {
		QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Update. \nPage : other_page -> Function : on_btn_like1_cliked");
		db.close();
		return;
	}

	q.clear();
	db.close();
}

void other_page::show_information()
{
	QString table_name = username_owner + "_information";
	if(!db.open()) {
		QMessageBox::warning(this, "Database Problem!", "Not Successes To Open Database. \nPage : other_page -> Function : show_information");
		return;
	}
	QSqlQuery q;
	if(!q.exec("SELECT * FROM '"+table_name +"'; ")) {
		QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Get Information. \nPage : other_page -> Function : show_information");
		db.close();
		return;
	}
	QString name, lastname, bio, text;
	while(q.next()) {
		name = q.value(2).toString();
		lastname = q.value(3).toString();
		bio = q.value(6).toString();
	}
	q.clear();
	db.close();

	text = "@" + username_owner + "\n\n" + "Name: " + name + " " + lastname + "\n\n" + "Bio: " + bio;
	ui->pte_information->setPlainText(text);
}

void other_page::show_number_followings_followers()
{
	QString table_name = username_owner + "_information";
	if(!db.open()) {
		QMessageBox::warning(this, "Database Problem!", "Not Successes To Open Database. \nPage : other_page -> Function : show_number_followings_followers");
		return;
	}
	QSqlQuery q;
	if(!q.exec("SELECT * FROM '"+table_name +"'; ")) {
		QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Get Numbers. \nPage : other_page -> Function : show_number_followings_followers");
		db.close();
		return;
	}
	QString number_followings, number_followers;
	while(q.next()) {
		number_followers = q.value(9).toString();
		number_followings = q.value(10).toString();
	}
	q.clear();
	db.close();

	ui->label_numberfollowers->setText(number_followers);
	ui->label_numberfollowings->setText(number_followings);
}

void other_page::show_posts()
{
	QString table_name = username_owner + "_posts";

	if(!db.open()) {
		QMessageBox::warning(this, "Database Problem!", "Not Successes To Open Database. \nPage : other_page -> Function : show_posts");
		return;
	}
	QSqlQuery q;
	if(!q.exec("SELECT * FROM '"+table_name +"' ORDER BY createDate DESC; ")) {
		QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Get Posts. \nPage : other_page -> Function : show_posts");
		db.close();
		return;
	}
	post post1;
	while(q.next()) {
		post1.id = q.value(0).toInt();
		post1.text = q.value(1).toString();
		post1.number_like = q.value(2).toInt();
		post1.users_like = q.value(3).toString();
		post1.create_date = q.value(4).toString();
		posts.push_back(post1);
	}

	q.clear();
	db.close();

	list_users_liked();
	int len = posts.length();
	liked.resize(len);
	first_post_index = 0;
	if (len > 0) {
		ui->pte_post1->setPlainText(posts[0].text);
		ui->label_numberlike1->setText("     " + QString::number(posts[0].number_like));
		if (check_like(0))
			ui->btn_like1->setStyleSheet(push_button_css_like);

	}
	if (len > 1) {
		ui->pte_post2->setPlainText(posts[1].text);
		ui->label_numberlike2->setText("     " + QString::number(posts[1].number_like));
		if (check_like(1))
			ui->btn_like2->setStyleSheet(push_button_css_like);

	}
	if (len > 2) {
		ui->pte_post3->setPlainText(posts[2].text);
		ui->label_numberlike3->setText("     " + QString::number(posts[2].number_like));
		if (check_like(2))
			ui->btn_like3->setStyleSheet(push_button_css_like);

	}
	if (len > 3) {
		ui->pte_post4->setPlainText(posts[3].text);
		ui->label_numberlike4->setText("     " + QString::number(posts[3].number_like));
		if (check_like(3))
			ui->btn_like4->setStyleSheet(push_button_css_like);

	}
	if (len > 4) {
		ui->pte_post5->setPlainText(posts[4].text);
		ui->label_numberlike5->setText("     " + QString::number(posts[4].number_like));
		if (check_like(4))
			ui->btn_like5->setStyleSheet(push_button_css_like);

	}
	if (len > 5) {
		ui->pte_post6->setPlainText(posts[5].text);
		ui->label_numberlike6->setText("     " + QString::number(posts[5].number_like));
		if (check_like(5))
			ui->btn_like6->setStyleSheet(push_button_css_like);

	}
}

void other_page::show_posts2()
{
	ui->pte_post1->setPlainText(posts[first_post_index].text);
	ui->pte_post2->setPlainText(posts[first_post_index+1].text);
	ui->pte_post3->setPlainText(posts[first_post_index+2].text);
	ui->pte_post4->setPlainText(posts[first_post_index+3].text);
	ui->pte_post5->setPlainText(posts[first_post_index+4].text);
	ui->pte_post6->setPlainText(posts[first_post_index+5].text);

	ui->pte_post1->setPlainText(posts[first_post_index].text);
	ui->label_numberlike1->setText("     " + QString::number(posts[first_post_index].number_like));
	if (check_like(first_post_index))
		ui->btn_like1->setStyleSheet(push_button_css_like);

	else
		ui->btn_like1->setStyleSheet(push_button_css_unlike);

	ui->pte_post2->setPlainText(posts[first_post_index + 1].text);
	ui->label_numberlike2->setText("     " + QString::number(posts[first_post_index + 1].number_like));
	if (check_like(first_post_index + 1))
		ui->btn_like2->setStyleSheet(push_button_css_like);

	else
		ui->btn_like2->setStyleSheet(push_button_css_unlike);

	ui->pte_post3->setPlainText(posts[first_post_index + 2].text);
	ui->label_numberlike3->setText("     " + QString::number(posts[first_post_index + 2].number_like));
	if (check_like(first_post_index + 2))
		ui->btn_like3->setStyleSheet(push_button_css_like);

	else
		ui->btn_like3->setStyleSheet(push_button_css_unlike);

	ui->pte_post4->setPlainText(posts[first_post_index + 3].text);
	ui->label_numberlike4->setText("     " + QString::number(posts[first_post_index + 3].number_like));
	if (check_like(first_post_index + 3))
		ui->btn_like4->setStyleSheet(push_button_css_like);

	else
		ui->btn_like4->setStyleSheet(push_button_css_unlike);

	ui->pte_post5->setPlainText(posts[first_post_index + 4].text);
	ui->label_numberlike5->setText("     " + QString::number(posts[first_post_index + 4].number_like));
	if (check_like(first_post_index + 4))
		ui->btn_like5->setStyleSheet(push_button_css_like);

	else
		ui->btn_like5->setStyleSheet(push_button_css_unlike);

	ui->pte_post6->setPlainText(posts[first_post_index + 5].text);
	ui->label_numberlike6->setText("     " + QString::number(posts[first_post_index + 5].number_like));
	if (check_like(first_post_index + 5))
		ui->btn_like6->setStyleSheet(push_button_css_like);

	else
		ui->btn_like6->setStyleSheet(push_button_css_unlike);
}

void other_page::on_btn_personalpage_clicked()
{
	personal_page * pg = new personal_page();
	pg->show();
	this->close();
}

void other_page::on_btn_followers_clicked()
{
	show_followers * sf = new show_followers(username_owner);
	sf->show();
	this->close();
}

void other_page::on_btn_followings_clicked()
{
	show_followings * sf = new show_followings(username_owner);
	sf->show();
	this->close();
}

void other_page::on_btn_like1_clicked()
{
	int len = posts.length();
	if (len > 0) {
		if (liked[first_post_index] == true) {
			ui->btn_like1->setStyleSheet(push_button_css_unlike);

			unlike(0);
		}
		else {
			ui->btn_like1->setStyleSheet(push_button_css_like);

			like(0);
		}
		ui->label_numberlike1->setText("     " + QString::number(posts[first_post_index].number_like));
	}
}

void other_page::on_btn_like2_clicked()
{
	int len = posts.length();
	if (len > 1) {
		if (liked[first_post_index+1] == true) {
			ui->btn_like2->setStyleSheet(push_button_css_unlike);

			unlike(1);
		}
		else {
			ui->btn_like2->setStyleSheet(push_button_css_like);

			like(1);
		}
		ui->label_numberlike2->setText("     " + QString::number(posts[first_post_index+1].number_like));
	}
}

void other_page::on_btn_like3_clicked()
{
	int len = posts.length();
	if (len > 2) {
		if (liked[first_post_index+2] == true) {
			ui->btn_like3->setStyleSheet(push_button_css_unlike);

			unlike(2);
		}
		else {
			ui->btn_like3->setStyleSheet(push_button_css_like);

			like(2);
		}
		ui->label_numberlike3->setText("     " + QString::number(posts[first_post_index+2].number_like));
	}
}

void other_page::on_btn_like4_clicked()
{
	int len = posts.length();
	if (len > 3) {
		if (liked[first_post_index+3] == true) {
			ui->btn_like4->setStyleSheet(push_button_css_unlike);

			unlike(3);
		}
		else {
			ui->btn_like4->setStyleSheet(push_button_css_like);

			like(3);
		}
		ui->label_numberlike4->setText("     " + QString::number(posts[first_post_index+3].number_like));
	}
}

void other_page::on_btn_like5_clicked()
{
	int len = posts.length();
	if (len > 4) {
		if (liked[first_post_index+4] == true) {
			ui->btn_like5->setStyleSheet(push_button_css_unlike);

			unlike(4);
		}
		else {
			ui->btn_like5->setStyleSheet(push_button_css_like);

			like(4);
		}
		ui->label_numberlike5->setText("     " + QString::number(posts[first_post_index+4].number_like));
	}
}

void other_page::on_btn_like6_clicked()
{
	int len = posts.length();
	if (len > 5) {
		if (liked[first_post_index+5] == true) {
			ui->btn_like6->setStyleSheet(push_button_css_unlike);

			unlike(5);
		}
		else {
			ui->btn_like6->setStyleSheet(push_button_css_like);

			like(5);
		}
		ui->label_numberlike6->setText("     " + QString::number(posts[first_post_index+5].number_like));
	}
}

void other_page::on_btn_follow_clicked()
{
	if(followed) {
		unfollow();
		ui->btn_follow->setText("Follow");
		ui->btn_follow->setStyleSheet(push_button_css_follow);
	}
	else {
		follow();
		if(is_public(username_owner)) {
			ui->btn_follow->setText("UnFollow");
			ui->btn_follow->setStyleSheet(push_button_css_unfollow);
		}
	}
}

void other_page::on_btn_back_clicked()
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

	QString pervious_page= pages.dequeue();
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
