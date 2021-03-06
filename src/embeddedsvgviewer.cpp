#include "embeddedsvgviewer.h"
#include <qmath.h>
#include<QTimer>
#include <QKeyEvent>
#include <QTextFormat>
/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the demonstration applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QPainter>
#include <QApplication>
#include <QDebug>
#include "embeddedsvgviewer.h"
#include <QDesktopWidget>
#include <QPoint>
#include <QDir>
#include <QMovie>
#include "monitorwidget.h"

EmbeddedSvgViewer::EmbeddedSvgViewer(const QString &filePath):AH_Painter(this),Dial_Painter(this),FixedHorizonTrans_Painter(this)
{
    /*    qApp->setStyleSheet(" QSlider:vertical { width: 50px; } \
                         QSlider::groove:vertical { border: 1px solid black; border-radius: 3px; width: 6px; } \
                          QSlider::handle:vertical { height: 25px; margin: 0 -22px; image: url('://res/v-slider-handle.svg'); } \
                       ");
*/
    QDesktopWidget *dwsktopwidget = QApplication::desktop();
    QRect deskrect = dwsktopwidget->availableGeometry();    //desktop size
    QRect screenrect = dwsktopwidget->screenGeometry();   //screen size
    mScreencenter.setX(screenrect.width()/2);
    mScreencenter.setY(screenrect.height()/2);


    m_renderer = new QSvgRenderer(filePath);
    m_imageSize = m_renderer->viewBox().size();

    m_viewBoxCenter = (QPointF(m_imageSize.width() / qreal(2.0), m_imageSize.height() / qreal(2.0)));

    m_zoomSlider = new QSlider(Qt::Vertical, this);
    m_zoomSlider->setMaximum(150);
    m_zoomSlider->setMinimum(1);

    connect(m_zoomSlider, SIGNAL(valueChanged(int)), this, SLOT(setZoom(int)));
    m_zoomSlider->setValue(100);

    if (m_renderer->animated())
        connect(m_renderer, SIGNAL(repaintNeeded()), this, SLOT(update()));

    QDir dir;
    QString pathname;
    pathname = dir.currentPath();
    qDebug()<<"CurrentPath:"<<pathname;

    QString AH_PATH = ":/Gyroscope/res/AH1.png";
    qDebug()<<"laoding AH.png from "<<AH_PATH;
    AH.load(AH_PATH);

    QString FixedHorizonTrans_PATH = ":/Gyroscope/res/FixedHorizonTrans.png";
    qDebug()<<"laoding FixedHorizonTrans.png from "<<FixedHorizonTrans_PATH;
    FixedHorizonTrans .load(FixedHorizonTrans_PATH);

    QString Dial_PATH = ":/Gyroscope/res/Dial.png";
    qDebug()<<"laoding Dial.png from "<<Dial_PATH;
    Dial.load(Dial_PATH);

    QSize gyroscopeAreaSize(screenrect.width()/2,screenrect.width()/2);
    QMatrix rotateMatrix;
    rotateMatrix.rotate(-90);
    AH=AH.transformed(rotateMatrix);
    AH=AH.scaledToWidth(gyroscopeAreaSize.width());
    //   qDebug()<<"AH size:"<<AH.width()<<AH.height();

    FixedHorizonTrans=FixedHorizonTrans.scaled(gyroscopeAreaSize);
    //    qDebug()<<"FixedHorizonTrans size:"<<FixedHorizonTrans.width()<<FixedHorizonTrans.height();

    Dial=Dial.scaled(gyroscopeAreaSize);
    //    qDebug()<<"Dial size:"<<Dial.width()<<Dial.height();

    mTimer = new QTimer();
    mTimer->setInterval(40);
    connect(mTimer, SIGNAL(timeout()),SLOT(update()));
    mTimer->start();


    QSize labelSize(    screenrect.width()/12,screenrect.width()/12);
    QRect pitchRect(0,screenrect.width()/2,
                    screenrect.width()/12,screenrect.height()-screenrect.width()/2);
    QRect pitchValueRect(screenrect.width()/12,screenrect.width()/2,
                         screenrect.width()/12,screenrect.height()-screenrect.width()/2);
    QPalette pe1;
    pe1.setColor(QPalette::WindowText,Qt::red);
    mPitchLabel = new AttitudeLabel("Pitch",this,pitchRect,QString::fromUtf8("font: 8pt \"Sans Serif\";"),pe1,true,":/Gyroscope/res/pitch.gif",labelSize,false);
    mPitchLabelValue = new AttitudeLabel("0",this,pitchValueRect,QString::fromUtf8("font: 8pt \"Sans Serif\";"),pe1,true,NULL,labelSize,false);


    QRect rollRect(screenrect.width()/6,screenrect.width()/2,
                   screenrect.width()/12,screenrect.height()-screenrect.width()/2);
    QRect rollValueRect(screenrect.width()/6+screenrect.width()/12,screenrect.width()/2,
                        screenrect.width()/12,screenrect.height()-screenrect.width()/2);
    QPalette pe2;
    pe2.setColor(QPalette::WindowText,Qt::green);
    mRollLabel = new AttitudeLabel("Roll",this,rollRect,QString::fromUtf8("font: 8pt \"Sans Serif\";"),pe2,true,":/Gyroscope/res/roll.gif",labelSize,false);
    mRollLabelValue = new AttitudeLabel("0",this,rollValueRect,QString::fromUtf8("font: 8pt \"Sans Serif\";"),pe2,true,NULL,labelSize,false);

    QRect yawRect(screenrect.width()/3,screenrect.width()/2,
                  screenrect.width()/12,screenrect.height()-screenrect.width()/2);
    QRect yawRectValue(screenrect.width()/3+screenrect.width()/12,screenrect.width()/2,
                       screenrect.width()/12,screenrect.height()-screenrect.width()/2);
    QPalette pe3;
    pe3.setColor(QPalette::WindowText,Qt::blue);
    mYawLabel = new AttitudeLabel("Yaw",this,yawRect,QString::fromUtf8("font: 8pt \"Sans Serif\";"),pe3,true,":/Gyroscope/res/yaw.gif",labelSize,false);
    mYawLabelValue = new AttitudeLabel("0",this,yawRectValue,QString::fromUtf8("font: 8pt \"Sans Serif\";"),pe3,true,NULL,labelSize,false);

    m_quitButton = new QPushButton("Quit", this);
    QRect quitButtonRect(mScreencenter.x()+FixedHorizonTrans.width()/2,
                         3*mScreencenter.y()/2,
                         mScreencenter.x()-FixedHorizonTrans.width()/2,
                         mScreencenter.y()/2);
    connect(m_quitButton, SIGNAL(pressed()), QApplication::instance(), SLOT(quit()));
    quitButtonTimer = new QTimer();
    quitButtonTimer->setInterval(2000);
    connect(quitButtonTimer, SIGNAL(timeout()),m_quitButton,SLOT(hide()));
    quitButtonTimer->start();
#if USE_GLC_LIB
    QRect ModelViewRect(screenrect.width()/2,0,
                        screenrect.width()/2,screenrect.width()/2);
    mModelViewGadgetWidget = new ModelViewGadgetWidget(this);
    mModelViewGadgetWidget->setGeometry(ModelViewRect);

    mModelViewGadgetWidget->reloadScene();
    connect(this, SIGNAL(updateModelView(QByteArray)), mModelViewGadgetWidget, SLOT(serialPortHandler(QByteArray)));
#endif

    QRect VideoViewRect(screenrect.width()/2,0,
                        screenrect.width()/2,screenrect.width()/2);
    videoView =new QLabel;
    videoView->setGeometry(VideoViewRect);
    vc = new VideoClient;
    connect(vc,SIGNAL(videoReady(QString)),this,SLOT(showVideoView(QString)));

    QRect MonitorWidgetRect(screenrect.width()/2,0,
                          screenrect.width()/2,screenrect.height()-screenrect.width()/2);
    mMonitorWidget = new MonitorWidget(this);
    mMonitorWidget->setGeometry(MonitorWidgetRect);
    mMonitorWidget->show();
    connect(this,SIGNAL(telemetryConnected()),mMonitorWidget,SLOT(telemetryConnected()));
    connect(this,SIGNAL(telemetryDisconnected()),mMonitorWidget,SLOT(telemetryDisconnected()));
    connect(this,SIGNAL(telemetryUpdated(double,double)),mMonitorWidget,SLOT(telemetryUpdated(double,double)));
}

