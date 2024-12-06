#pragma once
#include <itkImage.h>
#include <itkJPEGImageIO.h>
#include <itkBMPImageIO.h>
#include <itkPNGImageIO.h>
#include <itkTIFFImageIO.h>
#include <itkImageFileReader.h>
#include <itkFlipImageFilter.h>
#include <itkImageToVTKImageFilter.h>
#include <itkRGBPixel.h>
#include <itkResampleImageFilter.h>
#include <itkAffineTransform.h>
#include <itkNearestNeighborInterpolateImageFunction.h>
#include <itkSimilarity2DTransform.h>
#include <itkBinaryThresholdImageFilter.h>
#include <itkBinaryDilateImageFilter.h>
#include <itkBinaryErodeImageFilter.h>
#include <itkBinaryBallStructuringElement.h>
#include <itkSmartPointer.h>
#include <itkImageAdaptor.h>
#include <itkImageRegionIteratorWithIndex.h>
#include <itkRescaleIntensityImageFilter.h>
#include <itkConnectedComponentImageFilter.h>

#include <vtkActor.h>
#include <vtkActor2D.h>
#include <vtkImageActor.h>
#include <vtkPolyData.h>
#include <vtkImageData.h>
#include <vtkImageGradient.h> 
#include <vtkImageMagnitude.h>
#include <vtkContourFilter.h>
#include <vtkImageThreshold.h>
#include <vtkPolyDataMapper.h>
#include <vtkSmartPointer.h>
#include <vtkProperty.h>
#include <vtkPolyDataNormals.h>
#include <vtkPolyDataConnectivityFilter.h>
#include <vtkImageConnectivityFilter.h>
#include <vtkPolyDataNormals.h>
#include <vtkImageViewer2.h>
#include <vtkImageThreshold.h>
#include <vtkImageMathematics.h>
#include <vtkImageCanvasSource2D.h>
#include <vtkTransform.h>
#include <vtkImageMask.h>
#include <vtkImageReslice.h>
#include <vtkExtractVOI.h>
#include <vtkImageAccumulate.h>
#include <vtkImageProperty.h>
#include <vtkPolyDataToImageStencil.h>
#include <vtkImageStencilToImage.h>

struct paramImageInfo_Stru
{
	int imageSizeOrgX;
	int imageSizeOrgY;
	int scaleRatio;
	int imageSizeCurrentX;
	int imageSizeCurrentY;
};

const unsigned int Dimension = 2;
typedef itk::RGBPixel<unsigned char>
itk_PixelType;
typedef itk::Image<itk_PixelType, Dimension>
itk_RGBType;
typedef itk::ImageFileReader<itk_RGBType>
itk_RGBReaderType;
typedef itk::FlipImageFilter<itk_RGBType>
itk_RGBFlipFilterType;
typedef itk_RGBFlipFilterType::FlipAxesArrayType
itk_RGBFlipAxesArrayType;
typedef itk::ImageToVTKImageFilter<itk_RGBType>
itk2vtk_RGBFilterType;

typedef itk::Image<unsigned char, Dimension>
itk_ImageType, InputImageType, OutputImageType;
typedef itk::FlipImageFilter<itk_ImageType>
itk_FlipFilterType;
typedef itk::ImageToVTKImageFilter<itk_ImageType>
itk2vtk_FilterType;
typedef itk::BinaryThresholdImageFilter<InputImageType, OutputImageType>
itk_BinaryThresholdFilterType;

typedef unsigned char InputPixelType;
typedef unsigned char OutputPixelType;
typedef itk::Image< InputPixelType, Dimension > InputImageType;
typedef itk::Image< OutputPixelType, Dimension > OutputImageType;
typedef itk::BinaryBallStructuringElement<
	InputPixelType,
	Dimension > StructuringElementType;
typedef itk::BinaryErodeImageFilter<
	InputImageType,
	OutputImageType,
	StructuringElementType > ErodeFilterType;
typedef itk::BinaryDilateImageFilter<
	InputImageType,
	OutputImageType,
	StructuringElementType > DilateFilterType;

typedef itk::ResampleImageFilter<itk_RGBType, itk_RGBType> ResampleFilterType;
typedef itk::AffineTransform< double, Dimension > TransformType;
typedef itk::NearestNeighborInterpolateImageFunction<
	itk_RGBType, double > InterpolatorType;
typedef itk::Similarity2DTransform< double > SimiTransformType;

class GreenChannelPixelAccessor
{
public:
	typedef itk::RGBPixel<unsigned char>   InternalType;
	typedef               unsigned char    ExternalType;

	static ExternalType Get(const InternalType& input)
	{
		return static_cast<ExternalType>(input.GetGreen());
	}
};

typedef itk::ImageAdaptor<  itk_RGBType,
	GreenChannelPixelAccessor > ImageAdaptorType;

typedef itk::RescaleIntensityImageFilter< ImageAdaptorType,
	itk_ImageType>   RescalerType;

class CImageTIFF
{
public:
	// image information
	itk_RGBType::PointType origin_temp;
	itk_RGBType::SpacingType spacing_temp;
	itk_RGBType::RegionType inputRegion;
	itk_RGBType::SizeType sizeImage;
	itk_RGBType::IndexType start;

