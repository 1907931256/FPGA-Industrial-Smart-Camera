#ifndef COLORAREAWIDGET_H
#define COLORAREAWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLibrary>
#include <QTimer>
#include "TaskStepGlobal.h"
#include "TaskRstGlobal.h"
#include "PixelConvert.h"
#include "ColorAreaGlobal.h"

typedef QDialog* (*Fun_GetNumkeyBoardDlg)();
typedef void (*Fun_SetRangeInput)(QString max, QString min, int flag);
typedef QString (*Fun_GetRangeNum)();

typedef QDialog* (*Fun_GetLearnDialog)();
typedef void* (*Fun_SetResultString)(QString str);

namespace Ui {
class ColorAreaWidget;
}

class ColorAreaWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ColorAreaWidget(QWidget *parent = 0);
    ~ColorAreaWidget();
    QPoint Get_Image_Pos();
    int Set_Parameter_To_Ram();
    void Init_Input_Ptr(void *ptr,int i_step_index,int new_flag,void *pen_color);
protected:
    bool eventFilter(QObject *watched, QEvent *event);

private slots:
    void on_btnCalculate_clicked();
    void CalcTimerSlot();

private:
    Ui::ColorAreaWidget *ui;
    QLabel *m_label;
    COLOR_AREA_INPUT_PARAM *pInputPara;
    int m_step_index;//当前步骤索引
    int m_i_load_first;//
    QLibrary m_keyBoard_Lib;
    QLibrary m_Learn_Lib;
    QTimer *CalcTimer;

    void InitData();
    void LoadKeyBoardLib();
    void BlobAreaMinInputPro();
    void BlobAreaMaxInputPro();
    void SetDataToLib();
    void GetDataFromLib();
    void LoadLearnLib();
};

#endif // COLORAREAWIDGET_H
