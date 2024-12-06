
#include "ImageTIFF.h"

CImageTIFF::CImageTIFF()
{
	//itk::ObjectFactoryBase::RegisterFactory(itk::BMPImageIOFactory::New());

	imageRGB = NULL;
	imageOrgRGB = NULL;
	readerRGB = NULL;
	filterRGB = NULL;
	connectorRGB = NULL;
	ResamplefilterRGB = NULL;

	s[0] = 0; s[1] = 0; s[2] = 0;
	o[0] = 0; o[1] = 0; o[2] = 0;
	e = NULL;
	d = NULL;
	sizeImageOrgX = NULL;
	sizeImageOrgY = NULL;
	sizeImageCurrentX = NULL;
	sizeImageCurrentY = NULL;
	//scaleParam = xmlFileConfig.scale;
	scaleParam = 1;

	adaptor = NULL;
	rescaler = NULL;

	imageOrg = NULL;
	filter = NULL;
	connector = NULL;
	binaryThreshold = NULL;

	imageActor = NULL;

	imageDataRGB = NULL;

	imageStencilToImage = NULL;
	imageStencilToImageWhole = NULL;
	polyDataToImageStencil = NULL;
	polyDataToImageStencilWhole = NULL;

	contourAMDData = NULL;
	contourDMEData = NULL;
	contourDRYData = NULL;
	contourNMData = NULL;
	contourPCVData = NULL;
	contourPMData = NULL;

	contourAMDMapper = NULL;
	contourDMEMapper = NULL;
	contourDRYMapper = NULL;
	contourNMMapper = NULL;
	contourPCVMapper = NULL;
	contourPMMapper = NULL;

	contourAMDActor = NULL;
	contourDMEActor = NULL;
	contourDRYActor = NULL;
	contourNMActor = NULL;
	contourPCVActor = NULL;
	contourPMActor = NULL;

	imageDataAMD = NULL;
	imageDataDME = NULL;
	imageDataDRY = NULL;
	imageDataNM = NULL;
	imageDataPCV = NULL;
	imageDataPM = NULL;
}


CImageTIFF::~CImageTIFF()
{

}


bool CImageTIFF::ImageLoad(const CHAR* szFilePath)
{
	if (readerRGB)
	{
		// if Image has loaded, the first step should be to unload the image
		// pop out a dialog: save file? save image?
		// delete the pointer reader and etc..
		ImageUnLoad();
		//return FALSE;
	}

	if (!readerRGB)
		readerRGB = itk_RGBReaderType::New();
	readerRGB->SetFileName(szFilePath);
	readerRGB->SetImageIO(itk::TIFFImageIO::New());
	readerRGB->Update();

	imageOrgRGB = readerRGB->GetOutput();

	if (!imageRGB)
		imageRGB = itk_RGBType::New();
	//////////////////////////////////////////////////////////////////
	if (!ImageResampleRGB())
		return FALSE;

	return TRUE;
}


