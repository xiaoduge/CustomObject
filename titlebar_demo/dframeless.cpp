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

#include "dframeless.h"
#include <QWidget>
#include <QEvent>
#include <QHoverEvent>

DFrameless::DFrameless(QObject *parent)
    : QObject(parent),
      m_pWidget(nullptr),
      m_padding(8),
      m_moveEnable(true),
      m_resizeEnable(true),
      m_pressed(false),
      m_pressedLeft(false),
      m_pressedRight(false),
      m_pressedTop(false),
      m_pressedBottom(false),
      m_pressedLeftTop(false),
      m_pressedRightTop(false),
      m_pressedLeftBottom(false),
      m_pressedRightBottom(false)
{
    if(this->parent()->isWidgetType())
    {
        setWidget(static_cast<QWidget*>(parent));
    }
}

/**
 * @brief DFrameless::eventFilter [通过事件过滤器，实现指定窗口的缩放和拖动功能]
 * @param watched
 * @param event
 * @return
 */
bool DFrameless::eventFilter(QObject *watched, QEvent *event)
{
    if(m_pWidget && watched == m_pWidget)
    {
        if(event->type() == QEvent::Resize)
        {
            int width = m_pWidget->width();
            int height = m_pWidget->height();

            m_rectLeft = QRect(0, m_padding, m_padding, height - m_padding * 2); //左侧描点区域
            m_rectTop = QRect(m_padding, 0, width - m_padding * 2, m_padding);  //上侧描点区域
            m_rectRight = QRect(width - m_padding, m_padding, m_padding, height - m_padding * 2);  //右侧描点区域
            m_rectBottom = QRect(m_padding, height - m_padding, width - m_padding * 2, m_padding); //下侧描点区域

            m_rectLeftTop = QRect(0, 0, m_padding, m_padding); //左上角描点区域
            m_rectRightTop = QRect(width - m_padding, 0, m_padding, m_padding); //右上角描点区域
            m_rectLeftBottom = QRect(0, height - m_padding, m_padding, m_padding); //左下角描点区域
            m_rectRightBottom = QRect(width - m_padding, height - m_padding, m_padding, m_padding); //右下角描点区域
        }
        else if (event->type() == QEvent::HoverMove)
        {
            QHoverEvent * hoverEvent = static_cast<QHoverEvent*>(event);
            QPoint point = hoverEvent->pos();
            if(m_resizeEnable)
            {
                if(m_rectLeft.contains(point) || m_rectRight.contains(point))
                {
                    m_pWidget->setCursor(Qt::SizeHorCursor);
                }
                else if (m_rectTop.contains(point) || m_rectBottom.contains(point))
                {
                    m_pWidget->setCursor(Qt::SizeVerCursor);
                }
                else if (m_rectLeftTop.contains(point) || m_rectRightBottom.contains(point))
                {
                    m_pWidget->setCursor(Qt::SizeFDiagCursor);
                }
                else if (m_rectRightTop.contains(point) || m_rectLeftBottom.contains(point))
                {
                    m_pWidget->setCursor(Qt::SizeBDiagCursor);
                }
                else
                {
                    m_pWidget->setCursor(Qt::ArrowCursor);
                }
            }

            //根据当前鼠标位置,计算XY轴移动了多少
            int offsetX = point.x() - m_lastPos.x();
            int offsetY = point.y() - m_lastPos.y();

            //根据按下处的位置判断是否是移动控件还是拉伸控件
            if(m_moveEnable)
            {
                if(m_pressed)
                {
                    m_pWidget->move(m_pWidget->x() + offsetX, m_pWidget->y() + offsetY);
                }
            }
            if (m_resizeEnable)
            {
                if (m_pressedLeft)
                {
                    int resizeW = m_pWidget->width() - offsetX;
                    if (m_pWidget->minimumWidth() <= resizeW)
                    {
                        m_pWidget->setGeometry(m_pWidget->x() + offsetX, m_rectY, resizeW, m_rectH);
                    }
                }
                else if (m_pressedRight)
                {
                    m_pWidget->setGeometry(m_rectX, m_rectY, m_rectW + offsetX, m_rectH);
                }
                else if (m_pressedTop)
                {
                    int resizeH = m_pWidget->height() - offsetY;
                    if (m_pWidget->minimumHeight() <= resizeH)
                    {
                        m_pWidget->setGeometry(m_rectX, m_pWidget->y() + offsetY, m_rectW, resizeH);
                    }
                }
                else if (m_pressedBottom)
                {
                    m_pWidget->setGeometry(m_rectX, m_rectY, m_rectW, m_rectH + offsetY);
                }
                else if (m_pressedLeftTop)
                {
                    int resizeW = m_pWidget->width() - offsetX;
                    int resizeH = m_pWidget->height() - offsetY;
                    if (m_pWidget->minimumWidth() <= resizeW)
                    {
                        m_pWidget->setGeometry(m_pWidget->x() + offsetX, m_pWidget->y(), resizeW, resizeH);
                    }
                    if (m_pWidget->minimumHeight() <= resizeH)
                    {
                        m_pWidget->setGeometry(m_pWidget->x(), m_pWidget->y() + offsetY, resizeW, resizeH);
                    }
                }
                else if (m_pressedRightTop)
                {
                    int resizeW = m_rectW + offsetX;
                    int resizeH = m_pWidget->height() - offsetY;
                    if (m_pWidget->minimumHeight() <= resizeH)
                    {
                        m_pWidget->setGeometry(m_pWidget->x(), m_pWidget->y() + offsetY, resizeW, resizeH);
                    }
                }
                else if (m_pressedLeftBottom)
                {
                    int resizeW = m_pWidget->width() - offsetX;
                    int resizeH = m_rectH + offsetY;
                    if (m_pWidget->minimumWidth() <= resizeW)
                    {
                        m_pWidget->setGeometry(m_pWidget->x() + offsetX, m_pWidget->y(), resizeW, resizeH);
                    }
                    if (m_pWidget->minimumHeight() <= resizeH)
                    {
                        m_pWidget->setGeometry(m_pWidget->x(), m_pWidget->y(), resizeW, resizeH);
                    }
                }
                else if (m_pressedRightBottom)
                {
                    int resizeW = m_rectW + offsetX;
                    int resizeH = m_rectH + offsetY;
                    m_pWidget->setGeometry(m_pWidget->x(), m_pWidget->y(), resizeW, resizeH);
                }
            }
        }
        else if (event->type() == QEvent::MouseButtonPress)
        {
            //记住当前控件坐标和宽高以及鼠标按下的坐标
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            m_rectX = m_pWidget->x();
            m_rectY = m_pWidget->y();
            m_rectW = m_pWidget->width();
            m_rectH = m_pWidget->height();
            m_lastPos = mouseEvent->pos();

            //判断按下的手柄的区域位置
            if (m_rectLeft.contains(m_lastPos))
            {
                m_pressedLeft = true;
            }
            else if (m_rectRight.contains(m_lastPos))
            {
                m_pressedRight = true;
            }
            else if (m_rectTop.contains(m_lastPos))
            {
                m_pressedTop = true;
            }
            else if (m_rectBottom.contains(m_lastPos))
            {
                m_pressedBottom = true;
            }
            else if (m_rectLeftTop.contains(m_lastPos))
            {
                m_pressedLeftTop = true;
            }
            else if (m_rectRightTop.contains(m_lastPos))
            {
                m_pressedRightTop = true;
            }
            else if (m_rectLeftBottom.contains(m_lastPos))
            {
                m_pressedLeftBottom = true;
            }
            else if (m_rectRightBottom.contains(m_lastPos))
            {
                m_pressedRightBottom = true;
            }
            else
            {
                m_pressed = true;
            }
        }
        else if (event->type() == QEvent::MouseMove)
        {
            //改成用HoverMove识别
        }
        else if (event->type() == QEvent::MouseButtonRelease)
        {
            //恢复所有
            m_pressed = false;
            m_pressedLeft = false;
            m_pressedRight = false;
            m_pressedTop = false;
            m_pressedBottom = false;
            m_pressedLeftTop = false;
            m_pressedRightTop = false;
            m_pressedLeftBottom = false;
            m_pressedRightBottom = false;
            m_pWidget->setCursor(Qt::ArrowCursor);
        }
    }

    return QObject::eventFilter(watched, event);
}

/**
 * @brief DFrameless::setPadding [设置缩放拖动识别区]
 * @param iPadding
 */
void DFrameless::setPadding(int iPadding)
{
    m_padding = iPadding;
}

/**
 * @brief DFrameless::setMoveEnable [设置是否可以移动]
 * @param bEnable
 */
void DFrameless::setMoveEnable(bool bEnable)
{
    m_moveEnable = bEnable;
}

/**
 * @brief DFrameless::setResizeEnable [设置是否可以调整大小]
 * @param bEnable
 */
void DFrameless::setResizeEnable(bool bEnable)
{
    m_resizeEnable = bEnable;
}

/**
 * @brief DFrameless::setWidget  [设置需要放缩和拖动功能的窗口]
 * @param widget
 */
void DFrameless::setWidget(QWidget *widget)
{
    if(m_pWidget == nullptr)
    {
        m_pWidget = widget;
        m_pWidget->setMouseTracking(true);  //设置鼠标追踪为真
        m_pWidget->installEventFilter(this); //绑定事件过滤器

        //设置悬停为真,必须设置这个,不然当父窗体里边还有子窗体全部遮挡了识别不到MouseMove,需要识别HoverMove
        m_pWidget->setAttribute(Qt::WA_Hover, true);

    }
}
