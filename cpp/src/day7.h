#pragma once

#ifndef AOC_DAY7_H
#define AOC_DAY7_H

#include <iostream>
#include <regex>
#include <algorithm>
#include <numeric>
#include <string>
#include <map>
#include <cstdint>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/variant.hpp>
#include "utils.h"


namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

namespace aoc {

////////////////////////////////////////
// parse the rules with Boost.Spirit

using signal_type = unsigned short;
using variable_t = boost::variant<std::string, signal_type>;

enum class Ops {
    AND, OR, LSHIFT, RSHIFT, NOT, ASSIGN
};
struct expr {    
    variable_t operand1;
    Ops op;
    variable_t operand2;
    std::string result_var;
};

} // namespace aoc

BOOST_FUSION_ADAPT_STRUCT(
    aoc::expr,
    (aoc::variable_t, operand1)
    (aoc::Ops, op)
    (aoc::variable_t, operand2)
    (std::string, result_var)
)

namespace aoc {

struct command : public qi::grammar<std::string::const_iterator, expr(), ascii::space_type>
{
    command() : command::base_type(cmd, "cmd_grammar")
    {
        using boost::phoenix::at_c;
        namespace phoenix = boost::phoenix;

        variable_name %= qi::lexeme[+qi::alpha];
        variable_value %= qi::ushort_;
        variable %= (variable_name | variable_value);
        binary_op_name =
            qi::lit("AND")[qi::_val = Ops::AND] |
            qi::lit("OR")[qi::_val = Ops::OR] |
            qi::lit("LSHIFT")[qi::_val = Ops::LSHIFT] |
            qi::lit("RSHIFT")[qi::_val = Ops::RSHIFT];

        binary_op %= variable >> binary_op_name >> variable >> qi::lit("->") >> variable_name;
        not_op = 
            qi::eps[at_c<0>(qi::_val) = ""] >>
            qi::lit("NOT")[at_c<1>(qi::_val) = Ops::NOT] >> 
            variable[at_c<2>(qi::_val) = qi::_1] >> 
            "->" >> 
            variable_name[at_c<3>(qi::_val) = qi::_1];
        assign_op =
            qi::eps[at_c<0>(qi::_val) = ""] >>
            qi::eps[at_c<1>(qi::_val) = Ops::ASSIGN] >>
            variable[at_c<2>(qi::_val) = qi::_1] >>
            "->" >>
            variable_name[at_c<3>(qi::_val) = qi::_1];
        cmd = binary_op | not_op | assign_op;

        variable_name.name("variable_name");
        variable_value.name("variable_value");
        variable.name("variable");
        binary_op_name.name("binary_op_name");
        binary_op.name("binary_op");
        not_op.name("not_op");
        assign_op.name("assign_op");
        cmd.name("cmd");

        qi::on_error<qi::fail>
            (
                cmd
                , std::cout
                << phoenix::val("Error! Expecting ")
                << qi::_4                               // what failed?
                << phoenix::val(" here: \"")
                << phoenix::construct<std::string>(qi::_3, qi::_2)   // iterators to error-pos, end
                << phoenix::val("\"")
                << std::endl
            );
    }

    qi::rule<std::string::const_iterator, std::string(), ascii::space_type> variable_name;
    qi::rule<std::string::const_iterator, unsigned short(), ascii::space_type> variable_value;
    qi::rule<std::string::const_iterator, variable_t(), ascii::space_type> variable;
    qi::rule<std::string::const_iterator, Ops(), ascii::space_type> binary_op_name;
    qi::rule<std::string::const_iterator, expr(), ascii::space_type> binary_op;
    qi::rule<std::string::const_iterator, expr(), ascii::space_type> not_op;
    qi::rule<std::string::const_iterator, expr(), ascii::space_type> assign_op;
    qi::rule<std::string::const_iterator, expr(), ascii::space_type> cmd;
};


////////////////////////////////////////
// variable evaluation with Boost.Variant
struct Environment
{
    std::vector<expr> rules;
    std::map<std::string, signal_type> cache;
};

class visitor : public boost::static_visitor<signal_type>
{
public:
    visitor(Environment& env) :
        m_env(env)
    { }

    signal_type operator()(const signal_type& val)
    {
        return val;
    }
    signal_type operator()(const std::string& val)
    {
        auto it_cache = m_env.cache.find(val);
        if (it_cache != m_env.cache.end()) {
            return it_cache->second;
        }
        auto it = std::find_if(m_env.rules.begin(), m_env.rules.end(), [&val](const expr& e) { return val.compare(e.result_var) == 0; });
        if (it == m_env.rules.end()) {
            throw std::runtime_error(std::string("Could not find environment entry for variable ") + val);
        }
        signal_type v1 = 0, v2 = 0;
        switch (it->op)
        {
            case Ops::AND:
            case Ops::OR:
            case Ops::LSHIFT:
            case Ops::RSHIFT:
                v1 = boost::apply_visitor(*this, it->operand1);
                v2 = boost::apply_visitor(*this, it->operand2);
                break;
            case Ops::NOT:
            case Ops::ASSIGN:
                v2 = boost::apply_visitor(*this, it->operand2);
                break;
            default:
                throw std::runtime_error("Unsupported operation");
        }
        //std::cout << val << " -> " << v1 << " " << static_cast<int>(it->op) << " " << v2 << std::endl;
        signal_type v = 0;
        switch (it->op)
        {
            case Ops::AND:
                v = v1 & v2;
                break;
            case Ops::OR:
                v = v1 | v2;
                break;
            case Ops::LSHIFT:
                v = v1 << v2;
                break;
            case Ops::RSHIFT:
                v = v1 >> v2;
                break;
            case Ops::NOT:
                v = ~v2;
                break;
            case Ops::ASSIGN:
                v = v2;
                break;
            default:
                throw std::runtime_error("Unsupported operation");
        }
        m_env.cache[val] = v;
        return v;
    }
private:
    Environment& m_env;
};

static signal_type eval(const std::string var, Environment& env)
{
    return visitor(env)(var);
}


class Day7
{
public:
    static void execute()
    {
        Environment environment = parse_environment("../../../input/day7.txt");

        // uncomment for part 2
        //expr e;
        //e.result_var = "b";
        //e.op = Ops::ASSIGN;
        //e.operand1 = "";
        //e.operand2 = 46065;
        //auto it = std::find_if(environment.rules.begin(), environment.rules.end(), [](const expr& e) { return e.result_var.compare("b") == 0; });
        //*it = e;

        const std::string varName = "a";
        std::cout << "The value of " << varName << " is " << eval(varName, environment) << std::endl;
    }
private:
    static Environment parse_environment(const std::string& filepath)
    {
        Environment environment;
        const std::string input = read_file_into_string(filepath);
        std::istringstream input_stream(input);
        const command g;
        for (std::string line; std::getline(input_stream, line); )
        {
            std::string::const_iterator start = line.cbegin();
            std::string::const_iterator end = line.end();
            expr e;
            bool r = qi::phrase_parse(start, end,
                                      g,
                                      ascii::space,
                                      e
                                      );
            if (!r) { throw std::runtime_error(std::string("Parse failed at: ") + line); }
            environment.rules.push_back(e);
        }
        return environment;
    }

};

} //namespace

#endif // AOC_DAY7_H
