// Copyright Pubby 2016
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt

#ifndef LIB_FLAT_FLAT_SET_HPP
#define LIB_FLAT_FLAT_SET_HPP

#include "impl/flat_impl.hpp"

namespace fc {
namespace impl {

template<typename D, typename Key, typename Container, typename Compare,
         typename = void>
class flat_set_base
: public flat_container_base<D, Key, Container, Compare>
{
#include "impl/container_traits.hpp"
    using B = flat_container_base<D, Key, Container, Compare>;
    D const* self() const { return static_cast<D const*>(this); }
    D* self() { return static_cast<D*>(this); }
public:
    using value_compare = Compare;
    value_compare value_comp() const { return value_compare(); }

    using B::insert;
    using B::erase;

    // Modifiers

    std::pair<iterator, bool> insert(value_type const& value)
        { return insert_(value); }

    std::pair<iterator, bool> insert(value_type&& value)
        { return insert_(std::move(value)); }

    size_type erase(key_type const& key)
    {
        const_iterator it = self()->find(key);
        if(it == self()->end())
            return 0;
        self()->container.erase(it.underlying);
        return 1;
    }

    // Lookup

    size_type count(key_type const& key) const
    {
        return self()->find(key) != self()->end();
    }

private:
    template<typename V>
    std::pair<iterator, bool> insert_(V&& value)
    {
        iterator it = self()->lower_bound(value);
        if(it == self()->end() || self()->value_comp()(value, *it))
        {
            it = self()->container.insert(it.underlying, 
                                          std::forward<V>(value));
            return std::make_pair(it, true);
        }
        return std::make_pair(it, false);
    }
};

template<typename D, typename Key, typename Container, typename Compare>
class flat_set_base<D, Key, Container, Compare,
                    std::void_t<typename Compare::is_transparent>>
: public flat_set_base<D, Key, Container, Compare, int>
{
#include "impl/container_traits.hpp"
    using B = flat_set_base<D, Key, Container, Compare, int>;
    D const* self() const { return static_cast<D const*>(this); }
    D* self() { return static_cast<D*>(this); }
public:

    using B::insert;
    using B::count;

    // Modifiers
    
    template<class P>
    std::pair<iterator, bool> insert(P&& value)
    {
        iterator it = self()->lower_bound(value);
        if(it == self()->end() || self()->value_comp()(value, *it))
        {
            it = self()->container.insert(
                it.underlying, std::forward<P>(value));
            return std::make_pair(it, true);
        }
        return std::make_pair(it, false);
    }

    // Lookup

    template<typename K> 
    size_type count(K const& key) const
    {
        return self()->find(key) != self()->end();
    }
};

} // namespace impl

template<typename Container, typename Compare = std::less<void>>
class flat_set 
: public impl::flat_set_base<flat_set<Container, Compare>, 
    typename Container::value_type, Container, Compare>
{
    using D = flat_set;
    using Key = typename Container::value_type;
public:
#include "impl/container_traits.hpp"

    Container container;
};

template<typename T, typename Compare = std::less<void>>
using vector_set = flat_set<std::vector<T>, Compare>;

} // namespace fc

#endif