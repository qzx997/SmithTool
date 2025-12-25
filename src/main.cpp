/**
 * @file main.cpp
 * @brief SmithTool application entry point
 */

#include <QApplication>
#include "ui/mainwindow.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    
    // Set application information
    QApplication::setApplicationName("SmithTool");
    QApplication::setApplicationVersion("1.5.0");
    QApplication::setOrganizationName("SmithTool");
    
    // Create and show main window
    SmithTool::MainWindow mainWindow;
    mainWindow.show();
    
    return app.exec();
}
