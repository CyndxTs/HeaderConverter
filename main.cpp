/*/
* Projecto:            HeaderConverter
 * Nombre del Archivo:  main.cpp
 * Autor:               CyndxTs
/*/

#include <QApplication>
#include "gui.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setOrganizationName("CEDEX");
    app.setApplicationName("HeaderConverter");

    initGUI();

    return app.exec();
}