#ifndef ELISE_CORE_MODULES_PLUGINLOADER_PLUGINLOADER_H_
#define ELISE_CORE_MODULES_PLUGINLOADER_PLUGINLOADER_H_

#include <QtWidgets/QtWidgets>

class IPlugin;
class IDBPlugin;
class CoreAPI;

/*typedef struct {
	QString name;
	IDBPlugin* plugin;
} LoadedDBPlugin;*/

typedef struct {
	bool		loaded;
	IPlugin*	pluginInterface;
} Plugin;

class PluginLoader
{
private:
	static CoreAPI					coreAPI;
	static QMap<QString, Plugin>*	plugins;
	static QMap<QUuid, QString>*	interfaces;
	//static LoadedDBPlugin			loadedDBPlugin;
	static bool			isLoadingPluginDisabled(const QString& pluginModuleName);
public:
	static QDir			getPluginsDir();
	static int			LoadPluginLoader();
	static int			loadDBPlugin(const QString& pluginModuleName);
	static int			loadPlugins();
	static int			loadPlugin();
	static int			unloadPlugin();
	static int			unloadAllPlugins();
	static bool			isPluginLoaded(const QString& pluginModuleName) const;
	static bool			isPluginLoadable(const QString& pluginModuleName) const;
	static const QMap<QString, Plugin>*	getAvailablePlugins();
	static const QMap<QString, IDBPlugin*>*	getDBPlugins();
};

#endif // ELISE_CORE_MODULES_PLUGINLOADER_PLUGINLOADER_H_
