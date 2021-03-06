/* Luke Wegryn
*  9057-39535
*  lwegryn Assignment 8
*
*  Extra Credit: I implemented an algorithm that will attempt to block the player whenever possible. If you attempt
*  				to play 2 X's in a row, it will put an O in the third spot. If there are two ways for you to make
*				3 in a row, it will attempt to block one of the paths. SINCE A GAME THAT ALWAYS ENDS IN A TIE IS BORING			
*				I DECIDED NOT TO MAKE IT IMPOSSIBLE FOR THE PLAYER TO WIN.			
*				
*/
#include "ttt.h"
#include "QStreamThreaded.h"
#include <QtWidgets>
#include <QStackedWidget>
#include <QLineEdit>
#include <QMenu>
#include <QMenuBar>
#include <QCryptographicHash>
#include <QFile>
struct User{
	QString name;
	QString color;
};
User currentUser;
Database *myData = new Database;

/**************************MainWindow**************************************/

MainWindow::MainWindow(QWidget *parent):QWidget(parent)
{
	//setFixedSize(500, 500);
	extractFromDataStream();
	/*myData->colors.append(QString("Green"));
	myData->passwords["admin"] = QString("password").toUtf8();*/
	QMenuBar *menuBar = new QMenuBar(this);
	QMenu *userMenu = new QMenu(tr("&User"), this);
	QMenu *gameMenu = new QMenu(tr("&Game"), this);
	QMenu* editMenu = new QMenu(tr("&Edit"), this);

	registerAction = userMenu->addAction(tr("Register User"));
	logoutAction = userMenu->addAction(tr("Logout User"));
	exitAction = userMenu->addAction(tr("Exit"));
	newGameAction = gameMenu->addAction(tr("New Game"));
	endGameAction = gameMenu->addAction(tr("End Game"));
	changePasswordAction = editMenu->addAction(tr("Change Password"));
	//QAction *exitAction = userMenu->addAction(tr("E&xit"));
	menuBar->addMenu(userMenu);
	menuBar->addMenu(gameMenu);
	menuBar->addMenu(editMenu);

	QGridLayout *layout = new QGridLayout;

	myLogin = new Login;
	myWelcome = new Welcome;
	ChangePassword *myChangePassword = new ChangePassword;
	Register *myRegister = new Register;
	Game *myGame = new Game;

	stackedWidget = new QStackedWidget;

	//disable enable action buttons
	registerAction->setEnabled(true);
	logoutAction->setEnabled(false);
	exitAction->setEnabled(true);
	newGameAction->setEnabled(false);
	endGameAction->setEnabled(false);
	changePasswordAction->setEnabled(false);

	connect(myLogin, SIGNAL(exitClicked()), this, SLOT(exitMain()));
	connect(myLogin, SIGNAL(loginSuccessful()), this, SLOT(switchToWelcome()));
	connect(exitAction, SIGNAL(triggered()), this, SLOT(exitMain()));
	connect(myWelcome, SIGNAL(exitClicked()), this, SLOT(exitMain()));
	connect(myWelcome, SIGNAL(changePasswordClicked()), this, SLOT(switchToChangePassword()));
	connect(myChangePassword, SIGNAL(cancelClicked()), this, SLOT(switchToWelcome()));
	connect(myChangePassword, SIGNAL(goWriteDatabaseToFile()), this, SLOT(writeDatabaseToFile()));
	connect(myChangePassword, SIGNAL(passwordChanged()), this, SLOT(switchToLogin()));
	connect(registerAction, SIGNAL(triggered()), this, SLOT(switchToRegister()));
	connect(myRegister, SIGNAL(cancelClicked()), this, SLOT(switchToLogin()));
	connect(myRegister, SIGNAL(passwordChanged()), this, SLOT(switchToLogin()));
	connect(myWelcome, SIGNAL(startGameClicked()), this, SLOT(switchToGame()));
	connect(this, SIGNAL(clearLogin()), myLogin, SLOT(clearLogin()));
	connect(this, SIGNAL(clearRegister()), myRegister, SLOT(clear()));
	connect(this, SIGNAL(clearChangePassword()), myChangePassword, SLOT(clear()));
	connect(logoutAction, SIGNAL(triggered()), this, SLOT(switchToLogin()));
	connect(changePasswordAction, SIGNAL(triggered()), this, SLOT(switchToChangePassword()));
	connect(myRegister, SIGNAL(goWriteDatabaseToFile()), this, SLOT(writeDatabaseToFile()));
	connect(myGame, SIGNAL(endGameSwitchPanes()), this, SLOT(switchToWelcome()));
	connect(newGameAction, SIGNAL(triggered()), myGame, SLOT(newGame()));
	connect(newGameAction, SIGNAL(triggered()), this, SLOT(switchToGame()));
	connect(endGameAction, SIGNAL(triggered()), myGame, SLOT(endGame()));
	stackedWidget->addWidget(myLogin);
	stackedWidget->addWidget(myWelcome);
	stackedWidget->addWidget(myChangePassword);
	stackedWidget->addWidget(myRegister);
	stackedWidget->addWidget(myGame);
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
	currentUser.name = "";
	currentUser.color = "";
	emit clearLogin();
	emit clearRegister();
	stackedWidget->setCurrentIndex(0);
	registerAction->setEnabled(true);
	logoutAction->setEnabled(false);
	exitAction->setEnabled(true);
	newGameAction->setEnabled(false);
	endGameAction->setEnabled(false);
	changePasswordAction->setEnabled(false);

}

