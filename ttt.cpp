#include "ttt.h"
#include "QStreamThreaded.h"
#include <QtWidgets>
#include <QStackedWidget>

MainWindow::MainWindow(QWidget *parent):QWidget(parent)
{
	Login *myLogin = new Login;
	Welcome *myWelcome = new Welcome;
	stackedWidget = new QStackedWidget;
	connect(myLogin, SIGNAL(exitClicked()), this, SLOT(exitMain()));
	connect(myLogin, SIGNAL(loginClicked()), this, SLOT(switchToWelcome()));
	stackedWidget->addWidget(myLogin);
	stackedWidget->addWidget(myWelcome);
	stackedWidget->setCurrentWidget(myLogin);
	QGridLayout *layout = new QGridLayout;
	layout->addWidget(stackedWidget);
	setLayout(layout);
}

void MainWindow::exitMain()
{
	QOUT("Exit" << endl);
	this->close();

}

void MainWindow::switchToWelcome()
{
	stackedWidget->setCurrentIndex(1);
	//setLayout(myWelcome->welcomeLayout);
}
/***********************************Login******************************************/
Login::Login(QWidget *parent):QWidget(parent)
{
	QPushButton *loginButton = createButton(tr("Login"),SIGNAL(loginClicked()));
	QPushButton *exitButton = createButton(tr("Exit"), SIGNAL(exitClicked()));

	loginLayout = new QGridLayout;
	loginLayout->addWidget(loginButton,0,0,1,1);
	loginLayout->addWidget(exitButton,0,1,1,1);
	setLayout(loginLayout);
}

QPushButton *Login::createButton(const QString &text, const char *member)
{
	QPushButton *button = new QPushButton(text);
	connect(button, SIGNAL(clicked()), this, member);
	return button;
}

/***********************************Register******************************************/

Register::Register(QWidget *parent):QWidget(parent)
{
	
}

Welcome::Welcome(QWidget *parent):QWidget(parent)
{
	QLabel *myLabel = new QLabel("Welcome");
	welcomeLayout = new QGridLayout;
	welcomeLayout->addWidget(myLabel,0,0,1,1);
	setLayout(welcomeLayout);
	
}

ChangePassword::ChangePassword(QWidget *parent):QWidget(parent)
{
	
}

Game::Game(QWidget *parent):QWidget(parent)
{
	
}