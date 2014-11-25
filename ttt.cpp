#include "ttt.h"
#include "QStreamThreaded.h"
#include <QtWidgets>
#include <QStackedWidget>
#include <QLineEdit>
#include <QMenu>
#include <QMenuBar>
struct User {
	QString name;
};

User currentUser;

MainWindow::MainWindow(QWidget *parent):QWidget(parent)
{
	Login *myLogin = new Login;
	myWelcome = new Welcome;
	stackedWidget = new QStackedWidget;
	connect(myLogin, SIGNAL(exitClicked()), this, SLOT(exitMain()));
	connect(myLogin, SIGNAL(loginSuccessful()), this, SLOT(switchToWelcome()));
	stackedWidget->addWidget(myLogin);
	stackedWidget->addWidget(myWelcome);
	stackedWidget->setCurrentWidget(myLogin);
	QGridLayout *layout = new QGridLayout;
	layout->addWidget(stackedWidget);

	QMenuBar *menuBar = new QMenuBar(this);
	QMenu *userMenu = new QMenu(tr("&User"), this);
	QMenu *gameMenu = new QMenu(tr("&Game"), this);
	QMenu* editMenu = new QMenu(tr("&Edit"), this);

	QAction *registerAction = userMenu->addAction(tr("Register User"));
	QAction *logoutAction = userMenu->addAction(tr("Logout User"));
	//QAction *exitAction = userMenu->addAction(tr("E&xit"));
	menuBar->addMenu(userMenu);
	menuBar->addMenu(gameMenu);
	menuBar->addMenu(editMenu);

	setLayout(layout);
}

void MainWindow::exitMain()
{
	QOUT("Exit" << endl);
	this->close();

}

void MainWindow::switchToWelcome()
{
	myWelcome->setText(currentUser.name);
	stackedWidget->setCurrentIndex(1);
	//setLayout(myWelcome->welcomeLayout);
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
	if(username->text() == "admin" && password->text() == "password")
	{
		emit loginSuccessful();
	}
}
/***********************************Register******************************************/

Register::Register(QWidget *parent):QWidget(parent)
{
	
}

/**********************************Welcome*********************************************/
Welcome::Welcome(QWidget *parent):QWidget(parent)
{
	QString labelString;
	//currentUser.name = "Luke";
	QOUT(currentUser.name << endl);
	labelString = "Welcome " + currentUser.name;
	myLabel = new QLabel(labelString);
	welcomeLayout = new QGridLayout;
	welcomeLayout->addWidget(myLabel,0,0,1,1);
	setLayout(welcomeLayout);
	
}

void Welcome::setText(QString value)
{
	myLabel->setText("Welcome, " + value);
}

/*******************************Change Password****************************************/

ChangePassword::ChangePassword(QWidget *parent):QWidget(parent)
{
	
}

Game::Game(QWidget *parent):QWidget(parent)
{
	
}