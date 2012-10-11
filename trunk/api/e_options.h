#ifndef E_OPTIONS_H
#define E_OPTIONS_H

const QString	OPTIONS_SHOW		=	"Options/Show";
const QString	OPTIONS_ADD_PAGE	=	"Options/AddPage";

/*
QWidget* page size is 478x448;
*/

typedef struct
{
	QString		id;
	QString		parentId;
	QString		title;
	QWidget*	page;

} OPTIONSPAGE;


#endif // E_OPTIONS_H
