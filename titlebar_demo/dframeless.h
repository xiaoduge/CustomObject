/*---------------------------------------------------
**
** @author  : dcj
** @date    : 2020-08-28 13:38:46
** @version : V0.0.1
**
** @brief   : 为无边框窗体提供拖动和放缩功能。
** 如果其父类为Widget则为其父类提供拖放和放缩功能，
** 否则需要设置其为哪个Widget提供拖放和放缩功能。
**
----------------------------------------------------*/

#ifndef DFRAMELESS_H
#define DFRAMELESS_H

#include <QObject>
#include <QRect>

class DFrameless : public QObject
{
    Q_OBJECT
public:
    explicit DFrameless(QObject *parent = nullptr);

protected:
    bool eventFilter(QObject *watched, QEvent *event);

signals:

public slots:
    void setPadding(int iPadding);
    void setMoveEnable(bool bEnable);
    void setResizeEnable(bool bEnable);
    void setWidget(QWidget * widget);

private:
    QWidget *m_pWidget;                //无边框窗体
    int m_padding;                    //边距
    bool m_moveEnable;                //可移动
    bool m_resizeEnable;              //可拉伸

    bool m_pressed;                   //鼠标按下
    bool m_pressedLeft;               //鼠标按下左侧
    bool m_pressedRight;              //鼠标按下右侧
    bool m_pressedTop;                //鼠标按下上侧
    bool m_pressedBottom;             //鼠标按下下侧
    bool m_pressedLeftTop;            //鼠标按下左上侧
    bool m_pressedRightTop;           //鼠标按下右上侧
    bool m_pressedLeftBottom;         //鼠标按下左下侧
    bool m_pressedRightBottom;        //鼠标按下右下侧

    int m_rectX, m_rectY, m_rectW, m_rectH; //窗体坐标+宽高
    QPoint m_lastPos;                 //鼠标按下处坐标

    QRect m_rectLeft;                 //左侧区域
    QRect m_rectRight;                //右侧区域
    QRect m_rectTop;                  //上侧区域
    QRect m_rectBottom;               //下侧区域
    QRect m_rectLeftTop;              //左上侧区域
    QRect m_rectRightTop;             //右上侧区域
    QRect m_rectLeftBottom;           //左下侧区域
    QRect m_rectRightBottom;          //右下侧区域
};

#endif // DFRAMELESS_H