	double s[3];
	double o[3];
	int* e;
	int* d;

	int sizeImageOrgX;
	int sizeImageOrgY;
	int sizeImageCurrentX;
	int sizeImageCurrentY;
	int scaleParam;


private:
	// RGB image object
	itk_RGBType* imageOrgRGB;
	itk_RGBType::Pointer imageRGB;
	itk_RGBReaderType::Pointer readerRGB;
	itk_RGBFlipFilterType::Pointer filterRGB;
	itk2vtk_RGBFilterType::Pointer connectorRGB;
	ResampleFilterType::Pointer ResamplefilterRGB;

	ImageAdaptorType::Pointer adaptor;
	RescalerType::Pointer rescaler;

	// gray image object
	itk_ImageType::Pointer imageOrg;
	itk_FlipFilterType::Pointer filter;
	itk2vtk_FilterType::Pointer connector;
	itk_BinaryThresholdFilterType::Pointer binaryThreshold;

	vtkSmartPointer<vtkImageActor> imageActor;


public:
	vtkSmartPointer<vtkImageStencilToImage> imageStencilToImage;
	vtkSmartPointer<vtkImageStencilToImage> imageStencilToImageWhole;
	vtkSmartPointer<vtkPolyDataToImageStencil> polyDataToImageStencil;
	vtkSmartPointer<vtkPolyDataToImageStencil> polyDataToImageStencilWhole;

	vtkSmartPointer<vtkPolyData> contourAMDData;
	vtkSmartPointer<vtkPolyData> contourDMEData;
	vtkSmartPointer<vtkPolyData> contourDRYData;
	vtkSmartPointer<vtkPolyData> contourNMData;
	vtkSmartPointer<vtkPolyData> contourPCVData;
	vtkSmartPointer<vtkPolyData> contourPMData;

	vtkSmartPointer<vtkPolyDataMapper> contourAMDMapper;
	vtkSmartPointer<vtkPolyDataMapper> contourDMEMapper;
	vtkSmartPointer<vtkPolyDataMapper> contourDRYMapper;
	vtkSmartPointer<vtkPolyDataMapper> contourNMMapper;
	vtkSmartPointer<vtkPolyDataMapper> contourPCVMapper;
	vtkSmartPointer<vtkPolyDataMapper> contourPMMapper;

	vtkSmartPointer<vtkActor> contourAMDActor;
	vtkSmartPointer<vtkActor> contourDMEActor;
	vtkSmartPointer<vtkActor> contourDRYActor;
	vtkSmartPointer<vtkActor> contourNMActor;
	vtkSmartPointer<vtkActor> contourPCVActor;
	vtkSmartPointer<vtkActor> contourPMActor;

public:// image
	vtkSmartPointer<vtkImageData> imageDataRGB;

	vtkSmartPointer<vtkImageData> imageDataAMD;
	vtkSmartPointer<vtkImageData> imageDataDME;
	vtkSmartPointer<vtkImageData> imageDataDRY;
	vtkSmartPointer<vtkImageData> imageDataNM;
	vtkSmartPointer<vtkImageData> imageDataPCV;
	vtkSmartPointer<vtkImageData> imageDataPM;

public:
	CImageTIFF();
	~CImageTIFF();

	bool ImageLoad(const CHAR*);
	bool ImageResampleRGB();

	bool ImageUnLoad();
	bool ImageReset();

	itk_RGBType::Pointer GetImageRGB();
	vtkSmartPointer<vtkImageActor> GetImageActor(itk_RGBType::Pointer);
	vtkSmartPointer<vtkImageActor> GetImageActor();

	vtkSmartPointer<vtkImageData> GetImageDataRGB();

	vtkSmartPointer<vtkImageData> GetImageDataAMD();
	vtkSmartPointer<vtkImageData> GetImageDataDME();
	vtkSmartPointer<vtkImageData> GetImageDataDRY();
	vtkSmartPointer<vtkImageData> GetImageDataNM();
	vtkSmartPointer<vtkImageData> GetImageDataPCV();
	vtkSmartPointer<vtkImageData> GetImageDataPM();


	void setAMDPolyData(vtkSmartPointer<vtkPolyData>);
	void setDMEPolyData(vtkSmartPointer<vtkPolyData>);
	void setDRYPolyData(vtkSmartPointer<vtkPolyData>);
	void setNMPolyData(vtkSmartPointer<vtkPolyData>);
	void setPCVPolyData(vtkSmartPointer<vtkPolyData>);
	void setPMPolyData(vtkSmartPointer<vtkPolyData>);

	void setAMDContourActor();
	void setDMEContourActor();
	void setDRYContourActor();
	void setNMContourActor();
	void setPCVContourActor();
	void setPMContourActor();

	vtkSmartPointer<vtkActor> getAMDContourActor();
	vtkSmartPointer<vtkActor> getDMEContourActor();
	vtkSmartPointer<vtkActor> getDRYContourActor();
	vtkSmartPointer<vtkActor> getNMContourActor();
	vtkSmartPointer<vtkActor> getPCVContourActor();
	vtkSmartPointer<vtkActor> getPMContourActor();


	void exportMaskImage(const unsigned char);
};


