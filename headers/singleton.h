#ifndef SINGLETON_H
#define SINGLETON_H
#include <iostream>
//  singleton crtp

template <typename Derived, bool has_default_constructor=true>
class Singleton
{
protected:
    Singleton() = default;
public:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    static Derived& getInstance() { return getInstance_(std::bool_constant<has_default_constructor>{}); }
    ~Singleton() {}

    template <typename... Args>
    static void init(Args&&... args) {
        if(!instance)
            instance = new DerivedInstance(std::forward<Args>(args)...);
    }
    static void destroyInstance() {
        if (instance) {
            delete instance;
            instance = nullptr;
            std::cout << "Singleton instance destroyed." << std::endl;
        }
    }
private:
    static Derived& getInstance_(std::true_type) {
        if(!instance) {
            instance = new DerivedInstance;
        }
        return *instance;
    }
    static Derived& getInstance_(std::false_type) {
        return *instance;
    }
    class DerivedInstance : public Derived
    {
    public:
        template <typename... Args>
        explicit DerivedInstance(Args&&... args) : Derived(std::forward<Args>(args)...) {}
    };
    static Derived* instance;
};
template<typename Derived, bool has_default_constructor>
Derived* Singleton<Derived, has_default_constructor>::instance = nullptr;

#endif //SINGLETON_H