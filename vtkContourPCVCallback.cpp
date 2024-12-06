#include "MeasureOn6Disease.h"
#include "vtkContourPCVCallback.h"
#include "ImageTIFF.h"

extern CImageTIFF o_ImageTIFF;

vtkContourPCVCallback::vtkContourPCVCallback()
{
	polyDataToImageStencil = NULL;
	imageStencilToImage = NULL;

	polyDataCurve = vtkSmartPointer<vtkPolyData>::New();
}


vtkContourPCVCallback::~vtkContourPCVCallback()
{

}


void vtkContourPCVCallback::Execute(vtkObject* caller, unsigned long, void*)
{
	vtkContourWidget* myvtkContourWidget =
		reinterpret_cast<vtkContourWidget*> (caller);

	vtkContourRepresentation* contourRepresentation =
		static_cast<vtkContourRepresentation*> (myvtkContourWidget->GetRepresentation());

	if (myvtkContourWidget->GetWidgetState() != vtkContourWidget::Manipulate)
		return;

	// get the contour of PCV
	o_ImageTIFF.setPCVPolyData(contourRepresentation->GetContourRepresentationAsPolyData());
	// transfer the message to the main window to draw the contour of DME
	myvtkContourWidget->InvokeEvent(PCVContourDrawEvent, NULL);


	// export the DME image to the imageData of o_ImageTIFF
	o_ImageTIFF.exportMaskImage(5);
}
