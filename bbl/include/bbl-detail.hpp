#pragma once 

namespace babble {

template <class IteratorT> 
class Range {
    IteratorT _begin;
    IteratorT _end;

public:
    Range(IteratorT begin, IteratorT end) : _begin(begin), _end(end) {}

    auto cbegin() const -> IteratorT const {
        return _begin;
    }

    auto cend() const -> IteratorT const {
        return _end;
    }

    auto begin() -> IteratorT {
        return _begin;
    }

    auto end() -> IteratorT {
        return _end;
    }
};

class IndexIterator {
protected:
    size_t _index;

public:
    IndexIterator(size_t index) : _index(index) {}

    auto operator++() -> void {
        _index++;
    }

    auto operator==(IndexIterator const& rhs) {
        return _index == rhs._index;
    }

    auto operator!=(IndexIterator const& rhs) {
        return _index != rhs._index;
    }
};

}