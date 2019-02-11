#ifndef EX3_MTMMAT_H
#define EX3_MTMMAT_H


#include "MtmExceptions.h"
#include "Auxilaries.h"
#include "MtmVec.h"
#include "cmath"

using std::size_t;

namespace MtmMath {

    //MAT ITERATOR CLASS

    template<typename T>
    class MatIterator {
        MtmVec <T> *ptr;
        Dimensions matDimensions;
        Dimensions iteratorLocation;

    public:

        MatIterator(MtmVec <T> *ptr = NULL,
                    Dimensions matDimensions_t = Dimensions(),
                    Dimensions iteratorLocation_t = Dimensions()) : ptr(ptr) {
            matDimensions = matDimensions_t;
            iteratorLocation = iteratorLocation_t;
        }

        MatIterator(const MatIterator &toCopy) = default;

        ~MatIterator() = default;

        bool operator==(const MatIterator &toCompare) const;

        bool operator!=(const MatIterator &toCompare) const;

        T &operator*();

        MatIterator &operator=(const MatIterator &c);

        MatIterator operator++();
    };

    template<typename T>
    MatIterator<T> MatIterator<T>::operator++() {
        //get mat dimensions, and iterator location
        int matColumns = this->matDimensions.getCol();
        int matRows = this->matDimensions.getRow();
        int rowLocation = this->iteratorLocation.getRow();
        int colLocation = this->iteratorLocation.getCol();

        ++(ptr);
        rowLocation++;
        this->iteratorLocation.setRow(rowLocation);

        if (rowLocation == matRows) {

            colLocation++;
            this->iteratorLocation.setCol(colLocation);

            if (colLocation == matColumns) {
                --(ptr);
                rowLocation--;
                this->iteratorLocation.setRow(rowLocation);
            } else {
                ptr = ptr - matRows;
                this->iteratorLocation.setRow(0);
            }
        }
        return *this;
    }

    template<typename T>
    T &MatIterator<T>::operator*() {
        return (((*ptr))[iteratorLocation.getCol()]);
    }

    template<typename T>
    bool MatIterator<T>::operator==(const MatIterator &toCompare) const {
        return (ptr == toCompare.ptr &&
                iteratorLocation == toCompare.iteratorLocation);
    }

    template<typename T>
    bool MatIterator<T>::operator!=(const MatIterator &toCompare) const {
        return !(operator==(toCompare));
    }

    template<typename T>
    MatIterator<T> &MatIterator<T>::operator=(const MatIterator<T> &c) {
        if (this == &c) {
            return *this;
        }
        ptr = c.ptr;
        iteratorLocation = c.iteratorLocation;
        matDimensions = c.matDimensions;
        return *this;
    }


//NON ZERO ITERATOR CLASS

    template<typename T>
    class MatNonZeroIterator {
        MtmVec <T> *ptr;
        Dimensions matDimensions;
        Dimensions iteratorLocation;

    public:

        MatNonZeroIterator(MtmVec <T> *ptr = NULL,
                           Dimensions matDimensions_t = Dimensions(),
                           Dimensions iteratorLocation_t = Dimensions()) : ptr(
                ptr) {
            matDimensions = matDimensions_t;
            iteratorLocation = iteratorLocation_t;
        }

        MatNonZeroIterator(const MatNonZeroIterator &toCopy) = default;

        ~MatNonZeroIterator() = default;

        bool operator==(const MatNonZeroIterator &toCompare) const;

        bool operator!=(const MatNonZeroIterator &toCompare) const;

        T &operator*();

        MatNonZeroIterator &operator=(const MatNonZeroIterator &c);

        MatNonZeroIterator operator++();

    };

