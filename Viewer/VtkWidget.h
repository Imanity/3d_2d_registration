#pragma once

#include <vtkAutoInit.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkFloatArray.h>

#include <QVTKWidget.h>

#include "../3d_2d_registration/VolumeData.h"

VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);

enum RENDERING_MODE {
	VOLUME_RENDERING, 
	MESH_RENDERING, 
	MESH_RENDERING_WITH_SCALARS
};

class VtkWidget : public QVTKWidget {
	Q_OBJECT

public:
	VtkWidget(QWidget *parent);
	~VtkWidget();

public:
	vtkSmartPointer<vtkPolyData> mesh;

private:
	vtkSmartPointer<vtkRenderer> renderer;
	VolumeData *volume = NULL;
	RENDERING_MODE renderingMode = MESH_RENDERING;
	bool isFirstRead;

	vtkSmartPointer<vtkFloatArray> mesh_scalars;

public:
	void setVolume(VolumeData *);
	void setRenderingMode(RENDERING_MODE mode);
	void updateView();
	void updateScalars(std::vector<int> &scalars);
};
