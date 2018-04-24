#include "widget.h"
#include "ui_widget.h"
#include <ctime>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle("Book of recipes");

    sdb = QSqlDatabase::addDatabase("QSQLITE");
    sdb.setDatabaseName("book.db");

    if(!sdb.open()) {
        qDebug() << sdb.lastError().text();
        return;
    }
    else {
        qDebug() << "Success!";
    }
    model = new QSqlTableModel(this, sdb);
    model->setTable("recipes");
    model->select();
    ui->addRecipe->close();
    ui->testWin->close();
    ui->Recipe->close();
    query = QSqlQuery(sdb);
    if(!query.exec("select * from recipes")) {
        query.exec("CREATE TABLE recipes (id INTEGER PRIMARY KEY AUTOINCREMENT,name VARCHAR(100),description text,ingridients text,pic text,logic text,country text,type text,star text DEFAULT 0)");
    }
    query.exec("select * from recipes");
    while(query.next()) {
        if(query.value(8) == 1)
            this->recipes << query.value(1).toString() + ", " + query.value(6).toString() + ", " + query.value(7).toString() + "  ✰";
        else
            this->recipes << query.value(1).toString() + ", " + query.value(6).toString() + ", " + query.value(7).toString();

    }
    this->recModel.setStringList(recipes);
    ui->listView->setModel(&this->recModel);
}

Widget::~Widget() {
    delete ui;
}

void Widget::reselectForListView(QString inquiry)
{
    this->recipes.clear();
    query = QSqlQuery(sdb);
    query.exec(inquiry);
    while(query.next()) {
        if(query.value(8) == 1)
            this->recipes << query.value(1).toString() + ", " + query.value(6).toString() + ", " + query.value(7).toString() + "  ✰";
        else {
            if(this->favorMode != 1)
                this->recipes << query.value(1).toString() + ", " + query.value(6).toString() + ", " + query.value(7).toString();
        }
    }
    this->recModel.setStringList(recipes);
    ui->listView->setModel(&this->recModel);
}

QString cutString(QString str, QString d) {
    QString  reci= str.toUtf8();
    QChar *chRec = reci.data(), *chD = d.data();
    QString strEnd = "";
    for(int i = 0; i < 50; i++) {
        if(chRec[i] == chD[0])
            break;
        strEnd += chRec[i];
    }
    return strEnd;
}
void Widget::on_addButton_clicked()
{
    this->delRecipe = "";
    this->nameRec = "";
    this->typeRec = "";
    this->countryRec = "";
    this->changeRecipe = "";
    this->picName = "";
    ui->countryOfNewRecipe->setText("");
    ui->desOfNewRecipe->setText("");
    ui->ingOfNewRecipe->setText("");
    ui->logOfNewRecipe->setText("");
    ui->nameOfNewRecipe->setText("");
    ui->typeOfNewRecipe->setText("");
    ui->pic->clear();
    ui->addRecipe->show();
    ui->mainWid->close();
}

void Widget::on_delButton_clicked() {
    QString delRec = this->delRecipe;
    if(delRec != "") {
        ui->mainWid->close();
        ui->delRecipe->setText(delRec);
        ui->testWin->show();
    }
}

void Widget::searchName(QString text) {
    this->delRecipe = "";
    this->searchText = text;
    this->reselectForListView("select * from recipes where name like '%" + this->searchText + "%' and country like '%" + this->searchCountry + "%' and type like '%"  + this->searchType + "%'");
}

void Widget::on_listView_clicked(const QModelIndex &index)
{
    this->delRecipe = cutString(this->recipes.value(index.row()), ",");
}

void Widget::on_backButton_clicked()
{
    ui->addRecipe->close();
    ui->mainWid->show();
}
void Widget::on_backFromRecipe_clicked()
{
    ui->Recipe->close();
    ui->mainWid->show();
    query = QSqlQuery(sdb);
    this->reselectForListView("select * from recipes");
}

void Widget::on_saveButton_clicked()
{
    if(this->nameRec != "") {
        query = QSqlQuery(sdb);
        QString star = "0";
        if(query.exec("select * from recipes where name = '" + this->changeRecipe + "'")) {
            while(query.next()) {
                if(picName == "")
                    picName = query.value(4).toString();
                star = query.value(8).toString();
                query.exec("delete from recipes where name = '" + this->changeRecipe + "'");
            }
        }
        this->changeRecipe = "";
        this->logRec = ui->logOfNewRecipe->toPlainText();
        this->ingRec = ui->ingOfNewRecipe->toPlainText();
        this->desRec = ui->desOfNewRecipe->toPlainText();
        this->logRec = "<pre>" + logRec + "</pre>";
        this->ingRec = "<pre>" + ingRec + "</pre>";
        this->desRec = "<pre>" + desRec + "</pre>";


        QString sqlInsert = "insert into recipes (name, description, ingridients, pic, logic, country, type, star) "
                            "values ('" + nameRec + "', '" + desRec + "', '" + ingRec + "', '"  + picName + "', '" + logRec + "', '" + countryRec + "', '" + typeRec + "', '" + star + "');";
        query.exec(sqlInsert);
        ui->addRecipe->close();
        ui->mainWid->show();
        this->reselectForListView("select * from recipes");
        qDebug()<< "inserting row";
    }
}

void Widget::on_nameOfNewRecipe_textEdited(const QString &arg1)
{
    this->nameRec = arg1;
}

void Widget::on_typeOfNewRecipe_textEdited(const QString &arg1)
{
    this->typeRec = arg1;
}


