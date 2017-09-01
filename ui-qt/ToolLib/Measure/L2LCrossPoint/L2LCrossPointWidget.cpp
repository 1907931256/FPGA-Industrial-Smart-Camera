#include <QDebug>
#include "L2LCrossPointWidget.h"
#include "ui_L2LCrossPointWidget.h"
#include "FWL2LCrossPointLabel.h"
#include <QMessageBox>
#include "xmlInitGlobal.h"
#include "LinkDataGlobal.h"

L2LCrossPointWidget::L2LCrossPointWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::L2LCrossPointWidget)
{
    ui->setupUi(this);
    m_label = new FWL2LCrossPointLabel(this);
    m_label->setGeometry(10,10,640,480);
    m_label->setAutoFillBackground(true);
    QPalette p = m_label->palette();
    p.setColor(QPalette::Background,QColor(50,50,50,255));
    m_label->setPalette(p);
    connect(ui->comboBoxLine1, SIGNAL(currentIndexChanged(int)), this, SLOT(Line1TaskChangeSlot(int)));
    connect(ui->comboBoxLine2, SIGNAL(currentIndexChanged(int)), this, SLOT(Line2TaskChangeSlot(int)));
    connect(ui->comboBoxLine1Result, SIGNAL(activated(int)), this, SLOT(Line1ResultChangeSlot(int)));
    connect(ui->comboBoxLine2Result, SIGNAL(activated(int)), this, SLOT(Line2ResultChangeSlot(int)));
}

L2LCrossPointWidget::~L2LCrossPointWidget()
{
    delete ui;
}
/**
 * @brief L2LCrossPointWidget::Get_Image_Pos
 * @return
 * @author dgq
 * @note 获取图像显示区域的位置
 */
QPoint L2LCrossPointWidget::Get_Image_Pos()
{
    QPoint point(10,10);

    if(m_label != NULL)
    {
        point = m_label->pos();
    }

    return point;
}

/**
 * @brief L2LCrossPointWidget::Set_Parameter_To_Ram
 * @author dgq
 * @note 取样
 */
int L2LCrossPointWidget::Set_Parameter_To_Ram()
{
    unsigned int i_task_id;
    TASK_STEP_HEADER *pStephead = Task_Step_Head_Get(m_step_index);
    int i_task1_index = atoi(ui->comboBoxLine1->currentText().toStdString().c_str());;
    Task_Step_Type_ID_Get(i_task1_index,&i_task_id);
    int i_result1_index = Get_Point_Or_Line_Result_Id(i_task_id,ui->comboBoxLine1Result->currentIndex());
    int i_task2_index = atoi(ui->comboBoxLine2->currentText().toStdString().c_str());;
    Task_Step_Type_ID_Get(i_task2_index,&i_task_id);
    int i_result2_index = Get_Point_Or_Line_Result_Id(i_task_id,ui->comboBoxLine2Result->currentIndex());
    if(i_task1_index == i_task2_index && i_result1_index == i_result2_index)
    {
        return 0;
    }
    pStephead->step_link.link_count = 2;
    pStephead->step_link.link_info[0].step_index = i_task1_index;
    pStephead->step_link.link_info[0].result_index = i_result1_index;
    pStephead->step_link.link_info[1].step_index = i_task2_index;
    pStephead->step_link.link_info[1].result_index = i_result2_index;
    return 1;
}

/**
 * @brief L2LCrossPointWidget::Init_Input_Ptr
 * @param ptr
 * @param i_step_index
 * @param new_flag
 * @author dgq
 * @note 初始化设置Input结构体指针
 */
