#include "number.h"

uint239_t FromInt(uint32_t value, uint32_t shift) {
    uint239_t num;
    int num_arr[245] = {0};
    for (int i = 0; i < 35; ++i) {
        num.data[i] = 0;
    }

    for (int i = 244; i >= 0; --i) {
        if (value > 0) {
            num_arr[i] = value % 2;
            value /= 2;
        }
    }

    num_to_left(num_arr, shift);

    int bit_ind = 244;
    for (int byte_ind = 34; byte_ind >= 0; --byte_ind) {
        int byte = 0;
        for (int count = 0; count < 7; ++count) {
            byte += num_arr[bit_ind] * static_cast<int>(std::pow(2, count));
            --bit_ind;
        }
        num.data[byte_ind] = byte;
    }

    for (int byte_ind = 34; byte_ind >= 0; --byte_ind) {
        if (shift > 0) {
            num.data[byte_ind] |= ((shift % 2) << 7);
            shift /= 2;
        }
    }

    return num;
}

uint239_t FromString(const char* str, uint32_t shift) {
    uint239_t num;
    uint64_t value = std::stoull(str);
    int num_arr[245] = {0};
    for (int i = 0; i < 35; ++i) {
        num.data[i] = 0;
    }

    for (int i = 244; i >= 0; --i) {
        if (value > 0) {
            num_arr[i] = value % 2;
            value /= 2;
        }
    }

    num_to_left(num_arr, shift);

    int bit_ind = 244;
    for (int byte_ind = 34; byte_ind >= 0; --byte_ind) {
        int byte = 0;
        for (int count = 0; count < 7; ++count) {
            byte += num_arr[bit_ind] * static_cast<int>(std::pow(2, count));
            --bit_ind;
        }
        num.data[byte_ind] = byte;
    }

    for (int byte_ind = 34; byte_ind >= 0; --byte_ind) {
        if (shift > 0) {
            num.data[byte_ind] |= ((shift % 2) << 7);
            shift /= 2;
        }
    }

    return num;
}

uint239_t operator+(const uint239_t& lhs, const uint239_t& rhs) {
    int num_1[245] = {0}, num_2[245] = {0};
    int num_byte = 0, cdvig = 6;

    for (int i = 0; i < 245; ++i) {
        if (cdvig < 0) {
            ++num_byte;
            cdvig = 6;
        }
        num_1[i] = (lhs.data[num_byte] >> cdvig) & 1;
        num_2[i] = (rhs.data[num_byte] >> cdvig) & 1;
        --cdvig;
    }

    uint32_t rez_cdvig = GetShift(lhs) + GetShift(rhs);
    num_to_right(num_1, GetShift(lhs));
    num_to_right(num_2, GetShift(rhs));

    int32_t rez_num = num_in_10(num_1) + num_in_10(num_2);
    int rez_num_arr[245] = {0};
    for (int i = 244; i >= 0; --i) {
        if (rez_num > 0) {
            rez_num_arr[i] = rez_num % 2;
            rez_num /= 2;
        }
    }

    num_to_left(rez_num_arr, rez_cdvig);
    int32_t rez_num_after_shift = num_in_10(rez_num_arr);

    uint239_t rez;
    for (int i = 0; i < 35; ++i) {
        rez.data[i] = 0;
    }

    for (int byte_ind = 34; byte_ind >= 0; --byte_ind) {
        if (rez_num_after_shift > 0) {
            rez.data[byte_ind] = rez_num_after_shift % 128;
            rez_num_after_shift /= 128;
        }
    }

    for (int byte_ind = 34; byte_ind >= 0; --byte_ind) {
        if (rez_cdvig > 0) {
            rez.data[byte_ind] |= ((rez_cdvig % 2) << 7);
            rez_cdvig /= 2;
        }
    }

    std::cout << std::endl;
    return rez;
}