void Widget::on_countryOfNewRecipe_textEdited(const QString &arg1)
{
    this->countryRec = arg1;
}


void Widget::on_testNo_clicked()
{
    this->delRecipe = "";
    ui->testWin->close();
    ui->mainWid->show();
}

void Widget::on_testYes_clicked()
{
    this->delRecipe = "";
    QString delRec = ui->delRecipe->text();
    query = QSqlQuery(sdb);
    query.exec("delete from recipes where name = '" + delRec + "'");
    qDebug() << "delete row";
    ui->testWin->close();
    ui->mainWid->show();
    ui->searchLine->setText("");
    this->reselectForListView("select * from recipes");
}



void Widget::on_listView_doubleClicked(const QModelIndex &index)
{
    this->delRecipe = "";
    ui->Recipe->show();
    ui->mainWid->close();
    QString showRecipe = cutString(this->recipes.value(index.row()), ",");
    query = QSqlQuery(sdb);
    query.exec("select * from recipes where name = '" + showRecipe + "'");
    while(query.next()) {
        ui->recipeName->setText(query.value(1).toString());
        ui->desRecipe->setText(query.value(2).toString());
        ui->ingRecipe->setText(query.value(3).toString());
        QPixmap pic(QPixmap(query.value(4).toString()).scaled(421, 358, Qt::KeepAspectRatio));//resize картинки);
        ui->picShow->setPixmap(pic);
        ui->logRecipe->setText(query.value(5).toString());
        ui->countyRecipe->setText(query.value(6).toString());
        ui->typeRecipe->setText(query.value(7).toString());
        if(query.value(8).toString() == "1") {
            ui->favor->setText("Убрать из избранного");
        } else {
            ui->favor->setText("Добавить в избранное ✰");
        }
    }
}

void Widget::on_searchCountry_textEdited(const QString &arg1)
{
    this->delRecipe = "";
    this->searchCountry = arg1;
    this->reselectForListView("select * from recipes where name like '%" + this->searchText + "%' and country like '%" + this->searchCountry + "%' and type like '%"  + this->searchType + "%'");
}

void Widget::on_searchType_textEdited(const QString &arg1)
{
    this->delRecipe = "";
    this->searchType = arg1;
    this->reselectForListView("select * from recipes where name like '%" + this->searchText + "%' and country like '%" + this->searchCountry + "%' and type like '%"  + this->searchType + "%'");
}

void Widget::on_changeRecipe_clicked()
{
    this->changeRecipe = ui->recipeName->text();
    this->nameRec = ui->recipeName->text();
    this->typeRec = ui->typeRecipe->text();
    this->countryRec = ui->countyRecipe->text();
    ui->countryOfNewRecipe->setText(ui->countyRecipe->text());
    ui->desOfNewRecipe->setText(ui->desRecipe->toPlainText());
    ui->ingOfNewRecipe->setText(ui->ingRecipe->toPlainText());
    query = QSqlQuery(sdb);
    query.exec("select * from recipes where name = '" + this->nameRec + "'");
    QString pathPic;
    while(query.next()) {
         pathPic = query.value(4).toString();
    }
    QPixmap pic(QPixmap(pathPic).scaled(376, 339, Qt::KeepAspectRatio));//resize картинки);

    ui->pic->setPixmap(pic);
    ui->logOfNewRecipe->setText(ui->logRecipe->toPlainText());
    ui->nameOfNewRecipe->setText(ui->recipeName->text());
    ui->typeOfNewRecipe->setText(ui->typeRecipe->text());
    ui->Recipe->close();
    ui->addRecipe->show();

}

QString  GetRandomStringGetRandom () {
   const QString
   possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
   const int randomStringLength = 6;

   QString randomString;
   for(int i=0; i<randomStringLength; ++i)
   {
       int index = qrand() % possibleCharacters.length();
       QChar nextChar = possibleCharacters.at(index);
       randomString.append(nextChar);
   }
   return randomString;
}

void Widget::on_openButton_clicked()
{
    query = QSqlQuery(sdb);
    QString fileName = QFileDialog::getOpenFileName(this,tr("OpenFile"),"",tr("Pic Files (*.jpeg *.jpg *.png)"));
    if (fileName !=""){
        this->picName = "";
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)){
            QMessageBox::critical(this,tr("Error"),tr("Could not open file"));
            return;
        }
        this->picName = "images/" + GetRandomStringGetRandom() + ".jpg";
        file.copy(fileName, this->picName);
        QPixmap pic(QPixmap(this->picName).scaled(376, 339, Qt::KeepAspectRatio));//resize картинки);
        ui->pic->setPixmap(pic);
        file.close();
    }
}

void Widget::on_favor_clicked()
{
    query = QSqlQuery(sdb);
    qDebug() << ui->recipeName->text();
    query.exec("select * from recipes where name = '" + ui->recipeName->text() + "'");
    while(query.next()) {
        if(query.value(8) == 1) {
            query.exec("update recipes set star = '0' where name = '" + ui->recipeName->text() + "'");
            ui->favor->setText("Добавить в избранное ✰");
        } else {
            query.exec("update recipes set star = '1' where name = '" + ui->recipeName->text() + "'");
            ui->favor->setText("Убрать из избранного");
        }
    }
}


void Widget::on_favorButton_clicked()
{
    if(favorMode == 0) {
        ui->favorButton->setText("Все");
        favorMode = 1;
    } else {
        ui->favorButton->setText("Избранное ✰");
        favorMode = 0;
    }
    this->reselectForListView("select * from recipes");
}
