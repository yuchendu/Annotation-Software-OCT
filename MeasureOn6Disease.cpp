#include "MeasureOn6Disease.h"
#include "ImageTIFF.h"
#include <vtkTIFFWriter.h>

CImageTIFF o_ImageTIFF;

MeasureOn6Disease::MeasureOn6Disease(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	pvtkGenericRenderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
	pvtkRenderer = vtkSmartPointer<vtkRenderer>::New();
	pvtkInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	pvtkImageStyle = vtkSmartPointer<vtkInteractorStyleImage>::New();


	ui.pvtkQtWidget->SetRenderWindow(pvtkGenericRenderWindow);
	vtkSmartPointer<vtkRenderWindow> addrGenericWindow = ui.pvtkQtWidget->GetRenderWindow();
	ui.pvtkQtWidget->GetRenderWindow()->AddRenderer(pvtkRenderer);
	pvtkInteractor->SetInteractorStyle(pvtkImageStyle);
	ui.pvtkQtWidget->GetRenderWindow()->SetInteractor(pvtkInteractor);
	pvtkRenderer->SetBackground(0, 0, 0);
	ui.pvtkQtWidget->GetRenderWindow()->Render();

	contourAMDWidget = NULL;
	contourAMDCallback = NULL;


	// menu connect
	connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(OnFileOpen()));
	connect(ui.actionClose, SIGNAL(triggered()), this, SLOT(OnFileClose()));
	connect(ui.actionReset, SIGNAL(triggered()), this, SLOT(OnFileReset()));
	connect(ui.actionSave, SIGNAL(triggered()), this, SLOT(OnFileRSave()));
	connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(OnFileExit()));


	// button trigger callback function
	connect(ui.buttonResetColor, SIGNAL(click()), this, SLOT(OnClickCheckResetColor()));
	connect(ui.buttonResetWindow, SIGNAL(click()), this, SLOT(OnFileReset()));
	connect(ui.buttonClose, SIGNAL(click()), this, SLOT(OnFileClose()));
	connect(ui.buttonSave, SIGNAL(click()), this, SLOT(OnFileSave()));


	// button trigger callback function
	connect(ui.buttonAMD, SIGNAL(click(bool)), this, SLOT(OnClickAMD()));
	connect(ui.buttonDME, SIGNAL(click(bool)), this, SLOT(OnClickDME()));
	connect(ui.buttonDRY, SIGNAL(click(bool)), this, SLOT(OnClickDRY()));
	connect(ui.buttonNM, SIGNAL(click(bool)), this, SLOT(OnClickNM()));
	connect(ui.buttonPCV, SIGNAL(click(bool)), this, SLOT(OnClickPCV()));
	connect(ui.buttonPM, SIGNAL(click(bool)), this, SLOT(OnClickPM()));


	// message connect between vtk and Qt window
	vtkQtconnections = vtkSmartPointer<vtkEventQtSlotConnect>::New();
}

void MeasureOn6Disease::OnCleanBuffer()
{
	ui.buttonAMD->setChecked(false);
	ui.buttonDME->setChecked(false);
	ui.buttonDRY->setChecked(false);
	ui.buttonNM->setChecked(false);
	ui.buttonPCV->setChecked(false);
	ui.buttonPM->setChecked(false);
	OnClickAMD();
	OnClickDME();
	OnClickDRY();
	OnClickNM();
	OnClickPCV();
	OnClickPM();
	contourAMDWidget = NULL;
	contourDMEWidget = NULL;
	contourDRYWidget = NULL;
	contourNMWidget = NULL;
	contourPCVWidget = NULL;
	contourPMWidget = NULL;
	contourAMDCallback = NULL;
	contourDMECallback = NULL;
	contourDRYCallback = NULL;
	contourNMCallback = NULL;
	contourPCVCallback = NULL;
	contourPMCallback = NULL;

	pvtkRenderer->RemoveActor2D(o_ImageTIFF.getAMDContourActor());
	pvtkRenderer->RemoveActor2D(o_ImageTIFF.getDMEContourActor());
	pvtkRenderer->RemoveActor2D(o_ImageTIFF.getDRYContourActor());
	pvtkRenderer->RemoveActor2D(o_ImageTIFF.getNMContourActor());
	pvtkRenderer->RemoveActor2D(o_ImageTIFF.getPCVContourActor());
	pvtkRenderer->RemoveActor2D(o_ImageTIFF.getPMContourActor());
	o_ImageTIFF.ImageReset();

	pvtkRenderer->ResetCamera();
	pvtkRenderer->GetActiveCamera()->ParallelProjectionOn();
	
	ui.pvtkQtWidget->GetRenderWindow()->Render();
}