uint239_t operator-(const uint239_t& lhs, const uint239_t& rhs) {
    int num_1[245] = {0}, num_2[245] = {0};
    int num_byte = 0, cdvig = 6;
    int32_t rez_cdvig = static_cast<int32_t>(GetShift(lhs)) - static_cast<int32_t>(GetShift(rhs));

    for (int i = 0; i < 245; ++i) {
        if (cdvig < 0) {
            ++num_byte;
            cdvig = 6;
        }
        num_1[i] = (lhs.data[num_byte] >> cdvig) & 1;
        num_2[i] = (rhs.data[num_byte] >> cdvig) & 1;
        --cdvig;
    }

    num_to_right(num_1, GetShift(lhs));
    num_to_right(num_2, GetShift(rhs));

    int32_t rez_num = num_in_10(num_1) - num_in_10(num_2);
    int rez_num_arr[245] = {0};
    for (int i = 244; i >= 0; --i) {
        if (rez_num > 0) {
            rez_num_arr[i] = rez_num % 2;
            rez_num /= 2;
        }
    }

    if (rez_cdvig < 0) {
        rez_cdvig = -rez_cdvig;
        num_to_right(rez_num_arr, rez_cdvig);
    } else {
        num_to_left(rez_num_arr, rez_cdvig);
    }

    uint239_t rez;
    for (int i = 0; i < 35; ++i) {
        rez.data[i] = 0;
    }

    int bit_ind = 244;
    for (int byte_ind = 34; byte_ind >= 0; --byte_ind) {
        int byte = 0;
        for (int count = 0; count < 7; ++count) {
            byte += rez_num_arr[bit_ind] * static_cast<int>(std::pow(2, count));
            --bit_ind;
        }
        rez.data[byte_ind] = byte;
    }

    for (int byte_ind = 34; byte_ind >= 0; --byte_ind) {
        if (rez_cdvig > 0) {
            rez.data[byte_ind] |= ((rez_cdvig % 2) << 7);
            rez_cdvig /= 2;
        }
    }

    return rez;
}

bool operator==(const uint239_t& lhs, const uint239_t& rhs) {
    int num1[245] = {0}, num2[245] = {0};

    for (int byte_index = 0; byte_index < 35; ++byte_index) {
        for (int bit_index = 6; bit_index >= 0; --bit_index) {
            int i = byte_index * 7 + (6 - bit_index);
            num1[i] = (lhs.data[byte_index] >> bit_index) & 1;
            num2[i] = (rhs.data[byte_index] >> bit_index) & 1;
        }
    }

    num_to_right(num1, GetShift(lhs));
    num_to_right(num2, GetShift(rhs));

    return num_in_10(num1) == num_in_10(num2);
}

bool operator!=(const uint239_t& lhs, const uint239_t& rhs) {
    int num1[245] = {0}, num2[245] = {0};

    for (int byte_index = 0; byte_index < 35; ++byte_index) {
        for (int bit_index = 6; bit_index >= 0; --bit_index) {
            int i = byte_index * 7 + (6 - bit_index);
            num1[i] = (lhs.data[byte_index] >> bit_index) & 1;
            num2[i] = (rhs.data[byte_index] >> bit_index) & 1;
        }
    }

    num_to_right(num1, GetShift(lhs));
    num_to_right(num2, GetShift(rhs));

    return num_in_10(num1) != num_in_10(num2);
}

std::ostream& operator<<(std::ostream& stream, const uint239_t& value) {
    for (int i = 0; i < 35; ++i) {
        uint8_t byte = value.data[i];
        for (int bit = 7; bit >= 0; --bit) {
            stream << ((byte >> bit) & 1);
        }
    }
    return stream;
}

uint64_t GetShift(const uint239_t& value) {
    uint32_t rez = 0;
    for (int i = 34; i >= 0; --i) {
        if (value.data[i] >> 7) {
            rez += (value.data[i] >> 7) * static_cast<uint32_t>(std::pow(2, 34 - i));
        }
    }
    return rez;
}

void num_to_right(int* arr, uint32_t shift) {
    uint32_t do_shift = shift % 245;
    for (uint32_t s = 0; s < do_shift; ++s) {
        int last_bit = arr[244];
        for (int i = 244; i > 0; --i) {
            arr[i] = arr[i - 1];
        }
        arr[0] = last_bit;
    }
}

uint32_t num_in_10(int* arr) {
    uint32_t rez = 0;
    for (int i = 244; i >= 0; --i) {
        if (arr[i] == 1) {
            rez += static_cast<uint32_t>(std::pow(2, 244 - i));
        }
    }
    return rez;
}

void num_to_left(int* arr, uint32_t shift) {
    uint32_t shift_do = shift % 245;
    for (uint32_t s = 0; s < shift_do; ++s) {
        int first_bit = arr[0];
        for (int i = 0; i < 244; ++i) {
            arr[i] = arr[i + 1];
        }
        arr[244] = first_bit;
    }
}

