/****************************************************************************
**
** Copyright (C) 2013 Jolla Ltd.
** Contact: Vesa-Matti Hartikainen <vesa-matti.hartikainen@jollamobile.com>
**
****************************************************************************/


#include "declarativebrowsertab.h"
#include <QPixmap>
#include <QDeclarativeEngine>
#include <QDeclarativeContext>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QTransform>
#include <QDesktopServices>

DeclarativeBrowserTab::DeclarativeBrowserTab(QDeclarativeView* view, QObject *parent) :
    QObject(parent), m_view(view)
{
    view->engine()->rootContext()->setContextProperty("BrowserTab",this);
}

DeclarativeBrowserTab::~DeclarativeBrowserTab()
{
    for (int i = 0; i < paths.size(); ++i) {
        QFile f(paths.at(i));
        if (f.exists()) {
            f.remove();
        }
    }
    paths.clear();
}

QString DeclarativeBrowserTab::screenCapture(int x, int y, int width, int height)
{
    QPixmap pixmap = QPixmap::grabWindow(m_view->winId(), x, y, width, height);
    int randomValue = abs(qrand());
    QString cacheLocation = QDesktopServices::storageLocation(QDesktopServices::CacheLocation);
    QDir dir(cacheLocation);
    if(!dir.exists()) {
        if(!dir.mkpath(cacheLocation)) {
            qWarning() << "Can't create directory "+ cacheLocation;
            return "";
        }
    }
    QString path = QDesktopServices::storageLocation(QDesktopServices::CacheLocation) + QString::number(randomValue);
    path.append(QString("-thumb.png"));
    pixmap.save(path);
    paths << path;
    return path;
}