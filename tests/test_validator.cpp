//////////////////////////////////////////////////////////////////////////
#include <sjv/sjv.h>
#include <catch2/catch.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <filesystem>
//////////////////////////////////////////////////////////////////////////

using namespace sjv;

TEST_CASE("single_rule", "[validator]")
{
    json input = R"( 
        {
        }
        )"_json;

    json rules = R"( 
        [
        {
            "pointer": "/",
            "type": "skip_check"
        }
        ]
        )"_json;

    sjv::sjv sjv;

    REQUIRE(sjv.verify_json(input,rules));

}

TEST_CASE("only_one_rule_can_be_valid", "[validator]")
{
    json input = R"( 
        {
        }
        )"_json;

    json rules = R"( 
        [
        {
            "pointer": "/",
            "type": "skip_check"
        },
        {
            "pointer": "/",
            "type": "skip_check"
        }
        ]
        )"_json;

    sjv::sjv sjv;

    REQUIRE(!sjv.verify_json(input,rules));
}

TEST_CASE("min_bound_numeric", "[validator]")
{
    json input = R"( 
        {
            "field1": 48.5
        }
        )"_json;

    json rules = R"( 
        [
        {
            "pointer": "/",
            "type": "skip_check"
        },
        {
            "pointer": "/field1/",
            "type": "float",
            "min": 45
        }
        ]
        )"_json;

    sjv::sjv sjv;

    REQUIRE(sjv.verify_json(input,rules));

    input["field1"] = 40.5;

    REQUIRE(!sjv.verify_json(input,rules));
}

TEST_CASE("file_type", "[validator]")
{
    json input = R"( 
        {
            "file1": "CMakeCach.txt"
        }
        )"_json;

    json rules = R"( 
        [
        {
            "pointer": "/",
            "type": "skip_check"
        },
        {
            "pointer": "/file1/",
            "type": "file",
            "extensions": [".txt"]
        }
        ]
        )"_json;

    sjv::sjv sjv;
    sjv.cwd = std::filesystem::current_path();

    REQUIRE(!sjv.verify_json(input,rules));

    input["file1"] = "CMakeCache.txt";

    REQUIRE(sjv.verify_json(input,rules));

    rules[1]["extensions"][0] = ".msh";

    REQUIRE(!sjv.verify_json(input,rules));

}

TEST_CASE("type_string", "[validator]")
{
    json input = R"( 
        {
            "string1": "teststring"
        }
        )"_json;

    json rules = R"( 
        [
        {
            "pointer": "/",
            "type": "skip_check"
        },
        {
            "pointer": "/string1/",
            "type": "string"
        }
        ]
        )"_json;

    sjv::sjv sjv;

    REQUIRE(sjv.verify_json(input,rules));

    rules[1]["options"][0] = "blah";

    REQUIRE(!sjv.verify_json(input,rules));

    rules[1]["options"][1] = "teststring";

    REQUIRE(sjv.verify_json(input,rules));

}

TEST_CASE("type_object", "[validator]")
{
    json input = R"( 
        {
            "string1": "teststring"
        }
        )"_json;

    json rules = R"( 
        [
        {
            "pointer": "/",
            "type": "object",
            "required": ["string1"]
        }
        ]
        )"_json;

    sjv::sjv sjv;

    sjv.strict = true;
    REQUIRE(!sjv.verify_json(input,rules));
    sjv.strict = false;
    REQUIRE(sjv.verify_json(input,rules));

    rules[0]["required"][1] = "randomstring";

    REQUIRE(!sjv.verify_json(input,rules));

}

TEST_CASE("file1", "[validator]")
{
    json input = R"( 
        {
            "string1": "teststring"
        }
        )"_json;

    json rules = R"( 
        [
        {
            "pointer": "/",
            "type": "object",
            "required": ["string1"]
        }
        ]
        )"_json;

    sjv::sjv sjv;

    sjv.strict = true;

    bool r = sjv.verify_json(input,rules); 
    std:: string s = sjv.log2str();
    INFO(s);
    REQUIRE(r);
}
