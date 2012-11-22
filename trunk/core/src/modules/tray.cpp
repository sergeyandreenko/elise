#include "tray.h"
#include "../services.h"


const QLatin1String	TRAY_ADD_MENUITEM	=	QLatin1String("Tray/AddMenuItem");
const QLatin1String	TRAY_SET_ICON		=	QLatin1String("Tray/SetIcon");
const QLatin1String	TRAY_SINGLECLICK	=	QLatin1String("Tray/SingleClick");
const QLatin1String	TRAY_DOUBLECLICK	=	QLatin1String("Tray/DoubleClick");
const QLatin1String	TRAY_MIDDLECLICK	=	QLatin1String("Tray/MiddleClick");

EliseTray* trayElise;

void EliseTray::trayActivationNotify(QSystemTrayIcon::ActivationReason reason)
{
	switch (reason)
	{
		case QSystemTrayIcon::Trigger:
		{
			core::NotifyEventHooks(&TRAY_SINGLECLICK, 0, 0);
			break;
		}
		case QSystemTrayIcon::DoubleClick:
		{
			core::NotifyEventHooks(&TRAY_DOUBLECLICK, 0, 0);
			break;
		}
		case QSystemTrayIcon::MiddleClick:
		{
			core::NotifyEventHooks(&TRAY_MIDDLECLICK, 0, 0);
			break;
		}
		default:
			break;
	} //switch
	//return 0;
}

int addToContextMenu(intptr_t wParam,intptr_t)
{
	QAction* action = reinterpret_cast<QAction*>(wParam);
	trayElise->addToMenu(action);
	return 0;
}

int setTrayIcon(intptr_t wParam,intptr_t)
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
	core::CreateHookableEvent(&TRAY_SINGLECLICK);
	core::CreateHookableEvent(&TRAY_DOUBLECLICK);
	core::CreateHookableEvent(&TRAY_MIDDLECLICK);
	//QObject::connect(trayElise, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
	//				 trayElise, SLOT(trayActivationNotify(QSystemTrayIcon::ActivationReason)));
	QObject::connect(trayElise, &EliseTray::activated, trayElise, &EliseTray::trayActivationNotify);
	trayElise->setContextMenu(new QMenu());
	core::CreateServiceFunction(&TRAY_ADD_MENUITEM, &addToContextMenu);
	core::CreateServiceFunction(&TRAY_SET_ICON, &setTrayIcon);
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