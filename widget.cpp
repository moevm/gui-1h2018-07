#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle("Книга Рецептов");

    sdb = QSqlDatabase::addDatabase("QSQLITE");
    sdb.setDatabaseName("C:/sqlite/book.db");

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
    ui->listView->close();
    ui->addRecipe->close();
    ui->testWin->close();
}

Widget::~Widget() {
    delete ui;
}

void Widget::on_addButton_clicked()
{
    ui->addRecipe->show();
    ui->mainWid->close();
}

void Widget::on_delButton_clicked() {
    QString delRec = ui->recipeName->text();
    ui->mainWid->close();
    ui->delRecipe->setText(delRec);
    ui->testWin->show();

}

void Widget::searchName(QString text) {
    this->recipes.clear();
    this->searchText = text;
    query = QSqlQuery(sdb);
    query.exec("select * from recipes where name like '%" + this->searchText + "%'");
    while(query.next()) {
        recipes << query.value(1).toString();

    }
    this->recModel.setStringList(recipes);
    if(text != ""){
        ui->listView->setModel(&this->recModel);
        ui->listView->show();
    }
    else ui->listView->close();
}

void Widget::on_listView_clicked(const QModelIndex &index)
{
    ui->listView->close();
    QString showRecipe = this->recipes.value(index.row());
    ui->searchLine->setText(showRecipe);
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

void Widget::on_backButton_clicked()
{
    ui->addRecipe->close();
    ui->mainWid->show();
}

void Widget::on_saveButton_clicked()
{
    this->logRec = ui->logOfNewRecipe->toPlainText();
    this->ingRec = ui->ingOfNewRecipe->toPlainText();
    this->desRec = ui->desOfNewRecipe->toPlainText();
    this->logRec = "<pre>" + logRec + "</pre>";
    this->ingRec = "<pre>" + ingRec + "</pre>";
    this->desRec = "<pre>" + desRec + "</pre>";

    query = QSqlQuery(sdb);
    QString sqlInsert = "insert into recipes (name, description, ingridients, logic, country, type) "
                        "values ('" + nameRec + "', '" + desRec + "', '" + ingRec + "', '" + logRec + "', '" + countryRec + "', '" + typeRec + "');";
    query.exec(sqlInsert);
    ui->addRecipe->close();
    ui->mainWid->show();
    ui->searchLine->setText(nameRec);
    ui->recipeName->setText(nameRec);
    ui->desRecipe->setText(desRec);
    ui->ingRecipe->setText(ingRec);
    ui->logRecipe->setText(logRec);
    ui->countyRecipe->setText(countryRec);
    ui->typeRecipe->setText(typeRec);
    qDebug()<< "inserting row";

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
    ui->testWin->close();
    ui->mainWid->show();
}

void Widget::on_testYes_clicked()
{
    QString delRec = ui->delRecipe->text();
    query = QSqlQuery(sdb);
    query.exec("delete from recipes where name = '" + delRec + "'");
    qDebug() << "delete row";
    ui->testWin->close();
    ui->mainWid->show();
}
