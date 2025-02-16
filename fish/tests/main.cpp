/* Copytester is a test case for the fish KIO worker. It copies 100 files from /tmp to /tmp/test using fish's functions.
It was written to verify KDE bug 147948: https://bugs.kde.org/show_bug.cgi?id=147948 */

#include <copytester.h>
#include <kaboutdata.h>
#include <kapplication.h>
#include <kcmdlineargs.h>
#include <klocalizedstring.h>
#include <kmessagebox.h>

int main(int argc, char *argv[])
{
    const QByteArray &ba = QByteArray("test");
    const KLocalizedString name = ki18n("copytester");
    KAboutData aboutData(ba, ba, name, ba, name);
    KCmdLineArgs::init(argc, argv, &aboutData);
    KApplication copytester;

    Browser *mw = new Browser();
    copytester.exec();
}
