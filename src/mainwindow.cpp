#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QImage>
#include <QFileDialog>
#include <QString>
#include <qbytearray.h>
#include <qvariant.h>
#include <vtkPoints.h>
#include <vtkProperty.h>
#include <vtkNew.h>
#include <vtkConeSource.h>
#include <vtkSphereSource.h>
#include <vtkCubeSource.h>
#include <vtkCylinderSource.h>
#include <vtkTextActor.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkVersion.h>
#include <vtkTextProperty.h>
#include <vtkPyramid.h>
#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkCellArray.h>
#include <vtkDataSetMapper.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPoints.h>
#include <vtkPlane.h>
#include <vtkPyramid.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkUnstructuredGrid.h>
#include<vtkTransformFilter.h>
#include <vtkLineSource.h>
#include <vtkTransform.h>
#include <vtkSmartPointer.h>
#include <vtkTransform.h>
#include <vtkMath.h>
#include <vtkTubeFilter.h>
#include <vtkParametricTorus.h>
#include <vtkParametricFunctionSource.h>
#include <vtkClipPolyData.h>;
#include <vtkSTLWriter.h>;
#include <vtkSTLReader.h>;
#include<string>
#include<iostream>
#include<vtkAppendPolyData.h>
#include<vtkDataSetReader.h>
using namespace std;
double MainWindow::azimuth = 0;
double MainWindow::elevation = 0;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

	// Create a renderer, render window, and interactor
	mInteractor->SetInteractorStyle(mInteractorStyle);
	mInteractor->Initialize();

	ui->openGLWidget->setRenderWindow(renderWindow);
	renderWindow->SetInteractor(mInteractor);
	ui->openGLWidget->renderWindow()->AddRenderer(renderer);

	//set backgroundColor
	renderer->GradientBackgroundOn();
	vtkNew<vtkNamedColors> colors;
	renderer->SetBackground(colors->GetColor3d("Cyan").GetData());
	renderer->SetBackground2(colors->GetColor3d("Tomato").GetData());

	buttonsClicked();

	//initialize Camera
	renderer->GetActiveCamera()->SetPosition(0, 0, 5);
	renderer->GetActiveCamera()->SetFocalPoint(0, 0, 0);
	ui->openGLWidget->renderWindow()->SetWindowName("Hello");

	updateColorBox();
}


MainWindow::~MainWindow()
{
	delete ui;
}


void MainWindow::save() {
	vtkNew<vtkAppendPolyData> appendFilter;
	for (auto actor : shapes) {
		vtkSmartPointer<vtkMapper> mapper = actor->GetMapper();
		vtkNew<vtkPolyData> actorPolyData;
		mapper->Update();
		actorPolyData->ShallowCopy(mapper->GetInput());
		appendFilter->AddInputData(actorPolyData);
	}
	appendFilter->Update();
	vtkSmartPointer<vtkPolyData> combinedPolyData = appendFilter->GetOutput();


	vtkNew<vtkSTLWriter> writer;
	writer->SetFileName("output.stl");
	writer->SetInputData(combinedPolyData);
	writer->Write();
}



void MainWindow::saveImage() {
	QImage img = ui->openGLWidget->grabFramebuffer();
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image File"),
		QString(),
		tr("Images (*.png)"));
	if (!fileName.isEmpty())
	{
		img.save(fileName);
	}
}



void MainWindow::load() {
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open file"), "",
		"VTK Files (*.stl)");

	// Open file
	QFile file(fileName);
	file.open(QIODevice::ReadOnly);

	// Return on Cancel
	if (!file.exists())
		return;
}


void MainWindow::clear_Window() {
	renderer->RemoveAllViewProps();
	renderer->GetActiveCamera()->SetPosition(0, 0, 5);
	renderer->GetActiveCamera()->SetFocalPoint(0, 0, 0);
	updateColorBox();
	shapes.clear();
	renderWindow->Render();
}



void MainWindow::deleteLast() {
	if (!shapes.empty()) {
		renderer->RemoveActor(shapes.back());
		shapes.pop_back();
		renderWindow->Render();
	}
}


void MainWindow::updatePosition() {
	double* position = renderer->GetActiveCamera()->GetPosition();
	ui->cameraX->setValue(position[0]);
	ui->cameraY->setValue(position[1]);
	ui->cameraZ->setValue(position[2]);
}



void MainWindow::updateCameraX(double value) {
	double* position = renderer->GetActiveCamera()->GetPosition();
	renderer->GetActiveCamera()->SetPosition(value, position[1], position[2]);
	renderWindow->Render();
}



void MainWindow::updateCameraY(double value) {
	double* position = renderer->GetActiveCamera()->GetPosition();
	renderer->GetActiveCamera()->SetPosition(position[0], value, position[2]);
	renderWindow->Render();
}


