#include "MeasureOn6Disease.h"
#include "vtkContourAMDCallback.h"
#include "ImageTIFF.h"

extern CImageTIFF o_ImageTIFF;

vtkContourAMDCallback::vtkContourAMDCallback()
{
	polyDataToImageStencil = NULL;
	imageStencilToImage = NULL;

	polyDataCurve = vtkSmartPointer<vtkPolyData>::New();
}


vtkContourAMDCallback::~vtkContourAMDCallback()
{

}


void vtkContourAMDCallback::Execute(vtkObject* caller, unsigned long, void*)
{
	vtkContourWidget* myvtkContourWidget =
		reinterpret_cast<vtkContourWidget*> (caller);

	vtkContourRepresentation* contourRepresentation =
		static_cast<vtkContourRepresentation*> (myvtkContourWidget->GetRepresentation());

	if (myvtkContourWidget->GetWidgetState() != vtkContourWidget::Manipulate)
		return;

	// get the contour of AMD
	o_ImageTIFF.setAMDPolyData(contourRepresentation->GetContourRepresentationAsPolyData());
	// transfer the message to the main window to draw the contour of DME
	myvtkContourWidget->InvokeEvent(AMDContourDrawEvent, NULL);

	
	// export the DME image to the imageData of o_ImageTIFF
	o_ImageTIFF.exportMaskImage(1);
}