bool CImageTIFF::ImageResampleRGB()
{
	// resample the image
	if (!ResamplefilterRGB)
		ResamplefilterRGB = ResampleFilterType::New();
	int inputRegionX = imageOrgRGB->GetLargestPossibleRegion().GetSize()[0];
	int inputRegionY = imageOrgRGB->GetLargestPossibleRegion().GetSize()[1];
	int inputRegionZ = imageOrgRGB->GetLargestPossibleRegion().GetSize()[2];
	const double spacing[3] = { 1.0, 1.0, 1.0 };
	const double origin[3] = { 0.0, 0.0, 0.0 };
	ResamplefilterRGB->SetOutputSpacing(spacing);
	ResamplefilterRGB->SetOutputOrigin(origin);
	InputImageType::DirectionType direction;
	direction.SetIdentity();
	ResamplefilterRGB->SetOutputDirection(direction);

	sizeImageOrgX = inputRegionX;
	sizeImageOrgY = inputRegionY;


	SimiTransformType::Pointer transform = SimiTransformType::New();
	TransformType::OutputVectorType translation;
	SimiTransformType::InputPointType rotationCenter;
	rotationCenter[0] = origin[0] + spacing[0];
	rotationCenter[1] = origin[1] + spacing[1];
	transform->SetCenter(rotationCenter);


	InputImageType::SizeType size;


	translation[0] = 0; // X translation in millimeters
	translation[1] = 0; // Y translation in millimeters
	size[0] = inputRegionX; // number of pixels along X
	size[1] = inputRegionY; // number of pixels along Y

	transform->SetTranslation(translation);
	transform->SetScale(1);
	ResamplefilterRGB->SetTransform(transform);


	ResamplefilterRGB->SetSize(size);

	imageOrgRGB->SetOrigin(origin);
	imageOrgRGB->SetSpacing(spacing);
	ResamplefilterRGB->SetInput(imageOrgRGB);
	ResamplefilterRGB->Update();


	// allocate the new image
	origin_temp = ResamplefilterRGB->GetOutput()->GetOrigin();
	spacing_temp = ResamplefilterRGB->GetOutput()->GetSpacing();
	inputRegion = ResamplefilterRGB->GetOutput()->GetLargestPossibleRegion();
	sizeImage = inputRegion.GetSize();
	start = inputRegion.GetIndex();
	itk::ImageRegion<2> region(start, sizeImage);
	imageRGB->SetOrigin(origin_temp);
	imageRGB->SetSpacing(spacing_temp);
	imageRGB->SetRegions(region);
	imageRGB->Allocate();
	sizeImageCurrentX = sizeImage.GetSize()[0];
	sizeImageCurrentY = sizeImage.GetSize()[1];

	// value the new image
	imageRGB->Graft(ResamplefilterRGB->GetOutput());
	//imageRGB->Graft(imageOrgRGB);


	return TRUE;
}

bool CImageTIFF::ImageReset()
{
	if (adaptor)
		adaptor = NULL;

	if (rescaler)
		rescaler = NULL;

	if (imageOrg)
		imageOrg = NULL; ;

	if (filter)
		filter = NULL;

	if (connector)
		connector = NULL;

	imageDataRGB = NULL;

	imageStencilToImage = NULL;
	imageStencilToImageWhole = NULL;
	polyDataToImageStencil = NULL;
	polyDataToImageStencilWhole = NULL;

	contourAMDData = NULL;
	contourDMEData = NULL;
	contourDRYData = NULL;
	contourNMData = NULL;
	contourPCVData = NULL;
	contourPMData = NULL;

	contourAMDMapper = NULL;
	contourDMEMapper = NULL;
	contourDRYMapper = NULL;
	contourNMMapper = NULL;
	contourPCVMapper = NULL;
	contourPMMapper = NULL;

	contourAMDActor = NULL;
	contourDMEActor = NULL;
	contourDRYActor = NULL;
	contourNMActor = NULL;
	contourPCVActor = NULL;
	contourPMActor = NULL;

	return TRUE;
}

bool CImageTIFF::ImageUnLoad()
{
	ImageReset();

	if (imageRGB)
		imageRGB = NULL;

	if (imageOrgRGB)
		imageOrgRGB = NULL;

	if (readerRGB)
		readerRGB = NULL;

	if (filterRGB)
		filterRGB = NULL;

	if (connectorRGB)
		connectorRGB = NULL;

	if (ResamplefilterRGB)
		ResamplefilterRGB = NULL;

	if (imageDataRGB)
		imageDataRGB = NULL;

	if (imageActor)
		imageActor = NULL;

	sizeImageOrgX = NULL;
	sizeImageOrgY = NULL;
	sizeImageCurrentX = NULL;
	sizeImageCurrentY = NULL;

	imageDataAMD = NULL;
	imageDataDME = NULL;
	imageDataDRY = NULL;
	imageDataNM = NULL;
	imageDataPCV = NULL;
	imageDataPM = NULL;

	return TRUE;
}


itk_RGBType::Pointer CImageTIFF::GetImageRGB()
{
	if (!imageRGB)
	{
		//return NULL;
	}
	return imageRGB;
}


