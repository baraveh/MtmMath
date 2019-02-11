#ifndef EX3_MTMVEC_H
#define EX3_MTMVEC_H

#include <vector>
#include <iterator>
#include "MtmExceptions.h"
#include "Auxilaries.h"
#include "Complex.h"
#include "cmath"


using std::size_t;

namespace MtmMath {

    //VECTOR ITERATOR CLASS

    template<typename T>
    class VecIterator
            : public std::iterator<std::random_access_iterator_tag, T> {

        T *dataPtr;

    public:
        VecIterator(T *ptr = NULL) : dataPtr(ptr) {}

        VecIterator(const VecIterator &toCopy) : dataPtr(toCopy.dataPtr) {}

        virtual ~VecIterator() = default;

        bool operator==(const VecIterator &toCompare) const;

        bool operator!=(const VecIterator &toCompare) const;

        virtual T &operator*();

        VecIterator &operator=(const VecIterator &c);

        VecIterator operator++();
    };

    template<typename T>
    VecIterator<T> VecIterator<T>::operator++() {
        ++dataPtr;
        return *this;
    }

    template<typename T>
    T &VecIterator<T>::operator*() {
        return (*dataPtr);
    }

    template<typename T>
    bool VecIterator<T>::operator==(const VecIterator &toCompare) const {
        return (dataPtr == toCompare.dataPtr);
    }

    template<typename T>
    bool VecIterator<T>::operator!=(const VecIterator &toCompare) const {
        return (dataPtr != toCompare.dataPtr);
    }

    template<typename T>
    VecIterator<T> &VecIterator<T>::operator=(const VecIterator<T> &c) {
        if (this == &c) {
            return *this;
        }
        dataPtr = c.dataPtr;
        return *this;
    }

    //NON ZERO ITERATOR CLASS

    template<typename T>
    class VecNonZeroIterator {
        T *NonZeroPtr;
        int location;
        int size;


    public:

        VecNonZeroIterator(T *ptr = NULL, int size_t = 0, int location = 0) :
                NonZeroPtr(ptr), location(location), size(size_t) {}

        VecNonZeroIterator(const VecNonZeroIterator &toCopy) = default;

        ~VecNonZeroIterator() = default;

        VecNonZeroIterator operator++();

        bool operator==(const VecNonZeroIterator &toCompare) const;

        bool operator!=(const VecNonZeroIterator &toCompare) const;

        T &operator*();

        VecNonZeroIterator &operator=(const VecNonZeroIterator &c);

    };

    template<typename T>
    VecNonZeroIterator<T> VecNonZeroIterator<T>::operator++() {
        ++location;
        ++(this->NonZeroPtr);
        while ((location != size) && (*(this->NonZeroPtr)) == 0) {
            ++location;
            ++(this->NonZeroPtr);
        }
        return *this;
    }

    template<typename T>
    bool VecNonZeroIterator<T>::operator==(
            const VecNonZeroIterator &toCompare) const {
        return (NonZeroPtr == toCompare.NonZeroPtr);
    }

    template<typename T>
    bool VecNonZeroIterator<T>::operator!=(
            const VecNonZeroIterator &toCompare) const {
        return (NonZeroPtr != toCompare.NonZeroPtr);
    }

    template<typename T>
    T &VecNonZeroIterator<T>::operator*() {
        return (*NonZeroPtr);
    }

    template<typename T>
    VecNonZeroIterator<T> &
    VecNonZeroIterator<T>::operator=(const VecNonZeroIterator &c) {
        NonZeroPtr = c.NonZeroPtr;
        return *this;
    }

    //VECTOR CLASS

    template<typename T>
    class MtmVec : public std::vector<T> {

    protected:
        Dimensions objectDimensions;


    public:
        std::vector<bool> permissions;

        typedef VecIterator<T> iterator;
        typedef VecNonZeroIterator<T> nonzero_iterator;

        virtual void allowAllVec();

        /*
         * Vector constructor, m is the number of elements in it and val is the
         * initial value for the matrix elements
         */
        //Constructors declarations

        MtmVec(size_t m, const T &val = T()) try : std::vector<T>(m, val),
                                                   objectDimensions(m, 1),
                                                   permissions(m, true) {
            if (m <= 0) {
                throw MtmExceptions::IllegalInitialization();
            }
            if(m >= 100000000000){
                //I know this isn't the way it's supposed to be, but memory
                //errors aren't happening on my pc unless it's a super high num
                throw MtmExceptions::OutOfMemory();
            }
        }
        catch (std::bad_alloc &e) {
            throw MtmMath::MtmExceptions::OutOfMemory();
        }

        MtmVec() = default;

