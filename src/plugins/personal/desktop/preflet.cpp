/****************************************************************************
 * This file is part of System Preferences.
 *
 * Copyright (c) 2011-2012 Pier Luigi Fiorini
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * $BEGIN_LICENSE:GPL$
 *
 * System Preferences is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * System Preferences is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with System Preferences.  If not, see <http://www.gnu.org/licenses/>.
 *
 * $END_LICENSE$
 ***************************************************************************/

#include <QDirIterator>
#include <QStandardPaths>
#include <QUrl>

#include <VSettings>
#include <VDesktopFile>

#include "preflet.h"
#include "ui_desktoppreflet.h"
#include "wallpapermodel.h"
#include "backgrounditemdelegate.h"

Preflet::Preflet(QWidget *parent)
    : VPreferencesModule(parent)
    , ui(new Ui::DesktopPreflet)
{
    ui->setupUi(this);

    // Settings
    m_launcherSettings = new VSettings("org.hawaii.greenisland.desktop");
    m_bgSettings = new VSettings("org.maui.desktop.background");

    // Wallpapers model
    m_wallpaperModel = new WallpaperModel(this);
    ui->bgList->setItemDelegate(new BackgroundItemDelegate(this));
    ui->bgList->setModel(m_wallpaperModel);
    connect(m_wallpaperModel, SIGNAL(wallpapersAdded()),
            this, SLOT(slotAllWallpapersLoaded()));

    // Connect signals
    connect(ui->launcherIconSizeSlider, SIGNAL(valueChanged(int)),
            ui->launcherIconSizeSpin, SLOT(setValue(int)));
    connect(ui->launcherIconSizeSpin, SIGNAL(valueChanged(int)),
            ui->launcherIconSizeSlider, SLOT(setValue(int)));
    connect(ui->launcherIconSizeSpin, SIGNAL(valueChanged(int)),
            this, SLOT(slotLauncherIconSizeChanged(int)));
    connect(ui->bgCategory, SIGNAL(currentIndexChanged(int)),
            this, SLOT(slotBackgroundCategorySelected(int)));
    connect(ui->bgList, SIGNAL(clicked(QModelIndex)),
            ui->bgList, SIGNAL(activated(QModelIndex)));
    connect(ui->bgList, SIGNAL(activated(QModelIndex)),
            this, SLOT(slotBackgroundSelected(QModelIndex)));

    // Load settings
    loadSettings();
}

Preflet::~Preflet()
{
    delete ui;
    delete m_wallpaperModel;
    delete m_bgSettings;
    delete m_launcherSettings;
}

QString Preflet::name() const
{
    return tr("Desktop");
}

QString Preflet::comment() const
{
    return tr("Configure desktop, wallpaper and screen saver.");
}

QString Preflet::iconName() const
{
    return "preferences-desktop";
}

QStringList Preflet::keywords() const
{
    return tr("desktop;launcher;wallpaper;screen saver;hot corner").split(";");
}

VPreferencesModule::Category Preflet::category() const
{
    return VPreferencesModule::PersonalCategory;
}

int Preflet::weight() const
{
    return 50;
}

void Preflet::loadSettings()
{
    // Launcher icon size
    ui->launcherIconSizeSpin->setValue(m_launcherSettings->value("launcher-icon-size").toInt());

    // Load all the coatings
    slotBackgroundCategorySelected(0);
}

void Preflet::slotLauncherIconSizeChanged(int value)
{
    m_launcherSettings->setValue("launcher-icon-size", value);
}

void Preflet::slotAllWallpapersLoaded()
{
    // Select the current configured wallpaper
    if (ui->bgCategory->currentIndex() == 0) {
        QString wallpaperPath = m_bgSettings->value("wallpaper-uri").toUrl().toLocalFile();
        ui->bgList->selectionModel()->select(m_wallpaperModel->indexOf(wallpaperPath),
                                             QItemSelectionModel::Select | QItemSelectionModel::Current);
        slotBackgroundSelected(m_wallpaperModel->indexOf(wallpaperPath));
    }
}

void Preflet::slotBackgroundCategorySelected(int index)
{
    switch (index) {
        case 0: {
            // Populate the list with wallpapers
            QStringList paths = QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation);
            foreach(QString dir, paths) {
                QString path = QString("%1wallpapers").arg(dir);
                QDirIterator it(path, QDir::Dirs | QDir::Readable | QDir::NoDotAndDotDot,
                                QDirIterator::FollowSymlinks);
                while (it.hasNext()) {
                    QDir dir(it.next());
                    if (dir.exists("metadata.desktop"))
                        m_wallpaperModel->addPath(dir.absolutePath());
                }
            }
        }
        break;
        default:
            break;
    }
}

void Preflet::slotBackgroundModeSelected(int index)
{
}

void Preflet::slotBackgroundSelected(const QModelIndex &index)
{
    if (!index.isValid())
        return;

    QString type = m_wallpaperModel->data(index, WallpaperModel::TypeRole).toString();
    if (type == "wallpaper") {
        QString name = m_wallpaperModel->data(index, Qt::DisplayRole).toString();
        QString resolution = m_wallpaperModel->data(index, WallpaperModel::ResolutionRole).toString();
        QString path = m_wallpaperModel->data(index, WallpaperModel::AbsoluteFilePathRole).toString();
        ui->bgName->setText("<b>" + name + "</b>");
        ui->bgSize->setText(resolution);

        // Save settings
        m_bgSettings->setValue("type", "wallpaper");
        m_bgSettings->setValue("wallpaper-uri", path);
    }
}

#include "moc_preflet.cpp"