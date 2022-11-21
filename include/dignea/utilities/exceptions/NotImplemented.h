//
// Created by amarrero on 21/12/20.
//

#ifndef DIGNEA_NOTIMPLEMENTED_H
#define DIGNEA_NOTIMPLEMENTED_H

#include <stdexcept>

class NotImplemented : public std::logic_error {
   public:
    NotImplemented(std::string &where)
        : std::logic_error("Not implemented method: " + where){};

    virtual ~NotImplemented() = default;
};

#endif  // DIGNEA_NOTIMPLEMENTED_H