void EmbeddedSvgViewer::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    m_renderer->setViewBox(m_viewBox);
    m_renderer->render(&painter);

    qreal pitch,roll,yaw;
    pitch = mAttitudeState.Pitch;
    roll = mAttitudeState.Roll;
    yaw = mAttitudeState.Yaw;

    mPitchLabelValue->setText(QString("%1").arg(mAttitudeState.Pitch));
    mRollLabelValue->setText(QString("%1").arg(mAttitudeState.Roll));
    mYawLabelValue->setText(QString("%1").arg(mAttitudeState.Yaw));

    painter.translate(FixedHorizonTrans.width()/2,FixedHorizonTrans.height()/2);

    painter.rotate(-roll);
    QRect AHTargetArea(-AH.width()/2,
                       -FixedHorizonTrans.height()/2,
                       AH.width(),
                       FixedHorizonTrans.height());
    QRect AH_SourceSection(0,
                           (AH.height()-FixedHorizonTrans.height())/2-pitch*AH.height()/180,
                           AH.width(),
                           FixedHorizonTrans.height());
    painter.drawPixmap(AHTargetArea,
                       AH,
                       AH_SourceSection);

    painter.rotate(roll);
    painter.drawPixmap(-FixedHorizonTrans.width()/2,-FixedHorizonTrans.height()/2,FixedHorizonTrans.width(),FixedHorizonTrans.height(),FixedHorizonTrans);

    painter.rotate(yaw);
    painter.drawPixmap(-Dial.width()/2,-Dial.height()/2,Dial.width(),Dial.height(),Dial);

    mMonitorWidget->update();
}


