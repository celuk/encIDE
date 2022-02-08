#ifndef ENCIDEMAIN_H
#define ENCIDEMAIN_H

#include <wx/menu.h>
#include <wx/frame.h>
#include <wx/statusbr.h>
#include <wx/stc/stc.h>

class encIDEFrame: public wxFrame
{
    public:

        encIDEFrame(wxWindow* parent, wxWindowID id = -1);
        virtual ~encIDEFrame();

    private:

        wxMenuBar* topMenuBar;

        // File Menu
        wxMenu* fileMenu;

        wxMenuItem* openFileItem;
        static const long idMenuOpenFile;

        wxMenuItem* quitItem;
        static const long idMenuQuit;
        // End of File Menu

        // View Menu
        wxMenu* viewMenu;

        wxMenuItem* zoomInItem;
        static const long idMenuZoomIn;

        wxMenuItem* zoomOutItem;
        static const long idMenuZoomOut;
        // End of View Menu

        // Help Menu
        wxMenu* helpMenu;

        wxMenuItem* aboutItem;
        static const long idMenuAbout;
        // End of Help Menu

        wxStatusBar* statusBar;
        static const long idStatusBar;

        wxStyledTextCtrl* textEditor;
        static const long idTextEditor;

        wxString filePath;
        wxString compilerPath;
        wxString riscvRootPath;
        wxString riscvTargetOption;

        void setTextEditorStyle();

        void onOpenFile(wxCommandEvent& event);
        void onQuit(wxCommandEvent& event);

        void onZoomIn(wxCommandEvent& event);
        void onZoomOut(wxCommandEvent& event);

        void onAbout(wxCommandEvent& event);

        void onClose(wxCloseEvent& event);

        void readAndSetConfig();

        DECLARE_EVENT_TABLE()
};

#endif