//
//  vtkToscaWriter.cpp
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 28.08.14.
//
//

#include "vtkToscaWriter.h"

vtkStandardNewMacro(vtkToscaWriter);

vtkToscaWriter::vtkToscaWriter() {
    std::ofstream fout; // only used to extract the default precision
    this->DecimalPrecision = fout.precision();
    this->FileName = NULL;
}

vtkToscaWriter::~vtkToscaWriter() {}

void vtkToscaWriter::WriteData() {
    vtkPolyData *input = this->GetInput();
    if (!input)
        return;
    
    if (!this->FileName) {
        vtkErrorMacro(<< "No FileName specified! Can't write!");
        this->SetErrorCode(vtkErrorCode::NoFileNameError);
        return;
    }
    
    vtkDebugMacro(<<"Opening vert file for writing...");
    ostream *vertoutfilep = new ofstream(this->FileName, ios::out);
    if (vertoutfilep->fail()) {
        vtkErrorMacro(<< "Unable to open file: "<< this->FileName);
        this->SetErrorCode(vtkErrorCode::CannotOpenFileError);
        delete vertoutfilep;
        return;
    }
    
    std::ostream& vertoutfile = *vertoutfilep;
    
    std::regex reg(".vert$", std::regex_constants::ECMAScript | std::regex_constants::icase);
    std::string FileNameTri = std::regex_replace(FileName, reg, ".tri");
    vtkDebugMacro(<<"Opening tri file for writing...");
    ostream *trioutfilep = new ofstream(FileNameTri, ios::out);
    if (trioutfilep->fail()) {
        vtkErrorMacro(<< "Unable to open file: "<< FileNameTri);
        this->SetErrorCode(vtkErrorCode::CannotOpenFileError);
        delete trioutfilep;
        return;
    }

    for(vtkIdType i = 0; i < input->GetNumberOfPoints(); i++) {
        double p[3];
        input->GetPoints()->GetPoint(i, p);
        vertoutfile << p[0] <<" "<< p[1] <<" "<< p[2]<<endl;
    }
    
    std::ostream& trioutfile = *trioutfilep;
    for(vtkIdType i = 0; i < input->GetNumberOfCells(); i++) {
        vtkTriangle* triangle = (vtkTriangle*) input->GetCell(i);
        for(vtkIdType j = 0; j < 3; j++) {
            trioutfile << triangle->GetPointId(j)+1 << (j == 2 ? "" : " ");
        }
        trioutfile <<endl;
    }
    
    
    vtkDebugMacro(<<"Closing vtk files\n");
    delete vertoutfilep;
    delete trioutfilep;
    
    // Delete the file if an error occurred
    if (this->ErrorCode == vtkErrorCode::OutOfDiskSpaceError) {
        vtkErrorMacro("Ran out of disk space; deleting files: " << this->FileName <<", " << FileNameTri);
        std::remove(this->FileName);
        std::remove(FileNameTri.c_str());
    }
}

void vtkToscaWriter::PrintSelf(ostream& os, vtkIndent indent) {
    Superclass::PrintSelf(os, indent);
    os << indent << "DecimalPrecision: " << DecimalPrecision << "\n";
}

int vtkToscaWriter::FillInputPortInformation(int, vtkInformation *info) {
    info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkPolyData");
    return 1;
}

vtkPolyData* vtkToscaWriter::GetInput() {
    return vtkPolyData::SafeDownCast(this->Superclass::GetInput());
}

vtkPolyData* vtkToscaWriter::GetInput(int port) {
    return vtkPolyData::SafeDownCast(this->Superclass::GetInput(port));
}