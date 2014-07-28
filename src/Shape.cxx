#include "Shape.h"


///////////////////////////////////////////////////////////////////////////////
// Constructors and Destructor (and related functions)
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
Shape::Shape(
             vtkIdType shapeId,
             vtkSmartPointer<vtkPolyData> polyData,
             vtkSmartPointer<vtkRenderer> renderer
             )
: shapeId_(shapeId), polyData_(polyData), renderer_(renderer)
{
    
    initialize();
}


///////////////////////////////////////////////////////////////////////////////
Shape::Shape(vtkSmartPointer<vtkRenderer> renderer)
: renderer_(renderer)
{/* do not call initialize here! Poly data is not yet initialized! */}

void Shape::initialize() {
    //Visualize with normals. Looks smoother ;)
    polyDataNormals_ = vtkSmartPointer<vtkPolyDataNormals>::New();
    polyDataNormals_->SetInputData(polyData_);
    polyDataNormals_->ComputeCellNormalsOn();
    polyDataNormals_->Update();
    
    actor_ = vtkSmartPointer<vtkActor>::New();
    mapper_ = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper_->SetInputData(polyData_);
    
    
    actor_->SetMapper(mapper_);
    renderer_->AddActor(actor_);
    
    
    // set up box widget
    boxWidget_ = vtkSmartPointer<vtkBoxWidget>::New();
    boxWidget_->SetProp3D(actor_);
    boxWidget_->SetInteractor(renderer_->GetRenderWindow()->GetInteractor());
    boxWidget_->SetPlaceFactor(1.25);
    
    boxWidget_->PlaceWidget();
    
    fps_ = vtkSmartPointer<vtkIdList>::New();
    fpsActor_ = vtkSmartPointer<vtkActor>::New();
}


///////////////////////////////////////////////////////////////////////////////
void Shape::remove() {
    renderer_->RemoveActor(fpsActor_);
    renderer_->RemoveActor(actor_);
    boxWidget_->SetInteractor(nullptr);
    boxWidget_->SetProp3D(nullptr);
}


///////////////////////////////////////////////////////////////////////////////
// Public Functions
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
double Shape::calculateArea() {
    double area = 0.0;
    // add up the areas of all triangles
    for (int i = 0; i < polyData_->GetNumberOfCells(); i++) {
        // store point coordinates in a, b, c
        vtkSmartPointer<vtkPoints> ids = polyData_->GetCell(i)->GetPoints();
        double a[3], b[3], c[3];
        ids->GetPoint(0, a);
        ids->GetPoint(1, b);
        ids->GetPoint(2, c);
        
        // side lengths
        double ab = sqrt(pow(a[0] - b[0], 2) + pow(a[1] - b[1], 2) + pow(a[2] - b[2], 2));
        double bc = sqrt(pow(c[0] - b[0], 2) + pow(c[1] - b[1], 2) + pow(c[2] - b[2], 2));
        double ca = sqrt(pow(a[0] - c[0], 2) + pow(a[1] - c[1], 2) + pow(a[2] - c[2], 2));
        
        double s = (ab + bc + ca) / 2;
        
        area = area + sqrt(s * (s - ab) * (s - bc) + (s - ca));
    }
    
    return area;
}


///////////////////////////////////////////////////////////////////////////////
unsigned Shape::getRandomPoint() {
    return std::rand() % polyData_->GetPoints()->GetNumberOfPoints();
}


///////////////////////////////////////////////////////////////////////////////
void Shape::visualizeVoronoiCells() {
    if (fps_->GetNumberOfIds() > 0) {
        visualizeVoronoiCells(fps_);
    } else {
        visualizeVoronoiCells(getFPS(10));
    }
}