void MainWindow::switchToWelcome()
{
	if(currentUser.name != NULL)
	{
		emit clearLogin();
		emit clearChangePassword();
		myWelcome->setText(currentUser.name);
		stackedWidget->setCurrentIndex(1);

		registerAction->setEnabled(false);
		logoutAction->setEnabled(true);
		exitAction->setEnabled(true);
		newGameAction->setEnabled(true);
		endGameAction->setEnabled(false);
		changePasswordAction->setEnabled(true);
	}

}

void MainWindow::switchToChangePassword()
{
	if(currentUser.name != NULL)
	{
		stackedWidget->setCurrentIndex(2);
		registerAction->setEnabled(false);
		logoutAction->setEnabled(true);
		exitAction->setEnabled(true);
		newGameAction->setEnabled(false);
		endGameAction->setEnabled(false);
		changePasswordAction->setEnabled(false);
	}
}

void MainWindow::switchToRegister()
{
	stackedWidget->setCurrentIndex(3);
	registerAction->setEnabled(false);
	logoutAction->setEnabled(false);
	exitAction->setEnabled(true);
	newGameAction->setEnabled(false);
	endGameAction->setEnabled(false);
	changePasswordAction->setEnabled(false);
}

void MainWindow::switchToGame()
{
	if(currentUser.name != NULL)
	{
		stackedWidget->setCurrentIndex(4);

		registerAction->setEnabled(false);
		logoutAction->setEnabled(true);
		exitAction->setEnabled(true);
		newGameAction->setEnabled(false);
		endGameAction->setEnabled(true);
		changePasswordAction->setEnabled(false);
	}

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
	loginLayout->setColumnMinimumWidth(0, 150);
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
	if(myData->passwords[username->text()] == QCryptographicHash::hash(password->text().toUtf8(), QCryptographicHash::Sha1))
	{
		currentUser.name = username->text();
		currentUser.color = myData->colors[username->text()];
		emit loginSuccessful();
	}
	else{
		QMessageBox msgBox;
		msgBox.setText("The user/password combination does not match an existing user/password combination.");
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

	newPassword->setEchoMode(QLineEdit::Password);
	newPasswordAgain->setEchoMode(QLineEdit::Password);

	combo = new QComboBox;
	QListView *listView = new QListView(combo);
	combo->addItem("Red");
	combo->addItem("Blue");
	combo->addItem("Green");
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
		msgBox.setText("The password fields do not match eachother.");
		msgBox.exec();
		msgBox.show();
	}
	else if(myData->passwords.contains(username->text()))
	{
		QMessageBox msgBox;
		msgBox.setText("A user with that name already exists.");
		msgBox.exec();
		msgBox.show();
	}
	else{
		myData->colors[username->text()] = combo->currentText();
		myData->passwords[username->text()] = QCryptographicHash::hash(newPassword->text().toUtf8(), QCryptographicHash::Sha1);
		username->setText("");
		newPassword->setText("");
		newPasswordAgain->setText("");
		emit goWriteDatabaseToFile();
		emit passwordChanged();
	}
}

