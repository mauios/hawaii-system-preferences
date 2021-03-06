/****************************************************************************
 * This file is part of Hawaii.
 *
 * Copyright (C) 2015-2016 Pier Luigi Fiorini
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * $BEGIN_LICENSE:LGPL2.1+$
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * $END_LICENSE$
 ***************************************************************************/

#include <QtQml/QQmlExtensionPlugin>
#include <QtQml/QQmlComponent>

#include "keyboarddata.h"
#include "keyboardmodel.h"
#include "keyboardlayout.h"
#include "keyboardlayoutvariant.h"

class KeyboardPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface/1.0")
public:
    void registerTypes(const char *uri);
};

void KeyboardPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(QByteArray(uri) == QByteArray("org.hawaiios.systempreferences.keyboard"));

    // @uri org.hawaiios.systempreferences.keyboard
    qmlRegisterType<KeyboardData>(uri, 1, 0, "KeyboardData");
    qmlRegisterUncreatableType<KeyboardModel>(uri, 1, 0, "KeyboardModel",
                                              QStringLiteral("Cannot create KeyboardModel"));
    qmlRegisterUncreatableType<KeyboardLayout>(uri, 1, 0, "KeyboardLayout",
                                               QStringLiteral("Cannot create KeyboardLayout"));
    qmlRegisterUncreatableType<KeyboardLayoutVariant>(uri, 1, 0, "KeyboardLayoutVariant",
                                                      QStringLiteral("Cannot create KeyboardLayoutVariant"));
}

#include "plugin.moc"