void MeasureOn6Disease::OnResetScene()
{
	ui.buttonAMD->setChecked(false);
	ui.buttonDME->setChecked(false);
	ui.buttonDRY->setChecked(false);
	ui.buttonNM->setChecked(false);
	ui.buttonPCV->setChecked(false);
	ui.buttonPM->setChecked(false);
	OnClickAMD();
	OnClickDME();
	OnClickDRY();
	OnClickNM();
	OnClickPCV();
	OnClickPM();
	contourAMDWidget = NULL;
	contourDMEWidget = NULL;
	contourDRYWidget = NULL;
	contourNMWidget = NULL;
	contourPCVWidget = NULL;
	contourPMWidget = NULL;
	contourAMDCallback = NULL;
	contourDMECallback = NULL;
	contourDRYCallback = NULL;
	contourNMCallback = NULL;
	contourPCVCallback = NULL;
	contourPMCallback = NULL;

	pvtkRenderer->RemoveActor2D(o_ImageTIFF.getAMDContourActor());
	pvtkRenderer->RemoveActor2D(o_ImageTIFF.getDMEContourActor());
	pvtkRenderer->RemoveActor2D(o_ImageTIFF.getDRYContourActor());
	pvtkRenderer->RemoveActor2D(o_ImageTIFF.getNMContourActor());
	pvtkRenderer->RemoveActor2D(o_ImageTIFF.getPCVContourActor());
	pvtkRenderer->RemoveActor2D(o_ImageTIFF.getPMContourActor());

	pvtkRenderer->RemoveActor2D(o_ImageTIFF.GetImageActor());
	o_ImageTIFF.ImageUnLoad();

	pvtkRenderer->ResetCamera();
	pvtkRenderer->GetActiveCamera()->ParallelProjectionOn();
	
	ui.pvtkQtWidget->GetRenderWindow()->Render();
}

bool MeasureOn6Disease::okToContinue()
{
	return TRUE;
}

void MeasureOn6Disease::loadFile(QString& filename)
{
	QByteArray ba = filename.toLocal8Bit(); // must
	const char* ch = ba.data();
	if (o_ImageTIFF.ImageLoad(ch))
	{
		pvtkRenderer->AddActor2D(o_ImageTIFF.GetImageActor(o_ImageTIFF.GetImageRGB()));
		pvtkRenderer->ResetCamera();
		pvtkRenderer->GetActiveCamera()->ParallelProjectionOn();
		ui.pvtkQtWidget->GetRenderWindow()->Render();
	}
	else
	{
		int r = QMessageBox::warning(this, tr("Image load error"),
			tr("An Error happened when trying to load an image.\n"),
			QMessageBox::Close);
	}
}

void MeasureOn6Disease::OnFileOpen()
{
	if (okToContinue())
	{
		QString filter;
		filter = "TIFF image file(*.tif *tiff)";

		QString fileName = QFileDialog::getOpenFileName(this, QString(tr("OpenJPGImage")),
			lastPath, filter);
		fileinfo = QFileInfo(fileName);
		//文件名
		file_name = fileinfo.fileName();
		//文件后缀
		file_suffix = fileinfo.suffix();
		//绝对路径
		file_path = fileinfo.absolutePath();

		if (!fileName.isEmpty())
		{
			OnResetScene();
			loadFile(fileName);

			OnClickCheckResetColor();

			// 在SatusBar中实时显示
			ui.statusBar->showMessage(fileName);
		}
	}
}

void MeasureOn6Disease::OnFileClose()
{
	OnResetScene();
	OnClickCheckResetColor();
}

void MeasureOn6Disease::OnFileReset()
{
	OnCleanBuffer();
	OnClickCheckResetColor();
}

