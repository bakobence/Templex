#include "TypeCache.h"

#include <utility>

using namespace templex;
using namespace templex::parser;

void TypeCache::addClassTemplate(ClassTemplatePtr classTemplate)
{
    instantiations_.try_emplace(classTemplate, ClassInstantiations());
}
