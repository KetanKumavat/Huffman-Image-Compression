#include "gui.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    ImageCompressionGUI window;
    window.show();
    return app.exec();
}