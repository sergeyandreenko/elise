TEMPLATE	=	subdirs
CONFIG	   +=	ordered
SUBDIRS		=	core \
				plugins/newplugin \
				plugins/dbplugin \
				plugins/tests \
				depends/qxmpp


core.depends = qxmpp
