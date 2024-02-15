#include "explorer.h"
#include "ui_explorer.h"

explorer::explorer(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::explorer)
{
	ui->setupUi(this);
	this->setWindowState(Qt::WindowMaximized);
	db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName(addressDatabas);
	get_username();
	show_posts();
	add_to_stack("explorer");
}

explorer::~explorer()
{
	delete ui;
}

class explorer::post {
public:
	int id;
	QString text;
	int number_like;
	QString users_like;
	QList<QString> list_users_like;
	QString create_date;
	QString creator;
};

void explorer::add_to_stack(QString page_name)
{
	QFile stack_pages("stack.txt");
	if(!stack_pages.open(QFile::WriteOnly | QFile::Text )) {
		QMessageBox::warning(this, "File Problem!", "Didn't Open curUser.txt File. \nPage : explorer -> Function : add_to_stack");
		return;
	}
	QTextStream in(&stack_pages);
	in << page_name + "\n";
	stack_pages.close();
}

void explorer::on_btn_mypage_clicked()
{
	personal_page * pg = new personal_page();
	pg->show();
	this->close();
}

bool explorer::check_like(int index)
{
	int exist = posts[index].list_users_like.count(username);
	if(exist == 0)
		return false;
	return true;
}

void explorer::list_users_liked()
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

void explorer::like(int index)
{
	liked[first_post_index+index] = true;

	posts[first_post_index+index].list_users_like.push_back(username);
	QString users;
	if(posts[first_post_index+index].users_like == "")
		users = username;
	else
		users = posts[first_post_index+index].users_like + "," + username;
	posts[first_post_index+index].users_like = users;

	QString table_name = posts[first_post_index+index].creator + "_posts";
	QString id = QString::number(posts[first_post_index+index].id);
	QString number_like = QString::number(++(posts[first_post_index+index].number_like));

	if(!db.open()) {
		QMessageBox::warning(this, "Database Problem!", "Not Successes To Open Database. \nPage : followings_posts -> Function : on_btn_like1_clicked");
		return;
	}
	QSqlQuery q;
	if(!q.exec("UPDATE '"+table_name +"' SET likeUsers = '"+users +"' WHERE id = '"+id +"'; ")) {
		QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Update. \nPage : followings_posts -> Function : on_btn_like1_cliked");
		db.close();
		return;
	}
	q.clear();
	if(!q.exec("UPDATE '"+table_name +"' SET likeN = '"+number_like +"' WHERE id = '"+id +"'; ")) {
		QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Update. \nPage : followings_posts -> Function : on_btn_like1_cliked");
		db.close();
		return;
	}

	q.clear();
	db.close();
}

void explorer::unlike(int index)
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


	QString table_name = posts[first_post_index+index].creator + "_posts";
	QString id = QString::number(posts[first_post_index+index].id);
	QString number_like = QString::number(--(posts[first_post_index+index].number_like));

	if(!db.open()) {
		QMessageBox::warning(this, "Database Problem!", "Not Successes To Open Database. \nPage : followings_posts -> Function : on_btn_like1_clicked");
		return;
	}
	QSqlQuery q;
	if(!q.exec("UPDATE '"+table_name +"' SET likeUsers = '"+users +"' WHERE id = '"+id +"'; ")) {
		QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Update. \nPage : followings_posts -> Function : on_btn_like1_cliked");
		db.close();
		return;
	}
	q.clear();
	if(!q.exec("UPDATE '"+table_name +"' SET likeN = '"+number_like +"' WHERE id = '"+id +"'; ")) {
		QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Update. \nPage : followings_posts -> Function : on_btn_like1_cliked");
		db.close();
		return;
	}

	q.clear();
	db.close();
}

