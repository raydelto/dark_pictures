#include "darkpicturesapp.hpp"
#include <Qt/qdeclarativedebug.h>

Q_DECL_EXPORT int main(int argc, char **argv)
{
    // Call the main application constructor.
    Application app(argc, argv);

    // Initialize our app.
    DarkPicturesApp mainApp;

    // We complete the transaction started in the main application constructor and start the
    // client event loop here. When loop is exited the Application deletes the scene which
    // deletes all its children.
    return Application::exec();
}

