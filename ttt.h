#ifndef TTT_H
#define TTT_H
#include <QtWidgets>
#include <QPushButton>

class Database
{
	public:
		QHash<QString, QString> colors;
		QHash<QString, QByteArray> passwords;
};

QDataStream & operator<< (QDataStream& dataStream, const Database& database);
QDataStream & operator>> (QDataStream& dataStream, Database& database);

class LabelClick:public QLabel
{
	Q_OBJECT

	public:
		explicit LabelClick(QWidget *parent=0);
	signals:
		void clicked();
	protected:
		void mousePressEvent(QMouseEvent *event);

};

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
		QComboBox *combo;
	private slots:
		void okClicked();
		void clear();
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
		QLineEdit *oldPassword;
		QLineEdit *newPassword;
		QLineEdit *newPasswordAgain;
		QComboBox *combo;
	private slots:
		void okClicked();
		void clear();
	signals:
		void cancelClicked();	
		void passwordChanged();
		void goWriteDatabaseToFile();
	
};

class Game:public QWidget
{
	Q_OBJECT

	public:
		Game(QWidget *parent = 0);
	private:
		QPushButton* createButton(const QString &text, const char *member);
		LabelClick* createLabel(int i);
		QList<LabelClick*> boardList;
		QList<int> squareStatus; //0 is empty, 1 is X, 2 is O
		QSignalMapper *signalMapper;
		int numberOfPlays;
		int checkForWinner();
	private slots:
		void buttonClicked(int);
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
		//QFile *passFile;
		//QDataStream *outStream;
		void insertToDataStream();
		void extractFromDataStream();
	private slots:
		void exitMain();
		void switchToWelcome();
		void switchToChangePassword();
		void switchToRegister();
		void switchToLogin();
		void switchToGame();
		void writeDatabaseToFile();
	signals:
		void clearLogin();
		void clearRegister();
		void clearChangePassword();
};


#endif