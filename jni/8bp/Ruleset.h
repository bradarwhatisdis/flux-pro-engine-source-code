#pragma once

#include <string>
#include <vector>
#include <map>
#include <typeindex>

namespace rules {
    class IRule;
    enum Rules8BallFoulReason : int;
}

enum BallClassification : int;
class PoolPlayer;

struct Ruleset {
    int field_0x0; // 0x0
    int field_0x4; // 0x4
    std::string field_0x8; // 0x8
    int field_0x20; // 0x20
    // Padding 4 bytes
    std::map<std::type_index, rules::IRule *> field_0x28; // 0x28
    std::vector<rules::Rules8BallFoulReason> field_0x40; // 0x40
    bool field_0x58; // 0x58
    // Padding 7 bytes
    double field_0x60; // 0x60
    int field_0x68; // 0x68
    // Padding 4 bytes
    void* field_0x70; // 0x70
    void* field_0x78; // 0x78
    int field_0x80; // 0x80
    // Padding 4 bytes
    void* field_0x88; // 0x88
    int field_0x90; // 0x90
    // Padding 4 bytes
    std::vector<BallClassification> field_0x98; // 0x98
    std::vector<BallClassification> field_0xB0; // 0xB0
    std::vector<BallClassification> field_0xC8; // 0xC8
    std::vector<PoolPlayer *> field_0xE0; // 0xE0
    int field_0xF8; // 0xF8
    int field_0xFC; // 0xFC
    int field_0x100; // 0x100
    int field_0x104; // 0x104
    bool field_0x108; // 0x108
    // Padding 3 bytes
    int field_0x10C; // 0x10C
    char field_0x110; // 0x110
    char field_0x111; // 0x111
    bool field_0x112; // 0x112
    bool field_0x113; // 0x113
    bool field_0x114; // 0x114
    // Padding 3 bytes
    int field_0x118; // 0x118
    int field_0x11C; // 0x11C
    char field_0x120; // 0x120
    // Padding 3 bytes
    int field_0x124; // 0x124
    bool field_0x128; // 0x128
    // Padding 3 bytes
    int field_0x12C; // 0x12C
};

// ^{Ruleset=Ii{basic_string<char, std::char_traits<char>, std::allocator<char>>={__compressed_pair<std::basic_string<char>::__rep, std::allocator<char>>={__rep=(?={__long=QQ*}{__short=(?=CC)[23C]}{__raw=[3Q]})}}}I{map<std::type_index, rules::IRule *, std::less<std::type_index>, std::allocator<std::pair<const std::type_index, rules::IRule *>>>={__tree<std::__value_type<std::type_index, rules::IRule *>, std::__map_value_compare<std::type_index, std::__value_type<std::type_index, rules::IRule *>, std::less<std::type_index>, true>, std::allocator<std::__value_type<std::type_index, rules::IRule *>>>=^{__tree_end_node<std::__tree_node_base<void *> *>}{__compressed_pair<std::__tree_end_node<std::__tree_node_base<void *> *>, std::allocator<std::__tree_node<std::__value_type<std::type_index, rules::IRule *>, void *>>>={__tree_end_node<std::__tree_node_base<void *> *>=^{__tree_node_base<void *>}}}{__compressed_pair<unsigned long, std::__map_value_compare<std::type_index, std::__value_type<std::type_index, rules::IRule *>, std::less<std::type_index>, true>>=Q}}}{vector<rules::Rules8BallFoulReason, std::allocator<rules::Rules8BallFoulReason>>=^i^i{__compressed_pair<rules::Rules8BallFoulReason *, std::allocator<rules::Rules8BallFoulReason>>=^i}}Bdi@@I@I{vector<BallClassification, std::allocator<BallClassification>>=^i^i{__compressed_pair<BallClassification *, std::allocator<BallClassification>>=^i}}{vector<BallClassification, std::allocator<BallClassification>>=^i^i{__compressed_pair<BallClassification *, std::allocator<BallClassification>>=^i}}{vector<BallClassification, std::allocator<BallClassification>>=^i^i{__compressed_pair<BallClassification *, std::allocator<BallClassification>>=^i}}{vector<PoolPlayer *, std::allocator<PoolPlayer *>>=^@^@{__compressed_pair<PoolPlayer **, std::allocator<PoolPlayer *>>=^@}}iiiiBiccBBBIicIBI}
