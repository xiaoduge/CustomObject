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


#include "dtitlebar.h"
#include <QLabel>
#include <QPushButton>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QDebug>
#include <QApplication>
#include <QStyle>
#include <QMenu>
#include <QAction>

DTitleBar::DTitleBar(QWidget *parent)
    : QWidget(parent),
      m_bPressed(false),
      m_bMovable(false),
      m_iHeight(40)
{
    initUI();
    initSlot();
    initPopMenu();
}

DTitleBar::~DTitleBar()
{
    qDebug() << "~DTitleBar()";
}

/**
 * @brief DTitleBar::setBackgroundColor [设置标题栏背景颜色]
 * @param color
 */
void DTitleBar::setBackgroundColor(const QColor &color)
{
    QPalette palette = this->palette();
    palette.setColor(QPalette::Background, color);
    this->setPalette(palette);
}

/**
 * @brief DTitleBar::setParentMovable [设置标题栏是否可以拖动主窗口，如果主窗口自带拖动功能，需设置此属性为false]
 * @param bMove
 */
void DTitleBar::setParentMovable(bool bMove)
{
    m_bMovable = bMove;
}

/**
 * @brief DTitleBar::showTitleIcon [设置标题栏是否显示图标]
 * @param iShow
 */
void DTitleBar::showTitleIcon(bool iShow)
{
    if(iShow)
    {
        m_pIconBtn->show();
    }
    else
    {
        m_pIconBtn->hide();
    }

}

/**
 * @brief DTitleBar::setTitleFlags [设置标题栏所显示的按钮]
 * @param flags
 */
void DTitleBar::setTitleFlags(int flags)
{
    if(flags & MaxButtonShow)
    {
        m_pMaxBtn->show();
    }
    else
    {
        m_pMaxBtn->hide();
    }

    if(flags & MinButtonShow)
    {
        m_pMinBtn->show();
    }
    else
    {
        m_pMinBtn->hide();
    }

    if(flags & CloseButtonShow)
    {
        m_pCloseBtn->show();
    }
    else
    {
        m_pCloseBtn->hide();
    }
}


void DTitleBar::onCloseBtnClicked()
{
    qApp->quit();
}

void DTitleBar::onMaxBtnClicked()
{
    if(this->parentWidget()->isMaximized())
    {
        this->parentWidget()->showNormal();
        m_pMaxBtn->setIcon(m_icon[Icon_Max]);
    }
    else
    {
        this->parentWidget()->showMaximized();
        m_pMaxBtn->setIcon(m_icon[Icon_Normal]);
    }

}

void DTitleBar::onMinBtnClicked()
{
    this->parentWidget()->showMinimized();
}

void DTitleBar::onIconBtnClicked()
{
    m_pPopMenu->exec(QCursor::pos());
}

void DTitleBar::paintEvent(QPaintEvent *event)
{
    this->setFixedWidth(this->parentWidget()->width());
    this->setFixedHeight(m_iHeight);
    QWidget::paintEvent(event);
}

void DTitleBar::mousePressEvent(QMouseEvent *event)
{
    m_bPressed = true;
    m_startMovePos = event->globalPos();

    return QWidget::mousePressEvent(event);
}

void DTitleBar::mouseMoveEvent(QMouseEvent *event)
{
    if(m_bMovable && m_bPressed)
    {
        QPoint movePoint = event->globalPos() - m_startMovePos;
        QPoint widgetPos = this->parentWidget()->pos();
        m_startMovePos = event->globalPos();
        this->parentWidget()->move(widgetPos.x() + movePoint.x(), widgetPos.y() + movePoint.y());
    }
    return QWidget::mouseMoveEvent(event);
}

void DTitleBar::mouseReleaseEvent(QMouseEvent *event)
{
    m_bPressed = false;
    return QWidget::mouseReleaseEvent(event);
}

