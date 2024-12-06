#include "MeasureOn6Disease.h"
#include "vtkContourNMCallback.h"
#include "ImageTIFF.h"

extern CImageTIFF o_ImageTIFF;

vtkContourNMCallback::vtkContourNMCallback()
{
	polyDataToImageStencil = NULL;
	imageStencilToImage = NULL;

	polyDataCurve = vtkSmartPointer<vtkPolyData>::New();
}


vtkContourNMCallback::~vtkContourNMCallback()
{

}


void vtkContourNMCallback::Execute(vtkObject* caller, unsigned long, void*)
{
	vtkContourWidget* myvtkContourWidget =
		reinterpret_cast<vtkContourWidget*> (caller);

	vtkContourRepresentation* contourRepresentation =
		static_cast<vtkContourRepresentation*> (myvtkContourWidget->GetRepresentation());

	if (myvtkContourWidget->GetWidgetState() != vtkContourWidget::Manipulate)
		return;

	/// get the contour of NM
	o_ImageTIFF.setNMPolyData(contourRepresentation->GetContourRepresentationAsPolyData());
	// transfer the message to the main window to draw the contour of DME
	myvtkContourWidget->InvokeEvent(NMContourDrawEvent, NULL);


	// export the DME image to the imageData of o_ImageTIFF
	o_ImageTIFF.exportMaskImage(4);
}