vtkSmartPointer<vtkImageActor> CImageTIFF::GetImageActor()
{
	if (!imageActor)
		return NULL;
	return imageActor;
}


vtkSmartPointer<vtkImageActor> CImageTIFF::GetImageActor(itk_RGBType::Pointer imageInput)
{
	itk_RGBFlipAxesArrayType flipArray;
	flipArray[0] = 0;
	flipArray[1] = 1;

	if (!filterRGB)
		filterRGB = itk_RGBFlipFilterType::New();
	filterRGB->SetFlipAxes(flipArray);
	filterRGB->SetInput(imageInput);
	filterRGB->Update();

	if (!connectorRGB)
		connectorRGB = itk2vtk_RGBFilterType::New();
	connectorRGB->SetInput(filterRGB->GetOutput());
	connectorRGB->Update();

	//double org[3] = { 0 };
	//connectorRGB->GetOutput()->SetOrigin(org);

	connectorRGB->GetOutput()->GetSpacing(s);
	connectorRGB->GetOutput()->GetOrigin(o);
	d = connectorRGB->GetOutput()->GetDimensions();
	e = connectorRGB->GetOutput()->GetExtent();
	////////////////////////////get vtkImageData/////////////////////////

	if (!imageDataRGB)
		imageDataRGB = vtkSmartPointer<vtkImageData>::New();
	imageDataRGB->SetSpacing(s);
	imageDataRGB->SetDimensions(d);
	imageDataRGB->SetOrigin(o);
	imageDataRGB->SetExtent(e);
	imageDataRGB->AllocateScalars(VTK_UNSIGNED_CHAR, 1);

	imageDataRGB->DeepCopy(connectorRGB->GetOutput());

	if (!imageDataAMD)
		imageDataAMD = vtkSmartPointer<vtkImageData>::New();
	imageDataAMD->SetSpacing(s);
	imageDataAMD->SetDimensions(d);
	imageDataAMD->SetOrigin(o);
	imageDataAMD->SetExtent(e);
	imageDataAMD->AllocateScalars(VTK_UNSIGNED_CHAR, 1);

	if (!imageDataDME)
		imageDataDME = vtkSmartPointer<vtkImageData>::New();
	imageDataDME->SetSpacing(s);
	imageDataDME->SetDimensions(d);
	imageDataDME->SetOrigin(o);
	imageDataDME->SetExtent(e);
	imageDataDME->AllocateScalars(VTK_UNSIGNED_CHAR, 1);

	if (!imageDataDRY)
		imageDataDRY = vtkSmartPointer<vtkImageData>::New();
	imageDataDRY->SetSpacing(s);
	imageDataDRY->SetDimensions(d);
	imageDataDRY->SetOrigin(o);
	imageDataDRY->SetExtent(e);
	imageDataDRY->AllocateScalars(VTK_UNSIGNED_CHAR, 1);

	if (!imageDataNM)
		imageDataNM = vtkSmartPointer<vtkImageData>::New();
	imageDataNM->SetSpacing(s);
	imageDataNM->SetDimensions(d);
	imageDataNM->SetOrigin(o);
	imageDataNM->SetExtent(e);
	imageDataNM->AllocateScalars(VTK_UNSIGNED_CHAR, 1);

	if (!imageDataPCV)
		imageDataPCV = vtkSmartPointer<vtkImageData>::New();
	imageDataPCV->SetSpacing(s);
	imageDataPCV->SetDimensions(d);
	imageDataPCV->SetOrigin(o);
	imageDataPCV->SetExtent(e);
	imageDataPCV->AllocateScalars(VTK_UNSIGNED_CHAR, 1);

	if (!imageDataPM)
		imageDataPM = vtkSmartPointer<vtkImageData>::New();
	imageDataPM->SetSpacing(s);
	imageDataPM->SetDimensions(d);
	imageDataPM->SetOrigin(o);
	imageDataPM->SetExtent(e);
	imageDataPM->AllocateScalars(VTK_UNSIGNED_CHAR, 1);

	if (!imageActor)
		imageActor = vtkImageActor::New();
	imageActor->SetInputData(connectorRGB->GetOutput());
	imageActor->Update();

	return imageActor;
}

