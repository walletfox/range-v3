// Range v3 library
//
//  Copyright Eric Niebler 2013-2014
//  Copyright Gonzalo Brito Gadeschi 2014
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ericniebler/range-v3
//
// Implementation based on the code in libc++
//   http://http://libcxx.llvm.org/

#ifndef RANGES_V3_ALGORITHM_UNIQUE_HPP
#define RANGES_V3_ALGORITHM_UNIQUE_HPP

#include <range/v3/range_fwd.hpp>
#include <range/v3/begin_end.hpp>
#include <range/v3/range_concepts.hpp>
#include <range/v3/range_traits.hpp>
#include <range/v3/algorithm/adjacent_find.hpp>

namespace ranges
{
    inline namespace v3
    {
        /// \addtogroup group-algorithms
        /// @{
        struct unique_fn
        {
            /// \brief template function \c unique_fn::operator()
            ///
            /// range-based version of the \c unique std algorithm
            ///
            /// \pre \c Rng is a model of the ForwardRange concept
            /// \pre \c I is a model of the ForwardIterator concept
            /// \pre \c S is a model of the Sentinel concept
            /// \pre \c C is a model of the InvokableRelation concept
            ///
            template<typename I, typename S, typename C = equal_to, typename P = ident,
                CONCEPT_REQUIRES_(Sortable<I, C, P>() && IteratorRange<I, S>())>
            I operator()(I begin, S end, C pred_ = C{}, P proj_ = P{}) const
            {
                auto &&pred = invokable(pred_);
                auto &&proj = invokable(proj_);

                begin = adjacent_find(std::move(begin), end, std::ref(pred), std::ref(proj));

                if(begin != end)
                {
                    for(I i = next(begin); ++i != end;)
                        if(!pred(proj(*begin), proj(*i)))
                            *++begin = std::move(*i);
                    ++begin;
                }
                return begin;
            }

            template<typename Rng, typename C = equal_to, typename P = ident,
                typename I = range_iterator_t<Rng>,
                CONCEPT_REQUIRES_(Sortable<I, C, P>() && Iterable<Rng &>())>
            I operator()(Rng & rng, C pred = C{}, P proj = P{}) const
            {
                return (*this)(begin(rng), end(rng), std::move(pred), std::move(proj));
            }
        };

        constexpr unique_fn unique{};

        /// @}
    } // namespace v3
} // namespace ranges

#endif // include guard
