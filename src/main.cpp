#include "gui/procgui.h"
#include <QCoreApplication>
#include <QApplication>
#include <QCommandLineParser>
#include <klocalizedstring.h>
#include <kaboutdata.h>
#include <KConfigGroup>
#include "src/base/version.h"
#include "src/base/config.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	KLocalizedString::setApplicationDomain("procgui");
	KAboutData l_aboutData(
			"procgui"
		,	i18n("Process List")
		,	VERSION
		,	i18n("X")
		,	KAboutLicense::GPL
		,	i18n("(C) 2016")
		,	i18n("x")
		,	QStringLiteral("X")
		,	QStringLiteral("X")
		   );
	l_aboutData.addAuthor(i18n("Jeroen van Iddekinge"),i18n("X"),("iddekingej@lycos.com"));
	KAboutData::setApplicationData(l_aboutData);
	QCommandLineParser l_parser;
	l_parser.addHelpOption();
	l_parser.addVersionOption();
	l_aboutData.setupCommandLine(&l_parser);
	l_parser.process(app);
	l_aboutData.processCommandLine(&l_parser);    
	g_config.setup();

	TProcGui l_window;
	l_window.show();
	return app.exec();
}

