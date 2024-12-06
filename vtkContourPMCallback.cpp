#include "MeasureOn6Disease.h"
#include "vtkContourPMCallback.h"
#include "ImageTIFF.h"

extern CImageTIFF o_ImageTIFF;

vtkContourPMCallback::vtkContourPMCallback()
{
	polyDataToImageStencil = NULL;
	imageStencilToImage = NULL;

	polyDataCurve = vtkSmartPointer<vtkPolyData>::New();
}


vtkContourPMCallback::~vtkContourPMCallback()
{

}


void vtkContourPMCallback::Execute(vtkObject* caller, unsigned long, void*)
{
	vtkContourWidget* myvtkContourWidget =
		reinterpret_cast<vtkContourWidget*> (caller);

	vtkContourRepresentation* contourRepresentation =
		static_cast<vtkContourRepresentation*> (myvtkContourWidget->GetRepresentation());

	if (myvtkContourWidget->GetWidgetState() != vtkContourWidget::Manipulate)
		return;

	// get the contour of PM
	o_ImageTIFF.setPMPolyData(contourRepresentation->GetContourRepresentationAsPolyData());
	// transfer the message to the main window to draw the contour of DME
	myvtkContourWidget->InvokeEvent(PMContourDrawEvent, NULL);


	// export the DME image to the imageData of o_ImageTIFF
	o_ImageTIFF.exportMaskImage(6);
}
