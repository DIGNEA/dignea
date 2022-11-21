//
// Created by amarrero on 30/12/20.
//

#ifndef DIGNEA_MISMATCH_H
#define DIGNEA_MISMATCH_H

#include <stdexcept>

class Mismatch : public std::runtime_error {
   public:
    Mismatch(std::string &where)
        : std::runtime_error("Runtime Mismatch at: " + where){};

    virtual ~Mismatch() = default;
};

#endif  // DIGNEA_MISMATCH_H
