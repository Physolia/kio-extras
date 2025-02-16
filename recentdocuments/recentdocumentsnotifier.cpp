#include "recentdocumentsnotifier.h"

#include <KDirNotify>
#include <KDirWatch>
#include <KPluginFactory>
#include <KRecentDocument>
#include <QFileInfo>
#include <QUrl>

K_PLUGIN_CLASS_WITH_JSON(RecentDocumentsNotifier, "recentdocumentsnotifier.json")

RecentDocumentsNotifier::RecentDocumentsNotifier(QObject *parent, const QList<QVariant> &)
    : KDEDModule(parent)
{
    dirWatch = new KDirWatch(this);
    dirWatch->addDir(KRecentDocument::recentDocumentDirectory(), KDirWatch::WatchFiles);
    connect(dirWatch, &KDirWatch::created, this, &RecentDocumentsNotifier::dirty);
    connect(dirWatch, &KDirWatch::deleted, this, &RecentDocumentsNotifier::dirty);
    connect(dirWatch, &KDirWatch::dirty, this, &RecentDocumentsNotifier::dirty);
}

void RecentDocumentsNotifier::dirty(const QString &path)
{
    if (path.endsWith(QLatin1String(".desktop"))) {
        // Emitting FilesAdded forces a re-read of the dir
        QUrl url("recentdocuments:/");
        QFileInfo info(path);
        url.setPath(QStringLiteral("/") + info.fileName());
        org::kde::KDirNotify::emitFilesAdded(url);
    }
}

#include "moc_recentdocumentsnotifier.cpp"
#include "recentdocumentsnotifier.moc"
