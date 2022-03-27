//
// impressionistUI.h
//
// The header file for the UI part
//

#ifndef ImpressionistUI_h
#define ImpressionistUI_h

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/fl_file_chooser.H>		// FLTK file chooser
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Color_Chooser.H>

#include "Impressionist.h"
#include "OriginalView.h"
#include "PaintView.h"

#include "ImpBrush.h"
#include "LineBrush.h"

class ImpressionistUI {
public:
	ImpressionistUI();

	// The FLTK widgets
	Fl_Window*			m_mainWindow;
	Fl_Menu_Bar*		m_menubar;
	
	// The FLTK Color chooser for the b/w
	Fl_Color_Chooser*   m_colorChooser;
								
	PaintView*			m_paintView;
	OriginalView*		m_origView;

// for brush dialog
	Fl_Window*			m_brushDialog;
	Fl_Window*			m_colorDialog;

	Fl_Choice*			m_BrushTypeChoice;
	Fl_Choice*			m_DirectionTypeChoice;


	Fl_Slider*			m_BrushSizeSlider;
	
	Fl_Slider*			m_BrushLineWidthSlider;
	
	Fl_Slider*			m_BrushLineAngleSlider;
	
	Fl_Slider*			m_BrushAlphaValueSlider;

	Fl_Slider*			m_BrushAutoPaintSpaceSlider;

	Fl_Button*          m_ClearCanvasButton;

	Fl_Button*			m_AutoPaint;
	
	Fl_Button*			m_AutoPaintEdge;

	Fl_Button*			m_SwapViewsButton;

	Fl_Light_Button*	m_RandomSizeButton;

	Fl_Light_Button*	m_CustomEdgeClipButton;

	Fl_Light_Button*	m_EdgeClipButton;

	// Member functions
	void				setDocument(ImpressionistDoc* doc);
	ImpressionistDoc*	getDocument();

	void				show();
	void				resize_windows(int w, int h);

	// Interface to get attribute

	int					getSize();
	void				setSize(int size);
	int					getWidth();
	void				setWidth(int width);
	int					getAngle();
	void				setAngle(int angle);
	float				getAlpha();
	int					getSpace();
	int					getThreshold();
	void				setAlpha(float alpha);
	void				drawMarker(const Point& p);
	float				getRedScalar();
	float				getGreenScalar();
	float				getBlueScalar();
	bool				getSizeBool();
	bool				getCustomEdgeClip();
	bool				getEdgeClip();

private:
	ImpressionistDoc*	m_pDoc;		// pointer to document to communicate with the document

	// All attributes here
	int		m_nSize;
	int		m_nLineWidth;
	int		m_nLineAngle;
	float	m_nAlpha;
	int		m_nAutoSpace;
	int		m_nEdgeThreshold;
	bool	m_nSizeBool;
	bool	m_nCustomEdge;
	bool	m_nEdge;

	// Static class members
	static Fl_Menu_Item		menuitems[];
	static Fl_Menu_Item		brushTypeMenu[NUM_BRUSH_TYPE+1];
	static Fl_Menu_Item		DirectionTypeMenu[NUM_DIRECTION_TYPE+1];

	static ImpressionistUI*	whoami(Fl_Menu_* o);

	// All callbacks here.  Callbacks are declared 
	// static
	static void	cb_load_image(Fl_Menu_* o, void* v);
	static void	cb_save_image(Fl_Menu_* o, void* v);
	static void	cb_brushes(Fl_Menu_* o, void* v);
	static void cb_color_setting(Fl_Menu_* o, void* v);
	static void	cb_clear_canvas(Fl_Menu_* o, void* v);
	static void	cb_exit(Fl_Menu_* o, void* v);
	static void	cb_about(Fl_Menu_* o, void* v);
	static void	cb_brushChoice(Fl_Widget* o, void* v);
	static void	cb_StrokeDirectionChoice(Fl_Widget* o, void* v);
	static void	cb_clear_canvas_button(Fl_Widget* o, void* v);
	static void cb_auto_paint_button(Fl_Widget* o, void* v);
	static void cb_auto_paint_edge_button(Fl_Widget* o, void* v);
	static void	cb_sizeSlides(Fl_Widget* o, void* v);
	static void	cb_widthSlides(Fl_Widget* o, void* v);
	static void	cb_angleSlides(Fl_Widget* o, void* v);
	static void cb_alphaSlides(Fl_Widget* o, void* v);
	static void cb_AutoSpaceSlides(Fl_Widget* o, void* v);
	static void cb_EdgeThresholdSlides(Fl_Widget* o, void* v);
	static void cb_swap_views(Fl_Menu_* o, void* v);
	static void cb_undo(Fl_Menu_* o, void* v);
	static void cb_loadAnother(Fl_Menu_* o, void* v);
	static void cb_newMuralImage(Fl_Menu_* o, void* v);
	static void cb_load_alpha_brush(Fl_Menu_* o, void* v);
	static void cb_RandomSizeButton(Fl_Widget* o, void* v);
	static void cb_load_gradient_map(Fl_Menu_* o, void* v);
	static void cb_load_edge_map(Fl_Menu_* o, void* v);
	static void cb_CustomEdgeClipButton(Fl_Widget* o, void* v);
	static void cb_EdgeClipButton(Fl_Widget* o, void* v);
};

#endif
