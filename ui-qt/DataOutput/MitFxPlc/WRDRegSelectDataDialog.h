#ifndef WRDREGSELECTDATADIALOG_H
#define WRDREGSELECTDATADIALOG_H

#include <QDialog>
#include <QStringList>
namespace Ui {
class WRDRegSelectDataDialog;
}

class WRDRegSelectDataDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WRDRegSelectDataDialog(QWidget *parent = 0);
    ~WRDRegSelectDataDialog();
    QStringList GetSelectDataList(){return Select_Data_List;}

private slots:
    void on_tableWidget_task_cellClicked(int row, int column);

    void on_tableWidget_detail_cellClicked(int row, int column);
    void on_tableWidget_data_cellClicked(int row, int column);
    void on_pushButton_OK_clicked();

    void on_pushButton_Cancel_clicked();

private:
    Ui::WRDRegSelectDataDialog *ui;
    QStringList Select_Data_List;
    unsigned int task_type;
    unsigned int multi_flag;
    int task_pre_index;
    int detail_pre_index;
    void IniData();
    void InitTaskListTableWidget(int index, QString taskName);
    void InitTaskResultListTableWidget(int index, QString taskResult);
    void InitTaskDataListTableWidget(int index, QString taskResult);
};

#endif // WRDREGSELECTDATADIALOG_H
