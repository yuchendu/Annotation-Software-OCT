#include "MeasureOn6Disease.h"
#include "vtkContourDRYCallback.h"
#include "ImageTIFF.h"

extern CImageTIFF o_ImageTIFF;

vtkContourDRYCallback::vtkContourDRYCallback()
{
	polyDataToImageStencil = NULL;
	imageStencilToImage = NULL;

	polyDataCurve = vtkSmartPointer<vtkPolyData>::New();
}


vtkContourDRYCallback::~vtkContourDRYCallback()
{

}


void vtkContourDRYCallback::Execute(vtkObject* caller, unsigned long, void*)
{
	vtkContourWidget* myvtkContourWidget =
		reinterpret_cast<vtkContourWidget*> (caller);

	vtkContourRepresentation* contourRepresentation =
		static_cast<vtkContourRepresentation*> (myvtkContourWidget->GetRepresentation());

	if (myvtkContourWidget->GetWidgetState() != vtkContourWidget::Manipulate)
		return;

	// get the contour of DRY
	o_ImageTIFF.setDRYPolyData(contourRepresentation->GetContourRepresentationAsPolyData());
	// transfer the message to the main window to draw the contour of DRY
	myvtkContourWidget->InvokeEvent(DRYContourDrawEvent, NULL);


	// export the DME image to the imageData of o_ImageTIFF
	o_ImageTIFF.exportMaskImage(3);
}
