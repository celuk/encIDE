#include "encIDEApp.h"
#include "encIDEMain.h"

#include <wx/display.h>

#include <wx/msgdlg.h>

#include <wx/filedlg.h>
#include <wx/textfile.h>
#include <wx/textdlg.h>

#define DEFAULT_SCREEN_RATIO 0.9

#define TEXT_EDITOR_COLOR wxColor(180, 181, 185)
#define GRAY_COLOR wxColor(100, 100, 100)

#define MARGIN_LINE_NUMBERS 0
#define MARGIN_FOLD 1

#define CONFIG_FILE "encide.config"

// File Menu Items
const long encIDEFrame::idMenuOpenFile = wxNewId();
const long encIDEFrame::idMenuSaveFile = wxNewId();
const long encIDEFrame::idMenuCompileFile = wxNewId();
const long encIDEFrame::idMenuQuit = wxNewId();
// End of File Menu Items

// View Menu Items
const long encIDEFrame::idMenuZoomIn = wxNewId();
const long encIDEFrame::idMenuZoomOut = wxNewId();
// End of View Menu Items

// Options Menu Items
const long encIDEFrame::idMenuSetCompilerPath = wxNewId();
const long encIDEFrame::idMenuSetRiscvPath = wxNewId();
const long encIDEFrame::idMenuSetRiscvTargetOption = wxNewId();
const long encIDEFrame::idMenuCheckCompileString = wxNewId();
// End of Options Menu Items

// Help Menu Items
const long encIDEFrame::idMenuAbout = wxNewId();
// End of Help Menu Items

const long encIDEFrame::idStatusBar = wxNewId();

const long encIDEFrame::idTextEditor = wxNewId();


BEGIN_EVENT_TABLE(encIDEFrame, wxFrame)
    EVT_MENU(idMenuOpenFile, encIDEFrame::onOpenFile)
    EVT_MENU(idMenuSaveFile, encIDEFrame::onSaveFile)
    EVT_MENU(idMenuCompileFile, encIDEFrame::onCompileFile)
    EVT_MENU(idMenuQuit, encIDEFrame::onQuit)

    EVT_MENU(idMenuZoomIn, encIDEFrame::onZoomIn)
    EVT_MENU(idMenuZoomOut, encIDEFrame::onZoomOut)

    EVT_MENU(idMenuSetCompilerPath, encIDEFrame::onSetCompilerPath)
    EVT_MENU(idMenuSetRiscvPath, encIDEFrame::onSetRiscvPath)
    EVT_MENU(idMenuSetRiscvTargetOption, encIDEFrame::onSetRiscvTargetOption)
    EVT_MENU(idMenuCheckCompileString, encIDEFrame::onCheckCompileString)

    EVT_MENU(idMenuAbout, encIDEFrame::onAbout)
END_EVENT_TABLE()


