#include <QDebug>
#include "NumKeyBoard.h"
#include "ui_NumKeyBoard.h"
#include "stdio.h"
#include <QMessageBox>

NumKeyBoard::NumKeyBoard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NumKeyBoard)
{
    ui->setupUi(this);
    b_IP = false;
    this->strNum = "";
    ui->txtNum->setText("");
    ui->btnRangeEn->setVisible(false);
//    this->setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);

}


NumKeyBoard::~NumKeyBoard()
{
    delete ui;
}

void NumKeyBoard::ShowbtnRangeEn()
{
    ui->btnRangeEn->setVisible(true);
}

void NumKeyBoard::SetNum(QString str_Num)
{
   ui->txtNum->setText(str_Num);
   this->strNum = str_Num;
}

void NumKeyBoard::SetRangeNum(QString str_Num,int Max_flag)
{
    if(str_Num.contains("."))
    {
        float num = str_Num.toFloat();
        if(Max_flag == 1)
        {
            this->strNum = QString::number(qRound(num+0.5),'f',3);
        }
        else
        {
            this->strNum = QString::number(qRound(num-0.5),'f',3);
        }
    }
    else
    {
        this->strNum = str_Num;
    }
    ui->txtNum->setText(strNum);
}

void NumKeyBoard::RangeInput(QString max,QString min,int flag)
{
    m_flag = flag;
    if(flag == 1)
    {
        strNum = max;
        strCmp = min;
    }else
    {
        strNum = min;
        strCmp = max;
    }
    if(strNum.contains("."))
    {
        float num = strNum.toFloat();
        if(flag == 1)
        {
            this->strNum = QString::number(qRound(num+0.5),'f',3);
        }
        else
        {
            this->strNum = QString::number(qRound(num-0.5),'f',3);
        }
    }
    else
    {
        if(strNum == "---")
        {
            strNum = "";
        }
    }
    ui->txtNum->setText(strNum);
    ui->txtNum->selectAll();
}


void NumKeyBoard::SetOldNum(QString str_Num)
{
    ui->txtNum->setText(str_Num);
    this->strNum = str_Num;
}

void NumKeyBoard::InputIp()
{
    b_IP = true;
}

QString NumKeyBoard::GetNum()
{
//    return ui->txtNum->text();
    return strNum;
}


QString NumKeyBoard::GetRangeNum()
{
    if(m_flag == 0)
    {
        if(strNum == "---"){
            return strNum;
        }
        else if(strCmp != "---")
        {
            if(strNum.contains("."))
            {
                QString num = strNum.split(".")[0];
                if(num.length() > 8)
                {
                    strNum = strCmp;
                }
                else if(strNum.toFloat() > strCmp.toFloat())
                {
                    strNum = strCmp;
                }
                return strNum;
            }else
            {
                if(strNum.length() > 8)
                {
                    strNum = strCmp;
                }else if(strNum.toInt() > strCmp.toInt())
                {
                    strNum = strCmp;
                }
                return strNum;
            }
        }
        else if(strCmp == "---"){
            if(strNum.contains("."))
            {
                QString num = strNum.split(".")[0];
                if(num.length() > 8)
                {
                    strNum = "9999999.999";
                    return strNum;
                }else
                {
                    return strNum;
                }
            }else if(strNum.length() > 8)
            {
                strNum = "99999999";
                return strNum;
            }else
            {
                return strNum;
            }
        }
    }
    if(m_flag == 1)
    {
        if(strNum == "---"){
            return strNum;
        }
        else if(strCmp != "---")
        {
            if(strNum.contains("."))
            {
                QString num = strNum.split(".")[0];
                if(num.length() > 8)
                {
                    strNum = strCmp;
                }
                else if(strNum.toFloat() < strCmp.toFloat())
                {
                    strNum = strCmp;
                }
                return strNum;
            }else
            {
                if(strNum.length() > 8)
                {
                    strNum = strCmp;
                }else if(strNum.toInt() < strCmp.toInt())
                {
                    strNum = strCmp;
                }
                return strNum;
            }
        }
        else if(strCmp == "---")
        {
            if(strNum.contains("."))
            {
                QString num = strNum.split(".")[0];
                printf("num = %s\r\n",num.toLatin1().data());
                if(num.length() > 8)
                {
                    strNum = "9999999.999";
                    return strNum;
                }else
                {
                    return strNum;
                }
            }else if(strNum.length() > 8)
            {
                strNum = "99999999";
                return strNum;
            }else
            {
                return strNum;
            }
        }
    }
    return strNum;
}

void NumKeyBoard::SetNaturalNumberInputMode(bool b_value)
{
    ui->btnNegative->setEnabled(!b_value);
    ui->btnSpot->setEnabled(!b_value);
}