    template<typename T>
    MatNonZeroIterator<T> MatNonZeroIterator<T>::operator++() {
        //get mat dimensions, and iterator location
        int matColumns = this->matDimensions.getCol();
        int matRows = this->matDimensions.getRow();
        int rowLocation = this->iteratorLocation.getRow();
        int colLocation = this->iteratorLocation.getCol();

        ++(ptr);
        rowLocation++;
        this->iteratorLocation.setRow(rowLocation);

        if (rowLocation == matRows) {

            colLocation++;
            this->iteratorLocation.setCol(colLocation);

            if (colLocation == matColumns) {
                --(ptr);
                rowLocation--;
                this->iteratorLocation.setRow(rowLocation);
            } else {
                ptr = ptr - matRows;
                this->iteratorLocation.setRow(0);
            }
        }

        Dimensions endMat = Dimensions(
                (size_t) this->matDimensions.getRow() - 1,
                (size_t) this->matDimensions.getCol());
        if (this->iteratorLocation != endMat &&
            (*ptr)[iteratorLocation.getCol()] == 0) {
            MatNonZeroIterator<T>::operator++();
        }
        return *this;

    }

    template<typename T>
    T &MatNonZeroIterator<T>::operator*() {
        return (((*ptr))[iteratorLocation.getCol()]);
    }

    template<typename T>
    bool MatNonZeroIterator<T>::operator==(
            const MatNonZeroIterator &toCompare) const {
        return (ptr == toCompare.ptr &&
                iteratorLocation == toCompare.iteratorLocation);
    }

    template<typename T>
    bool MatNonZeroIterator<T>::operator!=(
            const MatNonZeroIterator &toCompare) const {
        return !(operator==(toCompare));
    }

    template<typename T>
    MatNonZeroIterator<T> &
    MatNonZeroIterator<T>::operator=(const MatNonZeroIterator<T> &c) {
        ptr = c.ptr;
        iteratorLocation = c.iteratorLocation;
        matDimensions = c.matDimensions;
    }

/*
 * MtmMat class!
 * The Main business is here ->
 */

    template<typename T>
    class MtmMat : public MtmVec<MtmVec < T>>

{


    public:

    typedef MatIterator<T> iterator;
    typedef MatNonZeroIterator<T> nonzero_iterator;

    /* Vector Constructor -
     * Create new Matrix constructed from one vector "newMatrix" sized as the num
     * of rows required that holds vectors sized as the num of cols required
     * Vectors are built as col vectors, which is why we are using .transpose
    */

    MtmMat(Dimensions const &dim_t = Dimensions(), const T &val = T()) {
        if (dim_t.getCol() < 0 || dim_t.getRow() < 0) {
            throw MtmExceptions::OutOfMemory();
        }

        if (dim_t.getCol() == 0 || dim_t.getRow() == 0) {
            throw MtmExceptions::IllegalInitialization();
        }

        MtmVec<T> defVec = MtmVec<T>((size_t) dim_t.getCol(), val);
        defVec.transpose();
        MtmVec<MtmVec<T>> newShape = MtmVec<MtmVec<T>>((size_t) dim_t.getRow(),
                                                       defVec);
        (*this) = newShape;
        this->objectDimensions = dim_t;
    }

// Copy Constructor

    MtmMat(const MtmMat<T> &toCopy) = default;

    MtmMat(const MtmVec <T> &toConvert) {
        MtmMat < T > vecToMat = MtmMat(toConvert.getDimensions(), T());
        if (toConvert.getDimensions().getCol == 1) {
            for (int i = 0; i < vecToMat.getDimensions.getRow(); i++) {
                vecToMat[i][0] = toConvert[i];
            }
        } else {
            for (int i = 0; i < vecToMat.getDimensions.getCol(); i++) {
                vecToMat[0][i] = toConvert[i];
            }
        }
        MtmVec<MtmVec<T>>::operator=(vecToMat);
    }

    MtmMat(const MtmVec <MtmVec<T>> &toConvert) {
        MtmMat < T > vecToMat = MtmMat(toConvert.getDimensions(), T());
        for (int i = 0; i < toConvert.getDimensions().getRow(); i++) {
            for (int j = 0; j < toConvert.getDimensions().getCol(); j++) {
                vecToMat[i][j] = toConvert[i][j];
            }
        }
        (*this) = vecToMat;
    }

    static int min(const int &a, const int &b) {
        if (a < b) {
            return a;
        }
        return b;
    }

    void allowAllVec()

