#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <qnamespace.h>

class QString;
class QWidget;

void keyboard(QWidget* w, Qt::Key key, const QString& text);
void keyboard(QWidget* w, Qt::Key key);

#endif // KEYBOARD_H
