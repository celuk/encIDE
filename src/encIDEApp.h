#ifndef ENCIDEAPP_H
#define ENCIDEAPP_H

#include <wx/app.h>

class encIDEApp : public wxApp
{
    public:
        virtual bool OnInit();
};

DECLARE_APP(encIDEApp);

#endif
