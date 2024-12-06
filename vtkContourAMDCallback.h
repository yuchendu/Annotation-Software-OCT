#pragma once
#include <vtkCommand.h>
#include <vtkContourWidget.h>
#include <vtkContourRepresentation.h>
#include <vtkPolyDataToImageStencil.h>
#include <vtkImageStencilToImage.h>
#include <vtkImageAccumulate.h>
#include <vtkImageData.h>
#include <vtkSmartPointer.h>
class vtkContourAMDCallback : public vtkCommand
{
public:
	//vtkTypeMacro(vtkContourAMDCallback, vtkCommand);
	static  vtkContourAMDCallback* New()
	{
		return new vtkContourAMDCallback;
	}
	vtkContourAMDCallback();
	~vtkContourAMDCallback();

	//vtkSmartPointer<vtkPolyData> polyDataCurve;
	vtkSmartPointer<vtkImageStencilToImage> imageStencilToImage;
	vtkSmartPointer<vtkPolyDataToImageStencil> polyDataToImageStencil;

	vtkSmartPointer<vtkPolyData> polyDataCurve;
	//vtkSmartPointer<vtkImageData> imageData;

	virtual void Execute(vtkObject* caller, unsigned long, void*);

	//vtkContourAMDCallback() { }
}; 