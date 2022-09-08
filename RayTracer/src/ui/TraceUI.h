//
// rayUI.h
//
// The header file for the UI part
//

#ifndef __rayUI_h__
#define __rayUI_h__

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Button.H>

#include <FL/fl_file_chooser.H>		// FLTK file chooser

#include "TraceGLWindow.h"

class TraceUI {
public:
	TraceUI();

	// The FLTK widgets
	Fl_Window*			m_mainWindow;
	Fl_Menu_Bar*		m_menubar;

	Fl_Slider*			m_sizeSlider;
	Fl_Slider*			m_depthSlider;
	Fl_Slider* m_thresholdSlider;
	Fl_Slider* m_constantAttenuationSlider;
	Fl_Slider* m_linearAttenuationSlider;
	Fl_Slider* m_quadricAttenuationSlider;
	Fl_Slider* m_ambientLightSlider;
	Fl_Slider* m_nSupPixelSlider;
	Fl_Light_Button* m_adaptSupSwitch;
	Fl_Light_Button* m_backgroundSwitch;

	Fl_Button*			m_renderButton;
	Fl_Button*			m_stopButton;
	Fl_Light_Button* m_superSampSwitch;

	TraceGLWindow*		m_traceGlWindow;

	// member functions
	void show();

	void		setRayTracer(RayTracer *tracer);

	int			getSize();
	int			getDepth();
	double		getThreshold();
	double		m_nConstantAtten;
	double		m_nLinearAtten;
	double		m_nQuadricAtten;
	double		m_nAmbientLight;

	int supSampleSize() {
		return m_nSupPixel;
	}
	bool superSamper() {
		return  m_superSamp;
	}
	bool adaptSup() {
		return m_adaptSup;
	}
	bool useBackground() {
		return m_background;
	}
private:
	RayTracer*	raytracer;

	int			m_nSize;
	int			m_nDepth;
	int m_nSupPixel;
	bool 		m_superSamp;
	bool 		m_adaptSup;
	bool m_background;
	double m_nthreshold;
// static class members
	static Fl_Menu_Item menuitems[];

	static TraceUI* whoami(Fl_Menu_* o);

	static void cb_load_scene(Fl_Menu_* o, void* v);
	static void cb_save_image(Fl_Menu_* o, void* v);
	static void cb_exit(Fl_Menu_* o, void* v);
	static void cb_about(Fl_Menu_* o, void* v);

	static void cb_exit2(Fl_Widget* o, void* v);

	static void cb_sizeSlides(Fl_Widget* o, void* v);
	static void cb_depthSlides(Fl_Widget* o, void* v);
	static void cb_thresholdSlides(Fl_Widget* o, void* v);
	static void cb_constantAttenSlides(Fl_Widget* o, void* v);
	static void cb_linearAttenSlides(Fl_Widget* o, void* v);
	static void cb_quadricAttenSlides(Fl_Widget* o, void* v);
	static void cb_ambientLightSlides(Fl_Widget* o, void* v);
	static void cb_num_sup_sampSlides(Fl_Widget* o, void* v);
	static void cb_superSampSwitch(Fl_Widget* o, void* v);
	static void cb_adaptSampSwitch(Fl_Widget* o, void* v);
	static void cb_load_background(Fl_Menu_* o, void* v);
	static void cb_backSwitch(Fl_Widget* o, void* v);

	static void cb_render(Fl_Widget* o, void* v);
	static void cb_stop(Fl_Widget* o, void* v);
	
};

#endif
