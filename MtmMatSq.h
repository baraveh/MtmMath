#ifndef EX3_MTMMATREC_H
#define EX3_MTMMATREC_H

#include "MtmExceptions.h"
#include "Auxilaries.h"
#include "MtmMat.h"

using std::size_t;

namespace MtmMath {

    template<typename T>
    class MtmMatSq : public MtmMat<T> {
    public:
        /*
         * Rectangular Matrix constructor, m is the number of rows and columns in the matrix
         * and val is the initial value for the matrix elements
         */
        MtmMatSq(size_t m, const T &val = T()) : MtmMat<T>(Dimensions(m, m),
                                                           val) {};

        MtmMatSq(const MtmMatSq<T> &toCopy) = default;

        MtmMatSq(const MtmMat <T> &toCopy) : MtmMat<T>(toCopy) {
            if (toCopy.getDimensions().getRow() !=
                toCopy.getDimensions().getCol()) {
                throw MtmExceptions::IllegalInitialization();
            }
        }

        void resize(Dimensions dim, const T &val);


        MtmMatSq() = default;


    };

    template<typename T>
    void MtmMatSq<T>::resize(Dimensions dim, const T &val) {
        if (dim.getCol() != dim.getRow()) {
            throw MtmExceptions::ChangeMatFail(this->getDimensions(), dim);
        }


        MtmMath::MtmMat < T > ::resize(dim, val);

    }


}

#endif //EX3_MTMMATREC_H