void MeasureOn6Disease::OnFileSave()
{
	if (okToContinue())
	{
		QString filter;
		filter = "TIFF image file(*.tif *tiff)";

		QString filePath = QFileDialog::getExistingDirectory(this, QString(tr("SaveTIFFImage")),
			lastPath);

		if (filePath.isEmpty())
		{
			QMessageBox::information(this, "Infomation", "Save Operation Failed!");
		}
		else
		{
			QString abs_filename = file_name;
			int index = abs_filename.lastIndexOf(".");  //一般来说，扩展名为最后一个“.”之后的字符；
			abs_filename.truncate(index);

			vtkSmartPointer<vtkTIFFWriter> writer0 =
				vtkSmartPointer<vtkTIFFWriter>::New();
			QString objFileName0 = filePath + "\\" + abs_filename + "_origin.tif";
			QByteArray ba0 = objFileName0.toLocal8Bit();
			writer0->SetFileName(ba0.data());
			writer0->SetInputData(o_ImageTIFF.GetImageDataRGB());
			writer0->Write();

			vtkSmartPointer<vtkTIFFWriter> writer1 =
				vtkSmartPointer<vtkTIFFWriter>::New();
			QString objFileName1 = filePath + "\\" + abs_filename + "_A.tif";
			QByteArray ba1 = objFileName1.toLocal8Bit();
			writer1->SetFileName(ba1.data());
			writer1->SetInputData(o_ImageTIFF.GetImageDataAMD());
			writer1->Write();

			vtkSmartPointer<vtkTIFFWriter> writer2 =
				vtkSmartPointer<vtkTIFFWriter>::New();
			QString objFileName2 = filePath +  "\\" + abs_filename + "_B.tif";
			QByteArray ba2 = objFileName2.toLocal8Bit();
			writer2->SetFileName(ba2.data());
			writer2->SetInputData(o_ImageTIFF.GetImageDataDME());
			writer2->Write();

			/*vtkSmartPointer<vtkTIFFWriter> writer3 =
				vtkSmartPointer<vtkTIFFWriter>::New();
			QString objFileName3 = filePath +  "\\" + abs_filename + "_C.tif";
			QByteArray ba3 = objFileName3.toLocal8Bit();
			writer3->SetFileName(ba3.data());
			writer3->SetInputData(o_ImageTIFF.GetImageDataDRY());
			writer3->Write();

			vtkSmartPointer<vtkTIFFWriter> writer4 =
				vtkSmartPointer<vtkTIFFWriter>::New();
			QString objFileName4 = filePath +  "\\" + abs_filename + "_D.tif";
			QByteArray ba4 = objFileName4.toLocal8Bit();
			writer4->SetFileName(ba4.data());
			writer4->SetInputData(o_ImageTIFF.GetImageDataNM());
			writer4->Write();

			vtkSmartPointer<vtkTIFFWriter> writer5 =
				vtkSmartPointer<vtkTIFFWriter>::New();
			QString objFileName5 = filePath + "\\" + abs_filename + "_E.tif";
			QByteArray ba5 = objFileName5.toLocal8Bit();
			writer5->SetFileName(ba5.data());
			writer5->SetInputData(o_ImageTIFF.GetImageDataPCV());
			writer5->Write();

			vtkSmartPointer<vtkTIFFWriter> writer6 =
				vtkSmartPointer<vtkTIFFWriter>::New();
			QString objFileName6 = filePath + "\\" + abs_filename + "_F.tif";
			QByteArray ba6 = objFileName6.toLocal8Bit();
			writer5->SetFileName(ba6.data());
			writer5->SetInputData(o_ImageTIFF.GetImageDataPM());
			writer5->Write();*/
		}
	}
	else
	{
		QMessageBox::information(this, "Infomation", "Save Operation Failed!");
	}
}

void MeasureOn6Disease::OnFileExit()
{

}

void MeasureOn6Disease::OnClickCheckResetColor()
{
	if (o_ImageTIFF.GetImageActor() == NULL)
		return;

	o_ImageTIFF.GetImageActor()->GetProperty()->SetColorLevel(127.5);
	o_ImageTIFF.GetImageActor()->GetProperty()->SetColorWindow(255);

	ui.pvtkQtWidget->GetRenderWindow()->Render();
}

