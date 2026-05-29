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
    int field_0x14; // 0x14
    std::map<std::type_index, rules::IRule *> field_0x18; // 0x18
    std::vector<rules::Rules8BallFoulReason> field_0x24; // 0x24
    bool field_0x30; // 0x30
    // Padding 3 bytes
    float field_0x34; // 0x34
    int field_0x38; // 0x38
    void* field_0x3C; // 0x3C
    void* field_0x40; // 0x40
    unsigned int field_0x44; // 0x44
    void* field_0x48; // 0x48
    unsigned int field_0x4C; // 0x4C
    std::vector<BallClassification> field_0x50; // 0x50
    std::vector<BallClassification> field_0x5C; // 0x5C
    std::vector<PoolPlayer *> field_0x68; // 0x68
    int field_0x74; // 0x74
    int field_0x78; // 0x78
    int field_0x7C; // 0x7C
    int field_0x80; // 0x80
    bool field_0x84; // 0x84
    // Padding 3 bytes
    int field_0x88; // 0x88
    char field_0x8C; // 0x8C
    char field_0x8D; // 0x8D
    bool field_0x8E; // 0x8E
    bool field_0x8F; // 0x8F
    bool field_0x90; // 0x90
    // Padding 3 bytes
    unsigned int field_0x94; // 0x94
    int field_0x98; // 0x98
    char field_0x9C; // 0x9C
    // Padding 3 bytes
    unsigned int field_0xA0; // 0xA0
    bool field_0xA4; // 0xA4
    // Padding 3 bytes
    unsigned int field_0xA8; // 0xA8
};

// ^{Ruleset=Ii{basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> >={__compressed_pair<std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> >::__rep, std::__ndk1::allocator<char> >={__rep=(?={__long=II*}{__short=(?=CC)[11C]}{__raw=[3I]})}}}I{map<std::__ndk1::type_index, rules::IRule *, std::__ndk1::less<std::__ndk1::type_index>, std::__ndk1::allocator<std::__ndk1::pair<const std::__ndk1::type_index, rules::IRule *> > >={__tree<std::__ndk1::__value_type<std::__ndk1::type_index, rules::IRule *>, std::__ndk1::__map_value_compare<std::__ndk1::type_index, std::__ndk1::__value_type<std::__ndk1::type_index, rules::IRule *>, std::__ndk1::less<std::__ndk1::type_index>, true>, std::__ndk1::allocator<std::__ndk1::__value_type<std::__ndk1::type_index, rules::IRule *> > >=^{__tree_node<std::__ndk1::__value_type<std::__ndk1::type_index, rules::IRule *>, void *>}{__compressed_pair<std::__ndk1::__tree_end_node<std::__ndk1::__tree_node_base<void *> *>, std::__ndk1::allocator<std::__ndk1::__tree_node<std::__ndk1::__value_type<std::__ndk1::type_index, rules::IRule *>, void *> > >={__tree_end_node<std::__ndk1::__tree_node_base<void *> *>=^{__tree_node_base<void *>}}}{__compressed_pair<unsigned int, std::__ndk1::__map_value_compare<std::__ndk1::type_index, std::__ndk1::__value_type<std::__ndk1::type_index, rules::IRule *>, std::__ndk1::less<std::__ndk1::type_index>, true> >=I}}}{vector<rules::Rules8BallFoulReason, std::__ndk1::allocator<rules::Rules8BallFoulReason> >=^i^i{__compressed_pair<rules::Rules8BallFoulReason *, std::__ndk1::allocator<rules::Rules8BallFoulReason> >=^i}}Bfi@@I@I{vector<BallClassification, std::__ndk1::allocator<BallClassification> >=^i^i{__compressed_pair<BallClassification *, std::__ndk1::allocator<BallClassification> >=^i}}{vector<BallClassification, std::__ndk1::allocator<BallClassification> >=^i^i{__compressed_pair<BallClassification *, std::__ndk1::allocator<BallClassification> >=^i}}{vector<PoolPlayer *, std::__ndk1::allocator<PoolPlayer *> >=^@^@{__compressed_pair<PoolPlayer **, std::__ndk1::allocator<PoolPlayer *> >=^@}}iiiiBiccBBBIicIBI}
