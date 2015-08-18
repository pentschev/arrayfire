/*******************************************************
 * Copyright (c) 2014, ArrayFire
 * All rights reserved.
 *
 * This file is distributed under 3-clause BSD license.
 * The complete license agreement can be obtained at:
 * http://arrayfire.com/licenses/BSD-3-Clause
 ********************************************************/

#include <Array.hpp>
#include <unwrap.hpp>
#include <kernel/unwrap.hpp>
#include <stdexcept>
#include <err_opencl.hpp>

namespace opencl
{
    template<typename T>
    Array<T> unwrap(const Array<T> &in, const dim_t wx, const dim_t wy,
                    const dim_t sx, const dim_t sy, const dim_t px, const dim_t py, const bool is_column)
    {
        af::dim4 idims = in.dims();

        dim_t nx = (idims[0] + 2 * px - wx) / sx + 1;
        dim_t ny = (idims[1] + 2 * py - wy) / sy + 1;

        af::dim4 odims(wx * wy, nx * ny, idims[2], idims[3]);

        if (!is_column) {
            std::swap(odims[0], odims[1]);
        }

        // Create output placeholder
        Array<T> outArray = createEmptyArray<T>(odims);

        if (is_column) {
            kernel::unwrap<T, true >(outArray, in, wx, wy, sx, sy, px, py, nx);
        } else {
            kernel::unwrap<T, false>(outArray, in, wx, wy, sx, sy, px, py, nx);
        }

        return outArray;
    }


#define INSTANTIATE(T)                                                                  \
    template Array<T> unwrap<T> (const Array<T> &in, const dim_t wx, const dim_t wy,    \
                    const dim_t sx, const dim_t sy, const dim_t px, const dim_t py, const bool is_column);


    INSTANTIATE(float)
    INSTANTIATE(double)
    INSTANTIATE(cfloat)
    INSTANTIATE(cdouble)
    INSTANTIATE(int)
    INSTANTIATE(uint)
    INSTANTIATE(intl)
    INSTANTIATE(uintl)
    INSTANTIATE(uchar)
    INSTANTIATE(char)
}
