#include <iostream>
#include <string>

#include <unicode/idna.h>
#include <unicode/unistr.h>
#include <unicode/ustream.h>

using namespace icu;

static void showErrors(const IDNAInfo &info)
{
    auto errors = info.getErrors();

#define SHOW_ERROR(name)                      \
    if (errors & UIDNA_ERROR_##name)          \
        std::cout << "    "#name << std::endl

    SHOW_ERROR(EMPTY_LABEL);
    SHOW_ERROR(LABEL_TOO_LONG);
    SHOW_ERROR(DOMAIN_NAME_TOO_LONG);
    SHOW_ERROR(LEADING_HYPHEN);
    SHOW_ERROR(TRAILING_HYPHEN);
    SHOW_ERROR(HYPHEN_3_4);
    SHOW_ERROR(LEADING_COMBINING_MARK);
    SHOW_ERROR(DISALLOWED);
    SHOW_ERROR(PUNYCODE);
    SHOW_ERROR(LABEL_HAS_DOT);
    SHOW_ERROR(INVALID_ACE_LABEL);
    SHOW_ERROR(BIDI);
    SHOW_ERROR(CONTEXTJ);
    SHOW_ERROR(CONTEXTO_PUNCTUATION);
    SHOW_ERROR(CONTEXTO_DIGITS);
#undef SHOW_ERROR
}

int main(int argc, char **argv)
{
    UErrorCode status = U_ZERO_ERROR;
    IDNA *idna = IDNA::createUTS46Instance(UIDNA_USE_STD3_RULES | UIDNA_CHECK_BIDI | UIDNA_CHECK_CONTEXTJ, status);

    if (U_FAILURE(status)) {
        std::cerr << "Failed to create IDNA instane: " << status << std::endl;
        return 1;
    }

    for (argc--, argv++; argc > 0; argc--, argv--) {
        UnicodeString in = UnicodeString::fromUTF8(*argv);
        UnicodeString out;
        IDNAInfo info;
        idna->nameToUnicode(in, out, info, status);

        if (U_FAILURE(status)) {
            std::cerr << "Conversion failed: " << status << std::endl;
            return 2;
        }

        std::cout << "Input:  " << in << std::endl;
        std::cout << "Output: " << out << std::endl;
        std::cout << "Errors: " << std::hex << "0x" << info.getErrors() << std::endl;
        showErrors(info);

        std::cout << std::endl;
    }
    return 0;
}
