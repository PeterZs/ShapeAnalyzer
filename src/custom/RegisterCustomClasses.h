//
//  RegisterCustomClasses.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 18.11.14.
//
//

#ifndef ShapeAnalyzer_RegisterCustomClasses_h
#define ShapeAnalyzer_RegisterCustomClasses_h

#include <memory>

#include <QWidget>

#include <vtkActor.h>

#include "Factory.h"

#include "tabs/CustomTab.h"
#include "tabs/ShapeInterpolationTab.h"
#include "tabs/MeshCheckTab.h"
#include "tabs/CorrespondenceColoringTab.h"

#include "contextMenuItems/CustomContextMenuItem.h"
#include "contextMenuItems/ColorEigenfunctionContextMenuItem.h"
#include "contextMenuItems/ColorMetricContextMenuItem.h"
#include "contextMenuItems/ColorSignatureContextMenuItem.h"
#include "contextMenuItems/HeatDiffusionContextMenuItem.h"
#include "contextMenuItems/VoronoiCellsContextMenuItem.h"
#include "contextMenuItems/ExtractPointSegmentContextMenuItem.h"

#include "../util/HashMap.h"

#include "../domain/Shape.h"

#include "../domain/correspondences/PointCorrespondenceData.h"
#include "../domain/correspondences/FaceCorrespondenceData.h"

#include "../domain/metric/Metric.h"
#include "../domain/metric/EuclideanMetric.h"
#include "../domain/metric/GeodesicMetric.h"

#include "../domain/signatures/Signature.h"
#include "../domain/signatures/WaveKernelSignature.h"

#include "../domain/segmentation/Segmentation.h"
#include "../domain/segmentation/VoronoiCellSegmentation.h"

typedef Factory<CustomTab*, const HashMap<vtkActor*, Shape*>&, const HashMap<PointCorrespondenceData*, bool>&, const HashMap<FaceCorrespondenceData*, bool>&, QWidget*> CustomTabFactory;

typedef Factory<shared_ptr<CustomContextMenuItem>> CustomContextMenuItemFactory;

using namespace signature;

//forward declarations
class ExtractPointSegmentContextMenuItem;
class ShapeInterpolationTab;

struct RegisterCustomClasses {
    static void registerTabs() {
        //tabs
        CustomTabFactory::getInstance()->Register<ShapeInterpolationTab>("shape_interpolation", "Shapes>>Shape Interpolation");
        CustomTabFactory::getInstance()->Register<MeshCheckTab>("mesh_check", "Shapes>>Mesh Checker");
        CustomTabFactory::getInstance()->Register<CorrespondenceColoringTab>("correspondence_coloring", "Correspondences>>Correspondence Coloring");
    }
    
    static void registerContextMenuItems() {
        //menu items
        CustomContextMenuItemFactory::getInstance()->Register<ColorMetricContextMenuItem<GeodesicMetric>>("color_metric_geodesic", "Coloring>>Metric>>Geodesic");
        
        CustomContextMenuItemFactory::getInstance()->Register<ColorMetricContextMenuItem<EuclideanMetric>>("color_metric_euclidean", "Coloring>>Metric>>Euclidean");
        
        CustomContextMenuItemFactory::getInstance()->Register<ColorSignatureContextMenuItem<WaveKernelSignature>>("color_signature_wavekernel", "Coloring>>Signature>>Wave Kernel");
        
        CustomContextMenuItemFactory::getInstance()->Register<VoronoiCellsContextMenuItem<GeodesicMetric>>("voronoicells_geodesic", "Segmentation>>Voronoi Cells>>Geodesic");
        
        CustomContextMenuItemFactory::getInstance()->Register<VoronoiCellsContextMenuItem<EuclideanMetric>>("voronoicells_euclidean", "Segmentation>>Voronoi Cells>>Euclidean");
        
        CustomContextMenuItemFactory::getInstance()->Register<ExtractPointSegmentContextMenuItem>("extract_point_segment", "Extract point segment as new Shape");
    }    
};

#endif
