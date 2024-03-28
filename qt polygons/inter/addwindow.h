#ifndef ADDWINDOW_H
#define ADDWINDOW_H

#include <QDialog>

namespace Ui {
class addwindow;
}

class addwindow : public QDialog
{
    Q_OBJECT

public:
    explicit addwindow(QWidget *parent = nullptr);
    ~addwindow();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::addwindow *ui;
};

#endif // ADDWINDOW_H
