namespace nostd {

template <typename T>
struct less_pointee {
    constexpr bool operator()(const std::shared_ptr<T>& lhs,
                              const std::shared_ptr<T>& rhs) const
    {
        return *lhs < *rhs;
    }
};

} // namespace nostd
