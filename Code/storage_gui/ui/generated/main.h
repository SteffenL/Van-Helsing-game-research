///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 17 2015)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __MAIN_H__
#define __MAIN_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/frame.h>
#include <wx/toolbar.h>
#include <wx/dataview.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/manager.h>
#include <wx/propgrid/advprops.h>
#include <wx/statbmp.h>
#include <wx/aui/aui.h>

///////////////////////////////////////////////////////////////////////////

#define ID_OPEN 1000
#define ID_SAVE 1001
#define ID_EXIT 1002
#define ID_ABOUT 1003

///////////////////////////////////////////////////////////////////////////////
/// Class MainFrameBase
///////////////////////////////////////////////////////////////////////////////
class MainFrameBase : public wxFrame 
{
	private:
	
	protected:
		wxMenuBar* m_menubar1;
		wxMenu* file;
		wxMenu* help;
		
		// Virtual event handlers, overide them in your derived class
		virtual void onIdle( wxIdleEvent& event ) = 0;
		virtual void openOnMenuSelection( wxCommandEvent& event ) = 0;
		virtual void saveOnMenuSelection( wxCommandEvent& event ) = 0;
		virtual void saveOnUpdateUI( wxUpdateUIEvent& event ) = 0;
		virtual void exitOnMenuSelection( wxCommandEvent& event ) = 0;
		virtual void aboutOnMenuSelection( wxCommandEvent& event ) = 0;
		
	
	public:
		
		MainFrameBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1077,673 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~MainFrameBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class StorageEditorPanelBase
///////////////////////////////////////////////////////////////////////////////
class StorageEditorPanelBase : public wxPanel 
{
	private:
	
	protected:
		wxPanel* m_panel5;
		wxToolBar* m_artifactBagToolBar;
		wxDataViewCtrl* m_artifacts;
		wxPanel* m_panel6;
		wxDataViewCtrl* m_artifactEnchantments;
		wxPanel* m_panel7;
		wxPropertyGridManager* m_propertyManager;
		wxPropertyGridPage* m_propertiesEmptyPage;
		wxPropertyGridPage* m_propertiesArtifactPage;
		wxPGProperty* m_artifactRarityProperty;
		wxPGProperty* m_artifactQualityProperty;
		wxPGProperty* m_artifactQuantityProperty;
		wxPGProperty* m_artifactIdentifiedProperty;
		wxPGProperty* m_artifactProperty1Property;
		wxPGProperty* m_artifactProperty2Property;
		wxPropertyGridPage* m_propertiesEnchantmentPage;
		wxPGProperty* m_enchantmentValueIndexProperty;
		wxPGProperty* m_enchantmentValueScaleProperty;
		wxPanel* m_visualAppearancePanel;
		wxStaticBitmap* m_visualAppearanceImage;
		
		// Virtual event handlers, overide them in your derived class
		virtual void artifactsOnDataViewCtrlSelectionChanged( wxDataViewEvent& event ) { event.Skip(); }
		virtual void artifactEnchantmentsOnDataViewCtrlSelectionChanged( wxDataViewEvent& event ) { event.Skip(); }
		virtual void propertyManagerOnPropertyGridChanged( wxPropertyGridEvent& event ) { event.Skip(); }
		
	
	public:
		
		StorageEditorPanelBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 905,551 ), long style = wxTAB_TRAVERSAL ); wxAuiManager m_mgr;
		
		~StorageEditorPanelBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class DummyForEmbeddedImages
///////////////////////////////////////////////////////////////////////////////
class DummyForEmbeddedImages : public wxPanel 
{
	private:
	
	protected:
		wxStaticBitmap* m_bitmap1;
	
	public:
		
		DummyForEmbeddedImages( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxTAB_TRAVERSAL ); 
		~DummyForEmbeddedImages();
	
};

#endif //__MAIN_H__
