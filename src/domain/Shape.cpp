#include "Shape.h"


///////////////////////////////////////////////////////////////////////////////
// Constructors and Destructor (and related functions)
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
Shape::Shape(
             vtkIdType id,
             string name,
             vtkSmartPointer<vtkPolyData> polyData,
             vtkSmartPointer<vtkRenderer> renderer
             )
: id_(id), name_(name), polyData_(polyData), renderer_(renderer){}


///////////////////////////////////////////////////////////////////////////////
Shape::Shape(vtkSmartPointer<vtkRenderer> renderer)
: renderer_(renderer) {
    // do not call initialize here! Poly data is not yet initialized!
}

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
    boxWidget_ = vtkSmartPointer<vtkBoxWidget2>::New();
    vtkSmartPointer<vtkBoxRepresentation> boxRepresentation = vtkSmartPointer<vtkBoxRepresentation>::New();
    boxRepresentation->HandlesOff();
    boxRepresentation->SetPlaceFactor(1.25);
    boxRepresentation->PlaceWidget(actor_->GetBounds());
    boxWidget_->SetRepresentation(boxRepresentation);
    boxWidget_->SetInteractor(renderer_->GetRenderWindow()->GetInteractor());
}


///////////////////////////////////////////////////////////////////////////////
// Public Functions
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
void Shape::removeFromRenderer() {
    renderer_->RemoveActor(actor_);
    boxWidget_->SetInteractor(nullptr);
}


///////////////////////////////////////////////////////////////////////////////
void Shape::clearColoring() {
    coloring_.reset();
    
    mapper_->CreateDefaultLookupTable();
    mapper_->ScalarVisibilityOff();
    mapper_->SetColorModeToDefault();
    mapper_->InterpolateScalarsBeforeMappingOff();
    mapper_->SetScalarModeToDefault();
    mapper_->SetScalarMaterialModeToDefault();
    
    mapper_->Modified();
    
    renderer_->GetRenderWindow()->Render();
}


///////////////////////////////////////////////////////////////////////////////
void Shape::colorPointsCoordinates() {
    vtkSmartPointer<vtkUnsignedCharArray> colors = vtkSmartPointer<vtkUnsignedCharArray>::New();
    vtkSmartPointer<vtkPoints> points = polyData_->GetPoints();
    
    // color values
    colors->SetNumberOfComponents(3);
    colors->SetNumberOfTuples(points->GetNumberOfPoints());
    colors->SetName("Colors");
    
    // calculate range
    double xmin = 0, xmax = 0, ymin = 0, ymax = 0, zmin = 0, zmax = 0;
    
    for (vtkIdType i = 0; i < points->GetNumberOfPoints(); i++) {
        double point[3];
        points->GetPoint(i, point);
        
        // determine range of points
        xmin = std::min(xmin, point[0]);
        ymin = std::min(ymin, point[1]);
        zmin = std::min(zmin, point[2]);
        xmax = std::max(xmax, point[0]);
        ymax = std::max(ymax, point[1]);
        zmax = std::max(zmax, point[2]);
    }
    
    double xOffset = 0, yOffset = 0, zOffset = 0;
    
    if (xmin < 0) {
        xOffset = xmin * (-1);
        xmin = xmin + xOffset;
        xmax = xmax + xOffset;
    }
    if (ymin < 0) {
        yOffset = ymin * (-1);
        ymin = ymin + yOffset;
        ymax = ymax + yOffset;
    }
    if (zmin < 0) {
        zOffset = zmin * (-1);
        zmin = zmin + zOffset;
        zmax = zmax + zOffset;
    }
    
    // calculate colors
    for (vtkIdType i = 0; i < points->GetNumberOfPoints(); i++) {
        double point[3];
        points->GetPoint(i, point);
        
        point[0] = (point[0] + xOffset) / xmax * 255;
        point[1] = (point[1] + yOffset) / ymax * 255;
        point[2] = (point[2] + zOffset) / zmax * 255;
        
        colors->SetTuple(i, point);
    }
    
    shared_ptr<Coloring> coloring = make_shared<Coloring>();
    coloring->type = Coloring::Type::PointRgb;
    coloring->values = colors;
    setColoring(coloring);
}


