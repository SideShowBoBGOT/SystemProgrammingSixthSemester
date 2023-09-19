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

using TStLink = not_null_shared_ptr<SLink>;
using TStFile = not_null_shared_ptr<SFile>;
using TStDirectory = not_null_shared_ptr<SDirectory>;
using TOptStDirectory = std::shared_ptr<SDirectory>;
using TWDirectory = std::weak_ptr<SDirectory>;

using TStFileVariant = std::variant<TStLink, TStFile, TStDirectory>;

class TNotDirectoryException;
class TFileObjectNotExistException;

using TFileSystemVariantException = std::variant<TNotDirectoryException, TFileObjectNotExistException>;

#endif //LAB5_NNCOMMONDECLARATION_H
