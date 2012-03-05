
#include "commonheaders.h"

const PLUGINLINK* pluginLink;

PLUGININFO pluginInfo = {
	{0,0,0,1},
	"{ca0ae4d0-ea7c-4743-b34e-1a2c9c61991d}"
};


PLUGININFO* DBPlugin::ElisePluginInfo(EVersion)
{
	return &pluginInfo;
}

const QUuid* DBPlugin::ElisePluginInterfaces(void)
{
	return &pluginInfo.uuid;
}


int DBPlugin::Load(const PLUGINLINK* link)
{
	pluginLink = link;

	//-- Switch to profiles directory
	QDir curDir = QDir(qApp->applicationDirPath());
	if (!curDir.exists("Profiles"))
		curDir.mkdir("Profiles");
	curDir.cd("Profiles");
	QDir::setCurrent(curDir.path());

	if (!QFile::exists(elisesys)) {
		if (EliseDB::createSysDB())
			return -1;
		if (EliseDB::createNewAcc())
				return -1;
	}
	else {
		EliseDB::selectAcc();
		EliseDB::loadProfile();
	}


	//-- Switch backward to main directory
	QDir::current().cdUp();

	return 0;
}

int DBPlugin::Unload(void)
{
	return 0;
}

Q_EXPORT_PLUGIN2(dbplugin, DBPlugin)
