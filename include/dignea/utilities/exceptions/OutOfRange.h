//
// Created by amarrero on 21/12/20.
//

#ifndef DIGNEA_OUTOFRANGE_H
#define DIGNEA_OUTOFRANGE_H

#include <stdexcept>

class OutOfRange : public std::runtime_error {
   public:
    OutOfRange(std::string &where)
        : std::runtime_error("Runtime out-of-range at: " + where){};

    virtual ~OutOfRange() = default;
};

#endif  // DIGNEA_OUTOFRANGE_H