encIDEFrame::encIDEFrame(wxWindow* parent, wxWindowID id)
{
    Create(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _("wxID_ANY"));

    screenRatio = DEFAULT_SCREEN_RATIO;

    // Set strings
    filePath = "";
    compilerPath = "";
    riscvRootPath = "";
    riscvTargetOption = "";
    extraCompileFlags = "";

    // TODO set compile string from config file
    wxGetApp().compileString = "";
    wxGetApp().compileOptionsString = "";

    readAndSetConfig();

    wxDisplay display(wxDisplay::GetFromWindow(this));
    wxRect activeScreenRect = display.GetClientArea();

    this->SetClientSize(wxSize(screenRatio * activeScreenRect.GetWidth(), screenRatio * activeScreenRect.GetHeight()));

    this->CenterOnScreen();

    topMenuBar = new wxMenuBar();

    // TODO set menus within a function
    // File Menu
    fileMenu = new wxMenu();
    topMenuBar->Append(fileMenu, _("&File"));

    openFileItem = new wxMenuItem(fileMenu, idMenuOpenFile, _("Open File\tCtrl-O"), _("Open an existing file"), wxITEM_NORMAL);
    fileMenu->Append(openFileItem);

    saveFileItem = new wxMenuItem(fileMenu, idMenuSaveFile, _("Save File\tCtrl-S"), _("Save an existing or new file"), wxITEM_NORMAL);
    fileMenu->Append(saveFileItem);

    compileFileItem = new wxMenuItem(fileMenu, idMenuCompileFile, _("Compile File\tCtrl-B"), _("Execute compile string to compile existing file"), wxITEM_NORMAL);
    fileMenu->Append(compileFileItem);

    quitItem = new wxMenuItem(fileMenu, idMenuQuit, _("Quit\tAlt-F4"), _("Quit the application"), wxITEM_NORMAL);
    fileMenu->Append(quitItem);
    // End of File Menu

    // View Menu
    viewMenu = new wxMenu();
    topMenuBar->Append(viewMenu, _("&View"));

    zoomInItem = new wxMenuItem(viewMenu, idMenuZoomIn, _("Zoom In\tCtrl-+"), _("Zoom In"), wxITEM_NORMAL);
    zoomOutItem = new wxMenuItem(viewMenu, idMenuZoomOut, _("Zoom Out\tCtrl--"), _("Zoom Out"), wxITEM_NORMAL);

    // On macos set zoom key to 'option' or 'alt'
    #if defined(__APPLE__)
        zoomInItem->SetItemLabel(_("Zoom In\tAlt-+")); // Can be used with 'option' + 'shift' + '+'
        zoomOutItem->SetItemLabel(_("Zoom Out\tAlt--")); // Can be used with 'option' + '-'
    #endif

    viewMenu->Append(zoomInItem);
    viewMenu->Append(zoomOutItem);
    // End of View Menu

    // Options Menu
    optionsMenu = new wxMenu();
    topMenuBar->Append(optionsMenu, _("&Options"));

    setCompilerPathItem = new wxMenuItem(optionsMenu, idMenuSetCompilerPath, _("Set Compiler Path\tCtrl-P"), _("Set Compiler Path"), wxITEM_NORMAL);
    optionsMenu->Append(setCompilerPathItem);

    setRiscvPathItem = new wxMenuItem(optionsMenu, idMenuSetRiscvPath, _("Set RISCV Path\tCtrl-R"), _("Set RISCV Path"), wxITEM_NORMAL);
    optionsMenu->Append(setRiscvPathItem);

    setRiscvTargetOptionItem = new wxMenuItem(optionsMenu, idMenuSetRiscvTargetOption, _("Set RISCV Target Options\tCtrl-T"), _("Set RISCV Target Options"), wxITEM_NORMAL);
    optionsMenu->Append(setRiscvTargetOptionItem);

    checkCompileStringItem = new wxMenuItem(optionsMenu, idMenuCheckCompileString, _("Check Compile String\tCtrl-K"), _("Check Compile String"), wxITEM_NORMAL);
    optionsMenu->Append(checkCompileStringItem);
    // End of Options Menu

    // Help Menu
    helpMenu = new wxMenu();
    topMenuBar->Append(helpMenu, _("&Help"));

    aboutItem = new wxMenuItem(helpMenu, idMenuAbout, _("About\tF1"), _("Show info about this application"), wxITEM_NORMAL);
    helpMenu->Append(aboutItem);
    // End of Help Menu

    SetMenuBar(topMenuBar);

    // Status Bar
    statusBar = new wxStatusBar(this, idStatusBar, wxSTB_DEFAULT_STYLE, _("StatusBar"));
    int __wxStatusBarWidths_1[1] = { -1 };
    int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
    statusBar->SetFieldsCount(1, __wxStatusBarWidths_1);
    statusBar->SetStatusStyles(1, __wxStatusBarStyles_1);
    SetStatusBar(statusBar);
    // End of StatusBar

    // C++ Text Editor
    textEditor = new wxStyledTextCtrl(this, idTextEditor, wxDefaultPosition, wxDefaultSize, 0, "TextEditor");
    setTextEditorStyle();
}

