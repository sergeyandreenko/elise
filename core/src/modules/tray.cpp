#include "tray.h"
#include "../services.h"
//#include "../commonheaders.h"

EliseTray* trayElise;

void EliseTray::trayActivationNotify(QSystemTrayIcon::ActivationReason reason)
{
	switch (reason)
	{
		case QSystemTrayIcon::Trigger:
		{
			core::NotifyEventHooks(&Core::TRAY_SINGLECLICK, 0, 0);
			break;
		}
		case QSystemTrayIcon::DoubleClick:
		{
			core::NotifyEventHooks(&Core::TRAY_DOUBLECLICK, 0, 0);
			break;
		}
		case QSystemTrayIcon::MiddleClick:
		{
			core::NotifyEventHooks(&Core::TRAY_MIDDLECLICK, 0, 0);
			break;
		}
		default:
			break;
	} //switch
	//return 0;
}

int addToContextMenu(uintptr_t wParam,uintptr_t lParam)
{
	QAction* action = (QAction*)wParam;
	trayElise->addToMenu(action);
	return 0;
}

int setTrayIcon(uintptr_t wParam,uintptr_t lParam)
{
	QIcon* icon = (QIcon*)wParam;
	//QIcon icon2 = *icon;
	trayElise->setIcon(*icon);
	return 0;
}

int EliseTray::addToMenu(QAction* action)
{
	contextMenu()->addAction(action);
	return 0;
}

int LoadTrayModule()
{
	trayElise = new EliseTray();
	trayElise->setIcon(QIcon(":/icons/img/main.png"));
	trayElise->show();
	core::CreateHookableEvent(&core::TRAY_SINGLECLICK);
	core::CreateHookableEvent(&core::TRAY_DOUBLECLICK);
	core::CreateHookableEvent(&core::TRAY_MIDDLECLICK);
	QObject::connect(trayElise, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
					 trayElise, SLOT(trayActivationNotify(QSystemTrayIcon::ActivationReason)));
	//QObject::connect(trayElise, &EliseTray::activated, trayElise, &EliseTray::trayActivationNotify);
	trayElise->setContextMenu(new QMenu());
	core::CreateServiceFunction(&core::TRAY_ADD_MENUITEM, (ELISESERVICE)addToContextMenu);
	core::CreateServiceFunction(&core::TRAY_SET_ICON, (ELISESERVICE)setTrayIcon);
	return 0;
}

int UnloadTrayModule()
{
	trayElise->hide();
	trayElise->~EliseTray();
	return 0;
}

EliseTray::EliseTray()
{

}

EliseTray::~EliseTray()
{

}
