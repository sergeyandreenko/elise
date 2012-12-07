#ifndef ELISE_CORE_CORE_H_
#define ELISE_CORE_CORE_H_

#include <stdint.h>
#include "../../api/e_pluginapi.h"

class QLatin1String;
class QMutex;

//-- Services
extern const QLatin1String	kShutdown_service;		//"System/Shutdown"
extern const QLatin1String	kChangeProfile_service;	//"System/ChangeAcc"
extern const QLatin1String	kDBWriteSetting_service;//"DB/WriteSetting"
extern const QLatin1String	kDBReadSetting_service;	//"DB/ReadSetting"
extern const QLatin1String	kDBDellSetting_service;	//"DB/DeleteSetting"

//-- Hookable events
//const QLatin1String	TRAY_SINGLECLICK	=	QLatin1String("Tray/SingleClick");

/*typedef struct
{
	int num;
	int type;
	struct {
		union {
			EliseHook pfnHook;
		};
	};

} THookSubscriber;

typedef struct
{
	int  subscriberCount;
	QMap<int, THookSubscriber*>* qmapSubscribers;
	QMutex* qmutexHook;
} THookEvent;

typedef struct
{
	int type;
	union {
		EliseService pfnService;
	};
} TService;*/


class Core : public ICore
{
private:
	//-- Critical sections
	QMutex qmutexHooks;
	QMutex qmutexServices;

	//-- Arrays of hookable events and services
	QMap <QLatin1String, THookEvent*> qmapHooks;
	QMap <QLatin1String, TService*> qmapServices;

public:

	static ICore* interface;

	Core() { interface = this; }
	~Core() { interface = 0; }

	//-- Basic core functions
	inline static intptr_t	changeProfile(intptr_t, intptr_t)
	{
		if (loadProfile(false))
			return shutDown(-1, 0);

		return 0;
	}
	static int		loadCore();
	static int		loadProfile(bool);
	//-- 'true' means that this is launch of the application
	static int		shutDown(intptr_t result, intptr_t);

	//-- Modules --/////////////////////////////////////////////////////////////////////////////////
	static int	loadDefaultModules();
	static int	unloadDefaultModules();


	//-- Hook functions --//////////////////////////////////////////////////////////////////////////

/* CreateHookableEvent
 * Adds an named event to the list. The event will be automatically destroyed on exit, or can be
 * removed from the list earlier using
 *		DestroyHookableEvent();
 * Will fail if the given name has already been used.
 * Return 0 on success, -2 if the name is empty and -1 if the name has been already used.
 */
	int createHookableEvent(const QLatin1String* name);

/* DestroyHookableEvent
 * Removes the event 'name' from the list of events. All modules hooked to it are automatically
 * unhooked.
 *	  NotifyEventHooks(...);
 * will fail if called with this name again.
 * Return 0 on success, -2 if the name is empty and -1 if the 'name' not found in events list.
 */
	int destroyHookableEvent(const QLatin1String* name);

/* NotifyEventHooks
 * Calls every module in turn that has hooked 'name', using the parameters wParam and lParam.
 * If one of the hooks returned non-zero to indicate abort, returns that abort value immediately,
 * without calling the rest of the hooks in the chain.
 * Returns 0 on success, -2 if name is invalid, any non-zero value that indicates break of one
 * of called hooks.
 */
	int notifyEventHooks(const QLatin1String* name, uintptr_t wParam, uintptr_t lParam );

/* HookEvent
 * Adds a new hook to the chain 'name', to be called when the hook owner calls
 *	  NotifyEventHooks(...);
 * All hooks will be automatically destroyed when (their parent event is destroyed or)
 * the programm ends, but can be unhooked earlier using
 *	  UnhookEvent(..);
 * wParam and lParam in hookProc() are defined by the creator of the event when NotifyEventHooks()
 * is called. The return value is 0 to continue processing the other hooks, or non-zero to stop
 * immediately. This abort value is returned to the caller of NotifyEventHooks() and
 * should not be -1 since that is a special return code for NotifyEventHooks() (see above).
 * Returns -2 if name is empty, -1 if name not found in events list. If the hook created
 * successfully, returns its personal number that must be used by call UnhookEvent().
 */
	int hookEvent(const QLatin1String* name, EliseHook hookProc);

/* UnhookEvent
 * Removes a hook from its event chain. It will no longer receive any events.
 * hook.num in THook is personal number of destroying hook that was returned by HookEvent().
 * hook.name in THook is name of chain of events from which the remove is.
 * Returns 0 on success, -2 if name is empty, -1 if name is not found in the list of events and -3
 * if there is no hooks in chain 'name' or hook.num is wrong.
 */
	int unhookEvent(const THook hook);

	//-- Service functions --///////////////////////////////////////////////////////////////////////

/* CreateServiceFunction
 * Adds a new service function called 'name' to the global list. Service function pointers are
 * destroyed automatically on exit, but can be removed from the list earlier using
 *		DestroyServiceFunction()
 * serviceProc is defined by the caller as
 *		int ServiceProc(uintptr_t wParam, uintptr_t lParam)
 * where the creator publishes the meanings of wParam, lParam and the return value.
 * Service functions must not return SERVICE_NOTFOUND since that would confuse
 * callers of CallService().
 * Returns 0 on success, -2 if name is empty and -1 if name has been already used.
 */
	int createServiceFunction(const QLatin1String* name, EliseService serviceProc);

/* ServiceExists
 * Finds if a service with the given 'name' exists.
 * Returns non-zero if the service was found, and zero if it was not. Returns -2 if name is
 * empty.
 */
	int serviceExists(const QLatin1String* name);

/* CallService
 * Finds and calls the service function name using the parameters wParam and
 * lParam.
 * Returns -1 if there is no service found otherwise returns the value of the service
 * function.
 */
	intptr_t callService(const QLatin1String* name, uintptr_t wParam, uintptr_t lParam);

/* DestroyServiceFunction
 * Removes the function associated with name from the global service function
 * list. Modules calling CallService() will fail if they try to call this
 * service's name.
 * Returns 0 on success, -2 if name is empty and -1 if name not found in services list.
 */
	int destroyServiceFunction(const QLatin1String* name);
};

#endif // ELISE_CORE_CORE_H_
