#include "surfacegraph.h"
#include "ACO/acowidget.h"
#include "Annealing/annealingwidget.h"
#include "GA/GAsurfacegraph.h"
#include "Apriori/aprioriwidget.h"
#include "FuzzyLogic/flwidget.h"

#include <QtWidgets/qapplication.h>
#include <QtWidgets/qmessagebox.h>
#include <QtWidgets/qwidget.h>
#include <QtWidgets/qtabwidget.h>
#include <QtGui/qscreen.h>

using namespace Qt::StringLiterals;

int main(int argc, char **argv)
{
    qputenv("QSG_RHI_BACKEND", "opengl");

    QApplication app(argc, argv);

    QTabWidget tabWidget;
    tabWidget.setWindowTitle(u"Graph Gallery"_s);

    const QSize screenSize = tabWidget.screen()->size();
    const QSize minimumGraphSize{screenSize.width() / 2, qRound(screenSize.height() / 1.75)};

    // Create SPO widget
    //
    SurfaceGraph surface;

    // Create GA widget
    //
    GASurfaceGraph GAsurface;

    // Create Annealing widget
    //
    AnnealingWidget Annealingwidget;

    // Create ACO widget
    //
    ACOWidget ACOwidget;

    // Create Apriori widget
    //
    AprioriWidget apriori;

    // Create Fuzzy Logic widget
    //
    FlWidget fuzzy;

    if (!surface.initialize(minimumGraphSize, screenSize)
        || !GAsurface.initialize(minimumGraphSize, screenSize)) {
        QMessageBox::warning(nullptr, u"Graph Gallery"_s, u"Couldn't initialize the OpenGL context."_s);
        return -1;
    }

    // Add SPO widget
    //
    tabWidget.addTab(surface.surfaceWidget(), u"Swarm particle optimization"_s);

    // Add GA widget
    //
    tabWidget.addTab(GAsurface.surfaceWidget(), u"Genetic algorithm optimization"_s);

    // Add Annealing widget
    //
    tabWidget.addTab(Annealingwidget.getWidget(), u"Annealing optimization"_s);

    // Add ACO widget
    //
    tabWidget.addTab(ACOwidget.getWidget(), u"Ant colony optimization"_s);

    // Add Apriori widget
    //
    tabWidget.addTab(apriori.getWidget(), u"Data mining (Apriori algorithm)"_s);

    // Add Fuzzy Logic widget
    //
    tabWidget.addTab(fuzzy.getWidget(), u"Fuzzy Logic"_s);

    tabWidget.show();
    return app.exec();
}
