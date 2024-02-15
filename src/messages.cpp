#include "messages.h"
#include "ui_messages.h"

messages::messages(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::messages)
{
	ui->setupUi(this);
	this->setWindowState(Qt::WindowMaximized);
	db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName(addressDatabas);
	get_username();
	show_messages();
}

messages::~messages()
{
	delete ui;
}

void messages::get_username()
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

void messages::show_messages()
{
    ui->pte_message1->setPlainText("");
    ui->pte_message2->setPlainText("");
    ui->pte_message3->setPlainText("");
    ui->pte_message4->setPlainText("");
    ui->pte_message5->setPlainText("");
    ui->pte_message6->setPlainText("");

	QString table_name = username + "_messages";
    my_messages.clear();
    usernames.clear();
	if(!db.open()) {
		QMessageBox::warning(this, "Database Problem!", "Not Successes To Open Database. \nPage : personal_page -> Function : show_posts");
		return;
	}
	QSqlQuery q;
	if(!q.exec("SELECT usernameSend, txt FROM '"+table_name +"' ORDER BY createDate DESC; ")) {
		QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Get Posts. \nPage : personal_page -> Function : show_posts");
		db.close();
		return;
	}

	while(q.next()) {
		usernames.push_back(q.value(0).toString());
		my_messages.push_back(q.value(1).toString());
	}

	q.clear();
	db.close();


	int len = my_messages.length();
	first_post_index = 0;
	if (len > 0) {
		ui->pte_message1->setPlainText(my_messages[0]);
	}
	if (len > 1) {
		ui->pte_message2->setPlainText(my_messages[1]);
	}
	if (len > 2) {
		ui->pte_message3->setPlainText(my_messages[2]);
	}
	if (len > 3) {
		ui->pte_message4->setPlainText(my_messages[3]);
	}
	if (len > 4) {
		ui->pte_message5->setPlainText(my_messages[4]);
	}
	if (len > 5) {
		ui->pte_message6->setPlainText(my_messages[5]);
	}
}

void messages::show_messages2()
{
	ui->pte_message1->setPlainText(my_messages[first_post_index]);
	ui->pte_message2->setPlainText(my_messages[first_post_index + 1]);
	ui->pte_message3->setPlainText(my_messages[first_post_index + 2]);
	ui->pte_message4->setPlainText(my_messages[first_post_index + 3]);
	ui->pte_message5->setPlainText(my_messages[first_post_index + 4]);
	ui->pte_message6->setPlainText(my_messages[first_post_index + 5]);
}

void messages::follow(int index)
{
    QString table_name = usernames[index] + "_following";
    int number_followings = 0, number_followers = 0;
    if(!db.open()) {
        QMessageBox::warning(this, "Database Problem!", "Not Successes To Open Database. \nPage : recommend -> Function : follow");
        return;
    }
    QSqlQuery q, qu, que;
    qDebug() << table_name;
    // add new username to followings
    {
        if(!q.exec("INSERT INTO '"+table_name +"'(username) VALUES('"+ username +"'); ")) {
            QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Insert New Following. \nPage : recommend -> Function : follow");
            db.close();
            return;
        }
        q.clear();
    }

    // increase number of followings
    {
        table_name = usernames[index] + "_information";
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
        table_name = username + "_followers";
        if(!q.exec("INSERT INTO '"+table_name +"'(username) VALUES('"+usernames[index] +"'); ")) {
            QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Insert New Followers. \nPage : recommend -> Function : follow");
            db.close();
            return;
        }
        q.clear();
    }

    // increase number of followers
    {
        table_name = username + "_information";
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

void messages::on_btn_back_clicked()
{
	setting * se = new setting();
	se->show();
	this->close();
}

void messages::delete_message(int index)
{
    if(!db.open()) {
        QMessageBox::warning(this, "Database Problem!", "Not Successes To Open Database. \nPage : recommend -> Function : follow");
        return;
    }
    QSqlQuery q;
    QString table_name = username + "_messages";
    if(!q.exec("DELETE FROM '"+table_name +"' WHERE usernameSend = '"+usernames[index] +"'; ")) {
        QMessageBox::warning(this, "Query Problem!", "Not Execute Databas1 For Insert New Following. \nPage : recommend -> Function : follow");
        db.close();
        return;
    }
    q.clear();
    db.close();
}

void messages::on_btn_accept1_clicked()
{
    QString otherUser = ui->pte_message1->toPlainText();
    if(otherUser.isEmpty()) {
        return;
    }
    follow(first_post_index);
    delete_message(first_post_index);
    show_messages();
}

void messages::on_btn_accept2_clicked()
{
    QString otherUser = ui->pte_message2->toPlainText();
    if(otherUser.isEmpty()) {
        return;
    }
    follow(first_post_index + 1);
    delete_message(first_post_index + 1);
    show_messages();
}

void messages::on_btn_accept3_clicked()
{
    QString otherUser = ui->pte_message3->toPlainText();
    if(otherUser.isEmpty()) {
        return;
    }
    follow(first_post_index + 2);
    delete_message(first_post_index + 2);
    show_messages();
}

void messages::on_btn_accept4_clicked()
{
    QString otherUser = ui->pte_message4->toPlainText();
    if(otherUser.isEmpty()) {
        return;
    }
    follow(first_post_index + 3);
    delete_message(first_post_index + 3);
    show_messages();
}

void messages::on_btn_accept5_clicked()
{
    QString otherUser = ui->pte_message5->toPlainText();
    if(otherUser.isEmpty()) {
        return;
    }
    follow(first_post_index + 4);
    delete_message(first_post_index + 4);
    show_messages();
}

void messages::on_btn_accept6_clicked()
{
    QString otherUser = ui->pte_message6->toPlainText();
    if(otherUser.isEmpty()) {
        return;
    }
    follow(first_post_index + 5);
    delete_message(first_post_index + 5);
    show_messages();
}

void messages::on_btn_notaccept1_clicked()
{
    QString otherUser = ui->pte_message1->toPlainText();
    if(otherUser.isEmpty()) {
        return;
    }
    delete_message(first_post_index);
    show_messages();
}

void messages::on_btn_notaccept2_clicked()
{
    QString otherUser = ui->pte_message2->toPlainText();
    if(otherUser.isEmpty()) {
        return;
    }
    delete_message(first_post_index + 1);
    show_messages();
}

void messages::on_btn_notaccept3_clicked()
{
    QString otherUser = ui->pte_message3->toPlainText();
    if(otherUser.isEmpty()) {
        return;
    }
    delete_message(first_post_index + 2);
    show_messages();
}

void messages::on_btn_notaccept4_clicked()
{
    QString otherUser = ui->pte_message4->toPlainText();
    if(otherUser.isEmpty()) {
        return;
    }
    delete_message(first_post_index + 3);
    show_messages();
}

void messages::on_btn_notaccept5_clicked()
{
    QString otherUser = ui->pte_message5->toPlainText();
    if(otherUser.isEmpty()) {
        return;
    }
    delete_message(first_post_index + 4);
    show_messages();
}

void messages::on_btn_notaccept6_clicked()
{
    QString otherUser = ui->pte_message6->toPlainText();
    if(otherUser.isEmpty()) {
        return;
    }
    delete_message(first_post_index + 5);
    show_messages();
}

void messages::on_btn_up_clicked()
{
	if (first_post_index != 0) {
		first_post_index--;
        show_messages2();
	}
}

void messages::on_btn_down_clicked()
{
	int len = my_messages.length();
	if (first_post_index + 6 < len) {
		first_post_index++;
        show_messages2();
	}
}
