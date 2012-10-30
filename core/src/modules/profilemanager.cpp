#include "../commonheaders.h"

ProfileManager::ProfileManager(QMap<QString, IDBPlugin*>* availableDBPlugins)
{
	//-- Check it before call this function
	/*if (dbPlugins == 0 || dbPlugins->count() ==0) {
		QMessageBox::critical(0, "Error",
							  "No one db plugin was found.",
							  QMessageBox::Cancel);
	}*/
	DBPlugins = availableDBPlugins;
	profiles = 0;
	this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);

	this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

	QHBoxLayout* hboxMain = new QHBoxLayout(this);

	//-- Right staff
	QGroupBox* groupDB = new QGroupBox("DB plugins", this);
	QVBoxLayout* vboxDB = new QVBoxLayout(this);
	groupDB->setLayout(vboxDB);

	//-- Left staff
	QGroupBox* groupACC = new QGroupBox("Profile", this);
	QVBoxLayout* vboxACC = new QVBoxLayout(this);
	groupACC->setLayout(vboxACC);

	hboxMain->addWidget(groupDB);
	hboxMain->addWidget(groupACC);

	groupDB->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	hboxMain->setAlignment(groupDB, Qt::AlignTop);

	//-- Profile
	cmbProfiles = new QComboBox(this);
	cmbProfiles->setMinimumSize(120, 18);
	cmbProfiles->setEditable(true);
	vboxACC->addWidget(cmbProfiles);
	connect(cmbProfiles, SIGNAL(currentIndexChanged(const QString&)),
			this, SLOT(loadProfileDetails(const QString&)));
	/*connect(cmbProfiles,
			static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
			this,
			&ProfileManager::loadProfileDetails);*/

	//-- Password
	lePassword = new QLineEdit(this);
	lePassword->setPlaceholderText("password");
	lePassword->setEchoMode(QLineEdit::Password);
	vboxACC->addWidget(lePassword);
	//connect(lePassword, SIGNAL(returnPressed()), this, SLOT(ok()));
	connect (lePassword, &QLineEdit::returnPressed, this, &ProfileManager::ok);

	//-- Checkboxes, don't ask password on login
	cbSavePassword = new QCheckBox(this);
	cbSavePassword->setText("Save password");
	vboxACC->addWidget(cbSavePassword);
	connect(cbSavePassword, SIGNAL(stateChanged(int)), this, SLOT(setCBEnable(int)));
	//connect(cbSavePassword, &QCheckBox::stateChanged, this, &ProfileManager::setCBEnable);

	//-- Use as default profile
	cbDefaultProfile = new QCheckBox(this);
	cbDefaultProfile->setText("Default profile");
	cbDefaultProfile->setEnabled(false);
	vboxACC->addWidget(cbDefaultProfile);

	//-- OK, load profile
	QPushButton* button = new QPushButton(this);
	button->setText("OK");
	//connect(button, SIGNAL(clicked()), this, SLOT(ok()));
	connect(button, &QPushButton::clicked, this, &ProfileManager::ok);
	vboxACC->addWidget(button);

	//-- Creating list of dbPlugins
	cmbDBPlugins = new QComboBox(this);
	cmbDBPlugins->setMinimumSize(120, 18);
	vboxDB->addWidget(cmbDBPlugins);
	QMapIterator<QString, IDBPlugin*> iter(*DBPlugins);
	while (iter.hasNext()) {
		iter.next();
		cmbDBPlugins->addItem(iter.key());
	}
	connect(cmbDBPlugins, SIGNAL(currentIndexChanged(const QString&)),
			this, SLOT(loadProfiles(const QString&)));
	//connect(cmbDBPlugins, &QComboBox::currentIndexChanged, this, &ProfileManager::loadProfiles);

	//-- Create profile button
	button = new QPushButton(this);
	button->setText("Create");
	//connect(button, SIGNAL(clicked()), this, SLOT(createProfile()));
	connect(button, &QPushButton::clicked, this, &ProfileManager::createProfile);
	vboxDB->addWidget(button);

	//-- Import profile button
	button = new QPushButton(this);
	button->setText("Import");
	//connect(button, SIGNAL(clicked()), this, SLOT(importProfile()));
	connect(button, &QPushButton::clicked, this, &ProfileManager::importProfile);
	vboxDB->addWidget(button);
	vboxDB->setAlignment(button, Qt::AlignTop);

	//-- Cancel
	button = new QPushButton(this);
	button->setText("Cancel");
	//connect(button, SIGNAL(clicked()), this, SLOT(abort()));
	connect(button, &QPushButton::clicked, this, &ProfileManager::abort);
	button->resize(120, 23);
	button->move(21, 132);

	//cmbDBPlugins->setFocus();
	lePassword->setFocus();

	hboxMain->setSizeConstraint(QLayout::SetFixedSize);

	//-- First time initialize cmbAccount programmatically
	loadProfiles(cmbDBPlugins->currentText());
	loadProfileDetails(cmbProfiles->currentText());
}

ProfileManager::~ProfileManager()
{

}

