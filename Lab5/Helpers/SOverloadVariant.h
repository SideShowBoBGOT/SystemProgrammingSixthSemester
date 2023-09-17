#ifndef LAB5_SOVERLOADVARIANT_H
#define LAB5_SOVERLOADVARIANT_H

#include <gsl/gsl>
#include <variant>
#include <memory>

template<class... Ts>
struct SOverloadVariant : Ts... { using Ts::operator()...; };

template<class... Ts>
SOverloadVariant(Ts...) -> SOverloadVariant<Ts...>;

template<class... Ts>
std::variant<std::weak_ptr<Ts>...> DowngradeVariant(const std::variant<gsl::not_null<std::shared_ptr<Ts>>...>& var) {
    return std::visit([](const auto& ptr) { return std::variant<std::weak_ptr<Ts>...>(ptr.get()); }, var);
}

template<class... Ts>
std::variant<std::weak_ptr<Ts>...> DowngradeVariant(const std::variant<std::shared_ptr<Ts>...>& var) {
    return std::visit([](const auto& ptr) { return std::variant<std::weak_ptr<Ts>...>(ptr); }, var);
}

template<class... Ts>
std::variant<std::shared_ptr<Ts>...> UpgradeVariant(const std::variant<std::weak_ptr<Ts>...>& var) {
    return std::visit([](const auto& ptr) { return std::variant<std::shared_ptr<Ts>...>(ptr.lock()); }, var);
}

#endif //LAB5_SOVERLOADVARIANT_H