        MtmVec(const MtmVec &toCopy) = default;

        //destructor
        virtual ~MtmVec() = default;

        //operators declarations
        MtmVec &operator=(const MtmVec &c);

        MtmVec operator+=(const MtmVec &c);

        MtmVec operator-=(const MtmVec &c);

        MtmVec operator-() const;

        MtmVec operator+=(const T &c);

        MtmVec operator-=(const T &c);

        MtmVec operator*=(const T &c);

        MtmVec operator*=(const MtmVec &c);



        // T& operator[](int index);

        bool operator==(const MtmVec &c) const;

        bool operator!=(const MtmVec &c) const;

        T &operator[](int index) {

            if (index >= (int) (this->size()) || index < 0) {
                throw MtmExceptions::AccessIllegalElement();
            }

            if (!(this->permissions[index])) {
                throw MtmExceptions::AccessIllegalElement();
            }

            return std::vector<T>::operator[](index);
        }

        const T &operator[](int index) const {
            if (index >= (int) this->size()) {
                throw MtmExceptions::AccessIllegalElement();
            }

            return std::vector<T>::operator[](index);
        }


        nonzero_iterator nzend() {
            nonzero_iterator result = nonzero_iterator(
                    &(*this)[this->size() - 1], (int) this->size(),
                    (int) this->size() - 1);
            return ++result;
        }

        nonzero_iterator nzbegin() {
            for (int i = 0; i < (int) this->size(); i++) {
                if ((*this)[i] != 0) {
                    return nonzero_iterator(&(*this)[i], (int) this->size(), 0);
                }
            }

            return nonzero_iterator();
        }

        iterator end() {
            iterator result = iterator(&(*this)[this->size() - 1]);
            return ++result;
        }

        iterator begin() {
            return iterator(&((*this)[0]));
        }

        /*
         * Function that get function object f and uses it's () operator on each element in the vectors.
         * It outputs the function object's * operator after iterating on all the vector's elements
         */
        template<typename Func>
        T vecFunc(Func &f) const;

        /*
         * Resizes a vector to dimension dim, new elements gets the value val.
         * Notice vector cannot transpose through this method.
         */
        virtual void resize(Dimensions dim, const T &val);

        /*
         * Performs transpose operation on matrix
         */
        virtual void transpose();

        Dimensions getDimensions() const;


    };

    template<typename T>
    void MtmVec<T>::resize(Dimensions dim, const T &val) try {

        if (objectDimensions.getRow() == 1 && dim.getRow() == 1) {
            std::vector<T>::resize((size_t) dim.getCol(), val);
            permissions.resize((size_t) dim.getCol(), true);
            objectDimensions = dim;
            return;
        }

        if (objectDimensions.getCol() == 1 && dim.getCol() == 1) {
            std::vector<T>::resize((size_t) dim.getRow(), val);
            permissions.resize(dim.getRow(), true);
            objectDimensions = dim;
            return;
        }
        throw MtmExceptions::ChangeMatFail(this->getDimensions(), dim);
    }
    catch (std::bad_alloc &e) {
        throw MtmMath::MtmExceptions::OutOfMemory();
    }

    //More operators
    template<typename T>
    MtmVec<T> operator+(const MtmVec<T> &a, const MtmVec<T> &b);

    template<typename T>
    MtmVec<T> operator-(const MtmVec<T> &a, const MtmVec<T> &b);

    template<typename T>
    MtmVec<T> operator*(const T &num, const MtmVec<T> &a);

    template<typename T>
    MtmVec<T> operator*(const MtmVec<T> &a, const T &num);

    template<typename T>
    MtmVec<T> operator+(const T &num, const MtmVec<T> &a);

    template<typename T>
    MtmVec<T> operator+(const MtmVec<T> &a, const T &num);

    template<typename T>
    MtmVec<T> operator-(const T &num, const MtmVec<T> &a);

    template<typename T>
    MtmVec<T> operator-(const MtmVec<T> &a, const T &num);


    template<typename T>
    MtmVec<T> operator*(const MtmVec<T> &a, const MtmVec<T> &b) {
        MtmVec<T> result = a;
        result.allowAllVec();
        return result *= b;
    }

    template<typename T>
    MtmVec<T> operator+(const MtmVec<T> &a, const MtmVec<T> &b) {
        MtmVec<T> result = a;
        result.allowAllVec();
        return result += b;
    }


    template<typename T>
    MtmVec<T> operator-(const MtmVec<T> &a, const MtmVec<T> &b) {
        MtmVec<T> c = a;
        c.allowAllVec();
        return c + (-b);
    }

