/*
 * MIT License
 *
 * Copyright (c) 2021 CSCS, ETH Zurich
 *               2021 University of Basel
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*! @file
 * @brief Random coordinates generation for testing
 *
 * @author Sebastian Keller <sebastian.f.keller@gmail.com>
 */

#pragma once

#include <algorithm>
#include <random>
#include <vector>

#include "../sfc/hilbert.hpp"
#include "stl.hpp"

namespace cstone
{

template<class T, class KeyType>
class RandomCoordinates
{
public:
    RandomCoordinates(size_t n, Box<T> box, int seed = 42)
        : box_(std::move(box))
        , x_(n)
        , y_(n)
        , z_(n)
        , keys_(n)
    {
        // std::random_device rd;
        std::mt19937 gen(seed);
        std::uniform_real_distribution<T> disX(box_.xmin(), box_.xmax());
        std::uniform_real_distribution<T> disY(box_.ymin(), box_.ymax());
        std::uniform_real_distribution<T> disZ(box_.zmin(), box_.zmax());

        auto randX = [&disX, &gen]() { return disX(gen); };
        auto randY = [&disY, &gen]() { return disY(gen); };
        auto randZ = [&disZ, &gen]() { return disZ(gen); };

        std::generate(begin(x_), end(x_), randX);
        std::generate(begin(y_), end(y_), randY);
        std::generate(begin(z_), end(z_), randZ);

        computeSfcKeys(x_.data(), y_.data(), z_.data(), keys_.data(), n, box);

        std::vector<std::size_t> sfcOrder(n);
        std::iota(begin(sfcOrder), end(sfcOrder), std::size_t(0));
        sort_by_key(begin(keys_), end(keys_), begin(sfcOrder));

        std::vector<T> temp(x_.size());
        gather(sfcOrder.data(), sfcOrder.size(), x_.data(), temp.data());
        swap(x_, temp);
        gather(sfcOrder.data(), sfcOrder.size(), y_.data(), temp.data());
        swap(y_, temp);
        gather(sfcOrder.data(), sfcOrder.size(), z_.data(), temp.data());
        swap(z_, temp);
    }

    const std::vector<T>& x() const { return x_; }
    const std::vector<T>& y() const { return y_; }
    const std::vector<T>& z() const { return z_; }
    const std::vector<KeyType>& keys() const { return keys_; }

private:
    Box<T> box_;
    std::vector<T> x_, y_, z_;
    std::vector<KeyType> keys_;
};

} // namespace cstone