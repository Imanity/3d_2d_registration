#include "VtkWidget.h"

#include <vtkRenderWindow.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkImageImport.h>
#include <vtkFixedPointVolumeRayCastMapper.h>
#include <vtkColorTransferFunction.h>
#include <vtkVolumeProperty.h>
#include <vtkPiecewiseFunction.h>
#include <vtkMarchingCubes.h>
#include <vtkStripper.h>
#include <vtkPolyDataConnectivityFilter.h>
#include <vtkNew.h>
#include <vtkSmoothPolyDataFilter.h>
#include <vtkLookupTable.h>
#include <vtkPointData.h>

VtkWidget::VtkWidget(QWidget *parent) : QVTKWidget(parent) {
	renderingMode = MESH_RENDERING;

	mesh_scalars = vtkSmartPointer<vtkFloatArray>::New();

	this->renderer = vtkSmartPointer<vtkRenderer>::New();
	//this->renderer->SetBackground(255, 255, 255);
	this->GetRenderWindow()->AddRenderer(this->renderer);
	this->GetRenderWindow()->Render();
}

VtkWidget::~VtkWidget() {
}

void VtkWidget::setVolume(VolumeData *v) {
	volume = v;

	vtkSmartPointer<vtkImageImport> image_import = vtkSmartPointer<vtkImageImport>::New();
	image_import->SetDataSpacing(volume->dx, volume->dy, volume->dz);
	image_import->SetDataOrigin(0, 0, 0);
	image_import->SetWholeExtent(0, volume->nx - 1, 0, volume->ny - 1, 0, volume->nz - 1);
	image_import->SetDataExtentToWholeExtent();
	image_import->SetDataScalarTypeToShort();
	image_import->SetNumberOfScalarComponents(1);
	image_import->SetImportVoidPointer(volume->data);
	image_import->Update();

	vtkSmartPointer<vtkMarchingCubes> marchingCubes = vtkSmartPointer<vtkMarchingCubes>::New();
	marchingCubes->SetInputConnection(image_import->GetOutputPort());
	marchingCubes->SetValue(0, 1e4);

	vtkSmartPointer<vtkStripper> stripper = vtkSmartPointer<vtkStripper>::New();
	stripper->SetInputConnection(marchingCubes->GetOutputPort());

	vtkSmartPointer<vtkPolyDataConnectivityFilter> connectivityFilter = vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
	connectivityFilter->SetInputConnection(stripper->GetOutputPort());
	connectivityFilter->SetExtractionModeToLargestRegion();
	connectivityFilter->Update();

	vtkNew<vtkSmoothPolyDataFilter> smoothFilter;
	smoothFilter->SetInputData(connectivityFilter->GetOutput());
	smoothFilter->SetNumberOfIterations(15);
	smoothFilter->SetRelaxationFactor(0.1);
	smoothFilter->Update();

	mesh = smoothFilter->GetOutput();

	isFirstRead = true;
	updateView();
}

void VtkWidget::setRenderingMode(RENDERING_MODE mode) {
	renderingMode = mode;
	updateView();
}

void VtkWidget::updateView() {
	this->renderer->RemoveAllViewProps();

	if (renderingMode == VOLUME_RENDERING) {
		vtkSmartPointer<vtkImageImport> image_import = vtkSmartPointer<vtkImageImport>::New();
		image_import->SetDataSpacing(volume->dx, volume->dy, volume->dz);
		image_import->SetDataOrigin(0, 0, 0);
		image_import->SetWholeExtent(0, volume->nx - 1, 0, volume->ny - 1, 0, volume->nz - 1);
		image_import->SetDataExtentToWholeExtent();
		image_import->SetDataScalarTypeToUnsignedShort();
		image_import->SetNumberOfScalarComponents(1);
		image_import->SetImportVoidPointer(volume->data);
		image_import->Update();

		vtkSmartPointer<vtkFixedPointVolumeRayCastMapper> volumeMapper = vtkSmartPointer<vtkFixedPointVolumeRayCastMapper>::New();
		volumeMapper->SetInputConnection(image_import->GetOutputPort());

		vtkSmartPointer<vtkColorTransferFunction> volumeColor = vtkSmartPointer<vtkColorTransferFunction>::New();
		volumeColor->AddRGBPoint(0, 0.0, 0.0, 0.0);
		volumeColor->AddRGBPoint(100, 1.0, 0.1, 0.0);

		vtkSmartPointer<vtkPiecewiseFunction> volumeScalarOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
		volumeScalarOpacity->AddPoint(0, 0.00);
		volumeScalarOpacity->AddPoint(100, 1.0);

		vtkSmartPointer<vtkPiecewiseFunction> volumeGradientOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
		volumeGradientOpacity->AddPoint(0, 0.0);
		volumeGradientOpacity->AddPoint(100, 1.0);

		vtkSmartPointer<vtkVolumeProperty> volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
		volumeProperty->SetColor(volumeColor);
		volumeProperty->SetScalarOpacity(volumeScalarOpacity);
		volumeProperty->SetGradientOpacity(volumeGradientOpacity);
		volumeProperty->SetInterpolationTypeToLinear();
		volumeProperty->ShadeOn();
		volumeProperty->SetAmbient(0.6);
		volumeProperty->SetDiffuse(0.8);
		volumeProperty->SetSpecular(0.2);

		vtkSmartPointer<vtkVolume> volumeVTK = vtkSmartPointer<vtkVolume>::New();
		volumeVTK->SetMapper(volumeMapper);
		volumeVTK->SetProperty(volumeProperty);
		renderer->AddViewProp(volumeVTK);
	} else if (renderingMode == MESH_RENDERING) {
		vtkSmartPointer<vtkPolyDataMapper> vessel_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		vessel_mapper->SetInputData(mesh);
		vessel_mapper->ScalarVisibilityOff();

		vtkSmartPointer<vtkActor> vessel_actor = vtkSmartPointer<vtkActor>::New();
		vessel_actor->SetMapper(vessel_mapper);
		vessel_actor->GetProperty()->SetColor(1.0, 0.1, 0.0);

		this->renderer->AddActor(vessel_actor);
	} else if (renderingMode == MESH_RENDERING_WITH_SCALARS) {
		vtkSmartPointer<vtkLookupTable> color_table = vtkSmartPointer<vtkLookupTable>::New();
		color_table->SetNumberOfColors(2);
		color_table->SetTableValue(0, 1.0, 0.1, 0.0);
		color_table->SetTableValue(1, 0.2, 0.2, 0.2);
		color_table->Build();

		vtkSmartPointer<vtkPolyDataMapper> vessel_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		vessel_mapper->SetInputData(mesh);
		vessel_mapper->SetScalarRange(0, 1);
		vessel_mapper->SetLookupTable(color_table);

		vtkSmartPointer<vtkActor> vessel_actor = vtkSmartPointer<vtkActor>::New();
		vessel_actor->SetMapper(vessel_mapper);

		this->renderer->AddActor(vessel_actor);
	}

	if (isFirstRead) {
		this->renderer->ResetCamera();
		isFirstRead = false;
	}
	this->GetRenderWindow()->Render();
	this->update();
}

void VtkWidget::updateScalars(std::vector<int> &scalars) {
	for (int i = 0; i < scalars.size(); ++i) {
		mesh_scalars->InsertNextTuple1(scalars[i]);
	}
	mesh->GetPointData()->SetScalars(mesh_scalars);
}