void Register::clear()
{
	username->setText("");
	newPassword->setText("");
	newPasswordAgain->setText("");
}
/**********************************Welcome*********************************************/
Welcome::Welcome(QWidget *parent):QWidget(parent)
{
	QString labelString;
	labelString = "Welcome " + currentUser.name;
	myLabel = new QLabel(labelString);
	welcomeLayout = new QGridLayout;

	QFont font = myLabel->font();
	font.setPointSize(12);
	font.setBold(true);
	myLabel->setFont(font);
	//myLabel->setAlignment(Qt::AlignRight);

	QPushButton *changePasswordButton = createButton(tr("Change Password"),SIGNAL(changePasswordClicked()));
	QPushButton *startGameButton = createButton(tr("Start Game"),SIGNAL(startGameClicked()));
	QPushButton *exitButton = createButton(tr("Exit"),SIGNAL(exitClicked()));
	//welcomeLayout->setColumnMinimumWidth(0, 50);
	//welcomeLayout->setColumnMinimumWidth(0, 200);
	//welcomeLayout->setColumnMinimumWidth(, 100);
	//welcomeLayout->setColumnMinimumWidth(2, 110);
	welcomeLayout->addWidget(myLabel,0,1,1,1);
	welcomeLayout->addWidget(changePasswordButton, 1, 0, 1, 1);
	welcomeLayout->addWidget(startGameButton, 1, 1, 1, 1);
	welcomeLayout->addWidget(exitButton, 1, 2, 1, 1);
	setLayout(welcomeLayout);
	
}

void Welcome::setText(QString value)
{
	myLabel->setText("Welcome, " + value + ".");
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
	oldPassword = new QLineEdit;
	newPassword = new QLineEdit;
	newPasswordAgain = new QLineEdit;

	oldPassword->setEchoMode(QLineEdit::Password);
	newPassword->setEchoMode(QLineEdit::Password);
	newPasswordAgain->setEchoMode(QLineEdit::Password);

	combo = new QComboBox;
	QListView *listView = new QListView(combo);
	combo->addItem("Red");
	combo->addItem("Blue");
	combo->addItem("Green");
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
	if(newPassword->text() != newPasswordAgain->text())
	{
		QMessageBox msgBox;
		msgBox.setText("The password fields do not match each other.");
		msgBox.exec();
		msgBox.show();
	}
	else if (QCryptographicHash::hash(oldPassword->text().toUtf8(), QCryptographicHash::Sha1) != myData->passwords[currentUser.name])
	{
		QMessageBox msgBox;
		msgBox.setText("The old password field does not match the current password field.");
		msgBox.exec();
		msgBox.show();
	}
	else{
		myData->colors[currentUser.name] = combo->currentText();
		myData->passwords[currentUser.name] = QCryptographicHash::hash(newPassword->text().toUtf8(), QCryptographicHash::Sha1);
		oldPassword->setText("");
		newPassword->setText("");
		newPasswordAgain->setText("");
		emit goWriteDatabaseToFile();
		emit passwordChanged();
	}
}

QPushButton *ChangePassword::createButton(const QString &text, const char *member)
{
	QPushButton *button = new QPushButton(text);
	connect(button, SIGNAL(clicked()), this, member);
	return button;
}

void ChangePassword::clear()
{
	oldPassword->setText("");
	newPassword->setText("");
	newPasswordAgain->setText("");
}

/**********************************Game***********************************************/