void explorer::show_posts()
{
	QString table_name = "information";

	if(!db.open()) {
		QMessageBox::warning(this, "Database Problem!", "Not Successes To Open Database. \nPage : followings_posts -> Function : show_posts");
		return;
	}
	QSqlQuery q;
	if(!q.exec("SELECT username FROM '"+table_name +"' WHERE public = 1; ")) {
		QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Get Posts. \nPage : followings_posts -> Function : show_posts");
		db.close();
		return;
	}

	while(q.next())
		followings.push_back(q.value(0).toString());

	int number_followings = followings.length();
	post post1;

	for(int i = 0; i < number_followings; i++) {

		table_name = followings[i] + "_posts";

		if(!q.exec("SELECT * FROM '"+table_name +"' ORDER BY createDate DESC; ")) {
			QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Get Posts. \nPage : followings_posts -> Function : show_posts");
			db.close();
			return;
		}

		while(q.next()) {
			post1.id = q.value(0).toInt();
			post1.text = q.value(1).toString();
			post1.number_like = q.value(2).toInt();
			post1.users_like = q.value(3).toString();
			post1.create_date = q.value(4).toString();
			post1.creator = followings[i];
			posts.push_back(post1);
		}
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
		ui->label_username1->setText("@" + posts[0].creator);
		if (check_like(0))
			ui->btn_like1->setStyleSheet(push_button_css_like);
	}
	if (len > 1) {
		ui->pte_post2->setPlainText(posts[1].text);
		ui->label_numberlike2->setText("     " + QString::number(posts[1].number_like));
		ui->label_username2->setText("@" + posts[1].creator);
		if (check_like(1))
			ui->btn_like2->setStyleSheet(push_button_css_like);
	}
	if (len > 2) {
		ui->pte_post3->setPlainText(posts[2].text);
		ui->label_numberlike3->setText("     " + QString::number(posts[2].number_like));
		ui->label_username3->setText("@" + posts[2].creator);
		if (check_like(2))
			ui->btn_like3->setStyleSheet(push_button_css_like);
	}
	if (len > 3) {
		ui->pte_post4->setPlainText(posts[3].text);
		ui->label_numberlike4->setText("     " + QString::number(posts[3].number_like));
		ui->label_username4->setText("@" + posts[3].creator);
		if (check_like(3))
			ui->btn_like4->setStyleSheet(push_button_css_like);
	}
	if (len > 4) {
		ui->pte_post5->setPlainText(posts[4].text);
		ui->label_numberlike5->setText("     " + QString::number(posts[4].number_like));
		ui->label_username5->setText("@" + posts[4].creator);
		if (check_like(4))
			ui->btn_like5->setStyleSheet(push_button_css_like);
	}
	if (len > 5) {
		ui->pte_post6->setPlainText(posts[5].text);
		ui->label_numberlike6->setText("     " + QString::number(posts[5].number_like));
		ui->label_username6->setText("@" + posts[5].creator);
		if (check_like(5))
			ui->btn_like6->setStyleSheet(push_button_css_like);
	}
}

