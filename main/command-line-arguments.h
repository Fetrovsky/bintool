#ifndef COMMAND_LINE_ARGUMENTS__INCLUDED
#define COMMAND_LINE_ARGUMENTS__INCLUDED

#include <string>
#include <string_view>
#include <vector>

using std::string;
using std::string_view;
using std::vector;

class Switch
{
    private:
        string const _long_name;
        string const _short_name;
        bool _value;

    public:
        Switch(string_view long_name, string_view short_name, bool default_value):
            _long_name{long_name}, _short_name{short_name}, _value{default_value}
        {}

        Switch(string_view long_name, string_view short_name):
            _long_name{long_name}, _short_name{short_name}, _value{false}
        {}

        operator bool() const noexcept { return _value; }

        bool Parse(string_view name)
        {
            if ((_short_name == name) || (_long_name == name))
            {
                _value = true;
                return true;
            }

            return false;
        }
};

class Parameter
{
    private:
        string const _long_name;
        string const _short_name;
        string _value;

    public:
        Parameter(string_view long_name, string_view short_name, bool value):
            _long_name{long_name}, _short_name{short_name}, _value{value}
        {}

        Parameter(string_view long_name, string_view short_name):
            _long_name{long_name}, _short_name{short_name}, _value{}
        {}

        string_view Long_Name() const noexcept { return _long_name; }
        string_view Short_Name() const noexcept { return _short_name; }
        operator string() const noexcept { return _value; }

        bool Parse(string_view name, string_view value)
        {
            if ((_short_name == name) || (_long_name == name))
            {
                _value = string{value};
                return true;
            }

            return false;
        }
};

class Command_Line_Arguments
{
    private:
        std::vector<Switch> _switches;
        std::vector<Parameter> _parameters;
        std::vector<string> _standalone;

        bool _parse_parameter(string_view arg, string_view value)
        {
            for (auto& p: _parameters)
            {
                if (p.Parse(arg, value))
                {
                    return true;
                }
            }

            return false;
        }

        bool _parse_switch(string_view arg)
        {
            for (auto& s: _switches)
            {
                if (s.Parse(arg))
                {
                    return true;
                }
            }

            return true;
        }

    public:
        Command_Line_Arguments(std::initializer_list<Switch> switches, std::initializer_list<Parameter> parameters):
            _switches{switches}, _parameters{parameters}
        {}

        bool Parse(std::span<char const* const> args)
        {
            for (int i = 1; i < args.size(); ++i)
            {
                string_view arg{args[i]};

                if ((arg.length() > 1) && (arg[0] == '-') && (i < (args.size() - 1)))
                {
                    if (_parse_parameter(arg, args[i+1]))
                    {
                        ++i;
                        continue;
                    }
                }

                if ((arg.length() > 1) && (arg[0] == '-'))
                {
                    if (_parse_switch(arg))
                        continue;
                }

                _standalone.push_back(string{arg});
            }

            return true;
        }

        std::vector<Switch> const& Switches() const noexcept { return _switches; }
        std::vector<Parameter> const& Parameters() const noexcept { return _parameters; }
        std::vector<string> const& Standalone() const noexcept { return _standalone; }
};

#endif  // COMMAND_LINE_ARGUMENTS__INCLUDED