Game::Game(QWidget *parent):QWidget(parent)
{
	QGridLayout *gameLayout = new QGridLayout;
	QPushButton *endGameButton = new QPushButton("End Game");
	connect(endGameButton, SIGNAL(clicked()), this, SLOT(endGame()));
	signalMapper = new QSignalMapper(this);
	numberOfPlays = 0;
	QLabel *scoreLabel = new QLabel("Score");
	scoreLabel->setAlignment(Qt::AlignCenter);
	for(int i = 0; i < 9; i++)
	{
		squareStatus.append(0);
		boardList.append(createLabel(i));
	}

	for(int i = 0; i < 3; i++)
	{
		scores.append(0);
	}
	QString currentScoreString = "Computer " + QString::number(scores[2]) + "\nPlayer " + QString::number(scores[1]) + "\nDraw " + QString::number(scores[0]);
	currentScore = new QLabel(currentScoreString);
	currentScore->setAlignment(Qt::AlignCenter);
	connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(buttonClicked(int)));
	gameLayout->addWidget(boardList[0],0,0,1,1);
	gameLayout->addWidget(boardList[1],0,1,1,1);
	gameLayout->addWidget(boardList[2],0,2,1,1);
	gameLayout->addWidget(boardList[3],1,0,1,1);
	gameLayout->addWidget(boardList[4],1,1,1,1);
	gameLayout->addWidget(boardList[5],1,2,1,1);
	gameLayout->addWidget(boardList[6],2,0,1,1);
	gameLayout->addWidget(boardList[7],2,1,1,1);
	gameLayout->addWidget(boardList[8],2,2,1,1);
	gameLayout->addWidget(scoreLabel, 0,3,1,1);
	//gameLayout->addWidget(new QLabel("Computer"),0,3,1,1);
	gameLayout->addWidget(currentScore, 1,3,1,1);
	gameLayout->addWidget(endGameButton, 3,3,1,1);
	setLayout(gameLayout);
}

LabelClick* Game::createLabel(int i)
{
	QImage emptyImage;
	emptyImage.load("blank.png");
	LabelClick *squareLabel = new LabelClick;
	squareLabel->setMaximumSize(50,50);
	squareLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
	squareLabel->setPixmap(QPixmap::fromImage(emptyImage));
	signalMapper->setMapping(squareLabel,i);
	connect(squareLabel, SIGNAL(clicked()), signalMapper, SLOT(map()));
	return squareLabel;
}
void Game::endGame()
{
	for(int i = 0; i < 3; i++)
	{
		scores[i] = 0;
	}

	QImage emptyImage;
	emptyImage.load("blank.png");
	for(int i = 0; i < 9; i++)
	{
		squareStatus[i] = 0;
		boardList[i]->setPixmap(QPixmap::fromImage(emptyImage));
	}
	numberOfPlays = 0;
	QString currentScoreString = "Computer " + QString::number(scores[2]) + "\nPlayer " + QString::number(scores[1]) + "\nDraw " + QString::number(scores[0]);
	currentScore->setText(currentScoreString);
	emit endGameSwitchPanes();
}
void Game::newGame()
{
		for(int i = 0; i < 3; i++)
	{
		scores[i] = 0;
	}

	QImage emptyImage;
	emptyImage.load("blank.png");
	for(int i = 0; i < 9; i++)
	{
		squareStatus[i] = 0;
		boardList[i]->setPixmap(QPixmap::fromImage(emptyImage));
	}
	numberOfPlays = 0;
	QString currentScoreString = "Computer " + QString::number(scores[2]) + "\nPlayer " + QString::number(scores[1]) + "\nDraw " + QString::number(scores[0]);
	currentScore->setText(currentScoreString);
}

