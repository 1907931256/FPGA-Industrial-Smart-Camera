#include "CircleWidget.h"
#include "ui_CircleWidget.h"
#include "FWMatchCircleLabel.h"
#include <QMessageBox>
#include <qdebug.h>


CircleWidget::CircleWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CircleWidget)
{
    ui->setupUi(this);
//    qDebug()<<"CircleWidget begin";
    m_label = new FWMatchCircleLabel(this);
    connect((FWMatchCircleLabel*)m_label,&FWMatchCircleLabel::signalsCenter,this,&CircleWidget::slostCenter);
    m_label->setGeometry(10,10,640,480);
    m_label->setAutoFillBackground(true);
    QPalette p = m_label->palette();
    p.setColor(QPalette::Background,QColor(50,50,50,255));
    m_label->setPalette(p);
    ui->txtRadius->installEventFilter(this);
    TimerUP  = new QTimer;
    LoadKeyBoardLib();
    //LoadLearnLib();
}

CircleWidget::~CircleWidget()
{
    disconnect(this,0,0,0);
    ui->txtRadius->removeEventFilter(this);

    if(TimerUP != NULL)
    {
        delete TimerUP;
        TimerUP = NULL;
    }

    if(m_label !=NULL)
    {
        delete m_label;
        m_label = NULL;
    }
    /*if(m_keyBoard_Lib.isLoaded())
        m_keyBoard_Lib.unload();*/
    delete ui;
}
void CircleWidget::slostCenter()
{
    QPoint point = ((FWMatchCircleLabel*)m_label)->GetCenter();
    int r = ((FWMatchCircleLabel*)m_label)->GetR();
    ui->txtRadius->setText(QString::number(r));
    ui->txtCenterX->setText(QString::number(point.x()));
    ui->txtCenterY->setText(QString::number(point.y()));
}
QPoint CircleWidget::Get_Image_Pos()
{
    QPoint point(10,10);

    if(m_label != NULL)
    {
        point = m_label->pos();
    }

    return point;
}
void CircleWidget::SetDataToLib()
{
//    qDebug()<<"SetDataToLib";
    QPoint point = ((FWMatchCircleLabel*)m_label)->GetCenter();
    int r = ((FWMatchCircleLabel*)m_label)->GetR();
    pInputPara->radius_input = r;
    pInputPara->center_input.x_i = point.x();
    pInputPara->center_input.y_i = point.y();

    if(ui->radioButtonW2B->isChecked())
        pInputPara->color_tran_dir = 0;
    else
        pInputPara->color_tran_dir = 1;

}
int CircleWidget::Set_Parameter_To_Ram()
{
    SetDataToLib();
    return 1;
}