void L2LCrossPointWidget::Init_Input_Ptr(void *ptr,int i_step_index,int new_flag,void *pen_color)
{
    pInputPara = ptr;
    m_step_index = i_step_index;
    if(pInputPara == NULL)
    {
        QMessageBox::about(this,tr(""),tr("初始化数据失败"));
        return;
    }
    m_i_load_first = 0;
    unsigned int i_task_id = 0;
    int i = 0;
    ui->comboBoxLine1->addItem("");
    ui->comboBoxLine2->addItem("");
    for(i = 0; i < i_step_index-1; i++)
    {
        int ret =Task_Step_Type_ID_Get(i+1,&i_task_id);
        if(ret == 0)
        {
            QString name = InitLineCombobox(i+1,i_task_id);
            if(!name.isEmpty())
            {
                ui->comboBoxLine1->addItem(QString::number(i+1)+"."+name);
                ui->comboBoxLine2->addItem(QString::number(i+1)+"."+name);
            }
        }
    }
    if(new_flag)
    {
        TASK_STEP_HEADER *pStephead = Task_Step_Head_Get(i_step_index);
        pStephead->step_link.link_count = 0;
        pStephead->step_link.link_info[0].step_index = 0;
        pStephead->step_link.link_info[0].result_index = 0;
        pStephead->step_link.link_info[1].step_index = 0;
        pStephead->step_link.link_info[1].result_index = 0;
        Task_Step_Synthetic_Judge_Flag_Set(i_step_index,1);
    }
    InitData(i_step_index);
    m_i_load_first = 1;
}

/**
 * @brief L2LCrossPointWidget::InitLineCombobox
 * @param id
 * @return
 * @author dgq
 * @note 根据任务ID获取响应的点任务或线任务名称
 */
QString L2LCrossPointWidget::InitLineCombobox(int TaskIndex, int id)
{
    QString name;
    int ret = Get_Point_Or_Line_Flag(id);
    if(ret == 1)//线任务
    {
        char taskName[40];
        memset(taskName,0,40);
        int ret = Get_Task_Name(TaskIndex, taskName);
        if(ret != -1)
        {
            name = QString::fromUtf8(taskName);
        }
    }
    return name;
}

/**
 * @brief L2LCrossPointWidget::InitData
 * @param i_step_index
 * @author dgq
 * @note 初始化界面显示
 */
void L2LCrossPointWidget::InitData(int i_step_index)
{
    int i = 0;
    TASK_STEP_HEADER *pStephead = Task_Step_Head_Get(i_step_index);
    int i_task1_index = pStephead->step_link.link_info[0].step_index;
    int i_result1_index = pStephead->step_link.link_info[0].result_index;
    int i_task2_index = pStephead->step_link.link_info[1].step_index;
    int i_result2_index = pStephead->step_link.link_info[1].result_index;

    for(i = 0; i < ui->comboBoxLine1->count(); i++)
    {
        if(i_task1_index == atoi(ui->comboBoxLine1->itemText(i).toStdString().c_str()))
            ui->comboBoxLine1->setCurrentIndex(i);
    }
    for(i = 0; i < ui->comboBoxLine2->count(); i++)
    {
        if(i_task2_index == atoi(ui->comboBoxLine2->itemText(i).toStdString().c_str()))
            ui->comboBoxLine2->setCurrentIndex(i);
    }

    ui->comboBoxLine1Result->addItem(" ");
    ui->comboBoxLine2Result->addItem(" ");
    unsigned int i_mul_rst_cnt = 0;
    if(i_task1_index > 0)
    {
        int ret = TaskMultiRstNumGet(i_task1_index,1,&i_mul_rst_cnt);
        if(ret == 0)
        {
            for(unsigned int i = 1; i < i_mul_rst_cnt+1; i++)
            {
                ui->comboBoxLine1Result->addItem(tr("直线")+QString::number(i));
            }
        }
    }
    if(i_task2_index > 0)
    {
        int ret = TaskMultiRstNumGet(i_task2_index,1,&i_mul_rst_cnt);
        if(ret == 0)
        {
            for(unsigned int i = 1; i < i_mul_rst_cnt+1; i++)
            {
                ui->comboBoxLine2Result->addItem(tr("直线")+QString::number(i));
            }
        }
    }
    LineParameter pLinePara;
    unsigned int id;
    Task_Step_Type_ID_Get(i_task1_index,&id);
    int  Mul_Flag = Get_Multi_Result_Task_Flag(id);
    if(Mul_Flag == -1)
    {
        ui->comboBoxLine1Result->setEnabled(false);
    }else
    {
        ui->comboBoxLine1Result->setCurrentIndex(Point_Or_Line_Result_Id_Change_UI_Id(id,i_result1_index));
    }
    measure_ref_line_result_get(i_task1_index,i_result1_index,(void *)&pLinePara,1);
    QLine line;
    Line_Osd_Init(&pLinePara,&line);
    ((FWL2LCrossPointLabel*)m_label)->SetLine1(line);
    Task_Step_Type_ID_Get(i_task2_index,&id);
    Mul_Flag = Get_Multi_Result_Task_Flag(id);
    if(Mul_Flag == -1)
    {
        ui->comboBoxLine2Result->setEnabled(false);
    }else
    {
        ui->comboBoxLine2Result->setCurrentIndex(Point_Or_Line_Result_Id_Change_UI_Id(id,i_result2_index));
    }
    measure_ref_line_result_get(i_task2_index,i_result2_index,(void *)&pLinePara,1);
    Line_Osd_Init(&pLinePara,&line);
    ((FWL2LCrossPointLabel*)m_label)->SetLine2(line);
}

