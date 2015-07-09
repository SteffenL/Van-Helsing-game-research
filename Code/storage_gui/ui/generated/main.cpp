///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 17 2015)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "main.h"

///////////////////////////////////////////////////////////////////////////

MainFrameBase::MainFrameBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	m_menubar1 = new wxMenuBar( 0 );
	file = new wxMenu();
	wxMenuItem* open;
	open = new wxMenuItem( file, ID_OPEN, wxString( _("&Open...") ) + wxT('\t') + wxT("Ctrl+O"), wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	open->SetBitmaps( wxNullBitmap, wxNullBitmap );
	#endif
	file->Append( open );
	
	wxMenuItem* save;
	save = new wxMenuItem( file, ID_SAVE, wxString( _("&Save") ) + wxT('\t') + wxT("Ctrl+S"), wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	save->SetBitmaps( wxNullBitmap, wxNullBitmap );
	#endif
	file->Append( save );
	
	file->AppendSeparator();
	
	wxMenuItem* exit;
	exit = new wxMenuItem( file, ID_EXIT, wxString( _("E&xit") ) , wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	exit->SetBitmaps( wxNullBitmap, wxNullBitmap );
	#endif
	file->Append( exit );
	
	m_menubar1->Append( file, _("&File") ); 
	
	help = new wxMenu();
	wxMenuItem* about;
	about = new wxMenuItem( help, ID_ABOUT, wxString( _("&About") ) , wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	about->SetBitmaps( wxNullBitmap, wxNullBitmap );
	#endif
	help->Append( about );
	
	m_menubar1->Append( help, _("&Help") ); 
	
	this->SetMenuBar( m_menubar1 );
	
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_IDLE, wxIdleEventHandler( MainFrameBase::onIdle ) );
	this->Connect( open->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::openOnMenuSelection ) );
	this->Connect( save->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::saveOnMenuSelection ) );
	this->Connect( save->GetId(), wxEVT_UPDATE_UI, wxUpdateUIEventHandler( MainFrameBase::saveOnUpdateUI ) );
	this->Connect( exit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::exitOnMenuSelection ) );
	this->Connect( about->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::aboutOnMenuSelection ) );
}

MainFrameBase::~MainFrameBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_IDLE, wxIdleEventHandler( MainFrameBase::onIdle ) );
	this->Disconnect( ID_OPEN, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::openOnMenuSelection ) );
	this->Disconnect( ID_SAVE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::saveOnMenuSelection ) );
	this->Disconnect( ID_SAVE, wxEVT_UPDATE_UI, wxUpdateUIEventHandler( MainFrameBase::saveOnUpdateUI ) );
	this->Disconnect( ID_EXIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::exitOnMenuSelection ) );
	this->Disconnect( ID_ABOUT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::aboutOnMenuSelection ) );
	
}

