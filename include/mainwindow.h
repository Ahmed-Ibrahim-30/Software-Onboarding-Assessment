#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vtkNew.h>
#include <vtkNamedColors.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <QVTKInteractor.h>
#include <vtkInteractorStyle.h>
#include <vtkInteractorStyleTrackballActor.h>
#include <QVTKOpenGLNativeWidget.h>
#include <iostream>
#include <vector>
#include <vtkNew.h>
#include <MouseInteractorHighLightActor.h>
using namespace std;
namespace Ui {
class MainWindow;
}
/// <summary>
/// this is Main Window class
/// </summary>
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
/// <summary>
///   <para>
///       Constructor with Parameters for Class: MainWindow
///   </para>
///
///   <param name="* parent ">
///      <para>
///         Type: QWidget
///      </para>
///   </param>
/// </summary>
    explicit MainWindow(QWidget *parent = nullptr);
    
/// <summary>
///   <para>
///       Destructor for Class: MainWindow
///   </para>
/// </summary>
    ~MainWindow();
private slots:
/// <summary>
///   <para>
///       Function name: save
///       Function is part of Class: MainWindow
///		  Function to save OpenGLWidget content to file stl
///   </para>
/// </summary>
    void save();
/// <summary>
///   <para>
///       Function name: saveImage
///       Function is part of Class: MainWindow
///		  Function to save image from OpenGLWidget content .
///   </para>
/// </summary>
    void saveImage();
/// <summary>
///   <para>
///       Function name: load
///       Function is part of Class: MainWindow
///		  Function to load file from OpenGLWidget content .
///   </para>
/// </summary>
    void load();

private:
    Ui::MainWindow *ui;
    static double azimuth, elevation;
    vector<vtkActor*> shapes; //conatin all shapes stored in openGLNativeWidget
    vtkNew<vtkNamedColors> colors;
    vtkNew<vtkRenderer> renderer;
    vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
    vtkNew<QVTKInteractor> mInteractor;
    vtkNew<vtkInteractorStyleTrackballActor> mInteractorStyle;

public slots:
    /// <summary>
    ///   <para>
    ///       Function name: onDrawCube
    ///       Function is part of Class: MainWindow
    ///		 Draw Cube
    ///   </para>
    /// </summary>
    void onDrawCube();
    /// <summary>
    ///   <para>
    ///       Function name: onDrawSphere
    ///       Function is part of Class: MainWindow
    ///		  DrawSphere
    ///   </para>
    /// </summary>
    void onDrawSphere();
    /// <summary>
    ///   <para>
    ///       Function name: onDrawHemisphere
    ///       Function is part of Class: MainWindow
    ///		  DrawHemisphere
    ///   </para>
    /// </summary>
    void onDrawHemisphere();
    /// <summary>
    ///   <para>
    ///       Function name: onDrawCone
    ///       Function is part of Class: MainWindow
    ///		  DrawCone
    ///   </para>
    /// </summary>
    void onDrawCone();
    /// <summary>
    ///   <para>
    ///       Function name: onDrawPyramid
    ///       Function is part of Class: MainWindow
    ///		  DrawPyramid
    ///   </para>
    /// </summary>
    void onDrawPyramid();
    /// <summary>
    ///   <para>
    ///       Function name: onDrawCylinder
    ///       Function is part of Class: MainWindow
    ///		  DrawCylinder
    ///   </para>
    /// </summary>
    void onDrawCylinder();
    /// <summary>
    ///   <para>
    ///       Function name: onDrawTube
    ///       Function is part of Class: MainWindow
    ///		  DrawTube
    ///   </para>
    /// </summary>
    void onDrawTube();
    /// <summary>
    ///   <para>
    ///       Function name: onDrawDoughnut
    ///       Function is part of Class: MainWindow
    ///		  DrawDoughnut
    ///   </para>
    /// </summary>
    void onDrawDoughnut();
    /// <summary>
    ///   <para>
    ///       Function name: onDrawCurvedCylinder
    ///       Function is part of Class: MainWindow
    ///		  DrawCurvedCylinder
    ///   </para>
    /// </summary>
    void onDrawCurvedCylinder();
    /// <summary>
    ///   <para>
    ///       Function name: buttonsClicked
    ///       Function is part of Class: MainWindow
    ///   </para>
    /// </summary>
    void buttonsClicked();
    /// <summary>
    ///   <para>
    ///       Function name: clear_Window
    ///       Function is part of Class: MainWindow
    ///		  Function to remove all OpenGLWidget content .
    ///   </para>
    /// </summary>
    void clear_Window();
    /// <summary>
    ///   <para>
    ///       Function name: deleteLast
    ///       Function is part of Class: MainWindow
    ///		  Function to remove last added OpenGLWidget actor .
    ///   </para>
    /// </summary>
    void deleteLast();
    /// <summary>
    ///   <para>
    ///       Function name: selectedRGB
    ///       Function is part of Class: MainWindow
    ///   </para>
    ///   <param name="normalize">
    ///   <para>
    ///       Type: bool
    ///   </para>
    /// </summary>
    double* selectedRGB(bool normalize);
    /// <summary>
    ///   <para>
    ///       Function name: updateColorBox
    ///       Function is part of Class: MainWindow
    ///		  Function to updateColorBox
    ///   </para>
    /// </summary>
    void updateColorBox();
    /// <summary>
    ///   <para>
    ///       Function name: updatePosition
    ///       Function is part of Class: MainWindow
    ///		  Function to update Camera Position
    ///   </para>
    /// </summary>
    void updatePosition();
    /// <summary>
    ///   <para>
    ///       Function name: updateCameraX
    ///       Function is part of Class: MainWindow
    ///		  Function to add value for Camera X axis
    ///   </para>
    /// <param name="value">
    ///   <para>
    ///       Type: double
    ///   </para>
    /// </summary>
    void updateCameraX(double value);
    /// <summary>
    ///   <para>
    ///       Function name: updateCameraY
    ///       Function is part of Class: MainWindow
    ///		  Function to add value for Camera Y axis
    ///   </para>
    /// <param name="value">
    ///   <para>
    ///       Type: double
    ///   </para>
    /// </summary>
    void updateCameraY(double value);
    /// <summary>
    ///   <para>
    ///       Function name: updateCameraZ
    ///       Function is part of Class: MainWindow
    ///		  Function to add value for Camera Z axis
    ///   </para>
    /// <param name="value">
    ///   <para>
    ///       Type: double
    ///   </para>
    /// </summary>
    void updateCameraZ(double value);
    /// <summary>
    ///   <para>
    ///       Function name: updateCameraAzimuth
    ///       Function is part of Class: MainWindow
    ///		  Function to Rotate Camera H
    ///   </para>
    /// <param name="value">
    ///   <para>
    ///       Type: double
    ///   </para>
    /// </summary>
    void updateCameraAzimuth(double value);
    /// <summary>
    ///   <para>
    ///       Function name: updateCameraElevation
    ///       Function is part of Class: MainWindow
    ///		  Function to Rotate Camera V
    ///   </para>
    /// <param name="value">
    ///   <para>
    ///       Type: double
    ///   </para>
    ///</summary>
    void updateCameraElevation(double value);
};

#endif // MAINWINDOW_H
