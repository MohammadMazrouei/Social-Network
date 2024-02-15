#include "setting.h"
#include "ui_setting.h"
#include <QFile>

setting::setting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::setting)
{
    ui->setupUi(this);
    this->setWindowState(Qt::WindowMaximized);
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(addressDatabas);
    show_information();
}

setting::~setting()
{
    delete ui;
}

void setting::on_btn_back_clicked()
{
    personal_page * pg = new personal_page();
    pg->show();
    this->close();
}

void setting::on_btn_edit_clicked()
{
    change_profile * cp = new change_profile();
    cp->show();
    this->close();
}

void setting::on_btn_logout_clicked()
{
    QFile curUser("curUser.txt");
    curUser.open(QFile::WriteOnly | QFile::Text );
    curUser.close();

    MainWindow * mw = new MainWindow();
    mw->show();
    this->close();
}

void setting::show_information() {
    //	QString username = getUsername();
    QFile curUser("curUser.txt");
    curUser.open(QFile::ReadOnly | QFile::Text );
    QTextStream in(&curUser);
    QString username = in.readLine();
    curUser.close();

    if (!db.open()) {
        QMessageBox::warning(this, "Not Open Database!", "Not Successes To Open Database. \nPage : setting -> Function : show_informaion");
        return;
    }

    QString tmp = username + "_information";
    QSqlQuery q;
    if(!q.exec("SELECT * FROM '"+tmp +"' WHERE username = '"+username +"' ;")) {
        QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Login Search. \nPage : setting -> Function : show_information");
        db.close();
        return;
    }

    userInfo info;
    while(q.next()) {
        info.name = q.value(2).toString();
        info.lastname = q.value(3).toString();
        info.email = q.value(4).toString();
        info.phonenum = q.value(5).toString();
        info.bio = q.value(6).toString();
        info.birthday = q.value(7).toString();
        info.gender = q.value(8).toInt();
    }
    q.clear();
    db.close();

    ui->pte_username->setPlainText(username);
    ui->pte_name->setPlainText(info.name);
    ui->pte_lastname->setPlainText(info.lastname);
    ui->pte_email->setPlainText(info.email);
    ui->pte_phonenumber->setPlainText(info.phonenum);
    ui->pte_biography->setPlainText(info.bio);
    if(info.gender == 1)	 // female = 1, male = 0;
        ui->label_gender->setText("Female");
    else
        ui->label_gender->setText("Male");
    // year_month_day
    ui->pte_day->setPlainText(info.birthday.split('-')[2]);
    ui->pte_month->setPlainText(info.birthday.split('-')[1]);
    ui->pte_year->setPlainText(info.birthday.split('-')[0]);
}

QString setting::getUsername() {
    QFile curUser("curUser.txt");
    curUser.open(QFile::ReadOnly | QFile::Text );
    QTextStream in(&curUser);
    QString username = in.readLine();
    curUser.close();
    return username;
}

void setting::on_btn_exit_clicked()
{
    exit(0);
}

