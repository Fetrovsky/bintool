#ifndef ENUM_H__INCLUDED
#define ENUM_H__INCLUDED

#include <functional>
#include <string_view>
#include <type_traits>
#include <vector>

template<typename Enum>
struct Enable_Bitwise_Operations
{
    static const bool value = false;
};

#define ENABLE_BITWISE_OPERATIONS(Enum)         \
    template<>                                  \
    struct Enable_Bitwise_Operations<Enum>      \
    {                                           \
        static const bool value = true;         \
    };                                          \

template<typename Enum, typename base_type = typename std::underlying_type<Enum>::type>
static inline constexpr typename std::enable_if<Enable_Bitwise_Operations<Enum>::value, Enum>::type
Perform_Enum_Operation(Enum left, Enum right,std::function<base_type(base_type, base_type)> op) 
{
    static_assert(std::is_enum<Enum>::value, "Template type parameter is not an enum.");

    return Enum{op(base_type(left), base_type(right))};
}

#define DEFINE_BINARY_BITWISE_OPERATOR(op)                                                                      \
    template<typename Enum>                                                                                     \
    constexpr Enum operator op(Enum left, Enum right)                                                           \
    {                                                                                                           \
        using base_type = typename std::underlying_type<Enum>::type;                                            \
        std::function<base_type(base_type, base_type)> fn = [](auto left, auto right) {return left op right;};  \
        return Perform_Enum_Operation(left, right, fn);                                                         \
    }                                                                                                           \

DEFINE_BINARY_BITWISE_OPERATOR(&);
DEFINE_BINARY_BITWISE_OPERATOR(|);
DEFINE_BINARY_BITWISE_OPERATOR(^);

template<typename Enum>
static inline auto Get_Enum_Names(Enum value, std::function<std::string_view(Enum)> Get_Name)
{
    std::vector<std::string_view> names;

    for (int i = 0; i < sizeof(Enum) * 8; ++i)
    {
        auto probing = Enum(1<<i);

        if (int(value & probing) != 0)
            names.push_back(Get_Name(probing));
    }

    return names;
}


#endif  // ENUM_H__INCLUDED

