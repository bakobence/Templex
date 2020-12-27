#include "PageBase.h"

#include <stdexcept>

using namespace templex;
using namespace templex::frontend;

PageBase::PageBase() {}

PageBase::~PageBase() {}

void PageBase::initialize() {
    throw std::runtime_error("PageBase: initialize() not implemented");
}

void PageBase::deinitialize() {
    throw std::runtime_error("PageBase: deinitialize() not implemented");
}

QString PageBase::pageName() {
    throw std::runtime_error("PageBase: pageName() not implemented");
}
