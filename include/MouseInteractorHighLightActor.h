#pragma once
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkInteractorStyleTrackballActor.h>
#include <vtkMinimalStandardRandomSequence.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkObjectFactory.h>
#include <vtkPolyDataMapper.h>
#include <vtkPropPicker.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <iostream>
#include <vector>
#include <vtkSphereSource.h>
using namespace std;
namespace  {

	///<summary>
	///   <para>
	///       Class name: MouseInteractorHighLightActor
	///       Handle mouse interactions to select a particular actor
	///   </para>
	///</summary>
	class MouseInteractorHighLightActor : public vtkInteractorStyleTrackballActor
	{
	public:
		int selectedActor;
		vector<vtkActor*> shapes; //conatin all shapes stored in openGLNativeWidget
		static MouseInteractorHighLightActor* New();
		vtkTypeMacro(MouseInteractorHighLightActor,
			vtkInteractorStyleTrackballActor);

		MouseInteractorHighLightActor()
		{
			selectedActor = 0;
			LastPickedActor = NULL;
			LastPickedProperty = vtkProperty::New();
		}
		
		virtual ~MouseInteractorHighLightActor()
		{
			LastPickedProperty->Delete();
		}

		virtual void OnLeftButtonDown() override
		{
			/*
			vtkNew<vtkNamedColors> colors;

			
			int* clickPos = this->GetInteractor()->GetEventPosition();
			int adjustedX = clickPos[0] * 20;
			int adjustedY = clickPos[1] * 20;
			vtkNew<vtkPropPicker>picker;
			picker->Pick(adjustedX, adjustedY, 0, this->GetCurrentRenderer());
			if (LastPickedActor)
			{
				LastPickedActor->GetProperty()->DeepCopy(LastPickedProperty);
			}
			LastPickedActor = picker->GetActor();
			if (LastPickedActor)
			{
				for (int i = 0; i < shapes.size(); i++) {
					if (shapes[i] == LastPickedActor) {
						selectedActor = i;
						break;
					}
				}
				LastPickedProperty->DeepCopy(LastPickedActor->GetProperty());
				LastPickedActor->GetProperty()->SetColor(
					colors->GetColor3d("Gold").GetData());
				LastPickedActor->GetProperty()->SetDiffuse(1.0);
				LastPickedActor->GetProperty()->SetSpecular(0.0);
				LastPickedActor->GetProperty()->EdgeVisibilityOn();
			}
			
			*/
			vtkInteractorStyleTrackballActor::OnLeftButtonDown();
		}

		virtual void OnMiddleButtonDown() override
		{
			std::cout << "Pressed middle mouse button." << std::endl;
			// Forward events.
			vtkInteractorStyleTrackballActor::OnMiddleButtonDown();
		}

		virtual void OnRightButtonDown() override
		{
			std::cout << "Pressed right mouse button." << std::endl;
			// Forward events.
			vtkInteractorStyleTrackballActor::OnRightButtonDown();
		}
	private:
		bool actorIsSelected = false;
		vtkActor* LastPickedActor;
		vtkProperty* LastPickedProperty ;
	};

	vtkStandardNewMacro(MouseInteractorHighLightActor);
} 