///////////////////////////////////////////////////////////////////////////////
void Shape::colorFacesCoordinates() {
    vtkSmartPointer<vtkUnsignedCharArray> colors = vtkSmartPointer<vtkUnsignedCharArray>::New();
    vtkSmartPointer<vtkPoints> points = polyData_->GetPoints();
    
    // color values
    colors->SetNumberOfComponents(3);
    colors->SetNumberOfTuples(polyData_->GetNumberOfCells());
    colors->SetName("Colors");
    
    // calculate range
    double xmin = 0, xmax = 0, ymin = 0, ymax = 0, zmin = 0, zmax = 0;
    
    for (vtkIdType i = 0; i < points->GetNumberOfPoints(); i++) {
        double point[3];
        points->GetPoint(i, point);
        
        // determine range of points
        xmin = std::min(xmin, point[0]);
        ymin = std::min(ymin, point[1]);
        zmin = std::min(zmin, point[2]);
        xmax = std::max(xmax, point[0]);
        ymax = std::max(ymax, point[1]);
        zmax = std::max(zmax, point[2]);
    }
    
    double xOffset = 0, yOffset = 0, zOffset = 0;
    
    if (xmin < 0) {
        xOffset = xmin * (-1);
        xmin = xmin + xOffset;
        xmax = xmax + xOffset;
    }
    if (ymin < 0) {
        yOffset = ymin * (-1);
        ymin = ymin + yOffset;
        ymax = ymax + yOffset;
    }
    if (zmin < 0) {
        zOffset = zmin * (-1);
        zmin = zmin + zOffset;
        zmax = zmax + zOffset;
    }
    
    // calculate colors
    for (vtkIdType i = 0; i < polyData_->GetNumberOfCells(); i++) {
        vtkSmartPointer<vtkIdList> pointIds = polyData_->GetCell(i)->GetPointIds();
        
        double point1[3], point2[3], point3[3], color[3];
        
        points->GetPoint(pointIds->GetId(0), point1);
        points->GetPoint(pointIds->GetId(1), point2);
        points->GetPoint(pointIds->GetId(2), point3);
        
        // interpolate points to get color
        color[0] = (point1[0] + point2[0] + point3[0]) / 3;
        color[1] = (point1[1] + point2[1] + point3[1]) / 3;
        color[2] = (point1[2] + point2[2] + point3[2]) / 3;
        
        // range colors from 0 to 255
        color[0] = (color[0] + xOffset) / xmax * 255;
        color[1] = (color[1] + yOffset) / ymax * 255;
        color[2] = (color[2] + zOffset) / zmax * 255;
        
        colors->SetTuple(i, color);
    }
    
    shared_ptr<Coloring> coloring = make_shared<Coloring>();
    coloring->type = Coloring::Type::FaceRgb;
    coloring->values = colors;
    setColoring(coloring);
}