void DTitleBar::initUI()
{
    initIcon();
    this->setAutoFillBackground(true);
    QPalette palette = this->palette();
    palette.setColor(QPalette::Background, Qt::white);
    this->setPalette(palette);

    m_pCloseBtn = new QPushButton(this);
    m_pCloseBtn->setIcon(m_icon[Icon_Close]);
    m_pMaxBtn = new QPushButton(this);
    m_pMaxBtn->setIcon(m_icon[Icon_Max]);
    m_pMinBtn = new QPushButton(this);
    m_pMinBtn->setIcon(m_icon[Icon_Min]);

    m_pCloseBtn->setFlat( true );
    m_pCloseBtn->setStyleSheet("QPushButton{background: transparent;}");
    m_pMaxBtn->setFlat( true );
    m_pMaxBtn->setStyleSheet("QPushButton{background: transparent;}");
    m_pMinBtn->setFlat( true );
    m_pMinBtn->setStyleSheet("QPushButton{background: transparent;}");


    m_pIconBtn = new QPushButton(this);
    m_pIconBtn->setFlat( true );
    m_pIconBtn->setStyleSheet("QPushButton{background: transparent;}");
    m_pIconBtn->setIcon(QIcon(":/icons/sword.png"));

    m_pTitleText = new QLabel(this);
    m_pTitleText->setText(this->parentWidget()->windowTitle());

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(m_pIconBtn);
    mainLayout->addWidget(m_pTitleText);
    mainLayout->addStretch(1);
    mainLayout->addWidget(m_pMinBtn);
    mainLayout->addWidget(m_pMaxBtn);
    mainLayout->addWidget(m_pCloseBtn);

    this->setLayout(mainLayout);

}

void DTitleBar::initSlot()
{
    connect(m_pCloseBtn, SIGNAL(clicked()), this, SLOT(onCloseBtnClicked()));
    connect(m_pMaxBtn, SIGNAL(clicked()), this, SLOT(onMaxBtnClicked()));
    connect(m_pMinBtn, SIGNAL(clicked()), this, SLOT(onMinBtnClicked()));
    connect(m_pIconBtn, SIGNAL(clicked()), this, SLOT(onIconBtnClicked()));
}

void DTitleBar::initIcon()
{
#ifndef CUSTOMICON
    m_icon[Icon_Close] = this->style()->standardIcon(QStyle::SP_TitleBarCloseButton);
    m_icon[Icon_Max] = this->style()->standardIcon(QStyle::SP_TitleBarMaxButton);
    m_icon[Icon_Normal] = this->style()->standardIcon(QStyle::SP_TitleBarNormalButton);
    m_icon[Icon_Min] = this->style()->standardIcon(QStyle::SP_TitleBarMinButton);

#else
    m_icon[Icon_Close] = QIcon(":/images/closebtn.png");
    m_icon[Icon_Max] = QIcon(":/images/maxbtn1.png");
    m_icon[Icon_Normal] = QIcon(":/images/maxbtn2.png");
    m_icon[Icon_Min] = QIcon(":/images/minbtn.png");
#endif
}

void DTitleBar::initPopMenu()
{
    m_pPopMenu = new QMenu(this);
    QAction *normalAction = new QAction(style()->standardIcon(QStyle::SP_TitleBarNormalButton), tr("Restore"), this);
    QAction *minAction = new QAction(style()->standardIcon(QStyle::SP_TitleBarMinButton), tr("Minimize"), this);
    QAction *maxAction = new QAction(style()->standardIcon(QStyle::SP_TitleBarMaxButton), tr("Maximize"), this);
    QAction *closeAction = new QAction(style()->standardIcon(QStyle::SP_TitleBarCloseButton), tr("Close"), this);

    m_pPopMenu->addAction(normalAction);
    m_pPopMenu->addAction(minAction);
    m_pPopMenu->addAction(maxAction);
    m_pPopMenu->addSeparator();
    m_pPopMenu->addAction(closeAction);

    connect(normalAction, SIGNAL(triggered()), this->parentWidget(), SLOT(showNormal()));
    connect(minAction, SIGNAL(triggered()), this->parentWidget(), SLOT(showMinimized()));
    connect(maxAction, SIGNAL(triggered()), this->parentWidget(), SLOT(showMaximized()));
    connect(closeAction, SIGNAL(triggered()), this, SLOT(onCloseBtnClicked()));
}
