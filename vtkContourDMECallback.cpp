#include "MeasureOn6Disease.h"
#include "vtkContourDMECallback.h"
#include "ImageTIFF.h"

extern CImageTIFF o_ImageTIFF;

vtkContourDMECallback::vtkContourDMECallback()
{
	polyDataToImageStencil = NULL;
	imageStencilToImage = NULL;

	polyDataCurve = vtkSmartPointer<vtkPolyData>::New();
}


vtkContourDMECallback::~vtkContourDMECallback()
{

}


void vtkContourDMECallback::Execute(vtkObject* caller, unsigned long, void*)
{
	vtkContourWidget* myvtkContourWidget =
		reinterpret_cast<vtkContourWidget*> (caller);

	vtkContourRepresentation* contourRepresentation =
		static_cast<vtkContourRepresentation*> (myvtkContourWidget->GetRepresentation());

	if (myvtkContourWidget->GetWidgetState() != vtkContourWidget::Manipulate)
		return;

	// get the contour of DME
	o_ImageTIFF.setDMEPolyData(contourRepresentation->GetContourRepresentationAsPolyData());
	// transfer the message to the main window to draw the contour of DME
	myvtkContourWidget->InvokeEvent(DMEContourDrawEvent, NULL);


	// export the DME image to the imageData of o_ImageTIFF
	o_ImageTIFF.exportMaskImage(2);
}
