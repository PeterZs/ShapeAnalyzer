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
#include "tabs/ShapeInterpolationCustomTab.h"

#include "contextMenuItems/CustomContextMenuItem.h"
#include "contextMenuItems/ColorEigenfunctionContextMenuItem.h"
#include "contextMenuItems/ColorMetricContextMenuItem.h"
#include "contextMenuItems/ColorSignatureContextMenuItem.h"
#include "contextMenuItems/HeatDiffusionContextMenuItem.h"
#include "contextMenuItems/VoronoiCellsContextMenuItem.h"
#include "contextMenuItems/ExtractSegmentContextMenuItem.h"

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

//forward declarations
class ExtractSegmentContextMenuItem;
class ShapeInterpolationCustomTab;

struct RegisterCustomClasses {
    static void Register() {
        //tabs
        CustomTabFactory::getInstance()->Register<ShapeInterpolationCustomTab>("shape_interpolation", "Shapes>>Shape Interpolation");
        
        
        //menu items
        CustomContextMenuItemFactory::getInstance()->Register<ColorMetricContextMenuItem<GeodesicMetric>>("color_metric_geodesic", "Coloring>>Metric>>Geodesic");
        
        CustomContextMenuItemFactory::getInstance()->Register<ColorMetricContextMenuItem<EuclideanMetric>>("color_metric_euclidean", "Coloring>>Metric>>Euclidean");
        
        CustomContextMenuItemFactory::getInstance()->Register<ColorSignatureContextMenuItem<WaveKernelSignature>>("color_signature_wavekernel", "Coloring>>Signature>>Wave Kernel");
        
        CustomContextMenuItemFactory::getInstance()->Register<VoronoiCellsContextMenuItem<GeodesicMetric>>("voronoicells_metric_geodesic", "Segmentation>>Voronoi Cells>>Geodesic");
        
        CustomContextMenuItemFactory::getInstance()->Register<VoronoiCellsContextMenuItem<EuclideanMetric>>("voronoicells_metric_euclidean", "Segmentation>>Voronoi Cells>>Euclidean");
        
        CustomContextMenuItemFactory::getInstance()->Register<ExtractSegmentContextMenuItem>("extract_segment", "Extract Segment as new Shape");
    }    
};

#endif
