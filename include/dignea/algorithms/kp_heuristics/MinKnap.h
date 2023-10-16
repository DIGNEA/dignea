/**
 * @file MinKnap.h
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 2023-04-21
 *
 * @copyright Copyright (c) 2023
 *
 */

/** ======================================================================
           MINKNAP.C, David Pisinger   march 1993, revised feb. 1998
   ======================================================================

 * This is the C-code corresponding to the paper:
 *
 *   D. Pisinger
 *   A minimal algorithm for the 0-1 knapsack problem
 *   Operations Research, 45, 758-767 (1997).
 *
 * Further details on the project can also be found in
 *
 *   D. Pisinger
 *   Algorithms for Knapsack Problems
 *   Report 95/1, DIKU, University of Copenhagen
 *   Universitetsparken 1
 *   DK-2100 Copenhagen
 *
 * The algorithm may be used for academic, non-commercial purposes
 * only.
 * -------------------------------------------------------------------
 * The present code is a callable routine which solves a 0-1 Knapsack
 * Problem:
 *
 *           maximize   \sum_{j=1}^{n} p_{j} x_{j}
 *           subject to \sum_{j=1}^{n} w_{j} x_{j} \leq c
 *                      x_{j} \in \{0,1\}, j = 1,\ldots,n
 *
 * The minknap algorithm is called as
 *
 *          z = minknap(n, p, w, x, c)
 *
 * where p[], w[], x[] are arrays of integers. The optimal objective
 * value is returned in z, and x[] gives the solution vector.
 * If you need a different interface for your algorithm, minknap
 * may easily be adapted to your own datastructures since all tables
 * are copied to the internal representation.
 *
 * Since the minknap algorithm is based on dynamic programming, you
 * must give an upper limit MAXSTATES on the number of states. The
 * constant is defined below. Different types should be defined as
 * follows:
 *
 *    itype     should be sufficiently large to hold a profit or weight
 *    stype     should be sufficient to hold sum of profits/weights
 *    ptype     should hold the product of an stype and itype
 *
 * The code has been tested on a hp9000/735, and conforms with the
 * ANSI-C standard.
 *
 * Errors and questions are refered to:
 *
 *   David Pisinger, associate professor
 *   DIKU, University of Copenhagen,
 *   Universitetsparken 1,
 *   DK-2100 Copenhagen.
 *   e-mail: pisinger@diku.dk
 *   fax: +45 35 32 14 01
 */

/* ======================================================================
                                  definitions
   ====================================================================== */

#ifndef DIGNEA_KP_MinKnap_HEURISTIC_H
#define DIGNEA_KP_MinKnap_HEURISTIC_H

#include <dignea/core/Heuristic.h>
#include <dignea/problems/KPNR.h>

#include <nlohmann/json.hpp>
#include <vector>

using json = nlohmann::json;
using namespace std;
/**
 * @brief MinKnap Heuristic for the Knapsack Problem.
 * Tries to insert each item in the knapsack while there is room for more items.
 *
 */
class MinKnap : public Heuristic<BoolFloatSolution> {
   public:
    MinKnap();

    virtual ~MinKnap() = default;

    void run() override;

    string getName() const { return "MinKnap KP"; };

    string getID() const { return "MinKnap"; }

    void setProblem(shared_ptr<KP> prob);

    void setProblem(KP *prob);

    Front<BoolFloatSolution> getResults() const override;
};

#endif  // DIGNEA_KP_MinKnap_HEURISTIC_H