#include "recentdocuments.h"
#include "recentdocuemnts_log.h"

#include <QCoreApplication>
#include <QDBusInterface>
#include <QFileInfo>
#include <QUrl>

#include <KDesktopFile>
#include <KDirWatch>
#include <KIO/Job>
#include <KLocalizedString>
#include <KProtocolManager>
#include <KRecentDocument>

#include <stdio.h>

#ifdef Q_OS_WIN
#include <sys/stat.h>
#endif

// Pseudo plugin class to embed meta data
class KIOPluginForMetaData : public QObject
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.kde.kio.worker.recentdocuments" FILE "recentdocuments.json")
};

extern "C" int Q_DECL_EXPORT kdemain(int argc, char **argv)
{
    // necessary to use other kio worker
    QCoreApplication app(argc, argv);
    app.setApplicationName("kio_recentdocuments");
    if (argc != 4) {
        fprintf(stderr, "Usage: kio_recentdocuments protocol domain-socket1 domain-socket2\n");
        exit(-1);
    }
    // start the worker
    RecentDocuments worker(argv[2], argv[3]);
    worker.dispatchLoop();
    return 0;
}

bool isRootUrl(const QUrl &url)
{
    const QString path = url.adjusted(QUrl::StripTrailingSlash).path();
    return (!url.hasQuery() && (path.isEmpty() || path == QLatin1String("/")));
}

RecentDocuments::RecentDocuments(const QByteArray &pool, const QByteArray &app)
    : ForwardingWorkerBase("recentdocuments", pool, app)
{
    QDBusInterface kded("org.kde.kded5", "/kded", "org.kde.kded5");
    kded.call("loadModule", "recentdocumentsnotifier");
}

RecentDocuments::~RecentDocuments()
{
}

bool RecentDocuments::rewriteUrl(const QUrl &url, QUrl &newUrl)
{
    if (isRootUrl(url)) {
        return false;
    } else {
        const QString desktopFilePath = QString("%1/%2.desktop").arg(KRecentDocument::recentDocumentDirectory(), url.path());
        if (KDesktopFile::isDesktopFile(desktopFilePath)) {
            KDesktopFile file(desktopFilePath);
            if (file.hasLinkType())
                newUrl = QUrl(file.readUrl());
        }

        return !newUrl.isEmpty();
    }
}

KIO::WorkerResult RecentDocuments::listDir(const QUrl &url)
{
    if (isRootUrl(url)) {
        const QStringList list = KRecentDocument::recentDocuments();
        KIO::UDSEntryList udslist;
        QSet<QString> urlSet;
        for (const QString &entry : list) {
            if (KDesktopFile::isDesktopFile(entry)) {
                QFileInfo info(entry);
                KDesktopFile file(entry);

                QUrl urlInside(file.readUrl());
                QString toDisplayString = urlInside.toDisplayString();

                // Filter out things that can't be viewed in a file manager because they don't
                // meet the user definition of a file for the purpose of "recently accessed files"
                if (urlInside.scheme() == "recentdocuments" || !KProtocolManager::supportsListing(urlInside) || urlSet.contains(toDisplayString))
                    continue;

                KIO::UDSEntry uds;
                if (urlInside.isLocalFile()) {
                    KIO::StatJob *job = KIO::stat(urlInside, KIO::HideProgressInfo);
                    // we do not want to wait for the event loop to delete the job
                    QScopedPointer<KIO::StatJob> sp(job);
                    job->setAutoDelete(false);
                    if (job->exec()) {
                        uds = job->statResult();
                    }
                }

                urlSet.insert(toDisplayString);
                uds.replace(KIO::UDSEntry::UDS_NAME, info.completeBaseName());

                if (urlInside.isLocalFile()) {
                    uds.replace(KIO::UDSEntry::UDS_DISPLAY_NAME, urlInside.toLocalFile());
                    uds.replace(KIO::UDSEntry::UDS_LOCAL_PATH, urlInside.path());
                } else {
                    uds.fastInsert(KIO::UDSEntry::UDS_DISPLAY_NAME, toDisplayString);
                    uds.fastInsert(KIO::UDSEntry::UDS_ICON_NAME, file.readIcon());
                }
                uds.replace(KIO::UDSEntry::UDS_TARGET_URL, toDisplayString);
                udslist << uds;
            }
        }
        listEntries(udslist);
        return KIO::WorkerResult::pass();
    }

    return KIO::WorkerResult::fail(KIO::ERR_DOES_NOT_EXIST, url.toDisplayString());
}

QString RecentDocuments::desktopFile(KIO::UDSEntry &entry) const
{
    const QString name = entry.stringValue(KIO::UDSEntry::UDS_NAME);
    if (name == "." || name == "..")
        return QString();

    QUrl url = processedUrl();
    url = url.adjusted(QUrl::StripTrailingSlash);
    url.setPath(url.path() + '/' + name);

    if (KDesktopFile::isDesktopFile(url.toLocalFile()))
        return url.toLocalFile();

    return QString();
}

KIO::WorkerResult RecentDocuments::stat(const QUrl &url)
{
    if (isRootUrl(url)) {
        qCDebug(LOG_RECENTDOCUMENTS) << "Stat root" << url;
        //
        // stat the root path
        //
        QString dirName = i18n("Recent Documents");
        KIO::UDSEntry uds;
        uds.reserve(7);
        uds.fastInsert(KIO::UDSEntry::UDS_NAME, dirName);
        uds.fastInsert(KIO::UDSEntry::UDS_DISPLAY_NAME, dirName);
        uds.fastInsert(KIO::UDSEntry::UDS_DISPLAY_TYPE, dirName);
        uds.fastInsert(KIO::UDSEntry::UDS_ICON_NAME, QString::fromLatin1("document-open-recent"));
        uds.fastInsert(KIO::UDSEntry::UDS_FILE_TYPE, S_IFDIR);
        uds.fastInsert(KIO::UDSEntry::UDS_MIME_TYPE, QString::fromLatin1("inode/directory"));
#ifdef Q_OS_WIN
        uds.fastInsert(KIO::UDSEntry::UDS_ACCESS, _S_IREAD);
#else
        uds.fastInsert(KIO::UDSEntry::UDS_ACCESS, S_IRUSR | S_IXUSR);
#endif
        statEntry(uds);
        return KIO::WorkerResult::pass();
    }
    // results are forwarded

    qCDebug(LOG_RECENTDOCUMENTS) << "Stat forward" << url;
    return ForwardingWorkerBase::stat(url);
}

KIO::WorkerResult RecentDocuments::mimetype(const QUrl &url)
{
    qCDebug(LOG_RECENTDOCUMENTS) << url;

    // the root url is always a folder
    if (isRootUrl(url)) {
        mimeType(QString::fromLatin1("inode/directory"));
        return KIO::WorkerResult::pass();
    }
    // results are forwarded

    return ForwardingWorkerBase::mimetype(url);
}

#include "recentdocuments.moc"
