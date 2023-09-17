#ifndef LAB5_THELPERS_H
#define LAB5_THELPERS_H

#include <gsl/gsl>
#include <variant>
#include <expected>
#include <memory>
#include <iostream>
#include <functional>

template<class... Ts>
struct THelpers : Ts... { using Ts::operator()...; };

template<class... Ts>
THelpers(Ts...) -> THelpers<Ts...>;

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

template<typename T, typename... Errs>
int PrintErrGetVal(const std::expected<T, std::variant<Errs...>>& result) {
    const auto& err = result.error();
    return std::visit([](const auto& ex) {
        std::cout << ex.what() << "\n";
        return ex.Value();
    }, err);
}

template<typename T, typename... Args>
gsl::not_null<std::shared_ptr<T>> MakeNotNull(Args... args) {
    return gsl::make_not_null(std::make_shared<T>(args...));
}

#endif //LAB5_THELPERS_H
