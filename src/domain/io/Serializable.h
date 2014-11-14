#ifndef ShapeAnalyzer_Serializable_h
#define ShapeAnalyzer_Serializable_h

#include <iostream>
#include <string>
#include <sstream>

///
/// \brief Interface class providing basic read and write functionalities.
/// \details So far only implemented by Shape. The serialization of Correspondence objects has to be treated separately by SceneWriterReader since reading and writing of correspondences in the desired format at least requires the knowlegdge of the number of present shapes.
/// \author Emanuel Laude and Zorah Lähner
///
class Serializable {
    /// write all the fields representing this object to output stream os in binary.
    virtual ostream& writeBinary(ostream& os) = 0;

    /// write all the fields representing this object to output stream os in ASCII.
    virtual ostream& writeASCII(ostream& os) = 0;
    
    /// read all the fields representing this object from input stream is in binary.
    virtual istream& readBinary(istream& is) = 0;

    /// read all the fields representing this object from input stream is in ASCII.
    virtual istream& readASCII(istream& is) = 0;
    
};


#endif
