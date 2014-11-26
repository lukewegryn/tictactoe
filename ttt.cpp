#include "ttt.h"
#include "QStreamThreaded.h"
#include <QtWidgets>
#include <QStackedWidget>
#include <QLineEdit>
#include <QMenu>
#include <QMenuBar>
#include <QFile>

struct User{
	QString name;
};
User currentUser;
Database *myData = new Database;

/**************************MainWindow**************************************/

MainWindow::MainWindow(QWidget *parent):QWidget(parent)
{
	extractFromDataStream();
	/*myData->colors.append(QString("Green"));
	myData->passwords["admin"] = QString("password").toUtf8();*/
	QMenuBar *menuBar = new QMenuBar(this);
	QMenu *userMenu = new QMenu(tr("&User"), this);
	QMenu *gameMenu = new QMenu(tr("&Game"), this);
	QMenu* editMenu = new QMenu(tr("&Edit"), this);

	QAction *registerAction = userMenu->addAction(tr("Register User"));
	QAction *logoutAction = userMenu->addAction(tr("Logout User"));
	QAction *newGameAction = gameMenu->addAction(tr("New Game"));
	QAction *endGameAction = gameMenu->addAction(tr("End Game"));
	QAction *changePasswordAction = editMenu->addAction(tr("Change Password"));
	//QAction *exitAction = userMenu->addAction(tr("E&xit"));
	menuBar->addMenu(userMenu);
	menuBar->addMenu(gameMenu);
	menuBar->addMenu(editMenu);

	QGridLayout *layout = new QGridLayout;

	myLogin = new Login;
	myWelcome = new Welcome;
	ChangePassword *myChangePassword = new ChangePassword;
	Register *myRegister = new Register;

	stackedWidget = new QStackedWidget;

	connect(myLogin, SIGNAL(exitClicked()), this, SLOT(exitMain()));
	connect(myLogin, SIGNAL(loginSuccessful()), this, SLOT(switchToWelcome()));
	connect(myWelcome, SIGNAL(exitClicked()), this, SLOT(exitMain()));
	connect(myWelcome, SIGNAL(changePasswordClicked()), this, SLOT(switchToChangePassword()));
	connect(myChangePassword, SIGNAL(cancelClicked()), this, SLOT(switchToWelcome()));
	connect(registerAction, SIGNAL(triggered()), this, SLOT(switchToRegister()));
	connect(myRegister, SIGNAL(cancelClicked()), this, SLOT(switchToLogin()));
	connect(myRegister, SIGNAL(passwordChanged()), this, SLOT(switchToLogin()));
	connect(this, SIGNAL(clearLogin()), myLogin, SLOT(clearLogin()));
	connect(logoutAction, SIGNAL(triggered()), this, SLOT(switchToLogin()));
	connect(changePasswordAction, SIGNAL(triggered()), this, SLOT(switchToChangePassword()));
	connect(myRegister, SIGNAL(goWriteDatabaseToFile()), this, SLOT(writeDatabaseToFile()));
	stackedWidget->addWidget(myLogin);
	stackedWidget->addWidget(myWelcome);
	stackedWidget->addWidget(myChangePassword);
	stackedWidget->addWidget(myRegister);
	stackedWidget->setCurrentWidget(myLogin);

	layout->addWidget(stackedWidget);

	setLayout(layout);
}

void MainWindow::exitMain()
{
	QOUT("Exit" << endl);
	this->close();

}

void MainWindow::switchToLogin()
{
	emit clearLogin();
	stackedWidget->setCurrentIndex(0);
}

void MainWindow::switchToWelcome()
{
	myWelcome->setText(currentUser.name);
	stackedWidget->setCurrentIndex(1);
}

void MainWindow::switchToChangePassword()
{
	stackedWidget->setCurrentIndex(2);
}

void MainWindow::switchToRegister()
{
	stackedWidget->setCurrentIndex(3);
}

void MainWindow::insertToDataStream()
{
	QFile passFile("passwords.dat");
	passFile.open(QIODevice::WriteOnly);
	QDataStream dataStream(&passFile);

	dataStream.setVersion(QDataStream::Qt_4_6);
	dataStream << myData->colors;
	dataStream << myData->passwords;

	passFile.close();
}
void MainWindow::extractFromDataStream()
{
	QFile passFile("passwords.dat");
	passFile.open(QIODevice::ReadOnly);
	QDataStream dataStream(&passFile);

	dataStream.setVersion(QDataStream::Qt_4_6);
	dataStream >> myData->colors;
	dataStream >> myData->passwords;

	passFile.close();
}

void MainWindow::writeDatabaseToFile()
{
	insertToDataStream();
}
/***********************************Login******************************************/
Login::Login(QWidget *parent):QWidget(parent)
{
	QPushButton *loginButton = createButton(tr("Login"),SLOT(loginClicked()));
	QPushButton *exitButton = createButton(tr("Exit"), SIGNAL(exitClicked()));
	username = new QLineEdit;
	password = new QLineEdit;
	password->setEchoMode(QLineEdit::Password);
	loginLayout = new QGridLayout;
	loginLayout->addWidget(new QLabel("Username: "), 0, 0, 1, 1);
	loginLayout->addWidget(username, 0, 1, 1, 1);
	loginLayout->addWidget(new QLabel("Password: "), 1, 0, 1, 1);
	loginLayout->addWidget(password, 1, 1, 1, 1);
	loginLayout->addWidget(loginButton,2,0,1,1);
	loginLayout->addWidget(exitButton,2,1,1,1);
	setLayout(loginLayout);
}

QPushButton *Login::createButton(const QString &text, const char *member)
{
	QPushButton *button = new QPushButton(text);
	connect(button, SIGNAL(clicked()), this, member);
	return button;
}

