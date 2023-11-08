#include <sdk.h> // Code::Blocks SDK
#include <wx/textfile.h>
#include "ExporterBase.h"

ExporterBase::ExporterBase()
{
    //ctor
}

ExporterBase::~ExporterBase()
{
    //dtor
}

wxString ExporterBase::ConvertSlash(const wxString& source)
{
    wxString output = source;
    output.Replace(wxT("\\"), wxT("/"));
    return output;
}

wxArrayString ExporterBase::IncludeFilesArray()
{
    wxArrayString output;
    wxRegEx findInclude(wxT("^[[:blank:]]*#[[:blank:]]*include[[:blank:]]*<([^\">]+)>"));
    for(FilesList::iterator i = m_project->GetFilesList().begin(); i != m_project->GetFilesList().end(); i++)
    {
        ProjectFile* pf = *i;
        //bufferString << _T("\", \"") << ConvertSlash(pf->relativeFilename);
        wxTextFile file(pf->relativeFilename);
        if(file.Open())
        {
            for(unsigned int j = 0; j < file.GetLineCount(); j++)
            {
                wxString bufferString = file.GetLine(j);
                if(findInclude.Matches(bufferString))
                {
                    bufferString = findInclude.GetMatch(bufferString, 1);
                    if(output.Index(bufferString) == wxNOT_FOUND)
                    {
                        output.Add(bufferString);
                    }
                }
            }
        }
        file.Close();
    }
    output.Sort();
    return output;
}

wxArrayString ExporterBase::AppendOptionsArray(const wxArrayString& proj, const wxArrayString& targ, const OptionsRelation relation)
{
    wxArrayString output = proj;
    switch(relation)
    {
        case orUseTargetOptionsOnly:
        {
            output = targ;
            break;
        }
        case orPrependToParentOptions:
        {
            WX_PREPEND_ARRAY(output, targ);
            break;
        }
        case orAppendToParentOptions:
        {
            WX_APPEND_ARRAY(output, targ);
        }
        default:;
    }
    return output;
}

wxString ExporterBase::ReplSpace(const wxString& source)
{
    wxString output = source;
    output.Replace(wxT("&"), wxT("and"));
    output.Replace(wxT(" "), wxT("_"));
    return output;
}

wxString ExporterBase::RemLib(const wxString& source)
{
    if(wxT("lib") == source.Left(3))
    {
        return source.Mid(3);
    }
    return source;
}

bool ExporterBase::StringExists(const wxArrayString& source, wxString target)
{
    for(unsigned int i=0; i < source.Count(); i++)
    {
        if(source[i] == target)
            return true;
    }
    return false;
}