void MainWindow::updateCameraZ(double value) {
	double* position = renderer->GetActiveCamera()->GetPosition();
	renderer->GetActiveCamera()->SetPosition(position[0], position[1], value);
	renderWindow->Render();
}


void MainWindow::updateCameraAzimuth(double value) {
	renderer->GetActiveCamera()->Azimuth(azimuth);
	renderer->GetActiveCamera()->Azimuth(value);
	azimuth = value;
	updatePosition();
	renderWindow->Render();
}


void MainWindow::updateCameraElevation(double value) {
	renderer->GetActiveCamera()->Elevation(elevation);
	renderer->GetActiveCamera()->Elevation(value);
	elevation = value;
	updatePosition();
	renderWindow->Render();
}


void MainWindow::updateColorBox() {
	double* rgb = selectedRGB(false);
	QString* rgbTxt = new QString[3];
	for (int i = 0; i < 3; i++) {
		rgbTxt[i] = QString::number(rgb[i]);
	}
	ui->redValue->setText(rgbTxt[0]);
	ui->greenValue->setText(rgbTxt[1]);
	ui->blueValue->setText(rgbTxt[2]);
	ui->colorBox->setStyleSheet("background-color: rgb(" + rgbTxt[0] + "," + rgbTxt[1] + "," + rgbTxt[2] + ")");
	delete[] rgb;
	delete[] rgbTxt;
}

double* MainWindow::selectedRGB(bool normalize = true) {
	double don = normalize ? 255 : 1;
	return new double[3] { ui->redColorSlider->value() / don,
		ui->greenColorSlider->value() / don,
		ui->blueColorSlider->value() / don };
}


void MainWindow::onDrawCube() {
	vtkNew<vtkCubeSource> cubeSource;
	cubeSource->Update();
	vtkNew<vtkPolyDataMapper> cubeMapper;

	cubeMapper->SetInputData(cubeSource->GetOutput());
	vtkNew<vtkActor> cubeActor;
	cubeActor->SetMapper(cubeMapper);

	cubeActor->GetProperty()->SetColor(selectedRGB());

	renderer->AddActor(cubeActor);
	renderer->ResetCamera();
	shapes.push_back(cubeActor);
	updatePosition();
	renderWindow->Render();
}

void MainWindow::onDrawSphere() {
	vtkNew<vtkSphereSource> sphereSource;
	sphereSource->SetRadius(1.0);
	sphereSource->Update();
	vtkNew<vtkPolyDataMapper> sphereMapper;
	sphereMapper->SetInputConnection(sphereSource->GetOutputPort());
	vtkNew<vtkActor> sphereActor;
	sphereActor->SetMapper(sphereMapper);

	sphereActor->GetProperty()->SetColor(selectedRGB());

	renderer->AddActor(sphereActor);
	renderer->ResetCamera();
	shapes.push_back(sphereActor);
	updatePosition();
	renderWindow->Render();
}

void MainWindow::onDrawHemisphere() {
	vtkNew<vtkSphereSource> sphereSource;
	sphereSource->SetCenter(0.0, 0.0, 0.0);
	sphereSource->SetRadius(1.0);
	// Make the surface smooth.
	sphereSource->SetPhiResolution(10);
	sphereSource->SetThetaResolution(10);



	// Create a plane for clipping (the XY plane to extract the upper hemisphere)
	vtkSmartPointer<vtkPlane> clipPlane = vtkSmartPointer<vtkPlane>::New();
	clipPlane->SetOrigin(0, 0, 0);
	clipPlane->SetNormal(0, 0, 1); // Clipping along the Z-axis

	// Create a clip filter to extract the upper hemisphere
	vtkSmartPointer<vtkClipPolyData> clipFilter = vtkSmartPointer<vtkClipPolyData>::New();
	clipFilter->SetInputConnection(sphereSource->GetOutputPort());
	clipFilter->SetClipFunction(clipPlane);
	clipFilter->InsideOutOn(); // Extract inside of the clipping plane


	vtkNew<vtkPolyDataMapper> sphereMapper;
	sphereMapper->SetInputConnection(clipFilter->GetOutputPort());
	vtkNew<vtkActor> sphereActor;
	sphereActor->SetMapper(sphereMapper);


	sphereActor->GetProperty()->SetColor(selectedRGB());
	sphereActor->SetPosition(0, 0, -10);

	renderer->AddActor(sphereActor);
	renderer->ResetCamera();
	shapes.push_back(sphereActor);
	updatePosition();
	renderWindow->Render();
}

