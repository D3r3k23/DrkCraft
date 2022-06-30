#ifndef DRK_CORE_TYPE_VARIANT_HPP
#define DRK_CORE_TYPE_VARIANT_HPP

#include <variant>
#include <concepts>
#include <utility>

namespace DrkCraft
{
    using MonoState = std::monostate;

    template <typename ... Ts>
    class Variant;

    template <typename ... T>
    struct Visitor : T...
    {
        using T::operator()...;

        template <typename ... Ts>
        auto visit(Variant<Ts...>&& variant) const&
        {
            return visit(std::forward<Variant<Ts...>>(variant), *this);
        }

        template <typename ... Ts>
        auto visit(const Variant<Ts...>& variant) const&
        {
            return visit(variant, *this);
        }

        template <typename ... Ts>
        auto visit(Variant<Ts...>&& variant) &&
        {
            return visit(std::forward<Variant<Ts...>>(variant), std::move(*this));
        }

        template <typename ... Ts>
        auto visit(const Variant<Ts...>& variant) &&
        {
            return visit(variant, std::move(*this));
        }
    };

    template <typename Vis, typename ... Ts>
    concept VisitorConcept = std::derived_from<Vis, Visitor<Ts...>>;

    template <typename ... Ts>
    class Variant
    {
    private:
        using VariantType = std::variant<Ts...>;
        VariantType m_variant;

    public:
        // constexpr Variant(void)
        //   : m_variant()
        // { }

        template <typename ... Args>
        constexpr Variant(Args&&... args)
          : m_variant(std::forward<Args>(args)...)
        { }

        template <typename ... Args>
        constexpr Variant& operator=(Args&& ... a)
        {
            m_variant.operator=(std::forward<Args>(a)...);
            return *this;
        }

        auto emplace(auto&& ... args)
        {
            return m_variant.emplace(std::forward<decltype(args)>(args)...);
        }

        template <typename T>
        constexpr bool has(void) const
        {
            return std::holds_alternative<T>(m_variant);
        }

        template <typename T>
        constexpr const T& get(void) const
        {
            return std::get<T>(m_variant);
        }

        template <typename T>
        constexpr const T* get_if(void) const
        {
            return std::get_if<T>(&m_variant);
        }

        auto match(const auto& visitor)
        {
            return visitor.visit(*this);
        }

        auto match(const auto& visitor) const
        {
            return visitor.visit(*this);
        }

        auto match(auto&& visitor)
        {
            return std::move(visitor).visit(*this);
        }

        auto match(auto&& visitor) const
        {
            return std::move(visitor).visit(*this);
        }

        void swap(Variant& other)
        {
            m_variant.swap(other.m_variant);
        }

        auto hash(void) const
        {
            return std::hash<VariantType>(m_variant);
        }

        template <typename ... Ts>
        friend auto visit(Variant& variant, VisitorConcept auto&& visitor)
        {
            return std::visit(std::forward<decltype(visitor)>(visitor), variant.m_variant);
        }

        template <typename ... Ts>
        friend auto visit(Variant& variant, const VisitorConcept auto& visitor)
        {
            return std::visit(std::forward<decltype(visitor)>(visitor), variant.m_variant);
        }
    };

    template <typename T, typename ... Ts>
    const T& get(const Variant<Ts...>& variant)
    {
        return variant.get();
    }

    template <typename ... Ts>
    constexpr auto match(const Variant<Ts...>& variant)
    {
        return [&variant](VisitorConcept auto&& visitor)
        {
            return variant.match(visitor);
        };
    }

    template <typename ... Ts>
    constexpr auto match(Variant<Ts...>&& variant)
    {
        return [variant=std::move(variant)](VisitorConcept auto&& visitor)
        {
            return variant.match(visitor);
        };
    }

    template <typename ... Ts>
    void swap(Variant<Ts...>& var1, Variant<Ts...>& var2)
    {
        var1.swap(var2);
    }
}

namespace std
{
    template <typename ... Ts>
    struct hash<DrkCraft::Variant<Ts...>>
    {
        auto operator()(const DrkCraft::Variant<Ts...>& variant) const
        {
            return variant.hash();
        }
    };
}

#endif // DRK_CORE_TYPE_VARIANT_HPP
