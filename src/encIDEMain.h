#ifndef ENCIDEMAIN_H
#define ENCIDEMAIN_H

#include <wx/menu.h>
#include <wx/frame.h>
#include <wx/statusbr.h>

class encIDEFrame: public wxFrame
{
    public:

        encIDEFrame(wxWindow* parent, wxWindowID id = -1);
        virtual ~encIDEFrame();

    private:

        wxMenuBar* topMenuBar;

        // File Menu
        wxMenu* fileMenu;

        wxMenuItem* quitItem;
        static const long idMenuQuit;
        // End of File Menu

        // Help Menu
        wxMenu* helpMenu;

        wxMenuItem* aboutItem;
        static const long idMenuAbout;
        // End of Help Menu

        wxStatusBar* statusBar;
        static const long idStatusBar;

        void onQuit(wxCommandEvent& event);
        void onAbout(wxCommandEvent& event);
        void onClose(wxCloseEvent& event);

        DECLARE_EVENT_TABLE()
};

#endif