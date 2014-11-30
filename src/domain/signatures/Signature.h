#ifndef __ShapeAnalyzer__Signature__
#define __ShapeAnalyzer__Signature__

#include "../Shape.h"

/// Namespace grouping all signature classes.
namespace signature {
    /// \brief Abstract class for computation of signatures.
    class Signature {
    public:
        /// \brief Signature constructor.
        /// @param Shape* The shape on which the signature is computed.
        /// @param int Number of components that the signature should have.
        Signature(Shape* shape, int dimension) : shape_(shape), dimension_(dimension) {}
        
        /// \brief Virtual destructor.
        virtual ~Signature() {}
        
        /// \brief Returns the number of components of the signature that should be computed.
        int getDimension() {
            return dimension_;
        }
        
        /// \brief Returns i-th component of the signature.
        /// @param int Index of the component
        virtual vtkSmartPointer<vtkDoubleArray> getComponent(int i) = 0;
        
        
    protected:
        Shape* shape_;
        int dimension_;
    };
}

#endif /* defined(__ShapeAnalyzer__Signature__) */