vtkSmartPointer<vtkImageData> CImageTIFF::GetImageDataRGB()
{
	if (!imageDataRGB)
		return NULL;
	return imageDataRGB;
}


void CImageTIFF::setAMDPolyData(vtkSmartPointer<vtkPolyData> polyData)
{
	contourAMDData = NULL;
	contourAMDData = vtkSmartPointer<vtkPolyData>::New();

	contourAMDData->DeepCopy(polyData);
}

void CImageTIFF::setDMEPolyData(vtkSmartPointer<vtkPolyData> polyData)
{
	contourDMEData = NULL;
	contourDMEData = vtkSmartPointer<vtkPolyData>::New();

	contourDMEData->DeepCopy(polyData);
}

void CImageTIFF::setDRYPolyData(vtkSmartPointer<vtkPolyData> polyData)
{
	contourDRYData = NULL;
	contourDRYData = vtkSmartPointer<vtkPolyData>::New();

	contourDRYData->DeepCopy(polyData);
}

void CImageTIFF::setNMPolyData(vtkSmartPointer<vtkPolyData> polyData)
{
	contourNMData = NULL;
	contourNMData = vtkSmartPointer<vtkPolyData>::New();

	contourNMData->DeepCopy(polyData);
}

void CImageTIFF::setPCVPolyData(vtkSmartPointer<vtkPolyData> polyData)
{
	contourPCVData = NULL;
	contourPCVData = vtkSmartPointer<vtkPolyData>::New();

	contourPCVData->DeepCopy(polyData);
}

void CImageTIFF::setPMPolyData(vtkSmartPointer<vtkPolyData> polyData)
{
	contourPMData = NULL;
	contourPMData = vtkSmartPointer<vtkPolyData>::New();

	contourPMData->DeepCopy(polyData);
}

