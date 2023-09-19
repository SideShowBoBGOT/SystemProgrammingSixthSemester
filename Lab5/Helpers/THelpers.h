#ifndef LAB5_THELPERS_H
#define LAB5_THELPERS_H

#include <gsl/gsl>
#include <variant>
#include <expected>
#include <memory>
#include <iostream>
#include <functional>

template<class... Ts>
struct SOverloadVariant : Ts... { using Ts::operator()...; };

template<class... Ts>
SOverloadVariant(Ts...) -> SOverloadVariant<Ts...>;

template<typename... Errs>
int PrintErrGetVal(const std::variant<Errs...>& err) {
    return std::visit([](const auto& ex) {
        return PrintErrGetVal(ex);
    }, err);
}

template<typename Exception> requires std::is_base_of_v<std::exception, Exception>
int PrintErrGetVal(const Exception& ex) {
    std::cout << ex.what() << "\n";
    return ex.Value();
}

template<typename T, typename... Args>
gsl::not_null<std::shared_ptr<T>> MakeNotNull(Args... args) {
    return gsl::make_not_null(std::make_shared<T>(args...));
}

#endif //LAB5_THELPERS_H
