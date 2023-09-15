#ifndef LAB5_NNCOMMONDECLARATION_H
#define LAB5_NNCOMMONDECLARATION_H

#include <variant>
#include <memory>

#include <gsl/gsl>

class SLink;
class SFile;
class SDirectory;

template<typename T>
using not_null_shared_ptr = gsl::not_null<std::shared_ptr<T>>;

using PLink = not_null_shared_ptr<SLink>;
using PFile = not_null_shared_ptr<SFile>;
using PDirectory = not_null_shared_ptr<SDirectory>;

using TFileVariant = std::variant<PLink, PFile, PDirectory>;

#endif //LAB5_NNCOMMONDECLARATION_H