    template<typename T>
    MtmVec<T> operator*(const T &num, const MtmVec<T> &a) {
        MtmVec<T> result = MtmVec<T>(a);
        result.allowAllVec();
        return result *= num;
    }

    template<typename T>
    MtmVec<T> operator*(const MtmVec<T> &a, const T &num) {
        return num * a;
    }

    template<typename T>
    MtmVec<T> operator+(const T &num, const MtmVec<T> &a) {
        MtmVec<T> result = a;
        result.allowAllVec();
        return result += num;
    }

    template<typename T>
    MtmVec<T> operator+(const MtmVec<T> &a, const T &num) {
        return operator+(num, a);
    }

    template<typename T>
    MtmVec<T> operator-(const MtmVec<T> &a, const T &num) {
        return a + (-(num));
    }

    template<typename T>
    MtmVec<T> operator-(const T &num, const MtmVec<T> &a) {
        return num + (-(a));
    }


    template<typename T>
    void MtmVec<T>::transpose() {
        objectDimensions.transpose();
    }

    template<typename T>
    MtmVec<T> &MtmVec<T>::operator=(const MtmVec &c) {
        if (this == &c) {
            return *this;
        }

        objectDimensions = c.objectDimensions;
        permissions = c.permissions;
        std::vector<T>::operator=(c);
        return *this;
    }

    template<typename T>
    MtmVec<T> MtmVec<T>::operator-() const {
        MtmVec<T> result = MtmVec<T>(*this);
        result.allowAllVec();
        for (int i = 0; i < (int) this->size(); i++) {
            result[i] = (-1) * (*this)[i];
        }
        return MtmVec<T>(result);
    }

    template<typename T>
    bool MtmVec<T>::operator==(const MtmVec &c) const {
        if (this->objectDimensions != c.objectDimensions) {
            return false;
        }
        for (int i = 0; i < (int) (this->size()); i++) {
            if ((*this)[i] != c[i]) {
                return false;
            }
        }

        return true;
    }


    template<typename T>
    template<typename Func>
    T MtmVec<T>::vecFunc(Func &f) const {
        for (int i = 0; i < (int) this->size(); i++) {
            f((*this)[i]);
        }

        return *f;

    }

    template<typename T>
    Dimensions MtmVec<T>::getDimensions() const {
        return this->objectDimensions;
    }

    template<typename T>
    MtmVec<T> MtmVec<T>::operator-=(const MtmVec<T> &c) {
        return this->operator+=(-c);
    }

    template<typename T>
    MtmVec<T> MtmVec<T>::operator+=(const MtmVec<T> &c) {
        if (this->objectDimensions != c.getDimensions()) {
            throw MtmExceptions::DimensionMismatch(this->getDimensions(),
                                                   c.getDimensions());
        }

        MtmVec<T> result = MtmVec<T>(*this);
        result.allowAllVec();

        for (int i = 0; i < (int) this->size(); i++) {
            result[i] += c[i];
        }

        *this = result;
        return *this;
    }


    template<typename T>
    bool MtmVec<T>::operator!=(const MtmVec &c) const {
        return !((*this) == c);
    }

    template<typename T>
    void MtmVec<T>::allowAllVec() {
        for (int i = 0; i < (int) ((*this).size()); i++) {
            permissions[i] = true;
        }
    }

    template<typename T>
    MtmVec<T> MtmVec<T>::operator*=(const T &c) {
        MtmVec<T> result = MtmVec<T>(*this);
        result.allowAllVec();
        for (int i = 0; i < (int) this->size(); i++) {
            result[i] *= c;
        }

        (*this) = result;
        return *this;
    }

    template<typename T>
    MtmVec<T> MtmVec<T>::operator+=(const T &c) {
        MtmVec<T> result = MtmVec<T>(*this);
        result.allowAllVec();
        for (int i = 0; i < (int) (this->size()); i++) {
            result[i] += c;
        }
        (*this) = result;
        return *this;
    }

    template<typename T>
    MtmVec<T> MtmVec<T>::operator-=(const T &c) {
        return operator+=(-(c));
    }

    template<typename T>
    MtmVec<T> MtmVec<T>::operator*=(const MtmVec<T> &c) {
        if (this->getDimensions().getCol() != c.getDimensions().getRow()) {
            throw (MtmExceptions::DimensionMismatch(this->getDimensions(),
                                                    c.getDimensions()));
        }

        MtmVec<T> result = MtmVec<T>(1, T());
        result[0] = (*this)[0] * c[0];
        for (int i = 1; i < this->getDimensions().getCol(); i++) {
            result[0] += ((*this)[i] * c[i]);
        }

        *this = result;
        return *this;
    }


}

#endif //EX3_MTMVEC_H
