#ifndef LAB5_NNCOMMONDECLARATION_H
#define LAB5_NNCOMMONDECLARATION_H

#include <variant>
#include <memory>

#include <gsl/gsl>

struct SLink;
struct SFile;
struct SDirectory;

template<typename T>
using not_null_shared_ptr = gsl::not_null<std::shared_ptr<T>>;

using TStrongLink = not_null_shared_ptr<SLink>;
using TStrongFile = not_null_shared_ptr<SFile>;
using TStrongDirectory = not_null_shared_ptr<SDirectory>;
using TNullableDirectory = std::shared_ptr<SDirectory>;
using TWeakLink = std::weak_ptr<SLink>;
using TWeakFile = std::weak_ptr<SFile>;
using TWeakDirectory = std::weak_ptr<SDirectory>;

using TWeakFileVariant = std::variant<TWeakLink, TWeakFile, TWeakDirectory>;
using TStrongFileVariant = std::variant<TStrongLink, TStrongFile, TStrongDirectory>;

class TNotDirectoryException;
class TFileObjectNotExistException;

using TFileSystemVariantException = std::variant<TNotDirectoryException, TFileObjectNotExistException>;

#endif //LAB5_NNCOMMONDECLARATION_H
