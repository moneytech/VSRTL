#include <QApplication>
#include "ripes_adderandreg.h"
#include "ripes_componentgraphic.h"
#include "ripes_mainwindow.h"

#include <chrono>

#include <QDebug>
#include <QFile>

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    Q_INIT_RESOURCE(icons);

    ripes::MainWindow w;
    ripes::AdderAndReg design;
    design.createComponentGraph();

    auto components = design.getComponentSet();

    int x = 0;
    for (auto& c : components) {
        ripes::ComponentGraphic* i = new ripes::ComponentGraphic(c);
        w.addComponent(i);
        i->initialize();
        i->setPosition(QPointF(x, 0));
        x += 150;
    }
    w.show();

    app.exec();
}