/**
 * @brief L2LCrossPointWidget::Line1TaskChangeSlot
 * @param index
 * @author dgq
 * @note 直线一任务下拉框改变的响应函数
 */
void L2LCrossPointWidget::Line1TaskChangeSlot(int index)
{
    ((FWL2LCrossPointLabel*)m_label)->SetLine1(QLine(0,0,0,0));
    if(m_i_load_first == 1)
    {
        int index = atoi(ui->comboBoxLine1->currentText().toStdString().c_str());
        if(index == 0)
        {
            ui->comboBoxLine1Result->clear();
            return;
        }
        ui->comboBoxLine1Result->setEnabled(true);

        ui->comboBoxLine1Result->clear();
        ui->comboBoxLine1Result->addItem(" ");
        unsigned int id;
        Task_Step_Type_ID_Get(index,&id);
        int  Mul_Flag = Get_Multi_Result_Task_Flag(id);
        if(Mul_Flag ==1)
        {
            unsigned int i_mul_rst_cnt = 0;
            TaskMultiRstNumGet(index,1,&i_mul_rst_cnt);
            for(unsigned int i = 1; i <= i_mul_rst_cnt;i++)
            {
                QString strLineIndex;
                strLineIndex = "直线"+QString::number(i);
                ui->comboBoxLine1Result->addItem(strLineIndex);
            }
        }
        else
        {
            LineParameter pLinePara;
            ui->comboBoxLine1Result->setEnabled(false);
            int ret = measure_ref_line_result_get(index,Point_Or_Line_Result_Id_Change_UI_Id(id,0),(void*)&pLinePara,1);
            if(ret != 0)
            {
                QString info = tr("获取失败!")+tr("错误码:")+QString::number(ret,16);
                QMessageBox::about(this,tr(""),info);
                return;
            }
            QLine line;
            Line_Osd_Init(&pLinePara,&line);
            ((FWL2LCrossPointLabel*)m_label)->SetLine1(line);
        }

    }
}

/**
 * @brief L2LCrossPointWidget::Line2TaskChangeSlot
 * @param index
 * @author dgq
 * @note 直线二任务下拉框改变的响应函数
 */
void L2LCrossPointWidget::Line2TaskChangeSlot(int index)
{
    if(m_i_load_first == 1)
    {
        int index = atoi(ui->comboBoxLine2->currentText().toStdString().c_str());
        if(index == 0)
        {
            ui->comboBoxLine2Result->clear();
            return;
        }
        ui->comboBoxLine2Result->setEnabled(true);

        ui->comboBoxLine2Result->clear();
        ui->comboBoxLine2Result->addItem(" ");
        unsigned int id;
        Task_Step_Type_ID_Get(index,&id);
        int  Mul_Flag = Get_Multi_Result_Task_Flag(id);
        if(Mul_Flag ==1)
        {
            unsigned int i_mul_rst_cnt = 0;
            TaskMultiRstNumGet(index,1,&i_mul_rst_cnt);
            for(unsigned int i = 1; i <= i_mul_rst_cnt;i++)
            {
                QString strLineIndex;
                strLineIndex = "直线"+QString::number(i);
                ui->comboBoxLine2Result->addItem(strLineIndex);
            }
        }else
        {
            LineParameter pLinePara;
            ui->comboBoxLine2Result->setEnabled(false);
            int ret = measure_ref_line_result_get(index,Point_Or_Line_Result_Id_Change_UI_Id(id,0),(void*)&pLinePara,1);
            if(ret != 0)
            {
                QString info = tr("获取失败!")+tr("错误码:")+QString::number(ret,16);
                QMessageBox::about(this,tr(""),info);
                return;
            }
            QLine line;
            Line_Osd_Init(&pLinePara,&line);
            ((FWL2LCrossPointLabel*)m_label)->SetLine2(line);
        }

    }
}

