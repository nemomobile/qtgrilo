#include <GriloRegistry>
#include <QtCore>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    GriloRegistry *griloRegistry = new GriloRegistry();

    int result = a.exec();

    delete griloRegistry;

    return result;
}
