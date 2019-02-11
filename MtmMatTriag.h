#ifndef EX3_MTMMATTRIAG_H
#define EX3_MTMMATTRIAG_H


#include <vector>
#include "MtmExceptions.h"
#include "Auxilaries.h"
#include "MtmMatSq.h"

using std::size_t;

namespace MtmMath {


    template<typename T>
    class MtmMatTriag : public MtmMatSq<T> {
        bool isUpper;
    public:

        /*
         * Triangular Matrix constructor, m is the number of rows and columns in the matrix,
         * val is the initial value for the matrix elements and isUpper_ is whether it is upper
         * Rectangular matrix (true means it is)
         */
        MtmMatTriag<T>(size_t m, const T &val = T(), bool isUpper_t = true)
                : MtmMatSq<T>(m, val), isUpper(isUpper_t) {
            for (int i = 0; i < (int) m; i++) {
                for (int j = 0; j < (int) m; j++) {
                    if ((isUpper && j < i) || (!isUpper && j > i)) {
                        (*this)[i][j] = 0;
                        ((*this)[i]).permissions[j] = false;
                    }
                }
            }
        }

        MtmMatTriag(const MtmMatTriag<T> &toCopy) = default;

        MtmMatTriag(const MtmMatSq <T> &toCopy) : MtmMatSq<T>(toCopy) {
            bool isTriag = true;
            isUpper = true;
            for (int i = 0; i < toCopy.getDimensions().getRow(); i++) {
                for (int j = 0; j < i; j++) {
                    if (toCopy[i][j] != 0) {
                        isUpper = false;
                    }
                }
            }
            if (!isUpper) {
                for (int i = 0; i < toCopy.getDimensions().getRow(); i++) {
                    for (int j = i + 1;
                         j < toCopy.getDimensions().getCol(); j++) {
                        if (toCopy[i][j] != 0) {
                            isTriag = false;
                        }
                    }
                }
            }
            if (!isTriag) {
                throw MtmExceptions::IllegalInitialization();
            }
            for (int i = 0; i < toCopy.getDimensions().getRow(); i++) {
                for (int j = 0; j < toCopy.getDimensions().getRow(); j++) {
                    if ((isUpper && j < i) || (!isUpper && j > i)) {
                        ((*this)[i]).permissions[j] = false;
                    }
                }
            }
        }

        MtmMatTriag(const MtmMat <T> &toCopy) : MtmMatSq<T>(toCopy) {
            MtmMatTriag newMatrix = MtmMatTriag(MtmMatSq<T>(toCopy));
            *this = newMatrix;
        }


        MtmMatTriag() = default;

        MtmMatTriag &operator=(const MtmMatTriag<T> &c) {
            isUpper = c.isUpper;
            MtmMat<T>::operator=(c);
            return *this;
        }

        void resize(Dimensions dim, const T &val);

        void transpose() {
            MtmMatTriag<T> result = MtmMatTriag(*this);
            result.isUpper = !(isUpper);
            result.allowAllVec();

            for (int i = 0; i < result.getDimensions().getRow(); i++) {
                for (int j = 0; j < result.getDimensions().getCol(); j++) {
                    try {
                        result[i][j] = (*this)[j][i];
                    }
                    catch (MtmExceptions::AccessIllegalElement &e) {
                        result[i][j] = 0;
                    }
                    if ((result.isUpper && j < i) ||
                        (!result.isUpper && j > i)) {
                        ((result)[i]).permissions[j] = false;
                    }
                }
            }

            *this = result;
        }
    };

    template<typename T>
    void MtmMatTriag<T>::resize(Dimensions dim, const T &val) {

        MtmMatSq<T>::resize(dim, val);

        for (int i = 0; i < (*this).getDimensions().getRow(); i++) {
            for (int j = 0; j < (*this).getDimensions().getCol(); j++) {
                if ((isUpper && j < i) || (!isUpper && j > i)) {
                    (*this)[i][j] = 0;
                    ((*this)[i]).permissions[j] = false;
                }
            }
        }

    }

}

#endif //EX3_MTMMATTRIAG_H