void CImageTIFF::exportMaskImage(const unsigned char Index)
{
	if (!polyDataToImageStencil)
		polyDataToImageStencil = vtkSmartPointer<vtkPolyDataToImageStencil>::New();
	if (!polyDataToImageStencilWhole)
		polyDataToImageStencilWhole = vtkSmartPointer<vtkPolyDataToImageStencil>::New();
	if (!imageStencilToImage)
		imageStencilToImage = vtkSmartPointer<vtkImageStencilToImage>::New();
	if (!imageStencilToImageWhole)
		imageStencilToImageWhole = vtkSmartPointer<vtkImageStencilToImage>::New();

	polyDataToImageStencil->SetTolerance(0);
	polyDataToImageStencilWhole->SetTolerance(0);



	switch (Index)
	{
	case 1:
		polyDataToImageStencil->SetInputData(contourAMDData);
		polyDataToImageStencil->SetOutputOrigin(imageDataAMD->GetOrigin());
		polyDataToImageStencil->SetOutputSpacing(imageDataAMD->GetSpacing());
		polyDataToImageStencil->SetOutputWholeExtent(imageDataAMD->GetExtent());
		polyDataToImageStencil->Update();

		imageStencilToImage->SetInputConnection(polyDataToImageStencil->GetOutputPort());
		imageStencilToImage->SetInsideValue(255);
		imageStencilToImage->SetOutsideValue(0);
		imageStencilToImage->Update();

		imageDataAMD->DeepCopy(imageStencilToImage->GetOutput());
		break;
	case 2:
		polyDataToImageStencil->SetInputData(contourDMEData);
		polyDataToImageStencil->SetOutputOrigin(imageDataDME->GetOrigin());
		polyDataToImageStencil->SetOutputSpacing(imageDataDME->GetSpacing());
		polyDataToImageStencil->SetOutputWholeExtent(imageDataDME->GetExtent());
		polyDataToImageStencil->Update();

		imageStencilToImage->SetInputConnection(polyDataToImageStencil->GetOutputPort());
		imageStencilToImage->SetInsideValue(255);
		imageStencilToImage->SetOutsideValue(0);
		imageStencilToImage->Update();

		imageDataDME->DeepCopy(imageStencilToImage->GetOutput());
		break;
	case 3:
		polyDataToImageStencil->SetInputData(contourDRYData);
		polyDataToImageStencil->SetOutputOrigin(imageDataDRY->GetOrigin());
		polyDataToImageStencil->SetOutputSpacing(imageDataDRY->GetSpacing());
		polyDataToImageStencil->SetOutputWholeExtent(imageDataDRY->GetExtent());
		polyDataToImageStencil->Update();

		imageStencilToImage->SetInputConnection(polyDataToImageStencil->GetOutputPort());
		imageStencilToImage->SetInsideValue(255);
		imageStencilToImage->SetOutsideValue(0);
		imageStencilToImage->Update();

		imageDataDRY->DeepCopy(imageStencilToImage->GetOutput());
		break;
	case 4:
		polyDataToImageStencil->SetInputData(contourNMData);
		polyDataToImageStencil->SetOutputOrigin(imageDataNM->GetOrigin());
		polyDataToImageStencil->SetOutputSpacing(imageDataNM->GetSpacing());
		polyDataToImageStencil->SetOutputWholeExtent(imageDataNM->GetExtent());
		polyDataToImageStencil->Update();

		imageStencilToImage->SetInputConnection(polyDataToImageStencil->GetOutputPort());
		imageStencilToImage->SetInsideValue(255);
		imageStencilToImage->SetOutsideValue(0);
		imageStencilToImage->Update();

		imageDataNM->DeepCopy(imageStencilToImage->GetOutput());
		break;
	case 5:
		polyDataToImageStencil->SetInputData(contourPCVData);
		polyDataToImageStencil->SetOutputOrigin(imageDataPCV->GetOrigin());
		polyDataToImageStencil->SetOutputSpacing(imageDataPCV->GetSpacing());
		polyDataToImageStencil->SetOutputWholeExtent(imageDataPCV->GetExtent());
		polyDataToImageStencil->Update();

		imageStencilToImage->SetInputConnection(polyDataToImageStencil->GetOutputPort());
		imageStencilToImage->SetInsideValue(255);
		imageStencilToImage->SetOutsideValue(0);
		imageStencilToImage->Update();

		imageDataPCV->DeepCopy(imageStencilToImage->GetOutput());
		break;
	case 6:
		polyDataToImageStencil->SetInputData(contourPMData);
		polyDataToImageStencil->SetOutputOrigin(imageDataPM->GetOrigin());
		polyDataToImageStencil->SetOutputSpacing(imageDataPM->GetSpacing());
		polyDataToImageStencil->SetOutputWholeExtent(imageDataPM->GetExtent());
		polyDataToImageStencil->Update();

		imageStencilToImage->SetInputConnection(polyDataToImageStencil->GetOutputPort());
		imageStencilToImage->SetInsideValue(255);
		imageStencilToImage->SetOutsideValue(0);
		imageStencilToImage->Update();

		imageDataPM->DeepCopy(imageStencilToImage->GetOutput());
		break;
	default:
		break;
	}
}

void CImageTIFF::setAMDContourActor()
{
	// display disk contour
	if (!contourAMDMapper)
		contourAMDMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	contourAMDMapper->SetInputData(contourAMDData);
	contourAMDMapper->Update();

	if (!contourAMDActor)
		contourAMDActor = vtkSmartPointer<vtkActor>::New();
	contourAMDActor->SetMapper(contourAMDMapper);
	contourAMDActor->GetProperty()->SetColor(0, 0, 0.9);
	contourAMDActor->GetProperty()->SetLineWidth(2);
}

vtkSmartPointer<vtkActor> CImageTIFF::getAMDContourActor()
{
	return contourAMDActor;
}