void MainWindow::onDrawCone() {
	vtkNew<vtkConeSource> coneSource;
	coneSource->Update();
	vtkNew<vtkPolyDataMapper> coneMapper;
	coneMapper->SetInputConnection(coneSource->GetOutputPort());
	vtkNew<vtkActor> coneActor;
	coneActor->SetMapper(coneMapper);


	coneActor->GetProperty()->SetColor(selectedRGB());

	renderer->AddActor(coneActor);
	renderer->ResetCamera();
	shapes.push_back(coneActor);
	updatePosition();
	renderWindow->Render();
}

void MainWindow::onDrawPyramid() {
	vtkNew<vtkPoints> points;

	float p0[3] = { 1.0, 1.0, 1.0 };
	float p1[3] = { -1.0, 1.0, 1.0 };
	float p2[3] = { -1.0, -1.0, 1.0 };
	float p3[3] = { 1.0, -1.0, 1.0 };
	float p4[3] = { 0.0, 0.0, 2.0 };

	points->InsertNextPoint(p0);
	points->InsertNextPoint(p1);
	points->InsertNextPoint(p2);
	points->InsertNextPoint(p3);
	points->InsertNextPoint(p4);

	vtkNew<vtkPyramid> pyramid;
	pyramid->GetPointIds()->SetId(0, 0);
	pyramid->GetPointIds()->SetId(1, 1);
	pyramid->GetPointIds()->SetId(2, 2);
	pyramid->GetPointIds()->SetId(3, 3);
	pyramid->GetPointIds()->SetId(4, 4);

	vtkNew<vtkCellArray> cells;
	cells->InsertNextCell(pyramid);

	vtkNew<vtkUnstructuredGrid> ug;
	ug->SetPoints(points);
	ug->InsertNextCell(pyramid->GetCellType(), pyramid->GetPointIds());

	vtkNew<vtkNamedColors> colors;
	vtkNew<vtkDataSetMapper> mapper;
	mapper->SetInputData(ug);

	vtkNew<vtkActor> actor;
	actor->SetMapper(mapper);


	actor->GetProperty()->SetColor(selectedRGB());
	actor->SetPosition(0, 0, -10);

	renderer->AddActor(actor);
	renderer->ResetCamera();
	updatePosition();
	shapes.push_back(actor);
	renderWindow->Render();
}

void MainWindow::onDrawCylinder() {
	// Cylinder
	vtkNew<vtkCylinderSource> cylinderSource;
	cylinderSource->Update();
	vtkNew<vtkPolyDataMapper> cylinderMapper;
	cylinderMapper->SetInputConnection(cylinderSource->GetOutputPort());
	vtkNew<vtkActor> cylinderActor;
	cylinderActor->SetMapper(cylinderMapper);


	cylinderActor->GetProperty()->SetColor(selectedRGB());

	renderer->AddActor(cylinderActor);
	renderer->ResetCamera();
	updatePosition();
	shapes.push_back(cylinderActor);
	renderWindow->Render();
}

void MainWindow::onDrawTube() {
	vtkNew<vtkCylinderSource> cylinderSource;
	cylinderSource->SetCenter(0.0, 0.0, 0.0);
	cylinderSource->SetRadius(1.0);
	cylinderSource->SetHeight(3.0);
	cylinderSource->SetResolution(20);
	//stlWriter->SetInputConnection(cylinderSource);
	// Create a mapper and actor.
	vtkNew<vtkPolyDataMapper> mapper;
	mapper->SetInputConnection(cylinderSource->GetOutputPort());
	vtkNew<vtkActor> actor;
	actor->SetMapper(mapper);

	actor->GetProperty()->SetColor(selectedRGB());
	actor->SetPosition(0, 0, -10);


	// Add the actor to the scene.
	renderer->AddActor(actor);

	renderer->ResetCamera();
	updatePosition();
	shapes.push_back(actor);
	renderWindow->Render();
}

void MainWindow::onDrawDoughnut() {
	// Create a parametric torus
	vtkSmartPointer<vtkParametricTorus> torus = vtkSmartPointer<vtkParametricTorus>::New();
	torus->SetRingRadius(1.0); // Set the radius of the ring
	torus->SetCrossSectionRadius(0.5); // Set the radius of the cross-section
	//stlWriter->SetInputConnection(torus);//
	// Create a parametric function source
	vtkSmartPointer<vtkParametricFunctionSource> parametricSource = vtkSmartPointer<vtkParametricFunctionSource>::New();
	parametricSource->SetParametricFunction(torus);
	parametricSource->SetUResolution(30); // Set the resolution in the u direction
	parametricSource->SetVResolution(30); // Set the resolution in the v direction
	parametricSource->SetWResolution(30); // Set the resolution in the w direction
	parametricSource->Update();

	// Create a mapper and actor.
	vtkNew<vtkPolyDataMapper> mapper;
	//mapper->SetInputConnection(tubeFilter->GetOutputPort());
	mapper->SetInputData(parametricSource->GetOutput());
	vtkNew<vtkActor> actor;
	actor->SetMapper(mapper);

	actor->GetProperty()->SetColor(selectedRGB());
	actor->SetPosition(0, 0, -10);


	// Add the actor to the scene.
	renderer->AddActor(actor);

	renderer->ResetCamera();
	updatePosition();
	shapes.push_back(actor);
	renderWindow->Render();
}