encIDEFrame::~encIDEFrame()
{
    this->Destroy();
}

void encIDEFrame::setTextEditorStyle(){
    textEditor->StyleSetBackground(wxSTC_STYLE_DEFAULT, TEXT_EDITOR_COLOR);

    // To not erase background color while writing
    textEditor->StyleClearAll();

    textEditor->SetLexer(wxSTC_LEX_CPP);

    // Set lines
    textEditor->SetMarginWidth(MARGIN_LINE_NUMBERS, 50);
    textEditor->StyleSetForeground(wxSTC_STYLE_LINENUMBER, wxColor(75, 75, 75));
    textEditor->StyleSetBackground(wxSTC_STYLE_LINENUMBER, wxColor(220, 220, 220));
    textEditor->SetMarginType(MARGIN_LINE_NUMBERS, wxSTC_MARGIN_NUMBER);

    // Enable code folding
    textEditor->SetMarginType(MARGIN_FOLD, wxSTC_MARGIN_SYMBOL);
    textEditor->SetMarginWidth(MARGIN_FOLD, 1);
    textEditor->SetMarginMask(MARGIN_FOLD, wxSTC_MASK_FOLDERS);
    textEditor->StyleSetBackground(MARGIN_FOLD, wxColor(200, 200, 200));
    textEditor->SetMarginSensitive(MARGIN_FOLD, true);

    // Set scintilla properties
    textEditor->SetProperty(wxT("fold"), wxT("1"));
    textEditor->SetProperty(wxT("fold.comment"), wxT("1"));
    textEditor->SetProperty(wxT("fold.compact"), wxT("1"));

    // Set markers
    // Folder marker
    textEditor->MarkerDefine(wxSTC_MARKNUM_FOLDER, wxSTC_MARK_ARROW);
    textEditor->MarkerSetForeground(wxSTC_MARKNUM_FOLDER, GRAY_COLOR);
    textEditor->MarkerSetBackground(wxSTC_MARKNUM_FOLDER, GRAY_COLOR);

    // Folder open marker
    textEditor->MarkerDefine(wxSTC_MARKNUM_FOLDEROPEN, wxSTC_MARK_ARROWDOWN);
    textEditor->MarkerSetForeground(wxSTC_MARKNUM_FOLDEROPEN, GRAY_COLOR);
    textEditor->MarkerSetBackground(wxSTC_MARKNUM_FOLDEROPEN, GRAY_COLOR);

    // Folder sub marker
    textEditor->MarkerDefine(wxSTC_MARKNUM_FOLDERSUB, wxSTC_MARK_EMPTY);
    textEditor->MarkerSetForeground(wxSTC_MARKNUM_FOLDERSUB, GRAY_COLOR);
    textEditor->MarkerSetBackground(wxSTC_MARKNUM_FOLDERSUB, GRAY_COLOR);

    // Folder end marker
    textEditor->MarkerDefine(wxSTC_MARKNUM_FOLDEREND, wxSTC_MARK_ARROW);
    textEditor->MarkerSetForeground(wxSTC_MARKNUM_FOLDEREND, GRAY_COLOR);
    textEditor->MarkerSetBackground(wxSTC_MARKNUM_FOLDEREND, _("WHITE"));

    // Folder open mid marker
    textEditor->MarkerDefine(wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_ARROWDOWN);
    textEditor->MarkerSetForeground(wxSTC_MARKNUM_FOLDEROPENMID, GRAY_COLOR);
    textEditor->MarkerSetBackground(wxSTC_MARKNUM_FOLDEROPENMID, _("WHITE"));

    // Folder mid tail marker
    textEditor->MarkerDefine(wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_EMPTY);
    textEditor->MarkerSetForeground(wxSTC_MARKNUM_FOLDERMIDTAIL, GRAY_COLOR);
    textEditor->MarkerSetBackground(wxSTC_MARKNUM_FOLDERMIDTAIL, GRAY_COLOR);

    // Folder tail marker
    textEditor->MarkerDefine(wxSTC_MARKNUM_FOLDERTAIL, wxSTC_MARK_EMPTY);
    textEditor->MarkerSetForeground(wxSTC_MARKNUM_FOLDERTAIL, GRAY_COLOR);
    textEditor->MarkerSetBackground(wxSTC_MARKNUM_FOLDERTAIL, GRAY_COLOR);

    // Set wrap mode, other choice can be wxSCI_WRAP_NONE 
    textEditor->SetWrapMode(wxSTC_WRAP_WORD);

    // Set other style options and colors
    textEditor->StyleSetForeground(wxSTC_C_STRING, wxColor(150, 0, 0));
    textEditor->StyleSetForeground(wxSTC_C_PREPROCESSOR, wxColor(165, 105, 0));
    textEditor->StyleSetForeground(wxSTC_C_IDENTIFIER, wxColor(40, 0, 60));
    textEditor->StyleSetForeground(wxSTC_C_NUMBER, wxColor(0, 150, 0));
    textEditor->StyleSetForeground(wxSTC_C_CHARACTER, wxColor(150, 0, 0));

    textEditor->StyleSetForeground(wxSTC_C_WORD, wxColor(0, 0, 150));
    textEditor->StyleSetForeground(wxSTC_C_WORD2, wxColor(0, 150, 0));

    textEditor->StyleSetForeground(wxSTC_C_COMMENT, wxColor(150, 150, 150));
    textEditor->StyleSetForeground(wxSTC_C_COMMENTLINE, wxColor(150, 150, 150));
    textEditor->StyleSetForeground(wxSTC_C_COMMENTDOC, wxColor(150, 150, 150));

    textEditor->StyleSetForeground(wxSTC_C_COMMENTDOCKEYWORD, wxColor(0, 0, 200));
    textEditor->StyleSetForeground(wxSTC_C_COMMENTDOCKEYWORDERROR, wxColor(0, 0, 200));

    textEditor->StyleSetBold(wxSTC_C_WORD, true);
    textEditor->StyleSetBold(wxSTC_C_WORD2, true);
    textEditor->StyleSetBold(wxSTC_C_COMMENTDOCKEYWORD, true);

    // TODO add more c++ keyword
    // c++ keywords
    textEditor->SetKeyWords(0, wxT("return for while break continue class public private protected"));
    textEditor->SetKeyWords(1, wxT("const void int float char double"));
}

