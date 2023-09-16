#ifndef LAB5_SOVERLOADVARIANT_H
#define LAB5_SOVERLOADVARIANT_H

template<class... Ts>
struct SOverloadVariant : Ts... { using Ts::operator()...; };

template<class... Ts>
SOverloadVariant(Ts...) -> SOverloadVariant<Ts...>;

#endif //LAB5_SOVERLOADVARIANT_H