/**
 * @brief L2LCrossPointWidget::Line1ResultChangeSlot
 * @param index
 * @author dgq
 * @note 直线一结果下拉框改变的响应函数
 */
void L2LCrossPointWidget::Line1ResultChangeSlot(int index)
{
    if(m_i_load_first ==1)
    {
        LineParameter pLinePara;
        int Task_index = atoi(ui->comboBoxLine1->currentText().toStdString().c_str());
        int Result_index = ui->comboBoxLine1Result->currentIndex();
        if(Result_index == 0)
        {
            ((FWL2LCrossPointLabel*)m_label)->SetLine1(QLine(0,0,0,0));
            return;
        }
        unsigned int id;
        Task_Step_Type_ID_Get(Task_index,&id);
        int ret = measure_ref_line_result_get(Task_index,Get_Point_Or_Line_Result_Id(id,Result_index),(void*)&pLinePara,1);
        if(ret != 0)
        {
            QString info = tr("获取失败!")+tr("错误码:")+QString::number(ret,16);
            QMessageBox::about(this,tr(""),info);
            return;
        }
        QLine line;
        Line_Osd_Init(&pLinePara,&line);
        ((FWL2LCrossPointLabel*)m_label)->SetLine1(line);
    }
}

/**
 * @brief L2LCrossPointWidget::Line2ResultChangeSlot
 * @param index
 * @author dgq
 * @note 直线二结果下拉框改变的响应函数
 */
void L2LCrossPointWidget::Line2ResultChangeSlot(int index)
{
    if(m_i_load_first ==1)
    {
        LineParameter pLinePara;
        int Task_index = atoi(ui->comboBoxLine2->currentText().toStdString().c_str());
        int Result_index = ui->comboBoxLine2Result->currentIndex();
        if(Result_index == 0)
        {
            ((FWL2LCrossPointLabel*)m_label)->SetLine2(QLine(0,0,0,0));
            return;
        }
        unsigned int id;
        Task_Step_Type_ID_Get(Task_index,&id);
        int ret = measure_ref_line_result_get(Task_index,Get_Point_Or_Line_Result_Id(id,Result_index),(void*)&pLinePara,1);
        if(ret != 0)
        {
            QString info = tr("获取失败!")+tr("错误码:")+QString::number(ret,16);
            QMessageBox::about(this,tr(""),info);
            return;
        }
        QLine line;
        Line_Osd_Init(&pLinePara,&line);
        ((FWL2LCrossPointLabel*)m_label)->SetLine2(line);
    }
}

/**
 * @brief L2LAngleWidget::Line_Paint_Base_On_KB_SubPixel
 * @param pLinePara
 * @param pOsdLine
 * @return
 * @author dgq
 * @note 根据点斜式换算出直线跟640*480显示区域边界的两个交点的子函数
 */