void MeasureOn6Disease::OnClickAMD()
{
	if (ui.buttonAMD->isChecked())
	{
		ui.buttonDME->setChecked(false);
		ui.buttonDRY->setChecked(false);
		ui.buttonNM->setChecked(false);
		ui.buttonPCV->setChecked(false);
		ui.buttonPM->setChecked(false);
		OnClickDME();
		OnClickDRY();
		OnClickNM();
		OnClickPCV();
		OnClickPM();

		if (contourAMDWidget != 0)
		{
			contourAMDWidget->SetEnabled(1);
			pvtkInteractor->Render();
			return;
		}

		if (!contourAMDWidget)
			contourAMDWidget = vtkSmartPointer<vtkContourWidget>::New();
		contourAMDWidget->SetInteractor(pvtkInteractor);

		if (!contourAMDCallback)
			contourAMDCallback = vtkSmartPointer<vtkContourAMDCallback>::New();

		vtkSmartPointer<vtkOrientedGlyphContourRepresentation> contourRepresentation =
			vtkSmartPointer<vtkOrientedGlyphContourRepresentation>::New();

		vtkContourLineInterpolator* temp = contourRepresentation->GetLineInterpolator();
		
		contourRepresentation->GetLinesProperty()->SetColor(0, 0, 1); // Set color to g+b
		contourRepresentation->GetLinesProperty()->SetLineWidth(2);
		contourAMDWidget->SetRepresentation(contourRepresentation);
		contourAMDWidget->AddObserver(vtkCommand::EndInteractionEvent, contourAMDCallback);

		contourAMDWidget->SetEnabled(1);
		pvtkInteractor->Render();

		vtkQtconnections->Connect(contourAMDWidget,
			AMDContourDrawEvent,
			this,
			SLOT(DrawOnAMDContour()));

		ui.statusBar->setWindowIconText("AMD Drawing...");
		ui.statusBar->show();
	}
	else
	{
		if (contourAMDWidget != NULL)
			contourAMDWidget->SetEnabled(0);
		pvtkInteractor->Render();
	}
}

void MeasureOn6Disease::DrawOnAMDContour()
{
	o_ImageTIFF.setAMDContourActor();
	ui.pvtkQtWidget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(
		o_ImageTIFF.getAMDContourActor());
	ui.pvtkQtWidget->GetRenderWindow()->Render();
}

void MeasureOn6Disease::OnClickDME()
{
	if (ui.buttonDME->isChecked())
	{
		ui.buttonAMD->setChecked(false);
		ui.buttonDRY->setChecked(false);
		ui.buttonNM->setChecked(false);
		ui.buttonPCV->setChecked(false);
		ui.buttonPM->setChecked(false);
		OnClickAMD();
		OnClickDRY();
		OnClickNM();
		OnClickPCV();
		OnClickPM();

		if (contourDMEWidget != 0)
		{
			contourDMEWidget->SetEnabled(1);
			pvtkInteractor->Render();
			return;
		}

		if (!contourDMEWidget)
			contourDMEWidget = vtkSmartPointer<vtkContourWidget>::New();
		contourDMEWidget->SetInteractor(pvtkInteractor);

		if (!contourDMECallback)
			contourDMECallback = vtkSmartPointer<vtkContourDMECallback>::New();

		vtkSmartPointer<vtkOrientedGlyphContourRepresentation> contourRepresentation =
			vtkSmartPointer<vtkOrientedGlyphContourRepresentation>::New();

		contourRepresentation->GetLinesProperty()->SetColor(0, 1, 0); // Set color to g+b
		contourRepresentation->GetLinesProperty()->SetLineWidth(2);
		contourDMEWidget->SetRepresentation(contourRepresentation);
		contourDMEWidget->AddObserver(vtkCommand::EndInteractionEvent, contourDMECallback);

		contourDMEWidget->SetEnabled(1);
		pvtkInteractor->Render();

		vtkQtconnections->Connect(contourDMEWidget,
			DMEContourDrawEvent,
			this,
			SLOT(DrawOnDMEContour()));

		ui.statusBar->setWindowIconText("DME Drawing...");
		ui.statusBar->show();
	}
	else
	{
		if (contourDMEWidget != NULL)
			contourDMEWidget->SetEnabled(0);
		pvtkInteractor->Render();
	}
}

void MeasureOn6Disease::DrawOnDMEContour()
{
	o_ImageTIFF.setDMEContourActor();
	ui.pvtkQtWidget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(
		o_ImageTIFF.getDMEContourActor());
	ui.pvtkQtWidget->GetRenderWindow()->Render();
}

