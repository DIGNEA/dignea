//
// Created by amarrero on 21/12/20.
//

#ifndef DIGNEA_EMPTYPOPULATION_H
#define DIGNEA_EMPTYPOPULATION_H

#include <stdexcept>

class EmptyPopulation : public std::runtime_error {
   public:
    EmptyPopulation(std::string &where)
        : std::runtime_error("Empty population error at: " + where){};

    virtual ~EmptyPopulation() = default;
};

#endif  // DIGNEA_EMPTYPOPULATION_H
