#pragma once
#include <vtkCommand.h>
#include <vtkContourWidget.h>
#include <vtkContourRepresentation.h>
#include <vtkPolyDataToImageStencil.h>
#include <vtkImageStencilToImage.h>
#include <vtkImageAccumulate.h>
#include <vtkImageData.h>
#include <vtkSmartPointer.h>
class vtkContourNMCallback : public vtkCommand
{
public:
	//vtkTypeMacro(vtkContourNMCallback, vtkCommand);
	static  vtkContourNMCallback* New()
	{
		return new vtkContourNMCallback;
	}
	vtkContourNMCallback();
	~vtkContourNMCallback();

	//vtkSmartPointer<vtkPolyData> polyDataCurve;
	vtkSmartPointer<vtkImageStencilToImage> imageStencilToImage;
	vtkSmartPointer<vtkPolyDataToImageStencil> polyDataToImageStencil;

	vtkSmartPointer<vtkPolyData> polyDataCurve;
	//vtkSmartPointer<vtkImageData> imageData;

	virtual void Execute(vtkObject* caller, unsigned long, void*);

	//vtkContourNMCallback() { }
}; 