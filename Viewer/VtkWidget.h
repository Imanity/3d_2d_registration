#pragma once

#include <vtkAutoInit.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>

#include <QVTKWidget.h>

#include "../3d_2d_registration/VolumeData.h"

VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);

enum RENDERING_MODE {
	VOLUME_RENDERING, 
	MESH_RENDERING
};

class VtkWidget : public QVTKWidget {
	Q_OBJECT

public:
	VtkWidget(QWidget *parent);
	~VtkWidget();

private:
	vtkSmartPointer<vtkRenderer> renderer;
	vtkSmartPointer<vtkPolyData> mesh;
	VolumeData *volume = NULL;
	RENDERING_MODE renderingMode = VOLUME_RENDERING;
	bool isFirstRead;

public:
	void setVolume(VolumeData *);
	void setRenderingMode(RENDERING_MODE mode);
	void updateView();
};
