#pragma once
#include <vtkCommand.h>
#include <vtkContourWidget.h>
#include <vtkContourRepresentation.h>
#include <vtkPolyDataToImageStencil.h>
#include <vtkImageStencilToImage.h>
#include <vtkImageAccumulate.h>
#include <vtkImageData.h>
#include <vtkSmartPointer.h>
class vtkContourDRYCallback : public vtkCommand
{
public:
	//vtkTypeMacro(vtkContourDRYCallback, vtkCommand);
	static  vtkContourDRYCallback* New()
	{
		return new vtkContourDRYCallback;
	}
	vtkContourDRYCallback();
	~vtkContourDRYCallback();

	//vtkSmartPointer<vtkPolyData> polyDataCurve;
	vtkSmartPointer<vtkImageStencilToImage> imageStencilToImage;
	vtkSmartPointer<vtkPolyDataToImageStencil> polyDataToImageStencil;

	vtkSmartPointer<vtkPolyData> polyDataCurve;
	//vtkSmartPointer<vtkImageData> imageData;

	virtual void Execute(vtkObject* caller, unsigned long, void*);

	//vtkContourDRYCallback() { }
}; 