    override {
    MtmVec<MtmVec < T>>
    ::allowAllVec();
    for (
    int i = 0;
i < this->objectDimensions.

    getRow();

    i++) {
    (*this)[i].

    allowAllVec();
}
}

MtmMat &operator+=(const MtmMat <T> &c) {
    for (int i = 0; i < this->objectDimensions.getRow(); i++) {
        (*this)[i] += c[i];
    }
    return *this;
}

MtmMat &operator=(const MtmVec <T> &c);

MtmMat &operator=(const MtmMat <T> &c) {
    MtmVec < MtmVec < T >> ::operator=(c);
    return *this;
}

MtmMat &operator=(const MtmVec <MtmVec<T>> &c) {
    MtmVec < MtmVec < T >> ::operator=(c);
    return *this;
}


MtmMat &operator-=(const MtmMat &c) {
    operator+=(-(c));
}

MtmMat &operator*=(const MtmMat &c) {
    MtmMat <T> result = (*this) * c;
    (*this) = result;
}

MtmMat &operator+=(const T &c) {
    for (int i = 0; i < this->objectDimensions.getRow(); i++) {
        (*this)[i] += c;
    }
    return *this;
}

MtmMat &operator-=(const T &c) {
    return operator+=(-(c));
}

MtmMat &operator*=(const T &c) {
    MtmMat <T> result = MtmMat<T>(*this);
    for (int i = 0; i < result.getDimensions.getRow(); i++) {
        result[i] = (*this)[i] * c;
    }
    (*this) = result;
}


MtmVec <T> getColVector(int col) const {
    if (col >= this->objectDimensions.getCol()) {
        throw MtmExceptions::IllegalInitialization();
    }

    MtmVec <T>
            colVector = MtmVec<T>(this->objectDimensions.getRow(),
                                  T());
    for (int i = 0; i < this->objectDimensions.getRow(); i++) {
        colVector[i] = (*this)[i][col];
    }

    return MtmVec<T>(colVector);
}


/*
 * Function that get function object f and uses it's () operator on
 * each element in the matrix columns. It outputs a vector in the size
 * of the matrix columns where each element is the final output
 * by the function object's * operator
 */
template<typename Func>
MtmVec <T> matFunc(Func &f) const;

/*
 * resizes a matrix to dimension dim, new elements gets the value val.
 */
virtual void resize(Dimensions dim, const T &val);

/*
 * reshapes matrix so linear elements value are the same without
 * changing num of elements.
 */
virtual void reshape(Dimensions newDim);

/*
 * Performs transpose operation on matrix
 */
virtual void transpose();

/*
 * Iterator Functions - NZ and Normal
 */
nonzero_iterator nzend() {
    return nonzero_iterator(
            &(*this)[this->getDimensions().getRow() - 1],
            this->getDimensions(),
            Dimensions(this->getDimensions().getRow() - 1,
                       this->getDimensions().getCol()));
}

nonzero_iterator nzbegin() {
    for (int j = 0; j < (*this).getDimensions().getCol(); j++) {
        for (int i = 0; i < (*this).getDimensions().getRow(); i++) {
            if ((*this)[i][j] != 0) {
                return nonzero_iterator(&((*this)[i]), this->getDimensions(),
                                        Dimensions(i, j));
            }
        }
    }

    return nzend();
}

iterator end() {
    return iterator(&(*this)[this->getDimensions().getRow() - 1],
                    this->getDimensions(),
                    Dimensions(this->getDimensions().getRow() - 1,
                               this->getDimensions().getCol()));
}

iterator begin() {
    return iterator(&((*this)[0]), this->getDimensions(),
                    Dimensions(0, 0));
}

};


template<typename T>
MtmMat <T> operator*(const T &num, const MtmMat <T> &a);

template<typename T>
MtmMat <T> operator*(const T &num, const MtmMat <T> &a) {
    MtmMat <T> result = MtmMat<T>(a);
    for (int i = 0; i < a.getDimensions().getRow(); i++) {
        result[i] = a[i] * num;
    }

    return MtmMat<T>(result);
}

template<typename T>
MtmMat <T> operator-(const MtmMat <T> &a, const T &num) {
    return (-num) + a;
}


template<typename T>
MtmMat <T> operator*(const MtmMat <T> &a, const T &num) {
    return num * a;
}

template<typename T>
MtmMat <T> operator-(const T &num, const MtmMat <T> &a) {
    MtmMat <T> result = -a;
    return result += num;
}

template<typename T>
MtmMat <T> operator+(const T &num, const MtmMat <T> &a) {
    MtmMat <T> result = MtmMat<T>(a);
    for (int i = 0; i < a.getDimensions().getRow(); i++) {
        result[i] = a[i] + num;
    }

    return MtmMat<T>(result);
}


template<typename T>
MtmMat <T> operator+(const MtmMat <T> &a, const T &num) {
    return num + a;
}

template<typename T>
MtmMat <T> operator*(const MtmMat <T> &a, const MtmMat <T> &b) {
    if (a.getDimensions().getCol() != b.getDimensions().getRow()) {
        throw MtmExceptions::DimensionMismatch(a.getDimensions(),
                                               b.getDimensions());
    }

    Dimensions resultDimensions =
            Dimensions(a.getDimensions().getRow(),
                       b.getDimensions().getCol());
    MtmMat <T> result = MtmMat<T>(resultDimensions, T());
    for (int i = 0; i < result.getDimensions().getRow(); i++) {
        for (int j = 0; j < result.getDimensions().getCol(); j++) {
            MtmVec <T> oneOnOne = a[i] * (b.getColVector(j));
            ((result)[i])[j] = oneOnOne[0];
        }
    }

    return (MtmMat<T>(result));

}


template<typename T>
template<typename Func>
MtmVec <T> MtmMat<T>::matFunc(Func &f) const {
    MtmVec <T>
            result = MtmVec<T>((size_t) this->objectDimensions.getCol());

    MtmMat <T> temp = *this;
    temp.transpose();
    for (int i = 0; i < temp.objectDimensions.getRow(); i++) {
        MtmVec <T> tempVec = temp[i];
        result[i] = tempVec.vecFunc(f);
    }

    result.transpose();

    return result;
}

template<typename T>
void MtmMat<T>::resize(Dimensions dim, const T &val) try {
    if (!dim.getCol() || !dim.getRow()) {
        throw MtmExceptions::ChangeMatFail(this->getDimensions(), dim);
    }

    (*this).allowAllVec();

    MtmMat <T> newSize = MtmMat<T>(dim, val);
    for (int i = 0;
         i < min(dim.getRow(), this->objectDimensions.getRow()); i++) {
        for (int j = 0;
             j < min(dim.getCol(), this->objectDimensions.getCol()); j++) {
            newSize[i][j] = (*this)[i][j];
        }
    }
    (*this) = newSize;
}
catch (std::bad_alloc &e) {
    throw MtmMath::MtmExceptions::OutOfMemory();
}

template<typename T>
void MtmMat<T>::reshape(Dimensions newDim) {
    if (newDim.getRow() * newDim.getCol() !=
        this->objectDimensions.getCol() * this->objectDimensions.getRow()) {
        throw MtmExceptions::ChangeMatFail(this->getDimensions(), newDim);
    }

    MtmMat <T> newShape = MtmMat<T>(newDim);

    for (int i = 0; i < newDim.getRow() * newDim.getCol(); i++) {
        int oldRow = i % this->objectDimensions.getRow();
        int newRow = i % newDim.getRow();
        int oldCol = i / this->objectDimensions.getRow();
        int newCol = i / newDim.getRow();
        try {
            newShape[newRow][newCol] = (*this)[oldRow][oldCol];
        }
        catch (MtmMath::MtmExceptions::AccessIllegalElement &e) {
            throw MtmMath::MtmExceptions::ChangeMatFail(this->getDimensions(),
                                                        newDim);
        }
    }

    (*this) = newShape;
}

template<typename T>
void MtmMat<T>::transpose() {
    allowAllVec();
    Dimensions newDim = Dimensions(MtmMat<T>::getDimensions());
    newDim.transpose();
    MtmMat <T> newShape = MtmMat<T>(newDim, T());

    for (int i = 0; i < MtmMat<T>::getDimensions().getRow(); i++) {
        for (int j = 0; j < MtmMat<T>::getDimensions().getCol(); j++) {
            newShape[j][i] = (*this)[i][j];
        }
    }

    (*this) = newShape;

}


}


#endif //EX3_MTMMAT_H
