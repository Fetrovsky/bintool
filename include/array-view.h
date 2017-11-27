#ifndef ARRAY_VIEW_H__INCLUDED
#define ARRAY_VIEW_H__INCLUDED

template<typename T>
class array_view
{
    public:
        typedef T const* const_iterator;
        typedef T* iterator;

    private:
        iterator _begin;
        iterator _end;

    public:
        array_view(iterator begin, iterator end):
            _begin(begin),
            _end(end)
        {}

        array_view(iterator begin, size_t length):
            _begin(&begin[0]),
            _end(&begin[length])
        {}

        template<typename Container>
        array_view(Container const& container):
            array_view(container.begin(), container.end())
        {}

        size_t size() const             { return (_end - _begin); }
        size_t length() const           { return (_end - _begin); }

        const_iterator begin() const    { return _begin; }
        const_iterator end() const      { return _end; }

        iterator begin()                { return _begin; }
        iterator end()                  { return _end; }
};

#endif  // ARRAY_VIEW_H__INCLUDED