void MeasureOn6Disease::OnClickDRY()
{
	if (ui.buttonDRY->isChecked())
	{
		ui.buttonAMD->setChecked(false);
		ui.buttonDME->setChecked(false);
		ui.buttonNM->setChecked(false);
		ui.buttonPCV->setChecked(false);
		ui.buttonPM->setChecked(false);
		OnClickAMD();
		OnClickDME();
		OnClickNM();
		OnClickPCV();
		OnClickPM();

		if (contourDRYWidget != 0)
		{
			contourDRYWidget->SetEnabled(1);
			pvtkInteractor->Render();
			return;
		}

		if (!contourDRYWidget)
			contourDRYWidget = vtkSmartPointer<vtkContourWidget>::New();
		contourDRYWidget->SetInteractor(pvtkInteractor);

		if (!contourDRYCallback)
			contourDRYCallback = vtkSmartPointer<vtkContourDRYCallback>::New();

		vtkSmartPointer<vtkOrientedGlyphContourRepresentation> contourRepresentation =
			vtkSmartPointer<vtkOrientedGlyphContourRepresentation>::New();

		contourRepresentation->GetLinesProperty()->SetColor(1, 0, 0); // Set color to g+b
		contourRepresentation->GetLinesProperty()->SetLineWidth(2);
		contourDRYWidget->SetRepresentation(contourRepresentation);
		contourDRYWidget->AddObserver(vtkCommand::EndInteractionEvent, contourDRYCallback);

		contourDRYWidget->SetEnabled(1);
		pvtkInteractor->Render();

		vtkQtconnections->Connect(contourDRYWidget,
			DRYContourDrawEvent,
			this,
			SLOT(DrawOnDRYContour()));

		ui.statusBar->setWindowIconText("DRY Drawing...");
		ui.statusBar->show();
	}
	else
	{
		if (contourDRYWidget != NULL)
			contourDRYWidget->SetEnabled(0);
		pvtkInteractor->Render();
	}
}

void MeasureOn6Disease::DrawOnDRYContour()
{
	o_ImageTIFF.setDRYContourActor();
	ui.pvtkQtWidget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(
		o_ImageTIFF.getDRYContourActor());
	ui.pvtkQtWidget->GetRenderWindow()->Render();
}

void MeasureOn6Disease::OnClickNM()
{
	if (ui.buttonNM->isChecked())
	{
		ui.buttonAMD->setChecked(false);
		ui.buttonDME->setChecked(false);
		ui.buttonDRY->setChecked(false);
		ui.buttonPCV->setChecked(false);
		ui.buttonPM->setChecked(false);
		OnClickAMD();
		OnClickDME();
		OnClickDRY();
		OnClickPCV();
		OnClickPM();

		if (contourNMWidget != 0)
		{
			contourNMWidget->SetEnabled(1);
			pvtkInteractor->Render();
			return;
		}

		if (!contourNMWidget)
			contourNMWidget = vtkSmartPointer<vtkContourWidget>::New();
		contourNMWidget->SetInteractor(pvtkInteractor);

		if (!contourNMCallback)
			contourNMCallback = vtkSmartPointer<vtkContourNMCallback>::New();

		vtkSmartPointer<vtkOrientedGlyphContourRepresentation> contourRepresentation =
			vtkSmartPointer<vtkOrientedGlyphContourRepresentation>::New();

		contourRepresentation->GetLinesProperty()->SetColor(0, 1, 1); // Set color to g+b
		contourRepresentation->GetLinesProperty()->SetLineWidth(2);
		contourNMWidget->SetRepresentation(contourRepresentation);
		contourNMWidget->AddObserver(vtkCommand::EndInteractionEvent, contourNMCallback);

		contourNMWidget->SetEnabled(1);
		pvtkInteractor->Render();

		vtkQtconnections->Connect(contourNMWidget,
			NMContourDrawEvent,
			this,
			SLOT(DrawOnNMContour()));

		ui.statusBar->setWindowIconText("NM Drawing...");
		ui.statusBar->show();
	}
	else
	{
		if (contourNMWidget != NULL)
			contourNMWidget->SetEnabled(0);
		pvtkInteractor->Render();
	}
}

void MeasureOn6Disease::DrawOnNMContour()
{
	o_ImageTIFF.setNMContourActor();
	ui.pvtkQtWidget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(
		o_ImageTIFF.getNMContourActor());
	ui.pvtkQtWidget->GetRenderWindow()->Render();
}

