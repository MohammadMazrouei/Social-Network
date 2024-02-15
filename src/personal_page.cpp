#include "personal_page.h"
#include "ui_personal_page.h"

personal_page::personal_page(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::personal_page)
{
	ui->setupUi(this);
	this->setWindowState(Qt::WindowMaximized);
	db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName(addressDatabas);
	get_username();
	show_posts();
	show_number_followings_followers();
	show_information();
	set_stack();
}

personal_page::~personal_page()
{
	delete ui;
}

class personal_page::post {
public:
	int id;
	QString text;
	int number_like;
	QString users_like;
	QList<QString> list_users_like;
	QString create_date;
};

void personal_page::set_stack()
{
	QFile stack_pages("stack.txt");
	if(!stack_pages.open(QFile::WriteOnly | QFile::Text )) {
		QMessageBox::warning(this, "File Problem!", "Didn't Open curUser.txt File. \nPage : personal_page -> Function : set_stack");
		return;
	}
	QTextStream in(&stack_pages);
	in << "";
	stack_pages.close();
}

bool personal_page::check_like(int index)
{
	int exist = posts[index].list_users_like.count(username);
	if(exist == 0)
		return false;
	return true;
}

void personal_page::list_users_liked()
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

void personal_page::like(int index)
{
	liked[first_post_index+index] = true;

	posts[first_post_index+index].list_users_like.push_back(username);
	QString users;
	if(posts[first_post_index+index].users_like == "")
		users = username;
	else
		users = posts[first_post_index+index].users_like + "," + username;
	posts[first_post_index+index].users_like = users;

	QString table_name = username + "_posts";
	QString id = QString::number(posts[first_post_index+index].id);
	QString number_like = QString::number(++(posts[first_post_index+index].number_like));

	if(!db.open()) {
		QMessageBox::warning(this, "Database Problem!", "Not Successes To Open Database. \nPage : personal_page -> Function : on_btn_like1_clicked");
		return;
	}
	QSqlQuery q;
	if(!q.exec("UPDATE '"+table_name +"' SET likeUsers = '"+users +"' WHERE id = '"+id +"'; ")) {
		QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Update. \nPage : personal_page -> Function : on_btn_like1_cliked");
		db.close();
		return;
	}
	q.clear();
	if(!q.exec("UPDATE '"+table_name +"' SET likeN = '"+number_like +"' WHERE id = '"+id +"'; ")) {
		QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Update. \nPage : personal_page -> Function : on_btn_like1_cliked");
		db.close();
		return;
	}

	q.clear();
	db.close();
}

void personal_page::unlike(int index)
{
	liked[first_post_index+index] = false;

	int ind = posts[first_post_index+index].list_users_like.indexOf(username);
	posts[first_post_index+index].list_users_like.removeAt(ind);

	QString users = "";
	int len2 = posts[first_post_index+index].list_users_like.length();
	for (int i = 0; i < len2; i++) {
		if (i == len2-1)
			users = users + posts[first_post_index+index].list_users_like[i];
		else
			users = users + posts[first_post_index+index].list_users_like[i] + ",";
	}
	posts[first_post_index+index].users_like = users;


	QString table_name = username + "_posts";
	QString id = QString::number(posts[first_post_index+index].id);
	QString number_like = QString::number(--(posts[first_post_index+index].number_like));

	if(!db.open()) {
		QMessageBox::warning(this, "Database Problem!", "Not Successes To Open Database. \nPage : personal_page -> Function : on_btn_like1_clicked");
		return;
	}
	QSqlQuery q;
	if(!q.exec("UPDATE '"+table_name +"' SET likeUsers = '"+users +"' WHERE id = '"+id +"'; ")) {
		QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Update. \nPage : personal_page -> Function : on_btn_like1_cliked");
		db.close();
		return;
	}
	q.clear();
	if(!q.exec("UPDATE '"+table_name +"' SET likeN = '"+number_like +"' WHERE id = '"+id +"'; ")) {
		QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Update. \nPage : personal_page -> Function : on_btn_like1_cliked");
		db.close();
		return;
	}

	q.clear();
	db.close();
}

