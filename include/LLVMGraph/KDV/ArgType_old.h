#ifndef ARG_TYPE_H
#define ARG_TYPE_H

#include <llvm/Argument.h>
#include <llvm/DerivedTypes.h>
#include <iostream>

#include <string>

// This is a simplified class for argument types
class ArgType {

    friend class KernelVolume;
    friend class ArgParser;
    friend class BreadCrumbVertexFilter; 
    friend class DeadEndVertexFilter;
    template <class> friend class NodePropertyWriter;
    friend class ITypeHandler;
    friend class BaseTypeHandler;

public: 


protected:

public:
    /*
    friend std::ostream& operator<<(std::ostream &out, const edgeIterator &e) { //const ArgType &a) {
        //out << "Type: " << a.t << " Name: " << a.name;
        //return out;
    }
    */

    //uint64_t getBitWidth() const { return bitwidth; }
    //std::string getName() const { return name; }

    //llvm::StructType* getMapNodeType() const { return MapNodeType; }

};

class DependencyType : public ArgType {

    friend class KernelVolume;
    friend class ArgParser;
    template <class> friend struct edge_compare;
    template <class> friend class EdgePropertyWriter;

private:
    //unsigned int index = 0;

public:
    //unsigned int getIndex() const { return index; }

};

#endif /* ARG_TYPE_H */
