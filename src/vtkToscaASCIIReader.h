#ifndef vtkToscaASCIIReader_h
#define vtkToscaASCIIReader_h

#include <vtkObjectFactory.h>
#include <vtkInformationVector.h>
#include <vtkInformation.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataAlgorithm.h>
#include <vtkCellArray.h>
#include <vtkFloatArray.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkObjectFactory.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkTriangle.h>


#include <regex>
#include <sstream>
#include <fstream>

class vtkToscaASCIIReader : public vtkPolyDataAlgorithm {
public:
    vtkTypeMacro(vtkToscaASCIIReader, vtkPolyDataAlgorithm);
    
    void PrintSelf(ostream& os, vtkIndent indent);
    
    static vtkToscaASCIIReader *New();
    
    vtkSetStringMacro(FileName);
    
    vtkGetStringMacro(FileName);
protected:
    vtkToscaASCIIReader();
    ~vtkToscaASCIIReader();
    int FillOutputPortInformation(int port, vtkInformation* info);
    int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);
private:
    vtkToscaASCIIReader(const vtkToscaASCIIReader&);  // Not implemented.
    void operator=(const vtkToscaASCIIReader&);  // Not implemented.
    
    char* FileName;
};

#endif