void MainWindow::onDrawCurvedCylinder() {
	vtkNew<vtkCylinderSource> cylinderSource;
	cylinderSource->SetCenter(0.0, 0.0, 0.0);
	cylinderSource->SetRadius(1.0);
	cylinderSource->SetHeight(2.0);
	cylinderSource->SetResolution(20);
	//stlWriter->SetInputConnection(cylinderSource);//

	// Create a transform to orient the cylinder
	vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
	transform->RotateX(20); // Rotate the cylinder to be upright
	transform->RotateY(50); // Rotate the cylinder to be upright
	transform->RotateZ(50); // Rotate the cylinder to be upright

	// Apply the transform to the tube filter
	transform->Update();
	// Apply the transform to the tube filter using a transform filter
	vtkSmartPointer<vtkTransformFilter> transformFilter = vtkSmartPointer<vtkTransformFilter>::New();
	transformFilter->SetTransform(transform);
	transformFilter->SetInputConnection(cylinderSource->GetOutputPort());

	// Create a mapper and actor.
	vtkNew<vtkPolyDataMapper> mapper;
	//mapper->SetInputConnection(tubeFilter->GetOutputPort());
	mapper->SetInputConnection(transformFilter->GetOutputPort());
	vtkNew<vtkActor> actor;
	actor->SetMapper(mapper);

	actor->GetProperty()->SetColor(selectedRGB());
	actor->SetPosition(0, 0, -10);


	// Add the actor to the scene.
	renderer->AddActor(actor);

	renderer->ResetCamera();
	updatePosition();
	shapes.push_back(actor);
	renderWindow->Render();
}


void MainWindow::buttonsClicked() {
	connect(ui->Cube_Button, SIGNAL(clicked()), this, SLOT(onDrawCube()));
	connect(ui->Sphere_Button, SIGNAL(clicked()), this, SLOT(onDrawSphere()));
	connect(ui->Hemisphere_Button, SIGNAL(clicked()), this, SLOT(onDrawHemisphere()));
	connect(ui->Cone_Button, SIGNAL(clicked()), this, SLOT(onDrawCone()));
	connect(ui->Pyramid_Button, SIGNAL(clicked()), this, SLOT(onDrawPyramid()));
	connect(ui->Cylinder_Button, SIGNAL(clicked()), this, SLOT(onDrawCylinder()));
	connect(ui->Tube_Button, SIGNAL(clicked()), this, SLOT(onDrawTube()));
	connect(ui->Clear, SIGNAL(clicked()), this, SLOT(clear_Window()));
	connect(ui->Delete_Button, SIGNAL(clicked()), this, SLOT(deleteLast()));
	connect(ui->Doughnut_Button, SIGNAL(clicked()), this, SLOT(onDrawDoughnut()));
	connect(ui->CurvedCylinder_Button, SIGNAL(clicked()), this, SLOT(onDrawCurvedCylinder()));
	connect(ui->redColorSlider, &QSlider::valueChanged, this, &MainWindow::updateColorBox);
	connect(ui->greenColorSlider, &QSlider::valueChanged, this, &MainWindow::updateColorBox);
	connect(ui->blueColorSlider, &QSlider::valueChanged, this, &MainWindow::updateColorBox);
	connect(ui->cameraX, SIGNAL(valueChanged(double)), this, SLOT(updateCameraX(double)));
	connect(ui->cameraY, SIGNAL(valueChanged(double)), this, SLOT(updateCameraY(double)));
	connect(ui->cameraZ, SIGNAL(valueChanged(double)), this, SLOT(updateCameraZ(double)));
	connect(ui->cameraAzimuth, SIGNAL(valueChanged(double)), this, SLOT(updateCameraAzimuth(double)));
	connect(ui->cameraElevation, SIGNAL(valueChanged(double)), this, SLOT(updateCameraElevation(double)));
	connect(ui->action_Save, SIGNAL(triggered()), this, SLOT(save()));
	connect(ui->actionSaveImage, SIGNAL(triggered()), this, SLOT(saveImage()));
	connect(ui->action_Load_2, SIGNAL(triggered()), this, SLOT(load()));
}



