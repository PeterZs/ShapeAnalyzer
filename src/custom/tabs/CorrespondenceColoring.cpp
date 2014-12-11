////
////  CorrespondenceColoring.cpp
////  ShapeAnalyzer
////
////  Created by Zorah on 08.07.14.
////
////
//
//#include "CorrespondenceColoring.h"
//#include <utility>
//
/////////////////////////////////////////////////////////////////////////////////
//CorrespondenceColoring::CorrespondenceColoring(
//                                               const HashMap<vtkActor*, Shape*>& shapes,
//                                               const HashMap<PointCorrespondence*, bool>& pointCorrespondences,
//                                               const HashMap<FaceCorrespondence*, bool>&  faceCorrespondences,
//                                               Shape* reference)
//: shapes_(shapes), pointCorrespondences_(pointCorrespondences), faceCorrespondences_(faceCorrespondences), reference_(reference)
//{
//    if(reference_ == nullptr) {
//        reference_ = shapes_.begin()->second;
//    }
//}
//
//
/////////////////////////////////////////////////////////////////////////////////
//void CorrespondenceColoring::showPointCorrespondences(shared_ptr<vector<pair<vtkIdType, double>>> percentageMatched,
//                                                      shared_ptr<vector<pair<vtkIdType, double>>> percentageMultiple) {
//    pointAttributes_.clear();
//    
//    // completely color reference shape
//    reference_->colorPointsCoordinates();
//    
//    vtkIdType referenceId = reference_->getId();
//    vtkSmartPointer<vtkDataArray> referenceColors = reference_->getColoring()->values;
//    pointAttributes_.insert(make_pair(referenceId, referenceColors));
//    
//    // for evaluating matched points
//    unordered_map<vtkIdType, shared_ptr<vector<int>>> matched;
//    
//    // initialize color arrays for all shapes
//    for (auto entry : shapes_) {
//        if(entry.second != reference_) {
//            vtkSmartPointer<vtkUnsignedCharArray> array = vtkSmartPointer<vtkUnsignedCharArray>::New();
//            array->SetNumberOfComponents(3);
//            array->SetNumberOfTuples(entry.second->getPolyData()->GetPoints()->GetNumberOfPoints());
//            array->SetName("Colors");
//            pointAttributes_.insert(make_pair(entry.second->getId(), array));
//            
//
//            double white[3] = {255, 255, 255};
//            for(vtkIdType i = 0; i < entry.second->getPolyData()->GetPoints()->GetNumberOfPoints(); i++) {
//                array->SetTuple(i, white);
//            }
//            
//            // if requested, evaluate matched points
//            if (percentageMatched != nullptr || percentageMultiple != nullptr) {
//                shared_ptr<vector<int>> mArray = make_shared<vector<int>>(entry.second->getPolyData()->GetPoints()->GetNumberOfPoints(), 0);
//                matched.insert(make_pair(entry.second->getId(), mArray));
//            }
//        }
//    }
//    
//    // look up colors for non-reference shapes
//    for(auto entry : pointCorrespondences_) {
//        vector<vtkIdType> shapes = entry.first->getShapeIds();
//        vector<vtkIdType> points = entry.first->getCorrespondingIds();
//        
//        // id of correspondence on reference shape
//        vtkIdType referenceCorrespondence = -1;
//        
//        // check if reference is in correspondence
//        for(int i = 0; i < shapes.size(); i++) {
//            if (shapes[i] == referenceId) {
//                referenceCorrespondence = points[i];
//                break;
//            }
//        }
//        
//        // if reference found
//        if(referenceCorrespondence != -1) {
//            // look up color for each shape in correspondence
//            for (int i = 0; i < shapes.size(); i++) {
//                // dont color reference shape
//                if (shapes[i] != referenceId) {
//                    if(pointAttributes_.count(shapes[i]) != 0) {
//                        vtkSmartPointer<vtkDataArray> colors = pointAttributes_.find(shapes[i])->second;
//                        // color of point is the same as the corresponding point on the reference shape has
//                        double color[3];
//                        referenceColors->GetTuple(referenceCorrespondence, color);
//                        colors->SetTuple(points[i], color);
//                        
//                        // if requested, evaluate matched points
//                        if (percentageMatched != nullptr || percentageMultiple != nullptr) {
//                            shared_ptr<vector<int>> match = matched.find(shapes[i])->second;
//                            match->at(points[i]) = match->at(points[i]) + 1;
//                        }
//                    }
//                }
//            }
//        }
//    }
//    
//    // color all shapes
//    for (auto entry : shapes_) {
//        if (entry.second != reference_) {
//            vtkSmartPointer<vtkDataArray> colors = pointAttributes_.find(entry.second->getId())->second;
//            shared_ptr<Shape::Coloring> coloring = make_shared<Shape::Coloring>();
//            coloring->type = Shape::Coloring::Type::PointRgb;
//            coloring->values = colors;
//            entry.second->setColoring(coloring);
//            
//            // if requested, evaluate matched points
//            if (percentageMatched != 0 || percentageMultiple != 0) {
//                shared_ptr<vector<int>> match = matched.find(entry.second->getId())->second;
//                int numMatched = 0;
//                int numMultiple = 0;
//                
//                // count matched / multiple matched points
//                for (int i = 0; i < match->size(); i++) {
//                    if (match->at(i) > 0)
//                        numMatched++;
//                    if (match->at(i) > 1)
//                        numMultiple++;
//                }
//                
//                // add percentage matched points
//                if (percentageMatched != nullptr) {
//                    double per = ((double) numMatched) * 100 / match->size();
//                    percentageMatched->push_back(make_pair(entry.second->getId(), per));
//                }
//                
//                // add percentage mulitple matched points
//                if (percentageMultiple != nullptr) {
//                    double per = ((double) numMultiple) * 100 / match->size();
//                    percentageMultiple->push_back(make_pair(entry.second->getId(), per));
//                }
//            }
//            
//        }
//    }
//}
//
/////////////////////////////////////////////////////////////////////////////////
//void CorrespondenceColoring::showFaceCorrespondences(shared_ptr<vector<pair<vtkIdType, double>>> percentageMatched,
//                                                     shared_ptr<vector<pair<vtkIdType, double>>> percentageMultiple) {
//    faceAttributes_.clear();
//    
//    // completely color reference shape
//    reference_->colorFacesCoordinates();
//    
//    vtkIdType referenceId = reference_->getId();
//    vtkSmartPointer<vtkDataArray> referenceColors = reference_->getColoring()->values;
//    faceAttributes_.insert(make_pair(referenceId, referenceColors));
//    
//    // for evaluating matched points
//    unordered_map<vtkIdType, shared_ptr<vector<int>>> matched;
//    
//    // initialize color arrays for all shapes
//    for (auto entry : shapes_) {
//        if(entry.second != reference_) {
//            vtkSmartPointer<vtkUnsignedCharArray> array = vtkSmartPointer<vtkUnsignedCharArray>::New();
//            array->SetNumberOfComponents(3);
//            array->SetNumberOfTuples(entry.second->getPolyData()->GetNumberOfCells());
//            array->SetName("Colors");
//            faceAttributes_.insert(make_pair(entry.second->getId(), array));
//            
//            double white[3] = {255, 255, 255};
//            for(vtkIdType i = 0; i < entry.second->getPolyData()->GetNumberOfCells(); i++) {
//                array->SetTuple(i, white);
//            }
//            
//            // if requested, evaluate matched points
//            if (percentageMatched != nullptr || percentageMultiple != nullptr) {
//                shared_ptr<vector<int>> mArray = make_shared<vector<int>>(entry.second->getPolyData()->GetNumberOfCells(), 0);
//                matched.insert(make_pair(entry.second->getId(), mArray));
//            }
//        }
//    }
//    
//    // look up colors for non-reference shapes
//    for(auto entry : faceCorrespondences_) {
//        vector<vtkIdType> shapes = entry.first->getShapeIds();
//        vector<vtkIdType> points = entry.first->getCorrespondingIds();
//        
//        // id of correspondence on reference shape
//        vtkIdType referenceCorrespondence = -1;
//        
//        // check if reference is in correspondence
//        for(int i = 0; i < shapes.size(); i++) {
//            if (shapes[i] == referenceId) {
//                referenceCorrespondence = points[i];
//                break;
//            }
//        }
//        
//        if(referenceCorrespondence != -1) {
//            // look up color for each shape in correspondence
//            for (int i = 0; i < shapes.size(); i++) {
//                // dont color reference shape
//                if (shapes[i] != referenceId) {
//                    if(faceAttributes_.count(shapes[i]) != 0) {
//                        vtkSmartPointer<vtkDataArray> colors = faceAttributes_.find(shapes[i])->second;
//                        // color of point is the same as the corresponding point on the reference shape has
//                        double color[3];
//                        referenceColors->GetTuple(referenceCorrespondence, color);
//                        colors->SetTuple(points[i], color);
//                        
//                        // if requested, evaluate matched points
//                        if (percentageMatched != nullptr || percentageMultiple != nullptr) {
//                            shared_ptr<vector<int>> match = matched.find(shapes[i])->second;
//                            match->at(points[i]) = match->at(points[i]) + 1;
//                        }
//                    }
//                }
//            }
//        }
//    }
//    
//    // color all shapes
//    for (auto entry : shapes_) {
//        if(entry.second != reference_) {
//            vtkSmartPointer<vtkDataArray> colors = faceAttributes_.find(entry.second->getId())->second;
//            shared_ptr<Shape::Coloring> coloring = make_shared<Shape::Coloring>();
//            coloring->type = Shape::Coloring::Type::FaceRgb;
//            coloring->values = colors;
//            entry.second->setColoring(coloring);
//            
//            // if requested, evaluate matched points
//            if (percentageMatched != 0 || percentageMultiple != 0) {
//                shared_ptr<vector<int>> match = matched.find(entry.second->getId())->second;
//                int numMatched = 0;
//                int numMultiple = 0;
//                
//                // count matched / multiple matched points
//                for (int i = 0; i < match->size(); i++) {
//                    if (match->at(i) > 0)
//                        numMatched++;
//                    if (match->at(i) > 1)
//                        numMultiple++;
//                }
//                
//                // add percentage matched points
//                if (percentageMatched != nullptr) {
//                    double per = ((double) numMatched) * 100 / match->size();
//                    percentageMatched->push_back(make_pair(entry.second->getId(), per));
//                }
//                
//                // add percentage mulitple matched points
//                if (percentageMultiple != nullptr) {
//                    double per = ((double) numMultiple) * 100 / match->size();
//                    percentageMultiple->push_back(make_pair(entry.second->getId(), per));
//                }
//            }
//        }
//    }
//}