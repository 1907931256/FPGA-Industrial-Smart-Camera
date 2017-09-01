#include "WRDRegItem.h"
#include "ui_WRDRegItem.h"
#include "TaskStepGlobal.h"
#include "TaskHeaderGlobal.h"
#include "LinkDataGlobal.h"
#include <QDebug>
#include <QMessageBox>
WRDRegItem::WRDRegItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WRDRegItem)
{
    ui->setupUi(this);
    LoadKeyBoardLib();
}

WRDRegItem::~WRDRegItem()
{
    if(m_keyBoard_Lib.isLoaded())
    {
        m_keyBoard_Lib.unload();
    }
    ui->lineEdit_Input->removeEventFilter(this);
    delete ui;
}

void WRDRegItem::SetNumber(int index)
{
    ui->txt_SerialNum->setText(QString::number(index));
}

void WRDRegItem::SetRegAddr(int addr)
{
    ui->txt_Addr->setText(QString::number(addr));
}

void WRDRegItem::SetDataType(int Data_type, int Data_Length)
{
    Data_Type = Data_type;
    if(Data_Type == 8 || Data_Type == 9)
    {
        if((((ui->txt_SerialNum->text().toInt() - 1) % 4) != 0) || (Data_Length < ui->txt_SerialNum->text().toInt() + 3))
        {
            ui->radioButton_Input->hide();
            ui->radioButton_Quote->hide();
            ui->lineEdit_Input->hide();
            ui->pushButton_DataSelect->hide();
        }
    }
    else
    {
        if(((ui->txt_SerialNum->text().toInt() - 1) % 2) != 0 || (Data_Length < ui->txt_SerialNum->text().toInt() + 1))
        {
            ui->radioButton_Input->hide();
            ui->radioButton_Quote->hide();
            ui->lineEdit_Input->hide();
            ui->pushButton_DataSelect->hide();
        }
    }
}

void WRDRegItem::SetCurrentIndex(int index)
{
    Current_index = index;
}

void WRDRegItem::SetModbusMode(int mode)
{
    slave_or_host = mode;
}

void WRDRegItem::SetToolType(int tool)
{
    tool_type = tool;
}

void WRDRegItem::SetQuoteString(unsigned short task_index, unsigned short result_index )
{
    QString TaskNameString = QString::null;
    QString TaskResultString = QString::null;
    QString TaskDataString = "";
    unsigned int task_type = 0;
    int Task_Result_Count = 0;
    char taskName[40];
    if(Task_Step_Type_ID_Get(task_index, &task_type) == 0)
    {
        memset(taskName, 0, 40);
        int ret = Get_Task_Name(task_index, taskName);
        if(ret != -1)
        {
            TaskNameString = QString::number(task_index) +"."+ QString::fromUtf8(taskName) + ";";
        }
    }
    else
    {
        return;
    }

    int task_Result_index = ResultId_Change_UIID(&Task_Result_Count, Modbus_index, task_type, result_index);

    if(Task_Result_Count != -1)
    {
        RESULT_NAME Result_String[250];
        memset(Result_String, 0, sizeof(Result_String));
        Get_Multi_Result_Task_Result_String(Modbus_index, task_type, Task_Result_Count, Result_String);
        TaskResultString = QString::number(Task_Result_Count) + tr(".") + QString::fromUtf8(Result_String[Task_Result_Count -1].Result_Name) + ";";

        char TaskResult[100];
        memset(TaskResult, 0, sizeof(TaskResult));
        int rst_index = UI_ID_Change_Array_Index(Modbus_index, task_type, Task_Result_Count, task_Result_index);
        if(rst_index == -1){
            return;
        }
        int result = Get_Task_Result_String(Modbus_index, task_type, Task_Result_Count, rst_index, TaskResult);
        if(result == 1)
        {
            TaskDataString = QString::number(task_Result_index) + tr(".") + QString::fromUtf8(TaskResult) + ";";
        }
        else
        {
            printf("line = %d Get_Task_Result_String result = %d\n",__LINE__, result);
        }
    }
    else
    {
        char TaskResult[100];
        memset(TaskResult, 0, sizeof(TaskResult));
        int rst_index = UI_ID_Change_Array_Index(Modbus_index, task_type, Task_Result_Count, task_Result_index);
        if(rst_index == -1){
            return;
        }
        int result = Get_Task_Result_String(Modbus_index, task_type, Task_Result_Count, rst_index, TaskResult);
        if(result == 1)
        {
            TaskResultString = QString::number(task_Result_index) + tr(".") + QString::fromUtf8(TaskResult) + ";";
        }
        else
        {
            printf("line = %d Get_Task_Result_String result = %d\n",__LINE__, result);
        }
    }
    ui->lineEdit_Input->setText(TaskNameString + TaskResultString + TaskDataString);
}

