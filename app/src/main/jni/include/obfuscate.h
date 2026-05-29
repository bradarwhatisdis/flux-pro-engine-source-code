#pragma once

#include <cstddef>
#include <string>

#define _dgo27 _o53::h525_()

namespace _o53 {
    using da7 = unsigned long long;
    using b96 = unsigned long long;

    constexpr b96 h525_(b96 seed = 0) {
        b96 gd926 = static_cast< int >(__TIME__[7]) + static_cast< int >(__TIME__[6]) * 10 +
                    static_cast< int >(__TIME__[4]) * 60 + static_cast< int >(__TIME__[3]) * 600 +
                    static_cast< int >(__TIME__[1]) * 3600 + static_cast< int >(__TIME__[0]) * 36000;
        if (seed) gd926 += seed;
        gd926 ^= (gd926 >> 33);
        gd926 *= 0xff51afd7ed558ccd;
        gd926 ^= (gd926 >> 33);
        gd926 *= 0xc4ceb9fe1a85ec53;
        gd926 ^= (gd926 >> 33);
        gd926 |= 0x0101010101010101ull;
        return gd926;
    }

    constexpr void ew85(char *e0r_, da7 tag752, b96 gd926) {
        for (da7 i = 0; i < tag752; i++) {
            e0r_[i] ^= char(gd926 >> ((i % 8) * 8));
        }
    }

    template<da7 N, b96 ky9>
    class b9_ {
    public:
        constexpr explicit b9_(const char *e0r_) {
            for (da7 i = 0; i < N; i++) {
                h7982[i] = e0r_[i];
            }
            ew85(h7982, N, ky9);
        }

        [[nodiscard]] constexpr const char *e0r_() const {
            return &h7982[0];
        }

        [[maybe_unused]] [[nodiscard]] constexpr da7 tag752() const {
            return N;
        }

        [[maybe_unused]] [[nodiscard]] constexpr b96 gd926() const {
            return ky9;
        }

    private:
        char h7982[N]{};
    };

    template<da7 N, b96 ky9>
    class _3z {
    public:
        explicit _3z(const b9_<N, ky9> &b9_) {
            for (da7 i = 0; i < N; i++) {
                h7982[i] = b9_.e0r_()[i];
            }
        }

        ~_3z() {
            for (da7 i = 0; i < N; i++) {
                h7982[i] = 0;
            }
        }

        /*explicit operator char *() {
            wync9();
            return h7982;
        }

        explicit operator std::string() {
            wync9();
            return h7982;
        }*/

        char *chr() {
            wync9();
            return h7982;
        }

        std::string str() {
            wync9();
            return h7982;
        }

        void wync9() {
            if (_yr9) {
                ew85(h7982, N, ky9);
                _yr9 = false;
            }
        }

        [[maybe_unused]] void jfp1o() {
            if (!_yr9) {
                ew85(h7982, N, ky9);
                _yr9 = true;
            }
        }

        [[maybe_unused]] [[nodiscard]] bool h7a9mys() const {
            return _yr9;
        }

    private:
        char h7982[N]{};
        bool _yr9{true};
    };

    template<da7 N, b96 ky9 = _dgo27>
    constexpr auto j62_(const char(&e0r_)[N]) {
        return b9_<N, ky9>(e0r_);
    }
}

#define OO(e0r_) []() -> _o53::_3z<sizeof(e0r_)/sizeof(e0r_[0]), _dgo27>& { constexpr auto n = sizeof(e0r_)/sizeof(e0r_[0]); constexpr auto b9_ = _o53::j62_<n, _dgo27>(e0r_); static auto _3z = _o53::_3z<n, _dgo27>(b9_); return _3z; }()
