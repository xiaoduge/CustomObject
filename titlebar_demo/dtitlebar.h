/*---------------------------------------------------
**
** @author  : dcj
** @date    : 2020-08-31 10:51:08
** @version : V0.0.1
**
** @brief   : 自定义标题栏：
** 1. 按钮图标，点击可弹出菜单
** 2. 文字标题
** 3. 最小化按钮
** 4. 最大化按钮
** 5. 关闭按钮
----------------------------------------------------*/


#ifndef DTITLEBAR_H
#define DTITLEBAR_H

#include <QWidget>
#include <QIcon>

class QLabel;
class QPushButton;
class QMenu;

class DTitleBar : public QWidget
{
    Q_OBJECT
public:
    explicit DTitleBar(QWidget *parent = nullptr);
    ~ DTitleBar();

    enum TitleBarFlags
    {
        MaxButtonShow = 0x01,
        MinButtonShow = 0x02,
        CloseButtonShow = 0x04,
        AllButtonShow = MaxButtonShow | MinButtonShow | CloseButtonShow
    };

    void setBackgroundColor(const QColor& color);
    void setParentMovable(bool bMove);
    void showTitleIcon(bool iShow);
    void setTitleFlags(int flags);

signals:

protected slots:
    void onCloseBtnClicked();
    void onMaxBtnClicked();
    void onMinBtnClicked();
    void onIconBtnClicked();

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    enum ICON
    {
        Icon_Close = 0,
        Icon_Max,
        Icon_Normal,
        Icon_Min,
        Icon_Num
    };

    void initUI();
    void initSlot();
    void initIcon();
    void initPopMenu();

private:
    QLabel *m_pTitleText;
    QPushButton *m_pIconBtn;
    QPushButton *m_pCloseBtn;
    QPushButton *m_pMaxBtn;
    QPushButton *m_pMinBtn;

    QPoint m_startMovePos;
    bool m_bPressed;
    bool m_bMovable;

    int m_iHeight;

    QIcon m_icon[Icon_Num];

    QMenu *m_pPopMenu;
};

#endif // DTITLEBAR_H
