#ifndef __HASH__
#define __HASH__

unsigned long simple_hash(STRING_TYPE s) {
    unsigned long result = 0;
    for (int i = 0; i < s.length(); i++) {
        result += s[i] * (i+1);
    }
    return result;
}

#endif