void CImageTIFF::setDMEContourActor()
{
	// display disk contour
	if (!contourDMEMapper)
		contourDMEMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	contourDMEMapper->SetInputData(contourDMEData);
	contourDMEMapper->Update();

	if (!contourDMEActor)
		contourDMEActor = vtkSmartPointer<vtkActor>::New();
	contourDMEActor->SetMapper(contourDMEMapper);
	contourDMEActor->GetProperty()->SetColor(0, 0.9, 0);
	contourDMEActor->GetProperty()->SetLineWidth(2);
}

vtkSmartPointer<vtkActor> CImageTIFF::getDMEContourActor()
{
	return contourDMEActor;
}

void CImageTIFF::setDRYContourActor()
{
	// display disk contour
	if (!contourDRYMapper)
		contourDRYMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	contourDRYMapper->SetInputData(contourDRYData);
	contourDRYMapper->Update();

	if (!contourDRYActor)
		contourDRYActor = vtkSmartPointer<vtkActor>::New();
	contourDRYActor->SetMapper(contourDRYMapper);
	contourDRYActor->GetProperty()->SetColor(0.9, 0, 0);
	contourDRYActor->GetProperty()->SetLineWidth(2);
}

vtkSmartPointer<vtkActor> CImageTIFF::getDRYContourActor()
{
	return contourDRYActor;
}

void CImageTIFF::setNMContourActor()
{
	// display disk contour
	if (!contourNMMapper)
		contourNMMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	contourNMMapper->SetInputData(contourNMData);
	contourNMMapper->Update();

	if (!contourNMActor)
		contourNMActor = vtkSmartPointer<vtkActor>::New();
	contourNMActor->SetMapper(contourNMMapper);
	contourNMActor->GetProperty()->SetColor(0, 0.9, 0.9);
	contourNMActor->GetProperty()->SetLineWidth(2);
}

vtkSmartPointer<vtkActor> CImageTIFF::getNMContourActor()
{
	return contourNMActor;
}

void CImageTIFF::setPCVContourActor()
{
	// display disk contour
	if (!contourPCVMapper)
		contourPCVMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	contourPCVMapper->SetInputData(contourPCVData);
	contourPCVMapper->Update();

	if (!contourPCVActor)
		contourPCVActor = vtkSmartPointer<vtkActor>::New();
	contourPCVActor->SetMapper(contourPCVMapper);
	contourPCVActor->GetProperty()->SetColor(0.9, 0.9, 0);
	contourPCVActor->GetProperty()->SetLineWidth(2);
}

vtkSmartPointer<vtkActor> CImageTIFF::getPCVContourActor()
{
	return contourPCVActor;
}

void CImageTIFF::setPMContourActor()
{
	// display disk contour
	if (!contourPMMapper)
		contourPMMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	contourPMMapper->SetInputData(contourPMData);
	contourPMMapper->Update();

	if (!contourPMActor)
		contourPMActor = vtkSmartPointer<vtkActor>::New();
	contourPMActor->SetMapper(contourPMMapper);
	contourPMActor->GetProperty()->SetColor(0.9, 0, 0.9);
	contourPMActor->GetProperty()->SetLineWidth(2);
}

vtkSmartPointer<vtkActor> CImageTIFF::getPMContourActor()
{
	return contourPMActor;
}

vtkSmartPointer<vtkImageData> CImageTIFF::GetImageDataAMD()
{
	return imageDataAMD;
}

vtkSmartPointer<vtkImageData> CImageTIFF::GetImageDataDME()
{
	return imageDataDME;
}

vtkSmartPointer<vtkImageData> CImageTIFF::GetImageDataDRY()
{
	return imageDataDRY;
}

vtkSmartPointer<vtkImageData> CImageTIFF::GetImageDataNM()
{
	return imageDataNM;
}

vtkSmartPointer<vtkImageData> CImageTIFF::GetImageDataPCV()
{
	return imageDataPCV;
}

vtkSmartPointer<vtkImageData> CImageTIFF::GetImageDataPM()
{
	return imageDataPM;
}