///////////////////////////////////////////////////////////////////////////////
void Shape::visualizeVoronoiCells(vtkSmartPointer<vtkIdList> points) {
    // initialize
    vtkSmartPointer<vtkIdList> voronoi = getVoronoiCells(points);
    double max = points->GetNumberOfIds();
    
    vtkSmartPointer<vtkLookupTable> colorLookupTable =
    vtkSmartPointer<vtkLookupTable>::New();
    colorLookupTable->SetTableRange(0, max - 1);
    colorLookupTable->SetNumberOfColors(12);
    colorLookupTable->SetScaleToLinear();
    //colorLookupTable->SetRampToLinear();
    colorLookupTable->SetSaturationRange(0.5, 1.0);
    colorLookupTable->SetHueRange(0, 1.0);
    colorLookupTable->SetValueRange(0.8, 1);
    colorLookupTable->Build();
    
    // list of colors for vtk
    vtkSmartPointer<vtkUnsignedCharArray> colors =
    vtkSmartPointer<vtkUnsignedCharArray>::New();
    colors->SetNumberOfComponents(3);
    colors->SetName("Colors");
    
    for(int i = 0; i < this->getPolyData()->GetNumberOfCells(); i++) {
        vtkSmartPointer<vtkIdList> ids = vtkSmartPointer<vtkIdList>::New();
        
        // each cell has three id references to the corresponding points
        polyData_->GetCellPoints(i, ids);
        
        // assign the id that at least 2 points share, 0 otherwise
        double id;
        if(voronoi->GetId(ids->GetId(1)) == voronoi->GetId(ids->GetId(2))) {
            id = voronoi->GetId(ids->GetId(1));
        } else {
            id = voronoi->GetId(ids->GetId(0));
        }
        
        double dcolor[3];
        colorLookupTable->GetColor(id, dcolor);
        
        unsigned char color[3];
        for(unsigned int j = 0; j < 3; j++) {
            color[j] = static_cast<unsigned char>(255.0 * dcolor[j]);
        }
        
        colors->InsertNextTupleValue(color);
    }
    
    // update vtk
    polyData_->GetCellData()->SetScalars(colors);
    polyData_->Modified();
    
    polyDataNormals_->Update();
    polyDataNormals_->Modified();
    
    renderer_->GetRenderWindow()->Render();
}

// Voronoi Cells

///////////////////////////////////////////////////////////////////////////////
vtkSmartPointer<vtkIdList> Shape::getVoronoiCells(vtkSmartPointer<vtkIdList> points) {
    vtkGeodesic geodesic = vtkGeodesic(this, points);
    
    return geodesic.getVoronoiCells();
}


///////////////////////////////////////////////////////////////////////////////
// fps with given starting point
vtkSmartPointer<vtkIdList> Shape::getFPS(unsigned numberSamples, int start) {
    // random start if none was chosen
    if(start == -1)
        start = std::rand() % polyData_->GetPoints()->GetNumberOfPoints();
    
    // initialize id list with starting point
    vtkSmartPointer<vtkIdList> list = vtkSmartPointer<vtkIdList>::New();
    list->InsertNextId(start);
    
    //vtkGeodesic* geodesics = new vtkGeodesic(this, list);
    vtkGeodesic geodesics(this, list);
    
    for(int i = 1; i < numberSamples; i++) {
        
        list->InsertNextId(geodesics.findPointFurthestToAllSources());
        
        geodesics.changeSourcePoints(list);
        
    }
    
    return list;
}


///////////////////////////////////////////////////////////////////////////////
void Shape::setFPS(unsigned numberSamples, int start) {
    fps_ = getFPS(numberSamples, start);
    
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    polyData_->GetPoints()->GetPoints(fps_, points);
    
    vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
    vtkSmartPointer<vtkSphereSource> sphere = vtkSmartPointer<vtkSphereSource>::New();
    sphere->SetRadius(2.5);
    polyData->SetPoints(points);
    
    // transform points into vertices
    vtkSmartPointer<vtkGlyph3D> glyph3D = vtkSmartPointer<vtkGlyph3D>::New();

    glyph3D->SetSourceConnection(sphere->GetOutputPort());
    glyph3D->SetInputData(polyData);
    glyph3D->Update();
    
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(glyph3D->GetOutputPort());
    
    fpsActor_->SetMapper(mapper);
    fpsActor_->GetProperty()->SetColor(1, 0, 0);
    
    renderer_->AddActor(fpsActor_);
    
    
    renderer_->GetRenderWindow()->Render();
}