int Game::checkForWinner()
{	
	int winnerResult = 0; //return 0 if no winner, 1 if player wins, 2 if computer wins
	for(int i = 0; i < 3; i++)
	{
		if(squareStatus[i*3] == 1 && squareStatus[i*3+1] == 1 && squareStatus[i*3+2] == 1)
		{
			winnerResult = 1;
		}

		else if(squareStatus[i*3] == 2 && squareStatus[i*3+1] == 2 && squareStatus[i*3+2] == 2)
		{
			winnerResult = 2;
		}
	}

	if(squareStatus[0] == 1 && squareStatus[3] == 1 && squareStatus[6] == 1)
	{
		winnerResult = 1;
	}

	else if(squareStatus[1] == 1 && squareStatus[4] == 1 && squareStatus[7] == 1)
	{
		winnerResult = 1;
	}

	else if(squareStatus[2] == 1 && squareStatus[5] == 1 && squareStatus[8] == 1)
	{
		winnerResult = 1;
	}

	else if(squareStatus[0] == 2 && squareStatus[3] == 2 && squareStatus[6] == 2)
	{
		winnerResult = 2;
	}

	else if(squareStatus[1] == 2 && squareStatus[4] == 2 && squareStatus[7] == 2)
	{
		winnerResult = 2;
	}

	else if(squareStatus[2] == 2 && squareStatus[5] == 2 && squareStatus[8] == 2)
	{
		winnerResult = 2;
	}

	else if(squareStatus[0] == 1 && squareStatus[4] == 1 && squareStatus[8] == 1)
	{
		winnerResult = 1;
	}

	else if(squareStatus[2] == 1 && squareStatus[4] == 1 && squareStatus[6] == 1)
	{
		winnerResult = 1;
	}

	else if(squareStatus[0] == 2 && squareStatus[4] == 2 && squareStatus[8] == 2)
	{
		winnerResult = 2;
	}

	else if(squareStatus[2] == 2 && squareStatus[4] == 2 && squareStatus[6] == 2)
	{
		winnerResult = 2;
	}

	return winnerResult;
}

int Game::determineComputerMove()
{
	int nextMove = -1;
	//horizontals
	for(int i = 0; i < 3; i++)
	{
		if(squareStatus[i*3] == 1 && squareStatus[i*3+1] == 1 && squareStatus[i*3+2] == 0)
		{
			nextMove = i*3+2;
		}
	}

	if(squareStatus[1] == 1 && squareStatus[2] == 1 && squareStatus[0] == 0)
	{
		nextMove = 0;
	}

	else if(squareStatus[0] == 1 && squareStatus[2] == 1 && squareStatus[1] == 0)
	{
		nextMove = 1;
	}

	else if(squareStatus[4] == 1 && squareStatus[5] == 1 && squareStatus[3] == 0)
	{
		nextMove = 3;
	}

	else if(squareStatus[3] == 1 && squareStatus[5] == 1 && squareStatus[4] == 0)
	{
		nextMove = 4;
	}

	else if(squareStatus[7] == 1 && squareStatus[8] == 1 && squareStatus[6] == 0)
	{
		nextMove = 6;
	}

	else if(squareStatus[6] == 1 && squareStatus[8] == 1 && squareStatus[7] == 0)
	{
		nextMove = 7;
	}

	//diagonals
	else if(squareStatus[0] == 1 && squareStatus[4] == 1 && squareStatus[8] == 0)
	{
		nextMove = 8;
	}

	else if(squareStatus[0] == 1 && squareStatus[8] == 1 && squareStatus[4] == 0)
	{
		nextMove = 4;
	}

	else if(squareStatus[4] == 1 && squareStatus[8] == 1 && squareStatus[0] == 0)
	{
		nextMove = 0;
	}

	else if(squareStatus[2] == 1 && squareStatus[4] == 1 && squareStatus[6] == 0)
	{
		nextMove = 6;
	}

	else if(squareStatus[2] == 1 && squareStatus[6] == 1 && squareStatus[4] == 0)
	{
		nextMove = 4;
	}

	else if(squareStatus[4] == 1 && squareStatus[6] == 1 && squareStatus[2] == 0) 
	{
		nextMove = 2;
	}

	/************Here is the vertical logic*******************/
	//column 1
	else if(squareStatus[0] == 1 && squareStatus[3] == 1 && squareStatus[6] == 0)
	{
		nextMove = 6;
	}

	else if(squareStatus[0] == 1 && squareStatus[6] == 1 && squareStatus[3] == 0)
	{
		nextMove = 3;
	}

	else if(squareStatus[3] == 1 && squareStatus[6] == 1 && squareStatus[0] == 0)
	{
		nextMove = 0;
	}

	//column 2

	else if(squareStatus[1] == 1 && squareStatus[4] == 1 && squareStatus[7] == 0)
	{
		nextMove = 7;
	}

	else if(squareStatus[4] == 1 && squareStatus[7] == 1 && squareStatus[1] == 0)
	{
		nextMove = 1;
	}

	else if(squareStatus[1] == 1 && squareStatus[7] == 1 && squareStatus[4] == 0)
	{
		nextMove = 4;
	}

	//column 3

	else if(squareStatus[2] == 1 && squareStatus[8] == 1 && squareStatus[5] == 0)
	{
		nextMove = 5;
	}

	else if(squareStatus[2] == 1 && squareStatus[5] == 1 && squareStatus[8] == 0)
	{
		nextMove = 8;
	}

	else if(squareStatus[5] == 1 && squareStatus[8] == 1 && squareStatus[2] == 0)
	{
		nextMove = 2;
	}

	return nextMove;
}

