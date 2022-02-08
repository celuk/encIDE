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

        double screenRatio;

        wxMenuBar* topMenuBar;

        // File Menu
        wxMenu* fileMenu;

        wxMenuItem* openFileItem;
        static const long idMenuOpenFile;

        wxMenuItem* saveFileItem;
        static const long idMenuSaveFile;

        wxMenuItem* compileFileItem;
        static const long idMenuCompileFile;

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

        // Options Menu
        wxMenu* optionsMenu;

        wxMenuItem* setCompilerPathItem;
        static const long idMenuSetCompilerPath;

        wxMenuItem* checkCompileStringItem;
        static const long idMenuCheckCompileString;
        // End of Options Menu

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
        wxString extraCompileFlags;

        void setTextEditorStyle();

        void onOpenFile(wxCommandEvent& event);
        void onSaveFile(wxCommandEvent& event);
        void onCompileFile(wxCommandEvent& event);
        void onQuit(wxCommandEvent& event);

        void onZoomIn(wxCommandEvent& event);
        void onZoomOut(wxCommandEvent& event);

        void onSetCompilerPath(wxCommandEvent& event);
        void onCheckCompileString(wxCommandEvent& event);

        void onAbout(wxCommandEvent& event);

        void onClose(wxCloseEvent& event);

        void readAndSetConfig();

        DECLARE_EVENT_TABLE()
};

#endif