void CircleWidget::Init_Input_Ptr(void *ptr,int i_step_index,int new_flag,void *pen_color)
{
    ((FWMatchCircleLabel*)m_label)->SetPenColor(pen_color);
    pInputPara = (CIRCLE_INPUT_PARAM *)ptr;
    m_step_index = i_step_index;

    if(pInputPara == NULL)
    {
        QMessageBox::about(this,tr(""),tr("初始化数据失败"));
        return;
    }
    if(new_flag)
    {
        Task_Step_Synthetic_Judge_Flag_Set(i_step_index,1);
        ui->txtCenterX->setText(QString::number(320));
        ui->txtCenterY->setText(QString::number(240));
        ui->txtRadius->setText(QString::number(100));
        ui->radioButtonW2B->setChecked(true);
        ui->radioButtonB2W->setChecked(false);
        SetDataToLib();
    }
    InitData();
}
void CircleWidget::InitData()
{
    memset(&m_circle_input,0,sizeof(CIRCLE_INPUT_PARAM));
    memcpy(&m_circle_input,pInputPara,sizeof(CIRCLE_INPUT_PARAM));
    int x = pInputPara->center_input.x_i;
    int y = pInputPara->center_input.y_i;
    unsigned int r = pInputPara->radius_input;
    ((FWMatchCircleLabel*)m_label)->SetR(r);
    ((FWMatchCircleLabel*)m_label)->SetCenter(QPoint(x,y));
    ui->txtRadius->setText(QString::number(r));
    ui->txtCenterX->setText(QString::number(x));
    ui->txtCenterY->setText(QString::number(y));
    unsigned int idir = pInputPara->color_tran_dir;
    if(idir == 0)
    {
        ui->radioButtonW2B->setChecked(true);
        ui->radioButtonB2W->setChecked(false);
    }else
    {
        ui->radioButtonW2B->setChecked(false);
        ui->radioButtonB2W->setChecked(true);
    }
}
void CircleWidget::LoadKeyBoardLib()
{
    m_keyBoard_Lib.setFileName("libFrmInputKeyBoard.so");
    if(m_keyBoard_Lib.isLoaded())
    {
        qDebug()<<"load libFrmInputKeyBoard.so isLoaded";
        return;
    }
    if(m_keyBoard_Lib.load())
    {
        printf("libFrmInputKeyBoard.so OK\n");
    }
    else
    {
        qDebug()<<"load libFrmInputKeyBoard.so fail"<<m_keyBoard_Lib.errorString();
    }
}
void CircleWidget::Get_Result_From_Ram()
{
    void *pPrivate = NULL;
    pPrivate = Task_Step_Private_Para_Get(m_step_index);
    float dr = (float)((CIRCLE_PRIVATE_PARAM *)pPrivate)->model_circle_rst.pixel_radius;
    float  dx = (float)((CIRCLE_PRIVATE_PARAM *)pPrivate)->model_circle_rst.pixel_x;
    float  dy = (float)((CIRCLE_PRIVATE_PARAM *)pPrivate)->model_circle_rst.pixel_y;
//    qDebug()<<dr<<","<<dx<<","<<dy;
    int r;
    int x;
    int y;
    PixelCvt(&dr,&r,CVT_SUB2VGA|CVT_F2I|CVT_DATA);
    PixelCvt(&dx,&x,CVT_SUB2VGA|CVT_F2I|CVT_DATA);
    PixelCvt(&dy,&y,CVT_SUB2VGA|CVT_F2I|CVT_DATA);

//    qDebug()<<"PixelCvt=="<<r<<","<<x<<","<<y;
    ((FWMatchCircleLabel*)m_label)->SetSample(QPoint(x,y),r);
}
void CircleWidget::LoadLearnLib()
{
    m_Learn_Lib.setFileName("libLearningDialog.so");
    if(m_Learn_Lib.isLoaded())
    {
        qDebug()<<"load libLearningDialog.so isLoaded";
        return;
    }
    if(m_Learn_Lib.load())
    {
        printf("libLearningDialog.so OK\n");
    }
    else
    {
        qDebug()<<"load libLearningDialog.so fail"<<m_Learn_Lib.errorString();
    }
}
bool CircleWidget::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->txtRadius)
    {
        if (event->type()==QEvent::MouseButtonPress)
        {
            Fun_GetNumkeyBoardDlg GetNumKeyBoardDlg = (Fun_GetNumkeyBoardDlg)m_keyBoard_Lib.resolve("GetNumKeyBoardDialog");
            if(GetNumKeyBoardDlg)
            {
                QDialog *Numkey = GetNumKeyBoardDlg();
                int ret = Numkey->exec();
                if(ret== QDialog::Accepted)
                {
                    Fun_GetNumString GetNumString = (Fun_GetNumString)m_keyBoard_Lib.resolve("GetNum");
                    if(GetNumString)
                    {
                        QString num = GetNumString();
                        if( num.toInt()>0 && num.toInt()<= 240)
                        {
                            ui->txtRadius->setText(num);
                            ((FWMatchCircleLabel*)m_label)->SetR(num.toInt());
                        }
                        else
                        {
                            QString strTemp;
                            strTemp = "请输入正确的半径值";
                            QMessageBox::information(NULL,tr("提示"),strTemp);
                        }
                    }
                }
            }
        }
    }
    return QWidget::eventFilter(watched, event);
}


void CircleWidget::on_btnAddRadius_pressed()
{
    m_iPresstimes =1;
    connect(TimerUP,SIGNAL(timeout()),this,SLOT(addR()));
    TimerUP->start(100);
}

void CircleWidget::on_btnAddRadius_released()
{
    TimerUP->stop();
    disconnect(TimerUP,SIGNAL(timeout()),this,SLOT(addR()));
    addR();
}

void CircleWidget::on_btnSubRadius_pressed()
{
    m_iPresstimes =1;
    connect(TimerUP,SIGNAL(timeout()),this,SLOT(subR()));
    TimerUP->start(100);
}

void CircleWidget::on_btnSubRadius_released()
{
    TimerUP->stop();
    disconnect(TimerUP,SIGNAL(timeout()),this,SLOT(subR()));
    subR();
}