void setting::on_btn_deleteaccount_clicked()
{
    if (!db.open()) {
        QMessageBox::warning(this, "Not Open Database!", "Not Successes To Open Database. \nPage : setting -> Function : show_informaion");
        return;
    }

    QString username = getUsername();
    QString tableName = username + "_information";
    QSqlQuery q, qu;
    int followerN, followingN;
    // Reading followerN and followingN
    {
        if(!q.exec("SELECT * FROM '"+tableName +"'; ")) {
            QMessageBox::warning(this, "Query Problem!", "Not Execute Database For Reading User Information Table. \nPage : setting -> Function : on_btn_deleteaccount_clicked");
            db.close();
            return;
        }
        while(q.next()) {
            followerN = q.value(9).toInt();
            followingN = q.value(10).toInt();
        }
    }

    // Delete user information table
    {
        if(!q.exec("DROP TABLE IF EXISTS '"+tableName +"'; ")) {
            QMessageBox::warning(this, "Query Problem!", "Not Execute Database For Delete User Information Table. \nPage : setting -> Function : on_btn_deleteaccount_clicked");
            db.close();
            return;
        }
    }
    // Delete all data related to user followers
    tableName = username + "_followers";
    if(followerN != 0)
    {
        QString tmpUser, tmpTableName;

        if(!q.exec("SELECT * FROM '"+tableName +"'; ")) {
            QMessageBox::warning(this, "Query Problem!", "Not Execute Database For Find Followers. \nPage : setting -> Function : on_btn_deleteaccount_clicked");
            db.close();
            return;
        }
        while(q.next()) {
            tmpUser = q.value(1).toString();

            // Delete from followings
            tmpTableName = tmpUser + "_following";
            if(!qu.exec("DELETE FROM '"+tmpTableName +"' WHERE username = '"+username +"'; ")) {
                QMessageBox::warning(this, "Query Problem!", "Not Execute Database For Delete New Following. \nPage : setting -> Function : on_btn_deleteaccount_clicked");
                db.close();
                return;
            }

            // Decrease number of followings
            int followingNum;
            tmpTableName = tmpUser + "_information";
            if(!qu.exec("SELECT followingN FROM '"+tmpTableName +"'; ")) {
                QMessageBox::warning(this, "Query Problem!", "Not Execute Database For Get Number Of Followings. \nPage : setting -> Function : on_btn_deleteaccount_clicked");
                db.close();
                return;
            }
            while(qu.next())
                followingNum = qu.value(0).toInt();
            followingNum--;

            QString followingNumChar = QString::number(followingNum);

            if(!qu.exec("UPDATE '"+tmpTableName +"' SET followingN = '"+followingNumChar +"'; ")) {
                QMessageBox::warning(this, "Query Problem!", "Not Execute Database For Set Number Of Followings. \nPage : setting -> Function : on_btn_deleteaccount_clicked");
                db.close();
                return;
            }
            qu.clear();
        }

    }
    if(!q.exec("DROP TABLE IF EXISTS '"+tableName +"'; ")) {
        QMessageBox::warning(this, "Query Problem!", "Not Execute Database For Delete User Followers Table. \nPage : setting -> Function : on_btn_deleteaccount_clicked");
        db.close();
        return;
    }

    // Delete all data related to user followings
    tableName = username + "_following";
    if(followingN != 0)
    {
        QString tmpUser, tmpTableName;

        if(!q.exec("SELECT * FROM '"+tableName +"'; ")) {
            QMessageBox::warning(this, "Query Problem!", "Not Execute Database For Find Followings. \nPage : setting -> Function : on_btn_deleteaccount_clicked");
            db.close();
            return;
        }
        while(q.next()) {
            tmpUser = q.value(1).toString();

            // Delete from followers
            tmpTableName = tmpUser + "_followers";
            if(!qu.exec("DELETE FROM '"+tmpTableName +"' WHERE username = '"+username+"'; ")){
                QMessageBox::warning(this, "Query Problem!", "Not Execute Databas For Delete New Follower. \nPage : setting  -> Function : on_btn_deleteaccount_clicked");
                db.close();
                return;
            }

            // Decrease number of followers
            int followerNum;
            tmpTableName = tmpUser + "_information";
            if(!qu.exec("SELECT followerN FROM '"+tmpTableName +"'; ")) {
                QMessageBox::warning(this, "Query Problem!", "Not Execute Database For Get Number Of Followers. \nPage : setting -> Function : on_btn_deleteaccount_clicked");
                db.close();
                return;
            }
            while(qu.next())
                followerNum = qu.value(0).toInt();
            followerNum--;

            QString followerNumChar = QString::number(followerNum);

            if(!qu.exec("UPDATE '"+tmpTableName +"' SET followerN = '"+followerNumChar +"'; ")) {
                QMessageBox::warning(this, "Query Problem!", "Not Execute Database For Set Number Of Followers. \nPage : setting -> Function : on_btn_deleteaccount_clicked");
                db.close();
                return;
            }
            qu.clear();
        }

    }
    if(!q.exec("DROP TABLE IF EXISTS '"+tableName +"'; ")) {
        QMessageBox::warning(this, "Query Problem!", "Not Execute Database For Delete User Following Table. \nPage : setting -> Function : on_btn_deleteaccount_clicked");
        db.close();
        return;
    }

    // Delete user posts table
    {
        tableName = username + "_posts";
        if(!q.exec("DROP TABLE IF EXISTS '"+tableName +"'; ")) {
            QMessageBox::warning(this, "Query Problem!", "Not Execute Database For Delete User Posts Table. \nPage : setting -> Function : on_btn_deleteaccount_clicked");
            db.close();
            return;
        }
    }
    // Delete user messages table
    {
        tableName = username + "_messages";
        if(!q.exec("DROP TABLE IF EXISTS '"+tableName +"'; ")) {
            QMessageBox::warning(this, "Query Problem!", "Not Execute Database For Delete User Message Table. \nPage : setting -> Function : on_btn_deleteaccount_clicked");
            db.close();
            return;
        }
    }
    // Delete user from information table
    {
        if(!q.exec("DELETE FROM information WHERE username = '"+username +"'; ")){
            QMessageBox::warning(this, "Query Problem!", "Not Execute Database For Read Information Table. \nPage : setting -> Function : on_btn_deleteaccount_clicked");
            db.close();
            return;
        }
    }
    // Clearing current user file
    {
        QFile curUser("curUser.txt");
        curUser.open(QFile::WriteOnly | QFile::Text );
        curUser.close();
    }

    MainWindow * mw = new MainWindow();
    mw->show();
    this->close();
}