///////////////////////////////////////////////////////////////////////////////
double Shape::getArea() {
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
vtkIdType Shape::getRandomPoint() {
    return std::rand() % polyData_->GetPoints()->GetNumberOfPoints();
}

void Shape::setColoring(shared_ptr<Shape::Coloring> coloring) {
    // release old shared_ptr. Decrements ref count and if there is no other object referencing the coloring it will be destroyed.
    coloring_.reset();
    coloring_ = coloring;
    
    if(coloring_->type == Coloring::Type::PointScalar || coloring_->type == Coloring::Type::PointSegmentation) {
        double range[2];
        coloring_->values->GetRange(range);
        
        polyData_->GetPointData()->SetScalars(coloring_->values);
        polyData_->Modified();
        
        mapper_->SetScalarModeToUsePointData();
        mapper_->SetColorModeToMapScalars();
        mapper_->SetScalarRange(range[0], range[1]);
    } else if(coloring_->type == Coloring::Type::FaceScalar || coloring_->type == Coloring::Type::FaceSegmentation) {
        double range[2];
        coloring_->values->GetRange(range);
        
        polyData_->GetCellData()->SetScalars(coloring_->values);
        polyData_->Modified();
        
        mapper_->SetScalarModeToUseCellData();
        mapper_->SetColorModeToMapScalars();
        mapper_->SetScalarRange(range[0], range[1]);
    } else if(coloring_->type == Coloring::Type::PointRgb) {
        polyData_->GetPointData()->SetScalars(coloring_->values);
        
        mapper_->SetScalarModeToUsePointData();
        mapper_->SetColorModeToDefault();
        mapper_->ScalarVisibilityOn();
    } else if(coloring_->type == Coloring::Type::FaceRgb) {
        polyData_->GetCellData()->SetScalars(coloring_->values);
        
        mapper_->SetScalarModeToUseCellData();
        mapper_->ScalarVisibilityOn();
    } else {}
    
    mapper_->ScalarVisibilityOn();
    mapper_->Modified();
    
    polyDataNormals_->Update();
    polyDataNormals_->Modified();
    
    renderer_->GetRenderWindow()->Render();
}

///////////////////////////////////////////////////////////////////////////////
// Serialization Functions
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
ostream& Shape::writeBinary(ostream& os) {
    //write shape ID.
    int64_t id = (int64_t) id_;
    os.write(reinterpret_cast<const char*>(&id), sizeof(int64_t));
    
    const char* name = name_.c_str();
    
    int64_t length = name_.length();
    os.write(reinterpret_cast<const char*>(&length), sizeof(int64_t));
    os.write(name, length*sizeof(char));

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
istream& Shape::readBinary(istream& is) {
    //read shape ID
    int64_t id;
    is.read(reinterpret_cast<char*>(&id), sizeof(int64_t));
    id_ = id;
    
    int64_t length;
    is.read(reinterpret_cast<char*>(&length), sizeof(int64_t));
    char* name = new char[length+1];
    is.read(name, length*sizeof(char));
    name[length] = '\0';
    
    name_ = name;
    delete [] name;
    
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
    static_cast<vtkBoxRepresentation*>(boxWidget_->GetRepresentation())->SetTransform(transform);
    return is;
}


///////////////////////////////////////////////////////////////////////////////
ostream& Shape::writeASCII(ostream& os) {
    os << id_<< endl;
    os << name_ << endl;
    
    vtkSmartPointer<vtkMatrix4x4> transform = actor_->GetUserMatrix();
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
    
    os << polyData_->GetNumberOfPoints() << "\t" << polyData_->GetNumberOfCells() << endl;
    
    for(vtkIdType i = 0; i < polyData_->GetNumberOfPoints(); i++) {
        double point[3];
        polyData_->GetPoints()->GetPoint(i, point);
        os << point[0] << "\t" << point[1] << "\t" << point[2] << endl;
    }
    
    for(vtkIdType i = 0; i < polyData_->GetNumberOfCells(); i++) {
        os << polyData_->GetCell(i)->GetPointId(0) << "\t" << polyData_->GetCell(i)->GetPointId(1) << "\t" << polyData_->GetCell(i)->GetPointId(2) << endl;
    }
    
    return os;
}

///////////////////////////////////////////////////////////////////////////////
istream& Shape::readASCII(istream& is) {
    string line;
    
    //read shape ID.
    {
        getline(is, line);
        stringstream ss;
        ss << line;
        ss >> id_;
    }

    {
        getline(is, line);
        stringstream ss;
        ss << line;
        ss >> name_;
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
    
    polyData_ = vtkSmartPointer<vtkPolyData>::New();
    polyData_->SetPoints(points);
    polyData_->SetPolys(polys);
    
    initialize();
    
    actor_->SetUserMatrix(matrix);
    vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
    transform->SetMatrix(matrix);
    static_cast<vtkBoxRepresentation*>(boxWidget_->GetRepresentation())->SetTransform(transform);
    
    return is;
}