void EmbeddedSvgViewer::mouseMoveEvent ( QMouseEvent * event )
{
    if(m_quitButton->isHidden())
    {
        m_quitButton->show();
    }

    int incX = int((event->globalX() - m_mousePress.x()) * m_imageScale);
    int incY = int((event->globalY() - m_mousePress.y()) * m_imageScale);

    QPointF newCenter;
    newCenter.setX(m_viewBoxCenterOnMousePress.x() - incX);
    newCenter.setY(m_viewBoxCenterOnMousePress.y() - incY);

    QRectF newViewBox = getViewBox(newCenter);


    // Do a bounded move on the horizontal:
    if ( (newViewBox.left() >= m_viewBoxBounds.left()) &&
         (newViewBox.right() <= m_viewBoxBounds.right()) )
    {
        m_viewBoxCenter.setX(newCenter.x());
        m_viewBox.setLeft(newViewBox.left());
        m_viewBox.setRight(newViewBox.right());
    }

    // do a bounded move on the vertical:
    if (  (newViewBox.top() >= m_viewBoxBounds.top()) &&
          (newViewBox.bottom() <= m_viewBoxBounds.bottom()) )
    {
        m_viewBoxCenter.setY(newCenter.y());
        m_viewBox.setTop(newViewBox.top());
        m_viewBox.setBottom(newViewBox.bottom());
    }

    update();
}

void EmbeddedSvgViewer::mousePressEvent ( QMouseEvent * event )
{
    if(m_quitButton->isHidden())
    {
        m_quitButton->show();
    }
    m_viewBoxCenterOnMousePress = m_viewBoxCenter;
    m_mousePress = event->globalPos();
}