void explorer::show_posts2()
{
	ui->pte_post1->setPlainText(posts[first_post_index].text);
	ui->pte_post2->setPlainText(posts[first_post_index+1].text);
	ui->pte_post3->setPlainText(posts[first_post_index+2].text);
	ui->pte_post4->setPlainText(posts[first_post_index+3].text);
	ui->pte_post5->setPlainText(posts[first_post_index+4].text);
	ui->pte_post6->setPlainText(posts[first_post_index+5].text);

	ui->pte_post1->setPlainText(posts[first_post_index].text);
	ui->label_numberlike1->setText("     " + QString::number(posts[first_post_index].number_like));
	ui->label_username1->setText("@" + posts[0].creator);
	if (check_like(first_post_index))
		ui->btn_like1->setStyleSheet(push_button_css_like);
	else
		ui->btn_like1->setStyleSheet(push_button_css_unlike);

	ui->pte_post2->setPlainText(posts[first_post_index + 1].text);
	ui->label_numberlike2->setText("     " + QString::number(posts[first_post_index + 1].number_like));
	ui->label_username2->setText("@" + posts[1].creator);
	if (check_like(first_post_index + 1))
		ui->btn_like2->setStyleSheet(push_button_css_like);
	else
		ui->btn_like2->setStyleSheet(push_button_css_unlike);

	ui->pte_post3->setPlainText(posts[first_post_index + 2].text);
	ui->label_numberlike3->setText("     " + QString::number(posts[first_post_index + 2].number_like));
	ui->label_username3->setText("@" + posts[2].creator);
	if (check_like(first_post_index + 2))
		ui->btn_like3->setStyleSheet(push_button_css_like);
	else
		ui->btn_like3->setStyleSheet(push_button_css_unlike);

	ui->pte_post4->setPlainText(posts[first_post_index + 3].text);
	ui->label_numberlike4->setText("     " + QString::number(posts[first_post_index + 3].number_like));
	ui->label_username4->setText("@" + posts[3].creator);
	if (check_like(first_post_index + 3))
		ui->btn_like4->setStyleSheet(push_button_css_like);
	else
		ui->btn_like4->setStyleSheet(push_button_css_unlike);

	ui->pte_post5->setPlainText(posts[first_post_index + 4].text);
	ui->label_numberlike5->setText("     " + QString::number(posts[first_post_index + 4].number_like));
	ui->label_username5->setText("@" + posts[4].creator);
	if (check_like(first_post_index + 4))
		ui->btn_like5->setStyleSheet(push_button_css_like);
	else
		ui->btn_like5->setStyleSheet(push_button_css_unlike);

	ui->pte_post6->setPlainText(posts[first_post_index + 5].text);
	ui->label_numberlike6->setText("     " + QString::number(posts[first_post_index + 5].number_like));
	ui->label_username6->setText("@" + posts[5].creator);
	if (check_like(first_post_index + 5))
		ui->btn_like6->setStyleSheet(push_button_css_like);
	else
		ui->btn_like6->setStyleSheet(push_button_css_unlike);
}

void explorer::get_username()
{
	QFile curUser("curUser.txt");
	if(!curUser.open(QFile::ReadOnly | QFile::Text )) {
			QMessageBox::warning(this, "File Problem!", "Not Open curUser.txt File. \nPage : followings_posts -> Function : get_username");
			return;
	}
	QTextStream out(&curUser);
	out >> username;
	curUser.close();
}

void explorer::on_btn_like1_clicked()
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

void explorer::on_btn_like2_clicked()
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

void explorer::on_btn_like3_clicked()
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

void explorer::on_btn_like4_clicked()
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

void explorer::on_btn_like5_clicked()
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

void explorer::on_btn_like6_clicked()
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

void explorer::on_btn_seepage1_clicked()
{
	int len = posts.length();
	if(len > 0) {
		QString creator_post = posts[first_post_index].creator;
		other_page * op = new other_page(creator_post);
		op->show();
		this->close();
	}
}

void explorer::on_btn_seepage2_clicked()
{
	int len = posts.length();
	if(len > 1) {
		QString creator_post = posts[first_post_index + 1].creator;
		other_page * op = new other_page(creator_post);
		op->show();
		this->close();
	}
}

void explorer::on_btn_seepage3_clicked()
{
	int len = posts.length();
	if(len > 2) {
		QString creator_post = posts[first_post_index + 2].creator;
		other_page * op = new other_page(creator_post);
		op->show();
		this->close();
	}
}

void explorer::on_btn_seepage4_clicked()
{
	int len = posts.length();
	if(len > 3) {
		QString creator_post = posts[first_post_index + 3].creator;
		other_page * op = new other_page(creator_post);
		op->show();
		this->close();
	}
}

void explorer::on_btn_seepage5_clicked()
{
	int len = posts.length();
	if(len > 4) {
		QString creator_post = posts[first_post_index + 4].creator;
		other_page * op = new other_page(creator_post);
		op->show();
		this->close();
	}
}

void explorer::on_btn_seepage6_clicked()
{
	int len = posts.length();
	if(len > 5) {
		QString creator_post = posts[first_post_index + 5].creator;
		other_page * op = new other_page(creator_post);
		op->show();
		this->close();
	}
}

void explorer::on_btn_next_clicked()
{
	int len = posts.length();
	if (first_post_index + 6 < len) {
		first_post_index++;
		show_posts2();
	}
}

void explorer::on_btn_pervious_clicked()
{
	if (first_post_index != 0) {
		first_post_index--;
		show_posts2();
	}
}
