#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MeasureOn6Disease.h"

#include <QVTKOpenGLNativeWidget.h>
#include <QFileDialog>
#include <qmessagebox.h>
#include <qbytearray.h>
#include <qsettings.h>

#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2); // VTK was built with vtkRenderingOpenGL2
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);
#include <vtkSmartPointer.h>
#include <vtkProperty.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkCamera.h>
#include <vtkActor.h>
#include <vtkActor2D.h>
#include <vtkRendererCollection.h>
#include <vtkEventQtSlotConnect.h>
#include <vtkOrientedGlyphContourRepresentation.h>
#include <vtkContourWidget.h>
#include <vtkContourAMDCallback.h>
#include <vtkContourDMECallback.h>
#include <vtkContourDRYCallback.h>
#include <vtkContourNMCallback.h>
#include <vtkContourPCVCallback.h>
#include <vtkContourPMCallback.h>


#define AMDContourDrawEvent (vtkCommand::UserEvent+1)
#define DMEContourDrawEvent (vtkCommand::UserEvent+2)
#define DRYContourDrawEvent (vtkCommand::UserEvent+3)
#define NMContourDrawEvent (vtkCommand::UserEvent+4)
#define PCVContourDrawEvent (vtkCommand::UserEvent+5)
#define PMContourDrawEvent (vtkCommand::UserEvent+6)

class MeasureOn6Disease : public QMainWindow
{
	Q_OBJECT

public:
	MeasureOn6Disease(QWidget *parent = Q_NULLPTR);

private:
	Ui::MeasureOn6DiseaseClass ui;

private:
	inline bool okToContinue();
	inline void loadFile(QString&);
	void OnResetScene();
	void OnCleanBuffer();

private slots:
	void OnFileOpen();
	void OnFileClose();
	void OnFileReset();
	void OnFileSave();
	void OnFileExit();

	void OnClickCheckResetColor();

	void OnClickAMD();
	void OnClickDME();
	void OnClickDRY();
	void OnClickNM();
	void OnClickPCV();
	void OnClickPM();

	void DrawOnAMDContour();
	void DrawOnDMEContour();
	void DrawOnDRYContour();
	void DrawOnNMContour();
	void DrawOnPCVContour();
	void DrawOnPMContour();

private:
	QSettings* m_psetting;
	QString lastPath;
	QFileInfo fileinfo;
	QString file_name;
	QString file_suffix;
	QString file_path;

private:

	vtkSmartPointer<vtkGenericOpenGLRenderWindow> pvtkGenericRenderWindow;
	vtkSmartPointer<vtkRenderWindow> pvtkRenderWindow;
	vtkSmartPointer<vtkRenderer> pvtkRenderer;
	vtkSmartPointer<vtkRenderWindowInteractor> pvtkInteractor;
	vtkSmartPointer<vtkInteractorStyleImage> pvtkImageStyle;

	vtkSmartPointer<vtkEventQtSlotConnect> vtkQtconnections;

	// widget regarding AMD callback function
	vtkSmartPointer<vtkContourWidget> contourAMDWidget;
	vtkSmartPointer<vtkContourAMDCallback> contourAMDCallback;

	// widget regarding DME callback function
	vtkSmartPointer<vtkContourWidget> contourDMEWidget;
	vtkSmartPointer<vtkContourDMECallback> contourDMECallback;

	// widget regarding DRY callback function
	vtkSmartPointer<vtkContourWidget> contourDRYWidget;
	vtkSmartPointer<vtkContourDRYCallback> contourDRYCallback;

	// widget regarding NM callback function
	vtkSmartPointer<vtkContourWidget> contourNMWidget;
	vtkSmartPointer<vtkContourNMCallback> contourNMCallback;

	// widget regarding PCV callback function
	vtkSmartPointer<vtkContourWidget> contourPCVWidget;
	vtkSmartPointer<vtkContourPCVCallback> contourPCVCallback;

	// widget regarding PM callback function
	vtkSmartPointer<vtkContourWidget> contourPMWidget;
	vtkSmartPointer<vtkContourPMCallback> contourPMCallback;
};