void encIDEFrame::onOpenFile(wxCommandEvent& event)
{
    wxFileDialog openFileDlg(this, "Open file...", wxEmptyString, wxEmptyString, "All files (*.*)|*.*", wxFD_OPEN);

    if(openFileDlg.ShowModal() == wxID_OK){
        wxTextFile openTextFile;
        openTextFile.Open(openFileDlg.GetPath());

        if(openTextFile.IsOpened()){
            filePath = openFileDlg.GetPath();

            textEditor->ClearAll();

            textEditor->AddText(openTextFile.GetFirstLine());
            textEditor->AddText("\r\n");
            while(!openTextFile.Eof()){
                textEditor->AddText(openTextFile.GetNextLine());
                textEditor->AddText("\r\n");
            }

            statusBar->PushStatusText(filePath);

            openTextFile.Close();
        }
        else wxMessageBox("File cannot be opened!");
    }
}

// TODO Add new file support and distinguish between saving new file and saving existing file
void encIDEFrame::onSaveFile(wxCommandEvent& event)
{
    wxFileDialog saveFileDlg(this, "Save file...", wxEmptyString, wxEmptyString, "All files (*.*)|*.*", wxFD_SAVE);

    if(saveFileDlg.ShowModal() == wxID_OK){
        wxTextFile saveTextFile;
        saveTextFile.Create(saveFileDlg.GetPath());
        saveTextFile.Open(saveFileDlg.GetPath());

        if(saveTextFile.IsOpened()){
            filePath = saveFileDlg.GetPath();

            textEditor->SaveFile(filePath);

            statusBar->PushStatusText(filePath);

            saveTextFile.Close();
        }
        else wxMessageBox("File cannot be created!");
    }
}

