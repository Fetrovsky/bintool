#ifndef RANGE_H__INCLUDED
#define RANGE_H__INCLUDED

template<typename T>
class Range
{
    private:
        T _begin;
        T _end;

    public:
        Range(T begin, T end):
            _begin(begin),
            _end(end)
        {}

        T begin() const
        {
            return _begin;
        }

        T end() const
        {
            return _end;
        }

        size_t size() const
        {
            return length();
        }

        size_t length() const
        {
            return (end() - begin());
        }

        bool Contains(T value) const
        {
            return ((value >= begin()) && (value < end()));
        }
};

#endif  // RANGE_H__INCLUDED

