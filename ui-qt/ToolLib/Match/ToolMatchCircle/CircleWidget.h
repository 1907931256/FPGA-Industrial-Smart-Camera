/**
  * @file  [CircleWidget.h]
  * @brief  CircleWidget
  * @author  <szj>
  * @date  <2017/05/08>
  * @version  <1.0>
  * @note
  * detailed  description
  */
#ifndef CIRCLEWIDGET_H
#define CIRCLEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLibrary>
#include <QTimer>
#include "TaskStepGlobal.h"
#include "TaskRstGlobal.h"
#include "PixelConvert.h"
#include "CircleGlobal.h"


typedef QDialog* (*Fun_GetNumkeyBoardDlg)();
typedef void (*Fun_SetRangeInput)(QString max, QString min, int flag);
typedef QString (*Fun_GetRangeNum)();
typedef QString (*Fun_GetNumString)();

typedef QDialog* (*Fun_GetLearnDialog)();
namespace Ui {
class CircleWidget;
}
/**
  * @class  <CircleWidget>  [CircleWidget.h]  [CircleWidget]
  * @brief  圆形定位工具界面
  * @author  <szj>
  * @note
  * detailed  description
  */
class CircleWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CircleWidget(QWidget *parent = 0);
    ~CircleWidget();
    QPoint Get_Image_Pos();
    int Set_Parameter_To_Ram();
    void Init_Input_Ptr(void *ptr,int i_step_index,int new_flag,void *pen_color);
    void Get_Result_From_Ram();
protected:
    bool eventFilter(QObject *watched, QEvent *event);
private slots:
     void slostCenter();

    void addR();

    void subR();

    void upCenter();

    void downCenter();

    void leftCenter();

    void rightCenter();

    void on_btnAddRadius_pressed();

    void on_btnAddRadius_released();

    void on_btnSubRadius_pressed();

    void on_btnSubRadius_released();

    void on_btnCircleUP_pressed();

    void on_btnCircleUP_released();

    void on_btnCircleDown_pressed();

    void on_btnCircleDown_released();

    void on_btnCircleLeft_pressed();

    void on_btnCircleLeft_released();

    void on_btnCircleRight_pressed();

    void on_btnCircleRight_released();

private:
    Ui::CircleWidget *ui;
    QLabel *m_label;
    int m_step_index;//当前步骤索引
    CIRCLE_INPUT_PARAM *pInputPara;
    QLibrary m_keyBoard_Lib;
    QLibrary m_Learn_Lib;
    int m_iPresstimes;
    QTimer *TimerUP;
    CIRCLE_INPUT_PARAM m_circle_input;

    void InitData();
    void LoadKeyBoardLib();
    void LoadLearnLib();
    void SetDataToLib();
    int GetStepValue();


};

#endif // CIRCLEWIDGET_H