void CircleWidget::on_btnCircleUP_pressed()
{
    m_iPresstimes =1;
    connect(TimerUP,SIGNAL(timeout()),this,SLOT(upCenter()));
    TimerUP->start(100);
}

void CircleWidget::on_btnCircleUP_released()
{
    TimerUP->stop();
    disconnect(TimerUP,SIGNAL(timeout()),this,SLOT(upCenter()));
    upCenter();
}

void CircleWidget::on_btnCircleDown_pressed()
{
    m_iPresstimes =1;
    connect(TimerUP,SIGNAL(timeout()),this,SLOT(downCenter()));
    TimerUP->start(100);
}

void CircleWidget::on_btnCircleDown_released()
{
    TimerUP->stop();
    disconnect(TimerUP,SIGNAL(timeout()),this,SLOT(downCenter()));
    downCenter();
}

void CircleWidget::on_btnCircleLeft_pressed()
{
    m_iPresstimes =1;
    connect(TimerUP,SIGNAL(timeout()),this,SLOT(leftCenter()));
    TimerUP->start(100);
}

void CircleWidget::on_btnCircleLeft_released()
{
    TimerUP->stop();
    disconnect(TimerUP,SIGNAL(timeout()),this,SLOT(leftCenter()));
    leftCenter();
}

void CircleWidget::on_btnCircleRight_pressed()
{
    m_iPresstimes =1;
    connect(TimerUP,SIGNAL(timeout()),this,SLOT(rightCenter()));
    TimerUP->start(100);
}

void CircleWidget::on_btnCircleRight_released()
{
    TimerUP->stop();
    disconnect(TimerUP,SIGNAL(timeout()),this,SLOT(rightCenter()));
    rightCenter();
}
int CircleWidget::GetStepValue()
{
    if(this->m_iPresstimes>0 && m_iPresstimes< 8)
    {
        m_iPresstimes++;
        return 1;
    }
    int value=1;

    m_iPresstimes++;

    if(m_iPresstimes > 20)
        value = 12;
    else if(m_iPresstimes > 15)
        value = 4;
    else if(m_iPresstimes >10)
        value = 2;
    else
        value =1;
    return value;
}
void CircleWidget::addR()
{

    int step = GetStepValue();
    int r = ui->txtRadius->text().toInt();

    if(r+step <=240)
    {
        ui->txtRadius->setText(QString::number(r+step));
        ((FWMatchCircleLabel*)m_label)->SetR(r+step);
    }


}
void CircleWidget::subR()
{
    int step = GetStepValue();
    int r = ui->txtRadius->text().toInt();

    if(r - step >0)
    {
        ui->txtRadius->setText(QString::number(r - step));
        ((FWMatchCircleLabel*)m_label)->SetR(r - step);
    }
}
void CircleWidget::upCenter()
{
    int step = GetStepValue();
    int y = ui->txtCenterY->text().toInt();
    int x = ui->txtCenterX->text().toInt();

    if(y - step >= 0)
    {
        ((FWMatchCircleLabel*)m_label)->SetCenter(QPoint(x,y-step));
        ui->txtCenterY->setText(QString::number(y-step));
    }
}
void CircleWidget::downCenter()
{
    int step = GetStepValue();
    int y = ui->txtCenterY->text().toInt();
    int x = ui->txtCenterX->text().toInt();
    if(y + step <= 480)
    {
        ((FWMatchCircleLabel*)m_label)->SetCenter(QPoint(x,y+step));
        ui->txtCenterY->setText(QString::number(y+step));
    }
}
void CircleWidget::leftCenter()
{
    int step = GetStepValue();
    int y = ui->txtCenterY->text().toInt();
    int x = ui->txtCenterX->text().toInt();
    if(x - step >= 0)
    {
        ((FWMatchCircleLabel*)m_label)->SetCenter(QPoint(x-step,y));
        ui->txtCenterX->setText(QString::number(x-step));
    }
}
void CircleWidget::rightCenter()
{
    int step = GetStepValue();
    int y = ui->txtCenterY->text().toInt();
    int x = ui->txtCenterX->text().toInt();
    if(x + step <= 640)
    {
        ((FWMatchCircleLabel*)m_label)->SetCenter(QPoint(x+step,y));
        ui->txtCenterX->setText(QString::number(x+step));
    }
}

