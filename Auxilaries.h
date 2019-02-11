#ifndef EX3_AUXILARIES_H
#define EX3_AUXILARIES_H

#include <string>
#include <iostream>

using std::size_t;

//CHANGED THIS FILE DO NOT REPLACE WITH THE COURSE TEAM FILE!


namespace MtmMath {
    class Dimensions {
        size_t row, col;
    public:

        Dimensions(size_t row_t = 0, size_t col_t = 0) : row(row_t),
                                                         col(col_t) {}

        Dimensions(const Dimensions &toCopy) = default;


        std::string to_string() const {
            return "(" + std::to_string(row) + "," + std::to_string(col) + ")";
        }

        ~Dimensions() = default;


        Dimensions &operator=(const Dimensions &c) {
            if (this == &c) {
                return *this;
            }

            row = c.row;
            col = c.col;
            return *this;
        }

        bool operator==(Dimensions dim2) const {
            return (row == dim2.row) and (col == dim2.col);
        }

        bool operator!=(Dimensions dim2) const {
            return not(*this == dim2);
        }

        void transpose() {
            size_t prev_row = row;
            row = col;
            col = prev_row;
        }

        int getRow() const {
            return (int) row;
        }

        int getCol() const {
            return (int) col;
        }

        void setRow(int newRow) {
            row = (size_t) newRow;
        }

        void setCol(int newColumn) {
            col = (size_t) newColumn;
        }
    };

}

#endif //EX3_AUXILARIES_H
