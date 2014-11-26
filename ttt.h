#ifndef TTT_H
#define TTT_H
#include <QtWidgets>
#include <QPushButton>

class Database
{
	public:
		QList<QString> colors;
		QHash<QString, QByteArray> passwords;
};

QDataStream & operator<< (QDataStream& dataStream, const Database& database);
QDataStream & operator>> (QDataStream& dataStream, Database& database);

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
		void clearLogin();

};

class Register:public QWidget
{
	Q_OBJECT

	public:
		Register(QWidget *parent = 0);
	private:
		QPushButton *createButton(const QString &text, const char *member);
		QLineEdit *username;
		QLineEdit *newPassword;
		QLineEdit *newPasswordAgain;
	private slots:
		void okClicked();
	signals:
		void cancelClicked();	
		void passwordChanged();
		void goWriteDatabaseToFile();
	
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
	signals:
		void changePasswordClicked();
		void startGameClicked();
		void exitClicked();
};

class ChangePassword:public QWidget
{
	Q_OBJECT

	public:
		ChangePassword(QWidget *parent = 0);
	private:
		QPushButton *createButton(const QString &text, const char *member);
	private slots:
		void okClicked();
	signals:
		void cancelClicked();	
		void passwordChanged();
	
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
		Login *myLogin;
		QFile *passFile;
		QDataStream *outStream;
		void insertToDataStream(QDataStream& dataStream);
		void extractFromDataStream(QDataStream& dataStream);
	private slots:
		void exitMain();
		void switchToWelcome();
		void switchToChangePassword();
		void switchToRegister();
		void switchToLogin();
		void writeDatabaseToFile();
	signals:
		void clearLogin();
};


#endif