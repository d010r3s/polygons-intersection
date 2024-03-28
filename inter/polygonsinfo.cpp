#include "polygonsinfo.h"
#include "ui_polygonsinfo.h"


void PolygonsInfo::setLabel(const QString& x)
{
    ui->label->setText(x);
}

PolygonsInfo::PolygonsInfo(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PolygonsInfo)
{
    ui->setupUi(this);
}



PolygonsInfo::~PolygonsInfo()
{
    delete ui;
}