void WRDRegItem::GetQuoteData()
{
    int Index = 0;
    for(int i = 0; i < ui->txt_SerialNum->text().toUInt(); i++)
    {
        if(WR_D_Reg->data_source[i] == 1)
        {
            Index++;
        }
    }
    SetQuoteString(WR_D_Reg->link_info[Index - 1].step_index, WR_D_Reg->link_info[Index - 1].result_index);
}

void WRDRegItem::GetInputData()
{
    if(Data_Type == 0 || Data_Type == 1)
    {
        ui->lineEdit_Input->setText(QString::number(input_data.bit_i32[0]));
    }
    else if(Data_Type == 2 || Data_Type == 3)
    {
        ui->lineEdit_Input->setText(QString::number(input_data.bit_u32[0]));
    }
    else if(Data_Type == 4 || Data_Type == 5)
    {
        ui->lineEdit_Input->setText(QString::number(input_data.bit_f32[0]));
    }
    else if(Data_Type == 6 || Data_Type == 7)
    {
        ui->lineEdit_Input->setText(QString::number(input_data.bit_l32[0]));
    }
    else if(Data_Type == 8 || Data_Type == 9)
    {
        ui->lineEdit_Input->setText(QString::number(input_data.bit_d64));
    }
}

void WRDRegItem::SetDataSource(PLC_WRITE_D_REG* Data, int flag)
{
    WR_D_Reg = Data;
    if(Data_Type == 8 || Data_Type == 9)
    {
        if(((ui->txt_SerialNum->text().toInt() - 1) % 4) == 0)
        {
            if(WR_D_Reg->data_source[ui->txt_SerialNum->text().toUInt() - 1] == 0)
            {
                ui->radioButton_Input->setChecked(true);
                ui->lineEdit_Input->installEventFilter(this);
                ui->pushButton_DataSelect->setEnabled(false);
                memcpy(input_data.bit_u16, &WR_D_Reg->reg_value[ui->txt_SerialNum->text().toUInt() - 1], 8);
                if(flag == 1)
                    GetInputData();
            }
            else
            {
                ui->radioButton_Quote->setChecked(true);
                ui->pushButton_DataSelect->setEnabled(true);
                if(flag == 1)
                    GetQuoteData();
            }
        }
    }
    else
    {
        if(((ui->txt_SerialNum->text().toInt() - 1) % 2) == 0)
        {
            if(WR_D_Reg->data_source[ui->txt_SerialNum->text().toUInt() - 1] == 0)
            {
                ui->radioButton_Input->setChecked(true);
                ui->lineEdit_Input->installEventFilter(this);
                ui->pushButton_DataSelect->setEnabled(false);
                memcpy(input_data.bit_u16, &WR_D_Reg->reg_value[ui->txt_SerialNum->text().toUInt() - 1], 4);
                if(flag == 1)
                    GetInputData();
            }
            else
            {
                ui->radioButton_Quote->setChecked(true);
                ui->pushButton_DataSelect->setEnabled(true);
                if(flag == 1)
                    GetQuoteData();
            }
        }
    }
}

void WRDRegItem::LoadKeyBoardLib()
{
    m_keyBoard_Lib.setFileName("libFrmInputKeyBoard.so");
    if(m_keyBoard_Lib.isLoaded())
    {
        return;
    }
    if(m_keyBoard_Lib.load())
    {
    }
    else
    {
        qDebug()<<"load libFrmInputKeyBoard.so fail"<<m_keyBoard_Lib.errorString();
    }
}

void WRDRegItem::NumKeyBoardDlg(QObject *watched, QEvent *event, int Min_value, int Max_value)
{
    Fun_GetNumkeyBoardDlg GetNumKeyBoardDlg = (Fun_GetNumkeyBoardDlg)m_keyBoard_Lib.resolve("GetNumKeyBoardDialog");
    if(GetNumKeyBoardDlg)
    {
        Numkey = GetNumKeyBoardDlg();
        int ret = Numkey->exec();
        if(ret == QDialog::Accepted)
        {
            Fun_GetNumString GetNumString = (Fun_GetNumString)m_keyBoard_Lib.resolve("GetNum");
            if(GetNumString)
            {
                QString num = GetNumString();
                ((QLineEdit *)watched)->setText(num);
            }
        }
        delete Numkey;
    }
    else
    {
        qDebug()<<"get GetDialog func fail:"<<m_keyBoard_Lib.errorString();
    }
}