void personal_page::show_information()
{
	QString table_name = username + "_information";
	if(!db.open()) {
		QMessageBox::warning(this, "Database Problem!", "Not Successes To Open Database. \nPage : personal_page -> Function : show_information");
		return;
	}
	QSqlQuery q;
	if(!q.exec("SELECT * FROM '"+table_name +"'; ")) {
		QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Get Information. \nPage : personal_page -> Function : show_information");
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

	text = "@" + username + "\n\n" + "Name: " + name + " " + lastname + "\n\n" + "Bio: " + bio;
	ui->pte_information->setPlainText(text);
}

void personal_page::show_number_followings_followers()
{
	QString table_name = username + "_information";
	if(!db.open()) {
		QMessageBox::warning(this, "Database Problem!", "Not Successes To Open Database. \nPage : personal_page -> Function : show_number_followings_followers");
		return;
	}
	QSqlQuery q;
	if(!q.exec("SELECT * FROM '"+table_name +"'; ")) {
		QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Get Numbers. \nPage : personal_page -> Function : show_number_followings_followers");
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

void personal_page::get_username()
{
	QFile curUser("curUser.txt");
	if(!curUser.open(QFile::ReadOnly | QFile::Text )) {
			QMessageBox::warning(this, "File Problem!", "Not Open curUser.txt File. \nPage : personal_page -> Function : get_username");
			return;
	}
	QTextStream out(&curUser);
	out >> username;
	curUser.close();
}

void personal_page::show_posts()
{
	QString table_name = username + "_posts";

	if(!db.open()) {
		QMessageBox::warning(this, "Database Problem!", "Not Successes To Open Database. \nPage : personal_page -> Function : show_posts");
		return;
	}
	QSqlQuery q;
	if(!q.exec("SELECT * FROM '"+table_name +"' ORDER BY createDate DESC; ")) {
		QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Get Posts. \nPage : personal_page -> Function : show_posts");
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
	liked.resize(len+10);
	for(int i = 0; i < len; i++)
		liked[i] = check_like(i);

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

void personal_page::show_posts2()
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

void personal_page::on_btn_addpost_clicked()
{
	add_post * ap = new add_post();
	ap->show();
	this->close();
}

void personal_page::on_btn_followers_clicked()
{
	show_followers * sf = new show_followers(username);
	sf->show();
	this->close();
}

void personal_page::on_btn_followings_clicked()
{
    show_followings * sf = new show_followings(username);
	sf->show();
	this->close();
}

void personal_page::on_btn_setting_clicked()
{
	setting * se = new setting();
	se->show();
	this->close();
}

void personal_page::on_btn_search_clicked()
{
	search * sr = new search();
	sr->show();
	this->close();
}

void personal_page::on_btn_followingsposts_clicked()
{
	Followings_posts * fp = new Followings_posts();
	fp->show();
	this->close();
}

void personal_page::on_btn_explorer_clicked()
{
	explorer * ex = new explorer();
	ex->show();
	this->close();
}

void personal_page::on_pte_post1_selectionChanged()
{
	int len = posts.length();
	if (len > 0) {
		show_post * sp = new show_post(username, posts[first_post_index].id);
		sp->show();
		this->close();
	}
}

void personal_page::on_pte_post2_selectionChanged()
{
	int len = posts.length();
	if (len > 1) {
		show_post * sp = new show_post(username, posts[first_post_index + 1].id);
		sp->show();
		this->close();
	}
}

void personal_page::on_pte_post3_selectionChanged()
{
	int len = posts.length();
	if (len > 2) {
		show_post * sp = new show_post(username, posts[first_post_index + 2].id);
		sp->show();
		this->close();
	}
}

void personal_page::on_pte_post4_selectionChanged()
{
	int len = posts.length();
	if (len > 3) {
		show_post * sp = new show_post(username, posts[first_post_index + 3].id);
		sp->show();
		this->close();
	}
}

void personal_page::on_pte_post5_selectionChanged()
{
	int len = posts.length();
	if (len > 4) {
		show_post * sp = new show_post(username, posts[first_post_index + 4].id);
		sp->show();
		this->close();
	}
}

void personal_page::on_pte_post6_selectionChanged()
{
	int len = posts.length();
	if (len > 5) {
		show_post * sp = new show_post(username, posts[first_post_index + 5].id);
		sp->show();
		this->close();
	}
}

void personal_page::on_btn_next_clicked()
{
	int len = posts.length();
	if (first_post_index + 6 < len) {
		first_post_index++;
		show_posts2();
	}
}

void personal_page::on_btn_pervious_clicked()
{
	if (first_post_index != 0) {
		first_post_index--;
		show_posts2();
	}
}

void personal_page::on_btn_like1_clicked()
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

void personal_page::on_btn_like2_clicked()
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

void personal_page::on_btn_like3_clicked()
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

void personal_page::on_btn_like4_clicked()
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

void personal_page::on_btn_like5_clicked()
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

void personal_page::on_btn_like6_clicked()
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
