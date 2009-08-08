#include "en-US/strings.h"
#include "es-ES/strings.h"
#include "gl-ES/strings.h"
#include "ko-KR/strings.h"

struct {
	wchar_t *code;
	struct strings *strings;
} languages[] = {
	{L"en-US", &en_US},
	{L"es-ES", &es_ES},
	{L"gl-ES", &gl_ES},
	{L"ko-KR", &ko_KR},
};

int num_languages = 4;