bool WRDRegItem::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->lineEdit_Input)
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            NumKeyBoardDlg(watched, event, 0, 0);
        }
    }
    return QWidget::eventFilter(watched, event);
}



void WRDRegItem::on_pushButton_DataSelect_clicked()
{
    unsigned int task_type = 0;
    int result_id = 0;
    WRDRegSelectDataDialog *dlg = new WRDRegSelectDataDialog;
    int ret = dlg->exec();
    if(ret == QDialog::Accepted)
    {
        Select_Data_List = dlg->GetSelectDataList();
        QString strTask = Select_Data_List.at(0);
        QString strResult = Select_Data_List.at(1);
        QString strData = Select_Data_List.at(2);
        QString Task_Index = strTask.split(".").at(0);
        QString Result_Index = strResult.split(".").at(0);
        QString Data_Index = strData.split(".").at(0);
        ui->lineEdit_Input->setText(strTask + strResult + strData);
        if(Task_Step_Type_ID_Get(Task_Index.toInt(), &task_type) == 0)
        {
            if(Get_Multi_Result_Task_Flag(task_type) == 1)
            {
                result_id = UIId_Change_ResultID(Modbus_index, task_type, Result_Index.toInt(), Data_Index.toInt());
            }
            else
            {
                result_id = UIId_Change_ResultID(Modbus_index, task_type, -1, Result_Index.toInt());
            }

            Data_cnt = 0;
            for(int i = 0; i < ui->txt_SerialNum->text().toUInt(); i++)
            {
                if(WR_D_Reg->data_source[i] == 1)
                {
                    Data_cnt++;
                }
            }

            WR_D_Reg->link_info[Data_cnt - 1].reg_index = ui->txt_Addr->text().toUShort();
            WR_D_Reg->link_info[Data_cnt - 1].step_index = Task_Index.toUShort();
            WR_D_Reg->link_info[Data_cnt - 1].result_index = (unsigned short)result_id;
        }
    }
}

void WRDRegItem::on_radioButton_Input_clicked()
{
    ui->lineEdit_Input->installEventFilter(this);
    WR_D_Reg->data_source[ui->txt_SerialNum->text().toUInt() - 1] = 0;

    ui->pushButton_DataSelect->setEnabled(false);
    ui->lineEdit_Input->setText("");
}

void WRDRegItem::on_radioButton_Quote_clicked()
{
    ui->lineEdit_Input->removeEventFilter(this);
    WR_D_Reg->data_source[ui->txt_SerialNum->text().toUInt() - 1] = 1;

    ui->pushButton_DataSelect->setEnabled(true);
    ui->lineEdit_Input->setText("");
}

void WRDRegItem::SetInputValue(unsigned short* value)
{
    if(Data_Type == 0 || Data_Type == 1)
    {
        input_data.bit_i32[0] = ui->lineEdit_Input->text().toInt();
        memcpy(value, input_data.bit_u16, sizeof(int));
    }
    else if(Data_Type == 2 || Data_Type == 3)
    {
        input_data.bit_u32[0] = ui->lineEdit_Input->text().toUInt();
        memcpy(value, input_data.bit_u16, sizeof(unsigned int));
    }
    else if(Data_Type == 4 || Data_Type == 5)
    {
        input_data.bit_f32[0] = ui->lineEdit_Input->text().toFloat();
        memcpy(value, input_data.bit_u16, sizeof(float));
    }
    else if(Data_Type == 6 || Data_Type == 7)
    {
        input_data.bit_l32[0] = ui->lineEdit_Input->text().toLong();
        memcpy(value, input_data.bit_u16, sizeof(long));
    }
    else if(Data_Type == 8 || Data_Type == 9)
    {
        input_data.bit_d64 = ui->lineEdit_Input->text().toDouble();
        memcpy(value, input_data.bit_u16, sizeof(double));
    }
}

void WRDRegItem::on_lineEdit_Input_textChanged(const QString &arg1)
{
    if(ui->radioButton_Input->isChecked())
    {
        SetInputValue(&WR_D_Reg->reg_value[ui->txt_SerialNum->text().toUInt() - 1]);
    }
}
