//
// Created by amarrero on 5/4/21.
//

#ifndef DIGNEA_GATYPES_H
#define DIGNEA_GATYPES_H

using namespace std;

namespace GATypes {
using GAT = int;
const GAT NO_GA = -1;
const GAT STEADY = 0;
const GAT GENERATIONAL = 1;
const GAT FIRST_IMPRO = 2;
const GAT PARALLEL = 3;
}  // namespace GATypes

#endif  // DIGNEA_GATYPES_H
