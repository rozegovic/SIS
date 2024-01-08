#include <gui/WinMain.h>
#include "Application.h"

int main(int argc, const char * argv[])
{
    Application app(argc, argv);
    app.init("BA");

    if (!app.connectToDB()) 
    {
        std::cout << "Stavite bazu podataka na lokaciju: %HOME%/Work/CPProjects/SiS_v_Zadnji/Temp_Baza.db\n";
        
        return 1;
    }

    return app.run();
}

