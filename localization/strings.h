#include "en-US/strings.h"
#include "es-ES/strings.h"

struct {
	wchar_t *code;
	struct strings *strings;
} languages[]={
	{L"en-US",&en_US},
	{L"es-ES",&es_ES}
};

int num_languages=2;