void Game::buttonClicked(int i)
{
		QImage xImage;
		QImage oImage;
		if(currentUser.color == "Red"){
			xImage.load("rx.png");
			oImage.load("ro.png");
		}
		else if(currentUser.color == "Blue"){
			xImage.load("bx.png");
			oImage.load("bo.png");
		}
		else {
			xImage.load("gx.png");
			oImage.load("go.png");
		}
		bool userPlayed = false;
		int winner = 0;
		if(squareStatus[i] == 0) //user plays
		{
			boardList[i]->setPixmap(QPixmap::fromImage(xImage));
			squareStatus[i] = 1;
			userPlayed = true;
			numberOfPlays++;
			winner = checkForWinner();
		}

		if(winner == 1)
		{
			scores[1]++;
			QMessageBox msgBox;
			msgBox.setText("Congratulations, you win!");
			msgBox.exec();
			msgBox.show();

			QImage emptyImage;
			emptyImage.load("blank.png");
			for(int i = 0; i < 9; i++)
			{
				squareStatus[i] = 0;
				boardList[i]->setPixmap(QPixmap::fromImage(emptyImage));
			}
			numberOfPlays = 0;
		}

		else if(userPlayed && numberOfPlays <=8) //computer plays
		{	
			bool computerHasPlayed = false;
			if(determineComputerMove() != -1)
			{
				int nextMove = determineComputerMove();
				boardList[nextMove]->setPixmap(QPixmap::fromImage(oImage));
				squareStatus[nextMove] = 2;
				computerHasPlayed = true;
				numberOfPlays++;
				winner = checkForWinner();
			}
			else
			{
				while(!computerHasPlayed)
				{
					int randomNumber = qrand()%(9);
					if(squareStatus[randomNumber] == 0)
					{
						boardList[randomNumber]->setPixmap(QPixmap::fromImage(oImage));
						squareStatus[randomNumber] = 2;
						computerHasPlayed = true;
						numberOfPlays++;
						winner = checkForWinner();

					}
				
				}
			}
		}

		else if(numberOfPlays > 8)
		{
			scores[0]++;
			QMessageBox msgBox;
			msgBox.setText("Game drawn.");
			msgBox.exec();
			msgBox.show();

			QImage emptyImage;
			emptyImage.load("blank.png");
			for(int i = 0; i < 9; i++)
			{
				squareStatus[i] = 0;
				boardList[i]->setPixmap(QPixmap::fromImage(emptyImage));
			}
			numberOfPlays = 0;
		}

		if(winner == 2)
		{
			scores[2]++;
			QMessageBox msgBox;
			msgBox.setText("Sorry, you lost. Better luck next time.");
			msgBox.exec();
			msgBox.show();

			QImage emptyImage;
			emptyImage.load("blank.png");
			for(int i = 0; i < 9; i++)
			{
				squareStatus[i] = 0;
				boardList[i]->setPixmap(QPixmap::fromImage(emptyImage));
			}
			numberOfPlays = 0;
		}
		QString currentScoreString = "Computer " + QString::number(scores[2]) + "\nPlayer " + QString::number(scores[1]) + "\nDraw " + QString::number(scores[0]);
		currentScore->setText(currentScoreString);


}

/*******************************Custom LabelClick Class********************************/

LabelClick::LabelClick(QWidget *parent):QLabel(parent)
{

}
void LabelClick::mousePressEvent(QMouseEvent* event)
{
	emit clicked();
}