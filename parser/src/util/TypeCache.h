#pragma once

#include <map>
#include <memory>
#include <vector>

#include <llvm/ADT/DenseMap.h>

#include "model/ClassInstantiation.h"
#include "model/ClassTemplate.h"
#include "util/Comparators.h"

namespace templex {
namespace parser {

class TypeCache {

    using ClassInstantiations      = std::vector<ClassInstantiationPtr>;
    using ClassInstantiationsCache = std::map<ClassTemplatePtr,
                                              ClassInstantiations,
                                              ::nostd::less_pointee<ClassTemplate>>;

public:
    static TypeCache& getInstance()
    {
        // C++ standard 6.7.1
        static TypeCache instance;
        return instance;
    }

    TypeCache(const TypeCache&) = delete;
    void operator=(const TypeCache&) = delete;

public:
    void addClassTemplate(ClassTemplatePtr classTemplate);

private:
    TypeCache() {}

private:
    ClassInstantiationsCache instantiations_;
};

} // namespace parser
} // namespace templex