void setting::on_btn_publicprivate_clicked()
{
    QString username = getUsername();
    db.open();

    QSqlQuery q;
    if(!q.exec("SELECT * FROM information WHERE username = '"+username +"' ;")) {
        QMessageBox::warning(this, "Query Problem!", "Not Execute Database For Account State. \nPage : setting -> Function : on_btn_publicprivate_clicked");
        db.close();
        return;
    }

    bool publicc;
    while(q.next())
        publicc = q.value(3).toBool();

    // to private
    if(publicc) {
        int res = QMessageBox::information(this,"Change To Private", "Are You Sure You Want To Change Your Page To Private", QMessageBox::Yes, QMessageBox::Cancel);
        if(res == QMessageBox::Cancel) {
            db.close();
            return;
        }
        else{
            ui->btn_publicprivate->setText("Private");
            if(!q.exec("UPDATE information SET public = 0 WHERE username = '"+username +"'; ")) {
                QMessageBox::warning(this, "Query Problem!", "Not Execute Database For Update. \nPage : setting -> function : on_btn_publicprivate_clicked");
                db.close();
                return;
            }
            QString tableName = username + "_information";
            if(!q.exec("UPDATE '"+tableName +"' SET public = 0 ;")) {
                QMessageBox::warning(this, "Query Problem!", "Not Execute Database For Update. \nPage : setting -> function : on_btn_publicprivate_clicked");
                db.open();
                return;
            }
        }
    }
    // to public
    else {
        int res = QMessageBox::information(this,"Change To Public", "Are You Sure You Want To Change Your Page To Public", QMessageBox::Yes, QMessageBox::Cancel);
        if(res == QMessageBox::Cancel) {
            db.close();
            return;
        }
        else {
            ui->btn_publicprivate->setText("Public");
            if(!q.exec("UPDATE information SET public = 1 WHERE username = '"+username +"'; ")) {
                QMessageBox::warning(this, "Query Problem!", "Not Execute Database For Update. \nPage : setting -> function : on_btn_publicprivate_clicked");
                db.close();
                return;
            }
            QString tableName = username + "_information";
            if(!q.exec("UPDATE '"+tableName +"' SET public = 1 ;")) {
                QMessageBox::warning(this, "Query Problem!", "Not Execute Database For Update. \nPage : setting -> function : on_btn_publicprivate_clicked");
                db.close();
                return;
            }
        }
    }
    db.close();
    q.clear();
}

void setting::on_btn_changepassword_clicked()
{
    change_password * cp = new change_password();
    cp->show();
    this->close();
}

void setting::on_btn_messages_clicked()
{
	messages * me = new messages();
	me->show();
	this->close();
}
