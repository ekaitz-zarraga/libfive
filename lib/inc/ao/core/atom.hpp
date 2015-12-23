#pragma once

#include <vector>

// This is the storage size allocated to each array (in bytes)
//
// For efficiency, it should be sized to fit an non-fractional number of
// doubles, Intervals, and Gradient objects.  An error will be thrown
// at compile-time if this condition is not met.
#define ATOM_ARRAY_BYTES ((size_t)1024)

#define ATOM_DOUBLE_COUNT   (ATOM_ARRAY_BYTES / sizeof(double))
#define ATOM_INTERVAL_COUNT (ATOM_ARRAY_BYTES / sizeof(Interval))
#define ATOM_GRADIENT_COUNT (ATOM_ARRAY_BYTES / sizeof(Gradient))

#include "ao/core/opcode.hpp"
#include "ao/core/result.hpp"

class Token;

#define ATOM_FLAG_MASK 0xff
#define ATOM_FLAG_IGNORED 1
#define ATOM_FLAG_BOOLEAN 2

/*
 *  An Atom represent an operation in a math tree.
 */
class Atom
{
    /*
     *  Construct an atom from the given token, setting the token's
     *  atom pointer to this.
     *
     *  Requires that the token's children be packed into Atoms
     *  beforehand (otherwise will throw an assertion)
     */
    explicit Atom(Token* t);

    /*
     *  Construct an operation atom from scratch
     */
    explicit Atom(Opcode op, Atom* a=NULL, Atom* b=NULL);

    /*
     *  Construct an OP_MUTABLE atom from scratch
     */
    explicit Atom(double d);

protected:
    const Opcode op;

    /*  Populated for OP_CONST atoms */
    const double value;

    /*  Populated for OP_MUTABLE atoms  */
    double mutable_value;

    /*  Flags are set during evaluation for various purposes  */
    uint8_t flags=0;

    /*  Populated for operators with arguments */
    Atom* const a;
    Atom* const b;

    /*  Results are stored in a union */
    Result result;

    friend class Tree;
};