int L2LCrossPointWidget::Line_Paint_Base_On_KB_SubPixel(float k_f, float b_f, QLine *pOsd_line)
{
    int i = 0;
    int count = 0;
    QPointF calc_point[4];

    calc_point[0].setX(0);
    calc_point[0].setY(b_f);
    /* Point1 x=640, calculate y  */
    calc_point[1].setX(VGA_WIDTH);
    calc_point[1].setY(k_f*VGA_WIDTH+b_f);
    /* Point1 y=0, calculate x  */
    calc_point[2].setX((0-b_f)/k_f);
    calc_point[2].setY(0);
    /* Point1 y=480, calculate x  */
    calc_point[3].setX((VGA_HEIGHT-b_f)/k_f);
    calc_point[3].setY(VGA_HEIGHT);

    for(i = 0; i < 4; i++)
    {
        //DRV_PRINT(LOCATION_DEBUG, "point%d: x=%d, y=%d\n", i, (int)calc_point[i].x_f, (int)calc_point[i].y_f);
        if(calc_point[i].x()>=0 && calc_point[i].x()<=VGA_WIDTH && calc_point[i].y()>=0 && calc_point[i].y()<=VGA_HEIGHT)
        {
            if(count == 0)
            {
                count++;
                pOsd_line->setP1(QPoint(calc_point[i].x(),calc_point[i].y()));
            }
            else
            {
                pOsd_line->setP2(QPoint(calc_point[i].x(),calc_point[i].y()));
            }
        }
    }

    return 0;
}

/**
 * @brief L2LAngleWidget::Line_Osd_Init
 * @param pLinePara
 * @param pOsdLine
 * @return
 * @author dgq
 * @note 根据点斜式换算出直线跟640*480显示区域边界的两个交点
 */
int L2LCrossPointWidget::Line_Osd_Init(LineParameter *pLinePara, QLine *pOsdLine)
{
    float line_kf = 0;
    float line_bf = 0;
    float line_k, line_b;

    float pix_vga_y = 0.0;
    float pix_vga_x = 0.0;
    float pix_sub_y = (float)pLinePara->y;
    float pix_sub_x = (float)pLinePara->x;
    PixelCvt((void *)(&pix_sub_y),(void *)(&pix_vga_y),CVT_SUB2VGA|CVT_F2F|CVT_DATA);
    PixelCvt((void *)(&pix_sub_x),(void *)(&pix_vga_x),CVT_SUB2VGA|CVT_F2F|CVT_DATA);
    line_kf = tan(pLinePara->theta);
//    line_bf = (X_Y_CONVERT_SUB2UI((float)pLinePara->y) - line_kf * X_Y_CONVERT_SUB2UI((float)pLinePara->x));
    line_bf = (pix_vga_y - line_kf * pix_vga_x);
    line_k = (int)(line_kf*10000);
    line_b = (int)(line_bf*10000);

    if(line_k == 0x7fffffff || line_k == 0x80000000 || line_b == 0x7fffffff || line_b == 0x80000000)
    {
//        Vertical_Line_Paint((unsigned int)pLinePara->x, pOsdLine);

        pOsdLine->setP1(QPoint(pix_vga_x,0));
        pOsdLine->setP2(QPoint(pix_vga_x,VGA_HEIGHT));
    }else
    {
        Line_Paint_Base_On_KB_SubPixel(line_kf, line_bf, pOsdLine);
    }

    return 0;
}

/**
 * @brief L2LCrossPointWidget::Get_Result_From_Ram
 * @author dgq
 * @note 取样成功后获取内存中的取样结果更新OSD显示
 */
void L2LCrossPointWidget::Get_Result_From_Ram()
{
    L2L_CROSS_POINT_PRIVATE_PARAM *pPrivate = (L2L_CROSS_POINT_PRIVATE_PARAM *)Task_Step_Private_Para_Get(m_step_index);

    int pix_vga_x = 0;
    int pix_vga_y = 0;
    float pix_sub_x = pPrivate->model_l2l_corss_point_result.point_pixel.x_f;
    float pix_sub_y = pPrivate->model_l2l_corss_point_result.point_pixel.y_f;
    PixelCvt(&pix_sub_x,&pix_vga_x,CVT_SUB2VGA|CVT_F2I|CVT_DATA);
    PixelCvt(&pix_sub_y,&pix_vga_y,CVT_SUB2VGA|CVT_F2I|CVT_DATA);
    ((FWL2LCrossPointLabel*)m_label)->SetSet_Parameter_To_RamPoint(QPoint(pix_vga_x,pix_vga_y));
}
