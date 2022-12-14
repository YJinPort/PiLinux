#include <QApplication>
#include <QPushButton>

int main(int argc, char **argv)
{
	QApplication app(argc, argv);

	QPushButton *button = new QPushButton("Quit", 0);
	button->resize(120, 35);
	button->move(300, 300);
	
	QObject::connect(button, SIGNAL(clicked()), &app, SLOT(quit()));

	button->show();

	return app.exec();
}
