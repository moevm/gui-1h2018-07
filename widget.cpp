#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

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
    ui->comboBox->setModel(model);
    ui->comboBox->setModelColumn(1);

}

Widget::~Widget() {
    delete ui;
}

//void Widget::on_pushButton_clicked() {
//    qDebug() <<" whyyyyy";
//    query = QSqlQuery(sdb);
//    if(!query.exec("SELECT * FROM recipes")) {
//        qDebug() << query.lastError().databaseText();
//        return;
//    }

//    while(query.next()) {
//        qDebug() << query.record();
//    }
//}

void Widget::on_comboBox_currentTextChanged(const QString &arg1)
{
    QString name, desc, ing, comp;
    query = QSqlQuery(sdb);
    if(!query.exec("SELECT * FROM recipes")) {
        qDebug() << query.lastError().databaseText();
        return;
    }

    while(query.next()) {
        if(query.value(1).toString() == arg1) {
            name = query.value(1).toString();
            desc = query.value(2).toString();
            ing = query.value(3).toString();
            comp = query.value(4).toString();
            ui->name->setText(name);
            ui->desc->setText("<b>Description: </b>" + desc);
            ui->ing->setText("<b>Ingridients: </b>" + ing);
            ui->comp->setText("<b>Complexity: </b>" + comp);
        }
    }


}