///////////////////////////////////////////////////////////////////////////////
void Shape::transformFPS(vtkLinearTransform *t) {
    fpsActor_->SetUserTransform(t);
}



///////////////////////////////////////////////////////////////////////////////
// Serialization Functions
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//write shape binary
ostream& Shape::write(ostream& os) {
    //write shape ID.
    int64_t shapeId = (int64_t) shapeId_;
    os.write(reinterpret_cast<const char*>(&shapeId), sizeof(int64_t));

    vtkSmartPointer<vtkMatrix4x4> transform = actor_->GetUserMatrix();
    //if user has not transformed shape write identity.
    if(transform == nullptr) {
        transform = vtkSmartPointer<vtkMatrix4x4>::New();
        transform->Identity();
    }
    
    //write elements of 4x4 matrix
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            double value = transform->GetElement(i, j);
            os.write(reinterpret_cast<const char*>(&value), sizeof(double));
        }
    }
    
    //write number of points.
    int64_t numberOfPoints = (int64_t) polyData_->GetNumberOfPoints();
    os.write(reinterpret_cast<const char*>(&numberOfPoints), sizeof(int64_t));
    
    
    //write number of faces.
    int64_t numberOfFaces = (int64_t) polyData_->GetNumberOfCells();
    os.write(reinterpret_cast<const char*>(&numberOfFaces), sizeof(int64_t));
    
    //write points.
    for(int64_t i = 0; i < numberOfPoints; i++) {
        double point[3];
        polyData_->GetPoints()->GetPoint(i, point);
        
        os.write(reinterpret_cast<const char*>(point), 3 * sizeof(double));
    }
    
    //write faces.
    for(int64_t i = 0; i < numberOfFaces; i++) {
        for(int j = 0; j < 3; j++) {
            int64_t pointId = (int64_t) polyData_->GetCell(i)->GetPointId(j);
            os.write(reinterpret_cast<const char*>(&pointId), sizeof(int64_t));
        }
    }
    
    return os;
}


///////////////////////////////////////////////////////////////////////////////
//write as ascii txt
ostream& operator<<(ostream& os, const Shape& shape) {
    os << shape.shapeId_<< endl;
    
    vtkSmartPointer<vtkMatrix4x4> transform = shape.actor_->GetUserMatrix();
    if(transform == nullptr) {
        transform = vtkSmartPointer<vtkMatrix4x4>::New();
        transform->Identity();
    }
    
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            os << transform->GetElement(i, j) << (j != 3 ? "\t" : "");
        }
        os << endl;
    }
    
    os << shape.polyData_->GetNumberOfPoints() << "\t" << shape.polyData_->GetNumberOfCells() << endl;
    
    for(vtkIdType i = 0; i < shape.polyData_->GetNumberOfPoints(); i++) {
        double point[3];
        shape.polyData_->GetPoints()->GetPoint(i, point);
        os << point[0] << "\t" << point[1] << "\t" << point[2] << endl;
    }
    
    for(vtkIdType i = 0; i < shape.polyData_->GetNumberOfCells(); i++) {
        os << shape.polyData_->GetCell(i)->GetPointId(0) << "\t" << shape.polyData_->GetCell(i)->GetPointId(1) << "\t" << shape.polyData_->GetCell(i)->GetPointId(2) << endl;
    }
    
    return os;
}


