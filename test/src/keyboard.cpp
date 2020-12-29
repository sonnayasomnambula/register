#include "keyboard.h"

#include <QCoreApplication>
#include <QKeyEvent>
#include <QWidget>

void keyboard(QWidget* w, Qt::Key key, const QString& text)
{
    w->setFocus();

    QKeyEvent* press = new QKeyEvent(QEvent::KeyPress, key, Qt::NoModifier, text);
    QCoreApplication::postEvent(w, press);

    QCoreApplication::processEvents();

    QKeyEvent* release = new QKeyEvent(QEvent::KeyRelease, key, Qt::NoModifier, text);
    QCoreApplication::postEvent(w, release);

    QCoreApplication::processEvents();
}

void keyboard(QWidget* w, Qt::Key key)
{
    keyboard(w, key, "");
}
