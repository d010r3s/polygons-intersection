#ifndef POLYGONSINFO_H
#define POLYGONSINFO_H

#include <QDialog>

namespace Ui {
class PolygonsInfo;
}

class PolygonsInfo : public QDialog
{
    Q_OBJECT

public:
    void setLabel(const QString& x);
    explicit PolygonsInfo(QWidget *parent = nullptr);
    ~PolygonsInfo();


private:
    Ui::PolygonsInfo *ui;
};

#endif // POLYGONSINFO_H
