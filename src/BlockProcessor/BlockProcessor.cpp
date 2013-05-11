/*
 * BlockProcessor.cpp
 *
 *  Created on: Nov 11, 2012
 *      Author: bzcdress
 */

#include <LLVMGraph/BlockProcessor.h>

#include <llvm/Support/raw_ostream.h>

void BlockProcessor::run() {

    //std::string bbname = "4";
    std::string bbname = "*";

    llvm::errs() << "BB processor attached.\n";

    // First pass create all necessary "root" vertices
    for (auto it = F->begin(); it != F->end(); ++it) {

        // Insert new vertex into the graph and also register it
        // in the vertex / name map
        std::string BBName = it->getName().str();

        auto v = g->addVertex(it);
        v.first->second.name = BBName;
    }

    for (auto &it : *F) {
        analyzeBB(&it);
    }

}