void NumKeyBoard::SelectTextReplace(QPushButton *object)
{
    int i_len = ui->txtNum->selectedText().length();
    int i_cus_pos = 0;
    if(i_len != 0)/*���ѡ���ַ��ĳ��Ȳ�Ϊ��*/
    {
        i_cus_pos = ui->txtNum->selectionStart();
        if(object == ui->btnBack)
        {
            /*��ѡ�е��ַ�ɾ�������λ�ò���*/
            strNum.replace(i_cus_pos,i_len,"");
        }
        else
        {
            /*��ѡ�е��ַ��滻Ϊ�������ݣ����λ����ǰ�ƶ�һλ*/
            strNum.replace(i_cus_pos,i_len,object->text());
            i_cus_pos++;
        }
    }else /*���ѡ���ַ�����Ϊ�㣬��û��ѡ���κ��ַ�*/
    {
        i_cus_pos = ui->txtNum->cursorPosition();
        if(object == ui->btnBack)
        {
            /*ɾ�����ǰ��һ���ַ������λ�ú���һλ*/
            strNum.replace(i_cus_pos-1,1,"");
            i_cus_pos--;
        }
        else
        {
            /*�ڹ��λ�ò��밴�����ݣ����λ����ǰ��һλ*/
            strNum.insert(i_cus_pos,object->text());
            i_cus_pos++;
        }
    }
    ui->txtNum->setText(strNum);
    ui->txtNum->setCursorPosition(i_cus_pos);
}

void NumKeyBoard::on_btn1_clicked()
{
    SelectTextReplace(ui->btn1);
}

void NumKeyBoard::on_btn2_clicked()
{
    SelectTextReplace(ui->btn2);
}

void NumKeyBoard::on_btn3_clicked()
{
    SelectTextReplace(ui->btn3);
}

void NumKeyBoard::on_btn4_clicked()
{
    SelectTextReplace(ui->btn4);
}

void NumKeyBoard::on_btn5_clicked()
{
    SelectTextReplace(ui->btn5);
}

void NumKeyBoard::on_btn6_clicked()
{
    SelectTextReplace(ui->btn6);
}

void NumKeyBoard::on_btn7_clicked()
{
    SelectTextReplace(ui->btn7);
}

void NumKeyBoard::on_btn8_clicked()
{
    SelectTextReplace(ui->btn8);
}

void NumKeyBoard::on_btn9_clicked()
{
    SelectTextReplace(ui->btn9);
}

void NumKeyBoard::on_btnSpot_clicked()
{
    if(ui->txtNum->text().length() > 0)
    {
        if(b_IP)
        {
            SelectTextReplace(ui->btnSpot);
        }
        else if(!strNum.contains("."))
        {
            SelectTextReplace(ui->btnSpot);
        }
    }
}

void NumKeyBoard::on_btn0_clicked()
{
    SelectTextReplace(ui->btn0);
}

void NumKeyBoard::on_btnNegative_clicked()
{
    if(ui->txtNum->text() == "")
    {
        strNum += "-";
        ui->txtNum->setText(strNum);
    }
}

void NumKeyBoard::on_btnClear_clicked()
{
    strNum = "";
    ui->txtNum->setText(strNum);
}

void NumKeyBoard::on_btnBack_clicked()
{
//    if(strNum.length()> 0)
//    {
//        strNum = strNum.left(strNum.length() -1);
//        ui->txtNum->setText(strNum);
//    }

    SelectTextReplace(ui->btnBack);
}

void NumKeyBoard::on_btnOk_clicked()
{
    if(ui->txtNum->text()=="")
    {
        QMessageBox::about(NULL,tr("Alarm"),tr("Please enter the correct number"));
        return;
    }
    else if(ui->txtNum->text() == "-")
    {
        QMessageBox::about(NULL,tr("Alarm"),tr("Please enter the correct number"));
        return ;
    }else if(ui->txtNum->text().left(1) == ".")
    {
        QMessageBox::about(NULL,tr("Alarm"),tr("Please enter the correct number"));
        return;
    }else if(ui->txtNum->text().right(1)==".")
    {
        QMessageBox::about(NULL,tr("Alarm"),tr("Please enter the correct number"));
        return;
    }else
    {
        if(ui->txtNum->text().length() >= 2)
        {
          if(ui->txtNum->text().left(2) == "-.")
          {
              QMessageBox::about(NULL,tr("Alarm"),tr("Please enter the correct number"));
              return;
          }
        }
    }
    QDialog::accept();
}

void NumKeyBoard::on_btnQuit_clicked()
{
    QDialog::reject();
}


void NumKeyBoard::on_btnRangeEn_clicked()
{
    strNum.clear();
    strNum = "---";
    QDialog::accept();
}

void NumKeyBoard::on_txtNum_selectionChanged()
{
    m_StrSelect = ui->txtNum->selectedText();
}