QRectF EmbeddedSvgViewer::getViewBox(QPointF viewBoxCenter)
{
    QRectF result;
    result.setLeft(viewBoxCenter.x() - (m_viewBoxSize.width() / 2));
    result.setTop(viewBoxCenter.y() - (m_viewBoxSize.height() / 2));
    result.setRight(viewBoxCenter.x() + (m_viewBoxSize.width() / 2));
    result.setBottom(viewBoxCenter.y() + (m_viewBoxSize.height() / 2));
    return result;
}

void EmbeddedSvgViewer::keyPressEvent(QKeyEvent *event)
{
    //    qDebug()<<"key value:"<<hex<<event->key();
    if(event->key() == Qt::Key_Left)
    {
        if(event->modifiers() == Qt::ControlModifier)
        {;}
        else
        {;}
        event->accept();
        return;
    }

    if(event->key() == Qt::Key_Right)
    {
        if(event->modifiers() == Qt::ControlModifier)
        {;}
        else
        {;}
        event->accept();
        return;
    }

    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Select ||event->key() == Qt::Key_Return) {
        qApp->quit();;
        event->accept();
        return;
    }

    event->ignore();
}

void EmbeddedSvgViewer::updateImageScale()
{
    m_imageScale = qMax( (qreal)m_imageSize.width() / (qreal)width(),
                         (qreal)m_imageSize.height() / (qreal)height())*m_zoomLevel;

    m_viewBoxSize.setWidth((qreal)width() * m_imageScale);
    m_viewBoxSize.setHeight((qreal)height() * m_imageScale);
}


void EmbeddedSvgViewer::resizeEvent ( QResizeEvent * /* event */ )
{
    qreal origZoom = m_zoomLevel;

    // Get the new bounds:
    m_zoomLevel = 1.0;
    updateImageScale();
    m_viewBoxBounds = getViewBox(QPointF(m_imageSize.width() / 2.0, m_imageSize.height() / 2.0));

    m_zoomLevel = origZoom;
    updateImageScale();
    m_viewBox = getViewBox(m_viewBoxCenter);

    QRect sliderRect;
    sliderRect.setLeft(width() - m_zoomSlider->sizeHint().width());
    sliderRect.setRight(width());
    sliderRect.setTop(height()/4);
    sliderRect.setBottom(height() - (height()/4));
    m_zoomSlider->setGeometry(sliderRect);
}


void EmbeddedSvgViewer::setZoom(int newZoom)
{
    m_zoomLevel = qreal(newZoom) / qreal(100);

    updateImageScale();
    m_viewBox = getViewBox(m_viewBoxCenter);

    update();
}

/*
recognise packet and fetch data from it
*/
void EmbeddedSvgViewer::attitudeHandler(float pitch,float roll,float yaw)
{
    mAttitudeState.Pitch=pitch;
    mAttitudeState.Roll=roll;
    mAttitudeState.Yaw=yaw;

    emit telemetryConnected();
    update();
   // emit updateModelView(array);
    //   mModelViewGadgetWidget->serialPortHandler(array);
}

void EmbeddedSvgViewer::showVideoView(QString s)
{
    videoView->setText(s);
}

EmbeddedSvgViewer::AttitudeLabel::AttitudeLabel()
{

}

EmbeddedSvgViewer::AttitudeLabel::AttitudeLabel(QString defaultText,
                             QWidget *parent,
                             QRect geometry,
                            QString styleSheet,
                             QPalette pe,
                             bool isAutoFillBackground,
                             QString moviePath,QSize movieSize,bool isStartMovie)
{
    this->setParent(parent);
    this->setText(defaultText);
    setAutoFillBackground(isAutoFillBackground);
    setGeometry(geometry);
    setPalette(pe);
   setStyleSheet(styleSheet);
    if(NULL!=moviePath)
    {
        QMovie *mMovie=new QMovie(moviePath);
        mMovie->setScaledSize(movieSize);
        if(isStartMovie)
        {
            setMovie(mMovie);
             mMovie->start();
        }
    }
}