void MeasureOn6Disease::OnClickPCV()
{
	if (ui.buttonPCV->isChecked())
	{
		ui.buttonAMD->setChecked(false);
		ui.buttonDME->setChecked(false);
		ui.buttonDRY->setChecked(false);
		ui.buttonNM->setChecked(false);
		ui.buttonPM->setChecked(false);
		OnClickAMD();
		OnClickDME();
		OnClickDRY();
		OnClickNM();
		OnClickPM();

		if (contourPCVWidget != 0)
		{
			contourPCVWidget->SetEnabled(1);
			pvtkInteractor->Render();
			return;
		}

		if (!contourPCVWidget)
			contourPCVWidget = vtkSmartPointer<vtkContourWidget>::New();
		contourPCVWidget->SetInteractor(pvtkInteractor);

		if (!contourPCVCallback)
			contourPCVCallback = vtkSmartPointer<vtkContourPCVCallback>::New();

		vtkSmartPointer<vtkOrientedGlyphContourRepresentation> contourRepresentation =
			vtkSmartPointer<vtkOrientedGlyphContourRepresentation>::New();

		contourRepresentation->GetLinesProperty()->SetColor(1, 1, 0); // Set color to g+b
		contourRepresentation->GetLinesProperty()->SetLineWidth(2);
		contourPCVWidget->SetRepresentation(contourRepresentation);
		contourPCVWidget->AddObserver(vtkCommand::EndInteractionEvent, contourPCVCallback);

		contourPCVWidget->SetEnabled(1);
		pvtkInteractor->Render();

		vtkQtconnections->Connect(contourPCVWidget,
			PCVContourDrawEvent,
			this,
			SLOT(DrawOnPCVContour()));

		ui.statusBar->setWindowIconText("PCV Drawing...");
		ui.statusBar->show();
	}
	else
	{
		if (contourPCVWidget != NULL)
			contourPCVWidget->SetEnabled(0);
		pvtkInteractor->Render();
	}
}

void MeasureOn6Disease::DrawOnPCVContour()
{
	o_ImageTIFF.setPCVContourActor();
	ui.pvtkQtWidget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(
		o_ImageTIFF.getPCVContourActor());
	ui.pvtkQtWidget->GetRenderWindow()->Render();
}

void MeasureOn6Disease::OnClickPM()
{
	if (ui.buttonPM->isChecked())
	{
		ui.buttonAMD->setChecked(false);
		ui.buttonDME->setChecked(false);
		ui.buttonDRY->setChecked(false);
		ui.buttonNM->setChecked(false);
		ui.buttonPCV->setChecked(false);
		OnClickAMD();
		OnClickDME();
		OnClickDRY();
		OnClickNM();
		OnClickPCV();

		if (contourPMWidget != 0)
		{
			contourPMWidget->SetEnabled(1);
			pvtkInteractor->Render();
			return;
		}

		if (!contourPMWidget)
			contourPMWidget = vtkSmartPointer<vtkContourWidget>::New();
		contourPMWidget->SetInteractor(pvtkInteractor);

		if (!contourPMCallback)
			contourPMCallback = vtkSmartPointer<vtkContourPMCallback>::New();

		vtkSmartPointer<vtkOrientedGlyphContourRepresentation> contourRepresentation =
			vtkSmartPointer<vtkOrientedGlyphContourRepresentation>::New();

		contourRepresentation->GetLinesProperty()->SetColor(1, 0, 1); // Set color to g+b
		contourRepresentation->GetLinesProperty()->SetLineWidth(2);
		contourPMWidget->SetRepresentation(contourRepresentation);
		contourPMWidget->AddObserver(vtkCommand::EndInteractionEvent, contourPMCallback);

		contourPMWidget->SetEnabled(1);
		pvtkInteractor->Render();

		vtkQtconnections->Connect(contourPMWidget,
			PMContourDrawEvent,
			this,
			SLOT(DrawOnPMContour()));

		ui.statusBar->setWindowIconText("PM Drawing...");
		ui.statusBar->show();
	}
	else
	{
		if (contourPMWidget != NULL)
			contourPMWidget->SetEnabled(0);
		pvtkInteractor->Render();
	}
}

void MeasureOn6Disease::DrawOnPMContour()
{
	o_ImageTIFF.setPMContourActor();
	ui.pvtkQtWidget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(
		o_ImageTIFF.getPMContourActor());
	ui.pvtkQtWidget->GetRenderWindow()->Render();
}