void encIDEFrame::onCompileFile(wxCommandEvent& event)
{
    // TODO open command prompt and execute command

    #if defined(_WIN32)
        wxExecute("start cmd.exe");
    #elif defined(__APPLE__)
        wxExecute("open -a Terminal .");
    #else
        // every linux distribution has xterm?
        wxExecute("xterm");
    #endif
}

void encIDEFrame::onQuit(wxCommandEvent& event)
{
    this->Close();
}

void encIDEFrame::onZoomIn(wxCommandEvent& event)
{
    textEditor->ZoomIn();
}

void encIDEFrame::onZoomOut(wxCommandEvent& event)
{
    textEditor->ZoomOut();
}

void encIDEFrame::onSetCompilerPath(wxCommandEvent& event)
{
    wxTextEntryDialog setCompilerPathDlg(this, wxEmptyString, "Set Compiler Path", 
        wxEmptyString /*TODO add compiler path from config file*/, wxOK | wxCANCEL | wxCENTRE, wxDefaultPosition);
    
    if(setCompilerPathDlg.ShowModal() == wxID_OK)
        compilerPath = setCompilerPathDlg.GetValue();
}

void encIDEFrame::onSetRiscvPath(wxCommandEvent& event)
{
    wxTextEntryDialog setRiscvPathDlg(this, wxEmptyString, "Set RISCV Path", 
        wxEmptyString /*TODO add riscv path from config file*/, wxOK | wxCANCEL | wxCENTRE, wxDefaultPosition);
    
    if(setRiscvPathDlg.ShowModal() == wxID_OK)
        riscvRootPath = setRiscvPathDlg.GetValue();
}

void encIDEFrame::onSetRiscvTargetOption(wxCommandEvent& event)
{
    wxTextEntryDialog setRiscvTargetOptionDlg(this, wxEmptyString, "Set RISCV Target Options", 
        wxEmptyString /*TODO add riscv target options from config file*/, wxOK | wxCANCEL | wxCENTRE, wxDefaultPosition);
    
    if(setRiscvTargetOptionDlg.ShowModal() == wxID_OK)
        riscvTargetOption = setRiscvTargetOptionDlg.GetValue();
}

void encIDEFrame::onCheckCompileString(wxCommandEvent& event)
{
    wxTextEntryDialog checkCompileStringDlg(this, wxEmptyString, "Check Compile String", 
        wxGetApp().compileString, wxTE_MULTILINE | wxOK | wxCANCEL | wxCENTRE, wxDefaultPosition);
    
    if(checkCompileStringDlg.ShowModal() == wxID_OK)
        wxGetApp().compileString = checkCompileStringDlg.GetValue();
}

void encIDEFrame::onAbout(wxCommandEvent& event)
{
    wxMessageBox(wxVERSION_STRING, _("encIDE"));
}

// Helper function
wxString getSubStrAfter(wxString str, wxString delimiter){
    return str.substr(str.find(delimiter) + 1);
}

void encIDEFrame::readAndSetConfig()
{
    wxTextFile configFile;
    configFile.Open(CONFIG_FILE);

    // TODO Fix corresponding strings
    if(configFile.IsOpened()){
        // first line is compiler path
        compilerPath = getSubStrAfter(configFile.GetFirstLine(), "=");

        // second line is riscv root path
        riscvRootPath = getSubStrAfter(configFile.GetNextLine(), "=");

        // third line is riscv target option
        riscvTargetOption = getSubStrAfter(configFile.GetNextLine(), "=");

        // fourth line is extra compile flags
        extraCompileFlags = getSubStrAfter(configFile.GetNextLine(), "=");

        // fifth line is screen ratio
        getSubStrAfter(configFile.GetNextLine(), "=").ToDouble(&screenRatio);
    }
}

