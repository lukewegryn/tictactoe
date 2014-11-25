#ifndef TTT_H
#define TTT_H
#include <QtWidgets>
#include <QPushButton>
class Login:public QWidget
{
	Q_OBJECT

	public:
		Login(QWidget *parent = 0);
		QGridLayout *loginLayout;
	private:
		QPushButton *createButton(const QString &text, const char *member);
		QLineEdit *username;
		QLineEdit *password;
	signals:
		void exitClicked();
		void loginSuccessful();
	private slots:
		void loginClicked();

};

class Register:public QWidget
{
	Q_OBJECT

	public:
		Register(QWidget *parent = 0);
	private:
		QPushButton *createButton(const QString &text, const char *member);
		
	
};

class Welcome:public QWidget
{
	Q_OBJECT

	public:
		Welcome(QWidget *parent = 0);
		QGridLayout *welcomeLayout;
		void setText(QString);
	private:
		QPushButton *createButton(const QString &text, const char *member);
		QLabel *myLabel;
		
	
};

class ChangePassword:public QWidget
{
	Q_OBJECT

	public:
		ChangePassword(QWidget *parent = 0);
	private:
		QPushButton *createButton(const QString &text, const char *member);
		
	
};

class Game:public QWidget
{
	Q_OBJECT

	public:
		Game(QWidget *parent = 0);
	private:
		QPushButton *createButton(const QString &text, const char *member);
};

class MainWindow:public QWidget
{
	Q_OBJECT

	public:
		MainWindow(QWidget *parent = 0);
	private:
		QStackedWidget *stackedWidget; 
		Welcome *myWelcome;
	private slots:
		void exitMain();
		void switchToWelcome();
};


#endif