void ProfileManager::setCBEnable(int state)
{
	if (!state) {
		cbDefaultProfile->setEnabled(false);
		cbDefaultProfile->setCheckState(Qt::Unchecked);
	}
	else
		cbDefaultProfile->setEnabled(true);
}

void ProfileManager::loadProfiles(const QString& text)
{
	cmbProfiles->clear();
	//-- Destroy old list if exists
	if (profiles != 0) {
		QMapIterator<QString, PROFILE*> iter(*profiles);
		while (iter.hasNext()) {
			iter.next();
			delete iter.value();
		}
		profiles->~QMap();
	}
	//-- Getting list of profiles from plugin
	profiles = DBPlugins->value(text)->GetProfiles();
	if (profiles != 0) {
		if (profiles->count() != 0) {
			QMapIterator<QString, PROFILE*> iter(*profiles);
			while (iter.hasNext()) {
				iter.next();
				cmbProfiles->addItem(iter.key());
				if (iter.value()->defaultProfile)
					qsDefaultProfile = iter.key();
			}
		}
		else
			//-- No one profile was found
			cmbProfiles->addItem("Create profile first");
	}
	else
		cmbProfiles->addItem("Internal plugin error");
}

void ProfileManager::loadProfileDetails(const QString& name)
{
	PROFILE* p = profiles->value(name);
	if ( p != 0) {
		int tmp = 0;
		if (p->savePassword) {
			tmp = 2;
			QMessageBox::critical(0, QStringLiteral("Debug"),
								  QStringLiteral("2"),
								  QMessageBox::Cancel);
		}
		cbSavePassword->setCheckState((Qt::CheckState)tmp);
		tmp = 0;
		if (p->defaultProfile)
			tmp = 2;
		cbDefaultProfile->setCheckState((Qt::CheckState)tmp);
		if (p->savePassword)
			lePassword->setText(p->password);
		else
			lePassword->setText("");
	}
}

int ProfileManager::loadDefaultProfile()
{
	if (qsDefaultProfile.isEmpty())
		//-- No default acc found
		return 1;

	cmbProfiles->setCurrentIndex(cmbProfiles->findText(qsDefaultProfile));
	loadProfileDetails(qsDefaultProfile);

	IDBPlugin* dbPlugin = DBPlugins->value(cmbDBPlugins->currentText());
	//-- Try to login
	if (dbPlugin->Login(cmbProfiles->currentText(), lePassword->text(),
			cbSavePassword->isChecked(), cbDefaultProfile->isChecked()))
	{
		QMessageBox::critical(0, "Login error",
					"Failed to default login.\nAccount does not exist or password does not match.",
					QMessageBox::Cancel);
		return 1;
	}
	//-- Destroy list if exists
	if (profiles != 0) {
		QMapIterator<QString, PROFILE*> iter(*profiles);
		while (iter.hasNext()) {
			iter.next();
			delete iter.value();
		}
		profiles->~QMap();
	}

	return PluginLoader::loadDBPlugin(cmbDBPlugins->currentText(), dbPlugin);
}

void ProfileManager::abort()
{
	//-- Destroy profiles list if exists
	if (profiles != 0) {
		QMapIterator<QString, PROFILE*> iter(*profiles);
		PROFILE* p;
		while (iter.hasNext()) {
			iter.next();
			p = iter.value();
			delete p;
		}
		profiles->~QMap();
	}

	//-- Abort Elise loading and exit
	done(1);
}

void ProfileManager::ok()
{
	IDBPlugin* dbPlugin = DBPlugins->value(cmbDBPlugins->currentText());
	//-- Try to login
	if (dbPlugin->Login(cmbProfiles->currentText(), lePassword->text(),
			cbSavePassword->isChecked(), cbDefaultProfile->isChecked()))
	{
		QMessageBox::critical(0, "Login error",
							  "Failed to login.\nProfile does not exist or password does not match.",
							  QMessageBox::Cancel);
		return;
	}
	//-- if success then try fully load the plugin
	//-- Destroy profiles list if exists
	if (profiles != 0) {
		QMapIterator<QString, PROFILE*> iter(*profiles);
		while (iter.hasNext()) {
			iter.next();
			delete iter.value();
		}
		profiles->~QMap();
	}
	done(PluginLoader::loadDBPlugin(cmbDBPlugins->currentText(), dbPlugin));
}

void ProfileManager::createProfile()
{
	IDBPlugin* dbPlugin = DBPlugins->value(cmbDBPlugins->currentText());
	if (dbPlugin->CreateProfile(cmbProfiles->currentText(), lePassword->text()))
		QMessageBox::critical(0, "Create profile error",
				"Failed to create profile.\nProfile already exist or another internal plugin error.",
				QMessageBox::Cancel);
	else {
		QMessageBox::information(0, "Information",
								 "Profile created successfully.",
								 QMessageBox::Ok);
		loadProfiles(cmbDBPlugins->currentText());
	}
}

void ProfileManager::importProfile()
{
	QMessageBox::critical(0, "Error",
						  "Sorry, this function is not supported yet.",
						  QMessageBox::Cancel);
}