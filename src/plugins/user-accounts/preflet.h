/****************************************************************************
 * This file is part of System Preferences.
 *
 * Copyright (C) 2011-2013 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * Author(s):
 *    Pier Luigi Fiorini
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

#ifndef PREFLET_H
#define PREFLET_H

#include <QModelIndex>

#include <QtAccountsService/UsersModel>

#include <Hawaii/SystemPreferences/PreferencesModule>

class QTranslator;

namespace Ui
{
    class UsersPreflet;
}

QT_USE_NAMESPACE_ACCOUNTSSERVICE

namespace Hawaii
{
    namespace SystemPreferences
    {
        class Preflet : public PreferencesModule
        {
            Q_OBJECT
        public:
            explicit Preflet();
            ~Preflet();

            virtual QString name() const;
            virtual QString comment() const;
            virtual QString iconName() const;
            virtual QStringList keywords() const;
            virtual PreferencesModule::Category category() const;

        protected:
            void changeEvent(QEvent *event);

        private:
            Ui::UsersPreflet *ui;
            QTranslator *m_translator;
            UsersModel *m_model;
            QModelIndex m_currentIndex;

            void loadTranslations();
            void setUiEnabled(bool enabled);
            void clearUserSelection();

        private slots:
            void userSelected(const QModelIndex &index);
            void modelRowsRemoved(const QModelIndex &parent, int first, int last);
            void realNameClicked();
            void realNameChanged();
            void realNameEditingFinished();
            void changePasswordClicked();
            void addUser();
            void removeUser();
        };
    }
}

#endif // PREFLET_H
