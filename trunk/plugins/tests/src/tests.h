/*  Elise IM, free crossplatform IM client
	Copyright (C) 2012  Elise project

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ELISE_PLUGINS_TESTPLUG_TESTPLUG_H_
#define ELISE_PLUGINS_TESTPLUG_TESTPLUG_H_

#include <QtWidgets/QtWidgets>
#include "../../../api/e_pluginapi.h"

class TestPlugin : public QObject, IPlugin
 {
	Q_OBJECT
	Q_INTERFACES(IPlugin)
	Q_PLUGIN_METADATA(IID "elise.IPlugin")
	static QSet<QUuid>*		interfaces;
public:
	const PluginInfo*		ElisePluginInfo();
	const QSet<QUuid>*		ElisePluginInterfaces(void);
	int						Load(ICore* coreAPI);
	int						Unload(void);
};


#endif // ELISE_PLUGINS_TESTPLUG_TESTPLUG_H_
