#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle("Книга � ецептов");

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
        query.exec("CREATE TABLE recipes (id INTEGER PRIMARY KEY AUTOINCREMENT,name VARCHAR(100),description text,ingridients text,pic BLOB,logic text,country text,type text)");
    }
    query.exec("select * from recipes");
    while(query.next()) {
        recipes << query.value(1).toString() + ", " + query.value(6).toString() + ", " + query.value(7).toString();

    }
    this->recModel.setStringList(recipes);
    ui->listView->setModel(&this->recModel);
}

Widget::~Widget() {
    delete ui;
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
    ui->countryOfNewRecipe->setText("");
    ui->desOfNewRecipe->setText("");
    ui->ingOfNewRecipe->setText("");
    ui->logOfNewRecipe->setText("");
    ui->nameOfNewRecipe->setText("");
    ui->typeOfNewRecipe->setText("");
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
    this->recipes.clear();
    this->searchText = text;
    query = QSqlQuery(sdb);
    query.exec("select * from recipes where name like '%" + this->searchText + "%' and country like '%" + this->searchCountry + "%' and type like '%"  + this->searchType + "%'");
    while(query.next()) {
        recipes << query.value(1).toString() + ", " + query.value(6).toString() + ", " + query.value(7).toString();

    }
    this->recModel.setStringList(recipes);
    ui->listView->setModel(&this->recModel);
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
}

void Widget::on_saveButton_clicked()
{
    if(this->nameRec != "") {
        query = QSqlQuery(sdb);
        query.exec("delete from recipes where name = '" + this->changeRecipe + "'");
        this->changeRecipe = "";
        this->logRec = ui->logOfNewRecipe->toPlainText();
        this->ingRec = ui->ingOfNewRecipe->toPlainText();
        this->desRec = ui->desOfNewRecipe->toPlainText();
        this->logRec = "<pre>" + logRec + "</pre>";
        this->ingRec = "<pre>" + ingRec + "</pre>";
        this->desRec = "<pre>" + desRec + "</pre>";


        QString sqlInsert = "insert into recipes (name, description, ingridients, logic, country, type) "
                            "values ('" + nameRec + "', '" + desRec + "', '" + ingRec + "', '" + logRec + "', '" + countryRec + "', '" + typeRec + "');";
        query.exec(sqlInsert);
        ui->addRecipe->close();
        ui->mainWid->show();
        this->recipes.clear();
        query = QSqlQuery(sdb);
        query.exec("select * from recipes");
        while(query.next()) {
            recipes << query.value(1).toString() + ", " + query.value(6).toString() + ", " + query.value(7).toString();

        }
        this->recModel.setStringList(recipes);
        ui->listView->setModel(&this->recModel);
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
    this->recipes.clear();
    query.exec("select * from recipes");
    while(query.next()) {
        this->recipes << query.value(1).toString() + ", " + query.value(6).toString() + ", " + query.value(7).toString();

    }
    this->recModel.setStringList(recipes);
    ui->listView->setModel(&this->recModel);
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
        ui->logRecipe->setText(query.value(5).toString());
        ui->countyRecipe->setText(query.value(6).toString());
        ui->typeRecipe->setText(query.value(7).toString());
    }
}

void Widget::on_searchCountry_textEdited(const QString &arg1)
{
    this->delRecipe = "";
    this->recipes.clear();
    this->searchCountry = arg1;
    query = QSqlQuery(sdb);
    query.exec("select * from recipes where name like '%" + this->searchText + "%' and country like '%" + this->searchCountry + "%' and type like '%"  + this->searchType + "%'");
    while(query.next()) {
        recipes << query.value(1).toString() + ", " + query.value(6).toString() + ", " + query.value(7).toString();

    }
    this->recModel.setStringList(recipes);
    ui->listView->setModel(&this->recModel);
}

void Widget::on_searchType_textEdited(const QString &arg1)
{
    this->delRecipe = "";
    this->recipes.clear();
    this->searchType = arg1;
    query = QSqlQuery(sdb);
    query.exec("select * from recipes where name like '%" + this->searchText + "%' and country like '%" + this->searchCountry + "%' and type like '%"  + this->searchType + "%'");
    while(query.next()) {
        recipes << query.value(1).toString() + ", " + query.value(6).toString() + ", " + query.value(7).toString();

    }
    this->recModel.setStringList(recipes);
    ui->listView->setModel(&this->recModel);
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
    ui->logOfNewRecipe->setText(ui->logRecipe->toPlainText());
    ui->nameOfNewRecipe->setText(ui->recipeName->text());
    ui->typeOfNewRecipe->setText(ui->typeRecipe->text());
    ui->Recipe->close();
    ui->addRecipe->show();

}