///////////////////////////////////////////////////////////////////////////////
//read shape binary
istream& Shape::read(istream& is) {
    //read shape ID
    int64_t shapeId;
    is.read(reinterpret_cast<char*>(&shapeId), sizeof(int64_t));
    shapeId_ = shapeId;
    
    //read user transform. Set user transform in actor after poly data has been read and shape has been initialized and therefore actor_ != nullptr
    vtkSmartPointer<vtkMatrix4x4> matrix = vtkSmartPointer<vtkMatrix4x4>::New();
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            double value;
            is.read(reinterpret_cast<char*>(&value), sizeof(double));
            matrix->SetElement(i, j, value);
        }
    }
    
    //read number of points.
    int64_t numberOfPoints;
    is.read(reinterpret_cast<char*>(&numberOfPoints), sizeof(int64_t));
    //read number of faces.
    int64_t numberOfFaces;
    is.read(reinterpret_cast<char*>(&numberOfFaces), sizeof(int64_t));
    
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkCellArray> polys = vtkSmartPointer<vtkCellArray>::New();
    
    //read points
    for(int64_t i = 0; i < numberOfPoints; i++) {
        double point[3];
        is.read(reinterpret_cast<char*>(point), sizeof(double) * 3);
        points->InsertNextPoint(point);
    }
    
    //read faces
    for(int64_t i = 0; i < numberOfFaces; i++) {
        vtkSmartPointer<vtkTriangle> face = vtkSmartPointer<vtkTriangle>::New();
        for(int j = 0; j < 3; j++) {
            int64_t pointId;
            is.read(reinterpret_cast<char*>(&pointId), sizeof(int64_t));
            
            face->GetPointIds()->SetId(j, pointId);
        }
        polys->InsertNextCell(face);
    }
    polyData_ = vtkSmartPointer<vtkPolyData>::New();
    polyData_->SetPoints(points);
    polyData_->SetPolys(polys);
    
    initialize();
    
    actor_->SetUserMatrix(matrix);
    vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
    transform->SetMatrix(matrix);
    boxWidget_->SetTransform(transform);
    
    return is;
}


///////////////////////////////////////////////////////////////////////////////
//read shape as ascii txt
istream& operator>>(istream& is, Shape& shape) {
    string line;
    
    //read shape ID.
    {
        getline(is, line);
        stringstream ss;
        ss << line;
        ss >> shape.shapeId_;
    }

    //read user transform.
    vtkSmartPointer<vtkMatrix4x4> matrix = vtkSmartPointer<vtkMatrix4x4>::New();
    for(int i = 0; i < 4; i++) {
        getline(is, line);
        stringstream ss;
        ss << line;
        for(int j = 0; j < 4; j++) {
            double value;
            ss >> value;
            matrix->SetElement(i, j, value);
        }
    }
    
    //read number of points, number of faces
    vtkIdType numberOfPoints, numberOfFaces;
    {
        getline(is, line);
        stringstream ss;
        ss << line;
        ss >> numberOfPoints >> numberOfFaces;
    }
    
    //read points, faces
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkCellArray> polys = vtkSmartPointer<vtkCellArray>::New();
    
    for(vtkIdType i = 0; i < numberOfPoints; i++) {
        getline(is, line);
        stringstream ss;
        ss << line;
        double point[3];
        ss >> point[0] >> point[1] >> point[2];
        
        points->InsertNextPoint(point);

    }
    
    for(vtkIdType i = 0; i < numberOfFaces; i++) {
        getline(is, line);
        stringstream ss;
        ss << line;
        
        vtkSmartPointer<vtkTriangle> face = vtkSmartPointer<vtkTriangle>::New();

        for(int j = 0; j < 3; j++) {
            vtkIdType pointId;
            ss >> pointId;
            face->GetPointIds()->SetId(j, pointId);
        }
        polys->InsertNextCell(face);
    }
    
    shape.polyData_ = vtkSmartPointer<vtkPolyData>::New();
    shape.polyData_->SetPoints(points);
    shape.polyData_->SetPolys(polys);
    
    shape.initialize();
    
    shape.actor_->SetUserMatrix(matrix);
    vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
    transform->SetMatrix(matrix);
    shape.boxWidget_->SetTransform(transform);
    
    return is;
}