StorageEditorPanelBase::StorageEditorPanelBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	m_mgr.SetManagedWindow(this);
	m_mgr.SetFlags(wxAUI_MGR_DEFAULT);
	
	m_panel5 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_mgr.AddPane( m_panel5, wxAuiPaneInfo() .Name( wxT("artifacts") ).Center() .Caption( _("Artifacts") ).CloseButton( false ).Dock().Resizable().FloatingSize( wxDefaultSize ).Floatable( false ).Layer( 10 ) );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxHORIZONTAL );
	
	m_artifactBagToolBar = new wxToolBar( m_panel5, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_FLAT|wxTB_NODIVIDER|wxTB_TEXT|wxTB_VERTICAL ); 
	m_artifactBagToolBar->Realize(); 
	
	bSizer6->Add( m_artifactBagToolBar, 0, wxEXPAND, 5 );
	
	m_artifacts = new wxDataViewCtrl( m_panel5, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxDV_MULTIPLE|wxDV_VERT_RULES|wxNO_BORDER );
	bSizer6->Add( m_artifacts, 1, wxEXPAND, 5 );
	
	
	m_panel5->SetSizer( bSizer6 );
	m_panel5->Layout();
	bSizer6->Fit( m_panel5 );
	m_panel6 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_mgr.AddPane( m_panel6, wxAuiPaneInfo() .Name( wxT("enchantments") ).Right() .Caption( _("Enchantments") ).CloseButton( false ).Movable( false ).Dock().Resizable().FloatingSize( wxSize( -1,-1 ) ).Floatable( false ).BestSize( wxSize( 280,-1 ) ).Layer( 2 ) );
	
	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxVERTICAL );
	
	m_artifactEnchantments = new wxDataViewCtrl( m_panel6, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxDV_MULTIPLE|wxDV_VERT_RULES|wxNO_BORDER );
	bSizer10->Add( m_artifactEnchantments, 1, wxEXPAND, 5 );
	
	
	m_panel6->SetSizer( bSizer10 );
	m_panel6->Layout();
	bSizer10->Fit( m_panel6 );
	m_panel7 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_mgr.AddPane( m_panel7, wxAuiPaneInfo() .Name( wxT("objectInspector") ).Right() .Caption( _("Object inspector") ).CloseButton( false ).Movable( false ).Dock().Resizable().FloatingSize( wxSize( -1,-1 ) ).Floatable( false ).BestSize( wxSize( 280,-1 ) ).Layer( 2 ) );
	
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxVERTICAL );
	
	m_propertyManager = new wxPropertyGridManager(m_panel7, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_HIDE_CATEGORIES|wxPG_HIDE_MARGIN|wxPG_SPLITTER_AUTO_CENTER|wxNO_BORDER);
	m_propertyManager->SetExtraStyle( wxPG_EX_INIT_NOCAT|wxPG_EX_NO_FLAT_TOOLBAR ); 
	
	m_propertiesEmptyPage = m_propertyManager->AddPage( wxEmptyString, wxNullBitmap );
	
	m_propertiesArtifactPage = m_propertyManager->AddPage( wxEmptyString, wxNullBitmap );
	m_artifactRarityProperty = m_propertiesArtifactPage->Append( new wxEnumProperty( _("Rarity"), _("Rarity") ) ); 
	m_artifactQualityProperty = m_propertiesArtifactPage->Append( new wxEnumProperty( _("Quality"), _("Quality") ) ); 
	m_artifactQuantityProperty = m_propertiesArtifactPage->Append( new wxIntProperty( _("Quantity"), _("Quantity") ) ); 
	m_artifactIdentifiedProperty = m_propertiesArtifactPage->Append( new wxBoolProperty( _("Identified"), _("Identified") ) ); 
	m_artifactProperty1Property = m_propertiesArtifactPage->Append( new wxIntProperty( _("Property 1"), _("Property 1") ) );
	m_propertiesArtifactPage->SetPropertyHelpString( m_artifactProperty1Property, _("Armor: Defense") );
	m_artifactProperty2Property = m_propertiesArtifactPage->Append( new wxIntProperty( _("Property 2"), _("Property 2") ) );
	m_propertiesArtifactPage->SetPropertyHelpString( m_artifactProperty2Property, _("Armor: Essence capacity") );
	
	m_propertiesEnchantmentPage = m_propertyManager->AddPage( wxEmptyString, wxNullBitmap );
	m_enchantmentValueIndexProperty = m_propertiesEnchantmentPage->Append( new wxIntProperty( _("Value index"), _("Value index") ) );
	m_propertiesEnchantmentPage->SetPropertyHelpString( m_enchantmentValueIndexProperty, _("An index in the min/max value array specified for this item.\n\nPossible range: 0 to length of array minus 1.\nArray length is typically between 1 and 16.\n\nOnly the min value is used when there is no max value.\n\nSince a bounds check is not done for the index in-game, exceeding the bounds can produce interesting values.") );
	m_enchantmentValueScaleProperty = m_propertiesEnchantmentPage->Append( new wxFloatProperty( _("Value scale"), _("Value scale") ) );
	m_propertiesEnchantmentPage->SetPropertyHelpString( m_enchantmentValueScaleProperty, _("A percentage that scales the value between the minimum and maximum value specified for this item.") );
	bSizer9->Add( m_propertyManager, 1, wxEXPAND, 5 );
	
	
	m_panel7->SetSizer( bSizer9 );
	m_panel7->Layout();
	bSizer9->Fit( m_panel7 );
	m_visualAppearancePanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_mgr.AddPane( m_visualAppearancePanel, wxAuiPaneInfo() .Name( wxT("visualAppearance") ).Right() .Caption( _("Visual appearance") ).CloseButton( false ).Movable( false ).Dock().Resizable().FloatingSize( wxSize( -1,-1 ) ).Floatable( false ).Row( 0 ).Position( 0 ).BestSize( wxSize( 280,-1 ) ).Layer( 2 ) );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );
	
	
	bSizer4->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_visualAppearanceImage = new wxStaticBitmap( m_visualAppearancePanel, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( m_visualAppearanceImage, 0, wxALIGN_CENTER_HORIZONTAL, 5 );
	
	
	bSizer4->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	m_visualAppearancePanel->SetSizer( bSizer4 );
	m_visualAppearancePanel->Layout();
	bSizer4->Fit( m_visualAppearancePanel );
	
	m_mgr.Update();
	
	// Connect Events
	m_artifacts->Connect( wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( StorageEditorPanelBase::artifactsOnDataViewCtrlSelectionChanged ), NULL, this );
	m_artifactEnchantments->Connect( wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( StorageEditorPanelBase::artifactEnchantmentsOnDataViewCtrlSelectionChanged ), NULL, this );
	m_propertyManager->Connect( wxEVT_PG_CHANGED, wxPropertyGridEventHandler( StorageEditorPanelBase::propertyManagerOnPropertyGridChanged ), NULL, this );
}

StorageEditorPanelBase::~StorageEditorPanelBase()
{
	// Disconnect Events
	m_artifacts->Disconnect( wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( StorageEditorPanelBase::artifactsOnDataViewCtrlSelectionChanged ), NULL, this );
	m_artifactEnchantments->Disconnect( wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( StorageEditorPanelBase::artifactEnchantmentsOnDataViewCtrlSelectionChanged ), NULL, this );
	m_propertyManager->Disconnect( wxEVT_PG_CHANGED, wxPropertyGridEventHandler( StorageEditorPanelBase::propertyManagerOnPropertyGridChanged ), NULL, this );
	
	m_mgr.UnInit();
	
}

DummyForEmbeddedImages::DummyForEmbeddedImages( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_bitmap1 = new wxStaticBitmap( this, wxID_ANY, wxBitmap( wxT("../images/inventory.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_bitmap1, 0, 0, 5 );
	
	
	this->SetSizer( fgSizer1 );
	this->Layout();
}

DummyForEmbeddedImages::~DummyForEmbeddedImages()
{
}
