#ifndef EX3_MTMEXCEPTIONS_H
#define EX3_MTMEXCEPTIONS_H

#include <exception>
#include <string>
#include <iostream>
#include "Auxilaries.h"
#include <string.h>

namespace MtmMath {
    namespace MtmExceptions {
        class MtmExceptions : public std::exception {
        public:
            MtmExceptions() = default;

            virtual ~MtmExceptions() throw() = default;

            virtual const char *what() const throw() = 0;
        };

        /*
         * Exception for illegal initialization of an object, needs to output
         * "MtmError: Illegal initialization values" in what() class function
         */
        class IllegalInitialization : public MtmExceptions {
        public:
            const char *what() const throw() override {
                return "MtmError: Illegal initialization values";
            }
        };

        /*
         * Exception for Memory allocation failure for an object, needs to output
         * "MtmError: Out of memory" in what() class function
         */
        class OutOfMemory : public MtmExceptions {
        public:
            const char *what() const throw() override {
                return "MtmError: Out of memory";
            }
        };

        /*
         * Exception for dimension mismatch during a mathematical function, needs to output
         * "MtmError: Dimension mismatch: (<mat 1 row>,<mat 1 col>) (<mat 2 row>,<mat 2 col>)"
         * in what() class function
         */
        class DimensionMismatch : public MtmExceptions {

            std::string msg;

        public:

            DimensionMismatch(const Dimensions &mat1, const Dimensions &mat2) {
                msg = "MtmError: Dimension mismatch: " + mat1.to_string() +
                      " " + mat2.to_string();
            }

            const char *what() const throw() override {
                return msg.c_str();
            }
        };

        /*
         * Exception for error for changing matrix/vector shape in reshape and resize, needs to output
         * "MtmError: Change matrix shape failed from: (<mat row>,<mat col>) (<new mat row>,<new mat col>)"
         * in what() class function
         */
        class ChangeMatFail : public MtmExceptions {
            std::string msg;
        public:
            ChangeMatFail(const Dimensions &mat1, const Dimensions &mat2) {
                msg = "MtmError: Change matrix shape failed from: "
                      + mat1.to_string() + " " + mat2.to_string();
            }

            const char *what() const throw() override {
                return msg.c_str();
            }
        };

        /*
         * Exception for accessing an illegal element in matrix or vector, needs to output
         * "MtmError: Attempt access to illegal element" in what() class function
         */
        class AccessIllegalElement : public MtmExceptions {
        public:
            const char *what() const throw() override {
                return "MtmError: Attempt access to illegal element";
            }
        };
    }
}


#endif //EX3_MTMEXCEPTIONS_H
