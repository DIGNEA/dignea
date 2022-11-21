//
// Created by amarrero on 21/12/20.
//

#ifndef DIGNEA_NONMOALLOWED_H
#define DIGNEA_NONMOALLOWED_H

#include <stdexcept>

class NoMOAllowed : public std::logic_error {
   public:
    NoMOAllowed(std::string &where)
        : std::logic_error("No Multi-objective Optimization allowed with: " +
                           where){};

    virtual ~NoMOAllowed() = default;
};

#endif  // DIGNEA_NONMOALLOWED_H