void Login::loginClicked()
{
	currentUser.name = username->text();
	if(myData->passwords[username->text()] == password->text().toUtf8())
	{
		emit loginSuccessful();
	}
	else{
		QMessageBox msgBox;
		msgBox.setText("That is an incorrect username/password combination.");
		msgBox.exec();
		msgBox.show();
	}
}

void Login::clearLogin()
{
	username->setText("");
	password->setText("");
}
/***********************************Register******************************************/

Register::Register(QWidget *parent):QWidget(parent)
{
	QGridLayout *registerLayout = new QGridLayout;

	QPushButton *okButton = createButton(tr("OK"),SLOT(okClicked()));
	QPushButton *cancelButton = createButton(tr("Cancel"),SIGNAL(cancelClicked()));
	username = new QLineEdit;
	newPassword = new QLineEdit;
	newPasswordAgain = new QLineEdit;

	combo = new QComboBox;
	QListView *listView = new QListView(combo);
	combo->addItem("Red");
	combo->addItem("Blue");
	combo->addItem("Green");
	combo->addItem("Orange");
	combo->setView(listView);
	registerLayout->addWidget(new QLabel("Username: "),0,0,1,1);
	registerLayout->addWidget(username,0,1,1,1);
	registerLayout->addWidget(new QLabel("Password: "),1,0,1,1);
	registerLayout->addWidget(newPassword,1,1,1,1);
	registerLayout->addWidget(new QLabel("Password (again): "),2,0,1,1);
	registerLayout->addWidget(newPasswordAgain,2,1,1,1);
	registerLayout->addWidget(new QLabel("Color: "),3,0,1,1);
	registerLayout->addWidget(combo,3,1,1,1);
	registerLayout->addWidget(okButton, 4, 0, 1, 1);
	registerLayout->addWidget(cancelButton, 4, 1, 1, 1);
	setLayout(registerLayout);
}

QPushButton *Register::createButton(const QString &text, const char *member)
{
	QPushButton *button = new QPushButton(text);
	connect(button, SIGNAL(clicked()), this, member);
	return button;
}

void Register::okClicked()
{
	if(newPassword->text() != newPasswordAgain->text())
	{
		QMessageBox msgBox;
		msgBox.setText("The passwords do not match.");
		msgBox.exec();
		msgBox.show();
	}

	else{
		myData->colors.append(combo->currentText());
		myData->passwords[username->text()] = newPassword->text().toUtf8();
		emit goWriteDatabaseToFile();
		emit passwordChanged();
	}
}
/**********************************Welcome*********************************************/
Welcome::Welcome(QWidget *parent):QWidget(parent)
{
	QString labelString;
	labelString = "Welcome " + currentUser.name;
	myLabel = new QLabel(labelString);
	welcomeLayout = new QGridLayout;

	QPushButton *changePasswordButton = createButton(tr("Change Password"),SIGNAL(changePasswordClicked()));
	QPushButton *startGameButton = createButton(tr("Start Game"),SIGNAL(startGameClicked()));
	QPushButton *exitButton = createButton(tr("Exit"),SIGNAL(exitClicked()));

	welcomeLayout->addWidget(myLabel,0,2,1,3);
	welcomeLayout->addWidget(changePasswordButton, 1, 1, 1, 1);
	welcomeLayout->addWidget(startGameButton, 1, 2, 1, 1);
	welcomeLayout->addWidget(exitButton, 1, 3, 1, 1);
	setLayout(welcomeLayout);
	
}

void Welcome::setText(QString value)
{
	myLabel->setText("Welcome, " + value);
}

QPushButton *Welcome::createButton(const QString &text, const char *member)
{
	QPushButton *button = new QPushButton(text);
	connect(button, SIGNAL(clicked()), this, member);
	return button;
}

/*******************************Change Password****************************************/

ChangePassword::ChangePassword(QWidget *parent):QWidget(parent)
{
	QGridLayout *changePasswordLayout = new QGridLayout;

	QPushButton *okButton = createButton(tr("OK"),SLOT(okClicked()));
	QPushButton *cancelButton = createButton(tr("Cancel"),SIGNAL(cancelClicked()));
	QLineEdit *oldPassword = new QLineEdit;
	QLineEdit *newPassword = new QLineEdit;
	QLineEdit *newPasswordAgain = new QLineEdit;

	QComboBox *combo = new QComboBox;
	QListView *listView = new QListView(combo);
	combo->addItem("Red");
	combo->addItem("Blue");
	combo->addItem("Green");
	combo->addItem("Orange");
	combo->setView(listView);
	changePasswordLayout->addWidget(new QLabel("Password (old): "),0,0,1,1);
	changePasswordLayout->addWidget(oldPassword,0,1,1,1);
	changePasswordLayout->addWidget(new QLabel("Password: "),1,0,1,1);
	changePasswordLayout->addWidget(newPassword,1,1,1,1);
	changePasswordLayout->addWidget(new QLabel("Password (again): "),2,0,1,1);
	changePasswordLayout->addWidget(newPasswordAgain,2,1,1,1);
	changePasswordLayout->addWidget(new QLabel("Color: "),3,0,1,1);
	changePasswordLayout->addWidget(combo,3,1,1,1);
	changePasswordLayout->addWidget(okButton, 4, 0, 1, 1);
	changePasswordLayout->addWidget(cancelButton, 4, 1, 1, 1);
	setLayout(changePasswordLayout);
}

void ChangePassword::okClicked()
{
	emit passwordChanged();
}

QPushButton *ChangePassword::createButton(const QString &text, const char *member)
{
	QPushButton *button = new QPushButton(text);
	connect(button, SIGNAL(clicked()), this, member);
	return button;
}

/**********************************Game***********************************************/

Game::Game(QWidget *parent):QWidget(parent)
{
	
}